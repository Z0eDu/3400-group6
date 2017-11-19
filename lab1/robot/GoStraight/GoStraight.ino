#include <Servo.h>

Servo servoLeft;
Servo servoRight;

void setup() {
  servoLeft.attach(11);
  servoRight.attach(10);// attaches the servo on pin 9 to the servo object
}

void loop() {
  // scale it to use it with the servo (value between 0 and 180)
  servoRight.write(180);
  servoLeft.write(180);// sets the servo position according to the scaled value   94 is where it stopped
  delay(15);                           // waits for the servo to get there
}

 
