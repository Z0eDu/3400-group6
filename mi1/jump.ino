#define LEFT_OUT A4
#define LEFT_IN A3
#define RIGHT_IN A2
#define RIGHT_OUT A1
#define LINE_P 5

#define SERVO_LEFT 10
#define SERVO_RIGHT 11

#define REGULATION_DELAY 10

#define LINE_FOLLOW_GOOD 0
#define LINE_FOLLOW_STOP 1

#define LINE_BLACK 900
#define LINE_WHITE 500
#define LINE_THRESHOLD 40

#define DRIVE_NEUTRAL_LEFT 94
#define DRIVE_NEUTRAL_RIGHT 86
#define DRIVE_SCALE_FWD 75
#define DRIVE_SCALE_REV 120
#define DRIVE_FORWARDS 10
#define DRIVE_BACKWARDS 0
#define DRIVE_TURN_SPEED 10
#define DRIVE_MAX 180

#include <Servo.h>
Servo servo_left;
Servo servo_right;


void setup() {
  Serial.begin(9600);
  servo_left.attach(SERVO_LEFT);
  servo_right.attach(SERVO_RIGHT);
}

void drive(int left, int right) {

  if (right < 0) {
    right = right * DRIVE_SCALE_REV / 255;
    Serial.println(left);
  } else {
    right = right * DRIVE_SCALE_FWD / 255;
  }
  servo_left.write(DRIVE_NEUTRAL_LEFT + left);
  servo_right.write(DRIVE_MAX - (DRIVE_NEUTRAL_RIGHT + right));
  }

byte nsr(int pin) {
  int raw = analogRead(pin);
  Serial.print(pin);
  Serial.print(": ");
  Serial.println(raw);
  return 255 - map(constrain(raw, LINE_WHITE, LINE_BLACK), LINE_WHITE, LINE_BLACK, 0, 255);
}

int lineError() {
  int left = nsr(LEFT_IN);
  int right = nsr(RIGHT_IN);
  Serial.print(left);
  Serial.print(" ");
  Serial.print(right);
  Serial.print(" ");
  Serial.println(left - right);
  return left - right;
}

int lineStatus() {
  int left = nsr(LEFT_OUT);
  int right = nsr(RIGHT_OUT);
  Serial.print("ls : ");
  Serial.print(left);
  Serial.print(" ");
  Serial.println(right);
  if (left < LINE_THRESHOLD && right < LINE_THRESHOLD) {
    return LINE_FOLLOW_STOP;
  } else {
    return LINE_FOLLOW_GOOD;
  }
}

void drive(int dir) {
  int vl = LINE_P * dir / 255 + DRIVE_FORWARDS;
  int vr =  - LINE_P * dir / 255 + DRIVE_FORWARDS;
  Serial.print(vl);
  Serial.print(", ");
  Serial.println(vr);
  drive(vl, vr);
}


void rotate90(int dir) {
  //Serial.println("GOING");
  //lineFollow(10000);
  drive(10, 10);
  delay(2000);
  Serial.println("STOPPING");
  drive(0,0);
  int vl = dir * DRIVE_TURN_SPEED;
  int vr = - dir * DRIVE_TURN_SPEED;
  Serial.println("TURNING");
  drive(vl, vr);
//  
//  while (lineStatus() == LINE_FOLLOW_GOOD) {
//    delay(REGULATION_DELAY);
//  }
//  while (lineStatus() != LINE_FOLLOW_GOOD) {
//    delay(REGULATION_DELAY);
//  }
  delay(1700);
  drive(0, 0);
}

int lineFollow(unsigned long timeout) {
  unsigned long start = millis();
  while(lineStatus() == LINE_FOLLOW_GOOD && ((millis()-start) < timeout || timeout == 0)) {
    drive(lineError());
    delay(REGULATION_DELAY);
  }
}

int lineFollow() {
  return lineFollow(0);
}

void figureEight() {
  for(int i = 0; i < 8; i ++){
    if(i < 4) {
      rotate90(-1);
    }else{
      rotate90(1);
    }
    lineFollow();
  }
  
}

void loop() {
  figureEight();
  /*
  Serial.println("Starting!");
  lineFollow();
  Serial.println("Found intersection!");
  rotate90(-1);
  drive(0,0);
  delay(1000);*/
}
