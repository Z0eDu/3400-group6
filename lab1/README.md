# Lab 1

### Teams
**Team 1**: Ayomi, Emily, Jacob

**Team 2**: Drew, Eric, Joo Yeon

## Reading the Potentiometer

We connected the potentiometer to the Arduino like so:

![pot-arduino](https://lh4.googleusercontent.com/gH3Ndi3KH1D2QZhWMhl0BO8blufS9mbOZ5FZL_F1u9qA6LcSqTXGxjh26cy33M7byJCtSRBC05_yq0w=w958-h928)


We then modified the `AnalogReadSerial` example (found in the "Examples" menu in the Arduino IDE) to this:

```cpp
const int analogInPin = A3;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.println(sensorValue);

  // wait 0.5 s
  delay(500);
}
```

Running that code printed out the analog value to the serial monitor. It worked as expected. Here's a picture of the setup:

![pot-arduino-pic](https://lh3.googleusercontent.com/JNyjTZcaYBsktG-GgPXmr5g1LiwD3qYWUKJQjxEcN2iUXuDAzjWPYiJ3dkpXt9Dzm1st8-mC-3oZ_4lJfVMb9Q=w958-h928-rw)

The next phase was controlling an LED with the potentiometer reading. We built the following circuit:

![pot-led-arduino](https://lh3.googleusercontent.com/NvmLU-VTT4xxGs-HEeml8QHDkVfilKOdsGNLrtKcOV_cO_buRRkR3HlFNr8q5r9mPaAeqVNUQz1jw7s=w958-h928)

Note that the LED is connected to pin 9 on the Arduino, which is important because pin 9 is a PWM pin.

We then modified the `AnalogInOutSerial` example to this:

```cpp
const int analogInPin = A3;  // Analog input pin that the potentiometer is attached to
const int analogOutPin = 9; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);
  // map it to the range of the analog out:
  outputValue = map(sensorValue, 0, 1023, 0, 255);
  // change the analog out value:
  analogWrite(analogOutPin, outputValue);

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.print(sensorValue);
  Serial.print("\t output = ");
  Serial.println(outputValue);

  // wait 2 milliseconds before the next loop for the analog-to-digital
  // converter to settle after the last reading:
  delay(2);
}
```

This code reads the potentiometer value on analog pin 3, and then scales it from 0-1023 to 0-255. This is because `analogWrite` only writes values from 0-255. We ran this code, and it worked as expected. Here's a picture of the setup:

![pot-led-arduino-pic](https://lh4.googleusercontent.com/4FmPaz6YKEXsQJIiJLPTTpvqcTRwlgFndAD_v6Nyi3Hm2NhopjWdL9dQhDR1LKHmJh3NfO3MMjIRCqsTuZ5mKA=w958-h928-rw)

As we rotated the potentiometer, the LED changed brightness:

<iframe src="https://drive.google.com/file/d/0B1r9QYTd8YNrcDJ1dDNBdV81cWs/preview" width="640" height="480"></iframe>

Finally, we attached the LED leads to the oscilloscope to see what the PWM signal looked like. As we rotated the potentiometer, the duty cycle of the square wave changed:

<iframe src="https://drive.google.com/file/d/0B1r9QYTd8YNrTHhhUzBoUzY1aWM/preview" width="640" height="480"></iframe>

## Work Distribution
* Jacob: reading the potentiometer
