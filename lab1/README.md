# Lab 1

### Teams
**Team 1**: Ayomi, Emily, Jacob

**Team 2**: Drew, Eric, Joo Yeon

## Reading the Potentiometer

We connected the potentiometer to the Arduino like so:

<img src="https://docs.google.com/uc?id=0B1r9QYTd8YNrTmRwODRBZjV1OGs">

We then modified the `AnalogReadSerial` example (found in the "Examples" menu in the Arduino IDE). In order to communicate over USB, we initialized the serial connection in `setup()`:
```cpp
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}
```
We read the sensor value in an `analogRead` call:
```cpp
int sensorValue = analogRead(A3)
```
We printed it to the screen using `Serial` methods:
```cpp
Serial.print("sensor = ");
Serial.println(sensorValue);
```
Finally, we waited for half a second using `delay`:
```cpp
delay(500)
```

Running that code printed out the analog value to the serial monitor. It worked as expected. Here's a picture of the setup:

<img src="https://docs.google.com/uc?id=1agDfyTcCZsLoZ3OFB3EO3M6aQLcQPt0MUA" height="400">

The next phase was controlling an LED with the potentiometer reading. We built the following circuit:

<img src="https://docs.google.com/uc?id=0B1r9QYTd8YNrUTg1RnY0di1TWXM">

Note that the LED is connected to pin 9 on the Arduino, which is important because pin 9 is a PWM pin.

We then modified the `AnalogInOutSerial` example. After reading the sensor value (as discussed above), we scaled it from 0-1023 to 0-255, and then wrote it to the output pin:

```cpp
  int outputValue = map(sensorValue, 0, 1023, 0, 255);
  analogWrite(9, outputValue);
```
The scaling was needed because `analogWrite` only writes values from 0-255. We ran this code, and it worked as expected. Here's a picture of the setup:

<img src="https://docs.google.com/uc?id=1DkwUjAhXsz1BvK-RX3O2ZzwMXiiciTsKhQ" height="400">

As we rotated the potentiometer, the LED changed brightness:

<iframe src="https://drive.google.com/file/d/0B1r9QYTd8YNrcDJ1dDNBdV81cWs/preview" width="640" height="480"></iframe>

Finally, we attached the LED leads to the oscilloscope to see what the PWM signal looked like. As we rotated the potentiometer, the duty cycle of the square wave changed:

<iframe src="https://drive.google.com/file/d/0B1r9QYTd8YNrTHhhUzBoUzY1aWM/preview" width="640" height="480"></iframe>

## Work Distribution

*   Jacob: reading the potentiometer
