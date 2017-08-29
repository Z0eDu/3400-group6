#include <Servo.h>

int PINNAME = 0;
int val = 0;
Servo myservo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(11, OUTPUT);

  //set up the servo to pin 10 and turn it off
  myservo.attach(10);
  myservo.write(90);
}

void loop() {
  // put your main code here, to run repeatedly:

  val = analogRead(PINNAME);
  analogWrite(11, val);
  val = val/1.8;
  myservo.write(val);
  Serial.println(val);
  delay(500);

  //LED
  //frequency 500 Hz
  
  //servo
  //max duty: 1/8
  //min duty: 1/32
  //frequency: 50 Hz

}
