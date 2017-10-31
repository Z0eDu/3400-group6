#define LEFT_OUT A5
#define LEFT_IN A3
#define RIGHT_IN A2
#define RIGHT_OUT A5
#define LINE_P 8

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

#define MIC_st 0
#define LEFT_OUT_st 0
#define RIGHT_OUT_st 1
#define WALL_LEFT_st 3
#define WALL_RIGHT_st 4
#define WALL_FRONT_st 5

#include <Servo.h>
#include "dfs.h"
Servo servo_left;
Servo servo_right;
int Mux_State;

void setup() {
  muxSelect(MIC_st);
  Serial.begin(9600);
  servo_left.attach(SERVO_LEFT);
  servo_right.attach(SERVO_RIGHT);
}

void muxSelect(int state){
   Mux_State = state;
   digitalWrite(MUX_sel0, bitRead(state,0) ? HIGH : LOW);
   digitalWrite(MUX_sel1, bitRead(state,1) ? HIGH : LOW);
   digitalWrite(MUX_sel2, bitRead(state,2) ? HIGH : LOW);
}

// Effect: drives each motor at the given normalized velocity
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

// Returns: a normalized line sensor reading. 0 means black, 255 means white.
byte nsr(int pin) {
  int raw = analogRead(pin);
  Serial.print(pin);
  Serial.print(": ");
  Serial.println(raw);
  return 255 - map(constrain(raw, LINE_WHITE, LINE_BLACK), LINE_WHITE, LINE_BLACK, 0, 255);
}

// Returns: the error from being straight on the line. Between -255 and 255
// 0: perfect
// < 0: off to the right
// > 0: off to the left
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

// Returns: the current intsection status
// LINE_FOLLOW_STOP: at intersection
// LINE_FOLLOW_GOOD: not at intersection
int lineStatus() {
  muxSelect(LEFT_OUT_st);
  int left = nsr(LEFT_OUT);
  Serial.println("linestatus mux select");
  Serial.println(Mux_State);
  muxSelect(RIGHT_OUT_st);
  int right = nsr(RIGHT_OUT);
  Serial.print("ls : ");
  Serial.print(left);
  Serial.print(" ");
  Serial.println(right);
  Serial.print("mux_state");
  Serial.println(Mux_State);
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
  Serial.print(vl);
  Serial.print(", ");
  Serial.println(vr);
  drive(vl, vr);
}

// Effect: rotates the robot 90 degrees
// dir = -1: left
// dir = 1: right
void rotate90(int dir) {
  //Serial.println("GOING");
  //lineFollow(10000);
  drive(10, 10);
  delay(300);
  Serial.println("STOPPING");
  drive(0,0);
  int vl = dir * DRIVE_TURN_SPEED;
  int vr = - dir * DRIVE_TURN_SPEED;
  Serial.println("TURNING");
  drive(vl, vr);

  if(dir) {
    muxSelect(LEFT_OUT_st);
    Serial.println("rotate");
    Serial.println(Mux_State);
    Serial.println(A5);
    while(nsr(LEFT_OUT) > 40);
    while(nsr(LEFT_IN) > 40);

  }
  else {
    muxSelect(RIGHT_OUT_st);
    Serial.println(A5);
    while(nsr(RIGHT_OUT) > 40);
    while(nsr(RIGHT_IN) > 40);
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
  while(lineStatus() == LINE_FOLLOW_GOOD && ((millis()-start) < timeout || timeout == 0)) {
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
  for(int i = 0; i < 8; i ++){
    if(i < 4) {
      rotate90(-1);
    }else{
      rotate90(1);
    }
    lineFollow();
  }

}


void stopAtWall() {
  while (getDistance(1) > 7)
    drive(10, 10);

  drive(0,0);
}

//return the distance from the wall
float getDistance(int PINNAME) {
  float val = analogRead(PINNAME);   //read the value
  val = val * 5 /1023;               //convert the output to volts

  float cm = (12.9895 - .42*val) / (val+.0249221);   //convert the output to distance from wall (cm)

  return cm;
}

void loop() {
  //stopAtWall();

  explore_t state;
  dfs_init(&state, 0, 0, SOUTH);
  // dfs_mark_obstacle(&state, 1, 1, NORTH);
  // dfs_mark_obstacle(&state, 1, 1, EAST);
  // dfs_mark_obstacle(&state, 1, 1, SOUTH);
  // dfs_mark_obstacle(&state, 1, 1, WEST);
  //
  // dfs_mark_obstacle(&state, 1, 4, WEST);
  // dfs_mark_obstacle(&state, 2, 4, WEST);
  // dfs_mark_obstacle(&state, 3, 4, WEST);
  //
  // dfs_mark_obstacle(&state, 3, 0, NORTH);
  // dfs_mark_obstacle(&state, 3, 1, NORTH);
  // dfs_mark_obstacle(&state, 3, 2, NORTH);

  int last_rel_dir;
  while ((last_rel_dir = dfs_at_intersection(&state)) != -1) {
    switch (last_rel_dir) {
      case FORWARDS:
        drive(10,10);
        delay(500);
        break;
      case RIGHT:
        rotate90(1);
        break;
      case BACKWARDS:
        rotate90(-1);
        rotate90(-1);
        break;
      case LEFT:
        rotate90(-1);
        break;
    }

    lineFollow();
    drive(0,0);
    delay(500);
  }

  dfs_finalize(&state);
  // printf("Done:\n");
  // dfs_print_grid(&state);
  // delay_and_clear();
  // sleep(10);

  while(1);
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
