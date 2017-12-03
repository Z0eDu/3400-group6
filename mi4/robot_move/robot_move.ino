#define LEFT_OUT A5
#define LEFT_IN A3
#define RIGHT_IN A2
#define RIGHT_OUT A5
#define LINE_P 8

#define SERVO_LEFT 5
#define SERVO_RIGHT 6

#define REGULATION_DELAY 10

#define LINE_FOLLOW_GOOD 0
#define LINE_FOLLOW_STOP 1

#define LINE_BLACK 900
#define LINE_WHITE 500
#define LINE_THRESHOLD 50

#define DISTANCE_THRESHOLD 20

#define DRIVE_NEUTRAL_LEFT 94
#define DRIVE_NEUTRAL_RIGHT 88
#define DRIVE_SCALE_FWD 180
#define DRIVE_SCALE_REV 120
#define DRIVE_FORWARDS 10
#define DRIVE_BACKWARDS 0
#define DRIVE_TURN_SPEED 10
#define DRIVE_MAX 180

#define MUX A5 //input pin setup
#define MUX_sel0 2
#define MUX_sel1 3
#define MUX_sel2 4

#define MIC_st 2
#define LEFT_OUT_st 0
#define RIGHT_OUT_st 1
#define WALL_LEFT_st 3
#define WALL_RIGHT_st 4
#define WALL_FRONT_st 5

#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

#include <FFT.h>
#include <Servo.h>
#include "dfs.h"
Servo servo_left;
Servo servo_right;
int Mux_State;
/* Treasure code */
int fft_i = 0;
bool treasure_det = false;

void fastAdcSetup(int pin) {
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x44; // use adc1
  DIDR0 = 0x07; // turn off the digital input for adc0

  //  DIDR0 = 0x3F; // digital inputs disabled
  //  ADMUX = 0x40; // measuring on ADC0, use the internal 1.1 reference
  //  ADCSRA = 0xAC; // AD-converter on, interrupt enabled, prescaler = 16
  //  ADCSRB = 0x40; // AD channels MUX on, free running mode
  //  bitWrite(ADCSRA, 6, 1); // Start the conversion by setting bit 6 (=ADSC) in ADCSRA
  //sei(); // set interrupt flag
}

int fastAdcRead() {
  while (!(ADCSRA & 0x10)); // wait for adc to be ready
  ADCSRA = 0xf5; // restart adc
  byte m = ADCL; // fetch adc data
  byte j = ADCH;
  int k = (j << 8) | m; // form into an int
  k -= 0x0200; // form into a signed int
  k <<= 6; // form into a 16b signed int
  return k;
}

const uint64_t pipes[2] = { 0x0000000012LL, 0x0000000013LL };
RF24 radio(9, 10);

void setup() {
  muxSelect(MIC_st);
  Serial.begin(9600);
  Serial.println("setup");

  servo_left.attach(SERVO_LEFT);
  servo_right.attach(SERVO_RIGHT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A5, INPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);

  // Treasure setup
  fastAdcSetup(0x40);

  // put your setup code here, to run once:
  cli();
  TCCR2A = 0;// set entire TCCR2A register to 0
  TCCR2B = 0;// same for TCCR2B
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR2A = 51;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR2B |= (1 << CS21);

  //disable only the timer2 to begin

  TIMSK2 = 0;
  sei();


  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_HIGH);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);
  //
  // Open pipes to other nodes for communication
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);

  // Start listening
  radio.startListening();



}

volatile int count = 0;

// ISR for treasure detection
int max_data = 0;
int max_bin = 0;

ISR(TIMER2_COMPA_vect) {
  if (count == 0) {
    //Serial.println(count);
    // disable
    TIMSK2 = 0;
    return;
  }

  fft_input[fft_i] = fastAdcRead();
  fft_input[fft_i + 1] = 0;
  fft_i += 2;
  count--;
}


void transmit(unsigned short state) {
  //      // First, stop listening so we can talk.
  //   radio.stopListening();
  //
  //    bool update;
  //    printf("Now sending new robot update\n");
  //    update = false;
  //    while (!update) {
  //    update = radio.write( &state, sizeof(state) );
  //    };
}


void muxSelect(int state) {
  Mux_State = state;
  digitalWrite(MUX_sel0, bitRead(state, 0) ? HIGH : LOW);
  digitalWrite(MUX_sel1, bitRead(state, 1) ? HIGH : LOW);
  digitalWrite(MUX_sel2, bitRead(state, 2) ? HIGH : LOW);
}

// Effect: drives each motor at the given normalized velocity
void drive(int left, int right) {
  Serial.println("drive");
  if (right < 0) {
    right = right * DRIVE_SCALE_REV / 255;
    //Serial.println(left);
  } else {
    right = right * DRIVE_SCALE_FWD / 255;
  }
  servo_left.write(DRIVE_NEUTRAL_LEFT + left);
  servo_right.write(DRIVE_MAX - (DRIVE_NEUTRAL_RIGHT + right));
}

