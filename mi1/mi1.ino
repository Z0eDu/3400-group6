#define LEFT_OUT A4
#define LEFT_IN A3
#define RIGHT_IN A2
#define RIGHT_OUT A1
#define LINE_P 50

#define SERVO_LEFT 10
#define SERVO_RIGHT 11

#define REGULATION_DELAY 2

#define LINE_FOLLOW_GOOD 0
#define LINE_FOLLOW_STOP 1

#define LINE_BLACK 0
#define LINE_WHITE 1023
#define LINE_THRESHOLD 40

#include <servo.h>
Servo servo_left;
Servo servo_right;


void setup() {
  Serial.begin(9600);
  servo_left.attach(SERVO_LEFT);
  servo_right.attach(SERVO_RIGHT);
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

int lineFollow() {
  
}

void loop() {
  while (1) {
    Serial.println(nsr(A0));
  }
}
