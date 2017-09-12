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

#define LINE_BLACK 245
#define LINE_WHITE 150
#define LINE_THRESHOLD 40

#define DRIVE_NEUTRAL 90
#define DRIVE_FORWARDS 120
#define DRIVE_BACKWARDS 0
#define DRIVE_TURN_SPEED 20
#define DRIVE_MAX 180

#include <servo.h>
Servo servo_left;
Servo servo_right;


void setup() {
  Serial.begin(9600);
  servo_left.attach(SERVO_LEFT);
  servo_right.attach(SERVO_RIGHT);
}

void drive(int left, int right) {
  servo_left.write(DRIVE_NEUTRAL + left);
  servo_right.write(DRIVE_MAX - (DRIVE_NEUTRAL + right));
}

byte nsr(int pin) {
  return map(constrain(analogRead(pin), LINE_BLACK, LINE_WHITE), LINE_BLACK, LINE_WHITE, 0, 255);
}

int lineError() {
  int left = nsr(LEFT_IN);
  int right = nsr(RIGHT_IN);
  return left - right;
}

int lineStatus() {
  int left = nsr(LEFT_OUT);
  int right = nsr(RIGHT_OUT);
  if (left < LINE_THRESHOLD && right < LINE_THRESHOLD) {
    return LINE_FOLLOW_STOP;
  } else {
    return LINE_FOLLOW_GOOD;
  }
}

void drive(int dir) {
  int vl = LINE_P * dir / 255 + DRIVE_FORWARDS;
  int vr =  - LINE_P * dir / 255 + DRIVE_FORWARDS;
  drive(vl, vr);
}


void rotate90(int dir) {
  int vl = dir * DRIVE_TURN_SPEED;
  int vr = - dir * DRIVE_TURN_SPEED;

  drive(vl, vr)

  while (lineStatus() == LINE_FOLLOW_GOOD) {
    delay(REGULATION_DELAY);
  }

  drive(0, 0);
}

int lineFollow() {
  while(lineStatus() == LINE_FOLLOW_GOOD) {
    drive(lineError());
    delay(REGULATION_DELAY);
  }
}

void loop() {
  Serial.println("Starting!");
  lineFollow();
  Serial.println("Found intersection!");
  delay(5000);
}