// Returns: a normalized line sensor reading. 0 means black, 255 means white.
byte nsr(int pin) {
  int raw = analogRead(pin);
  return 255 - map(constrain(raw, LINE_WHITE, LINE_BLACK), LINE_WHITE, LINE_BLACK, 0, 255);
}

// Returns: the error from being straight on the line. Between -255 and 255
// 0: perfect
// < 0: off to the right
// > 0: off to the left
int lineError() {
  int left = nsr(LEFT_IN);
  int right = nsr(RIGHT_IN);
  return left - right;
}

// Returns: the current intsection status
// LINE_FOLLOW_STOP: at intersection
// LINE_FOLLOW_GOOD: not at intersection
int lineStatus() {
  muxSelect(LEFT_OUT_st);
  int left = nsr(LEFT_OUT);
  muxSelect(RIGHT_OUT_st);

  int right = nsr(RIGHT_OUT);

  if (left < LINE_THRESHOLD || right < LINE_THRESHOLD) {
    return LINE_FOLLOW_STOP;
  } else {
    return LINE_FOLLOW_GOOD;
  }
}

// Effect: causes the robot to drive with the specified curvature
// dir = 0: straight
// dir < 0: left
// dir > 0: right
void drive(int dir) {
  int vl = LINE_P * dir / 255 + DRIVE_FORWARDS;
  int vr =  - LINE_P * dir / 255 + DRIVE_FORWARDS;
  //  Serial.print(vl);
  //  Serial.print(", ");
  //  Serial.println(vr);
  drive(vl, vr);
}

// Effect: rotates the robot 90 degrees
// dir = -1: left
// dir = 1: right

void rotate180() {
  drive(10, 10);
  delay(500);

  drive(0, 0);
  int dir = -1;
  int vl = dir * DRIVE_TURN_SPEED;
  int vr = - dir * DRIVE_TURN_SPEED;
  drive(vl, vr);

  muxSelect(RIGHT_OUT_st);
  while (nsr(RIGHT_OUT) > 50);
  delay(300);
  while (nsr(RIGHT_IN) > 20);

  drive(10, 10);

  delay(100);
  drive(vl, vr);

  muxSelect(RIGHT_OUT_st);
  while (nsr(RIGHT_OUT) > 50);
  delay(300);
  while (nsr(RIGHT_IN) > 20);
  drive(0, 0);
}


void rotate90(int dir) {
  //Serial.println("GOING");
  //lineFollow(10000);
  drive(10, 10);
  if (dir == 1) delay(250);
  if (dir == -1) delay(500);
  //  Serial.println("STOPPING");
  drive(0, 0);
  int vl = dir * DRIVE_TURN_SPEED ;
  int vr = - dir * DRIVE_TURN_SPEED ;
  //  Serial.println("TURNING");
  drive(vl, vr);

  if (dir == 1) {
    muxSelect(LEFT_OUT_st);
    delayMicroseconds(10);
    //    Serial.println("rotate");
    //    Serial.println(Mux_State);
    //    Serial.println(A5);
    while (nsr(LEFT_OUT) > 50);
    delay(300);
    while (nsr(LEFT_IN) > 20);

  }
  else {
    muxSelect(RIGHT_OUT_st);
    //    delayMicroseconds(100);
    //    Serial.println(A5);
    while (nsr(RIGHT_OUT) > 50);
    delay(300);
    while (nsr(RIGHT_IN) > 20);
  }
  //
  //  while (lineStatus() == LINE_FOLLOW_GOOD) {
  //    delay(REGULATION_DELAY);
  //  }
  //  while (lineStatus() != LINE_FOLLOW_GOOD) {
  //    delay(REGULATION_DELAY);
  //  }
  //delay(850);
  drive(0, 0);
}

// Effect: follows the line, for at most timeout milliseconds.
// Stops at an intersection.
int lineFollow(unsigned long timeout) {
  unsigned long start = millis();
  while (lineStatus() == LINE_FOLLOW_GOOD && ((millis() - start) < timeout || timeout == 0)) {
    drive(lineError());
    delay(REGULATION_DELAY);
  }
}

// Effect: follows the line until an intersection
int lineFollow() {
  return lineFollow(0);
}

// Effect: drives the robot in a figure eight
void figureEight() {
  for (int i = 0; i < 8; i ++) {
    if (i < 4) {
      rotate90(-1);
    } else {
      rotate90(1);
    }
    lineFollow();
  }

}

//return the distance from the wall
float getDistance(int PINNAME) {
  //  muxSelect(muxsel);
  //Serial.println("getDistance");
  float val = analogRead(PINNAME);   //read the value
  //Serial.println("after analog read");
  //  Serial.println(val);
  val = val * 5 / 1023;              //convert the output to volts

  float cm = (12.9895 - .42 * val) / (val + .0249221); //convert the output to distance from wall (cm)

  return cm;
}

void markWalls(explore_t* state) {
  //Serial.println("IN mark walls");

  delay(500);
  muxSelect(WALL_LEFT_st);
  delayMicroseconds(10);

  if (getDistance(MUX) < DISTANCE_THRESHOLD) {
    dfs_mark_rel_obstacle(state, LEFT);
    //Serial.println("mark left");
  }
  delay(500);
  muxSelect(WALL_RIGHT_st);
  delayMicroseconds(10);
  if (getDistance(MUX) < DISTANCE_THRESHOLD) {
    dfs_mark_rel_obstacle(state, RIGHT);
    //Serial.println("mark right");
  }
  delay(500);
  muxSelect(WALL_FRONT_st);
  delayMicroseconds(10);
  if (getDistance(MUX) < DISTANCE_THRESHOLD) {
    dfs_mark_rel_obstacle(state, FORWARDS);
  }
}

void loop() {
  Serial.println("loop");

  explore_t state;
  dfs_init(&state, 3, 0, EAST);
  //Serial.println("dfs_init");

  markWalls(&state);

  //dfs_mark_treasure(&state, TREASURE_7KHZ);

  for (size_t row = 0; row < MAP_ROWS; row++) {
    for (size_t col = 0; col < MAP_COLS; col++) {
      unsigned short info = dfs_get_grid_info_to_transmit(&state, row, col);
      transmit(info);

    }
  }


  int last_rel_dir;
  while ((last_rel_dir = dfs_at_intersection(&state)) != -1) {
    drive(0, 0);
    dfs_print_grid(&state);
    //Serial.print("Going: ");
    switch (last_rel_dir) {
      case FORWARDS:
        Serial.print(" F ");
        break;
      case RIGHT:
        Serial.print(" R ");
        break;
      case BACKWARDS:
        Serial.print(" B ");
        break;
      case LEFT:
        Serial.print(" L ");
        break;
      default:
        Serial.print("   ");
    }
    Serial.print("\n");


    delay(100);
    switch (last_rel_dir) {
      case FORWARDS:
        drive(10, 10);
        delay(300);
        break;
      case RIGHT:
        rotate90(1);
        break;
      case BACKWARDS:
        rotate180();
        break;
      case LEFT:
        rotate90(-1);
        break;
    }

    lineFollow();
    drive(0, 0);
    delay(500);
    markWalls(&state);


    //check for treasures
    fft_i = 0;
    count = 256;
    TIMSK2 |= (1 << OCIE2A);
    while (TIMSK2 != 0);


    //  if (fft_i == 512){
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft

    fft_i = 0;

    //Serial.println("Start");
    for (byte i = 0; i < FFT_N / 2; i++) {
      if (i > 20 && fft_log_out[i] > max_data) {
        max_bin = i;
        max_data = fft_log_out[i];
      }
      //delay(2000);
      // }
      //if (i==127) {
      //Serial.println("Stop");
      //}
    }

    int foundTreasure = -1;
    //Serial.println("max_bin");
    //Serial.println(max_bin);

    // Treasure detection
    // 7 kHz
    if (max_bin > 40 && max_bin < 65) {
      foundTreasure = 0;
      //Serial.println("7kHz");
      // 12 kHz
    } else if (max_bin >= 65 && max_bin < 100) {
      foundTreasure = 1;
      Serial.println("12kHz");
      // 17 kHz
    } else if (max_bin >= 100 && max_bin < 130) {
      foundTreasure = 2;
      //Serial.println("17kHz");
    } else {
      foundTreasure = -1;
      Serial.println("No treasure detected");
    }


    for (size_t row = 0; row < MAP_ROWS; row++) {
      for (size_t col = 0; col < MAP_COLS; col++) {
        unsigned short info = dfs_get_grid_info_to_transmit(&state, row, col);
        transmit(info);

      }
    }

  }



  dfs_finalize(&state);
  for (size_t row = 0; row < MAP_ROWS; row++) {
    for (size_t col = 0; col < MAP_COLS; col++) {
      unsigned short info = dfs_get_grid_info_to_transmit(&state, row, col);
      transmit(info);

    }
  }

  // PRINT("Done:\n");
  // dfs_print_grid(&state);
  // delay_and_clear();
  // sleep(10);
  while (1) {
    transmit(30 << 9);
  }
  //Serial.println("DONE");
  while (1);
  //
  //  lineFollow();
  //  drive(0,0);

  //figureEight();
  /*
    Serial.println("Starting!");
    lineFollow();
    Serial.println("Found intersection!");
    rotate90(-1);
    drive(0,0);
    delay(1000);*/

}
