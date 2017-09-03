# Lab 1

### Teams
**Team 1**: Ayomi, Emily, Jacob

**Team 2**: Drew, Eric, Joo Yeon

## Internal LED

We downloaded Arduino IDE from [this link.](https://www.arduino.cc/en/Main/Software)

Using the "Blink" code in File> Examples> 1.Basics> Blink of Arduino IDE, we could make the internal LED blink.

[<img src = "https://docs.google.com/uc?id=0B1QvEdmy23tjMkxKWTdHTW5EMWc" height="400">](https://drive.google.com/file/d/0B1r9QYTd8YNrNDVkeWtic3RsVTg/view?usp=sharing)

## External LED

The external LED was connected in series with a 1k ohm resistor from pin 11 to ground on the Arduino as seen below. 

<img src="https://docs.google.com/uc?id=0BzwUNPRyfTQHeVZfMXVLR2U4Ylk" height="400">

We then modifyied the Blink code to incorperate an external LED by replacing all instances of LED_BUILTIN with 11 `setup()` and  `loop()`. 
E.g.
```cpp
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(11, OUTPUT);
}
```
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


## Assembling the Robot

We took cues from the sample robot in Lab 1 to put together our own. To begin with, we first assembled the materials we needed, as shown below.

<img src="https://docs.google.com/uc?id=1c0DkySEyc6ZJIq_NvBGxPMbfce-NcpBacw" height="350">



The first thing we mounted were the Parallax servos. The servo was first attached to the mount, and then to the baseboard.

<img src="https://docs.google.com/uc?id=0B1r9QYTd8YNrMFJWd3B5aWZJazA" width="350"/> <img src="https://docs.google.com/uc?id=0B1r9QYTd8YNrSlNid18tNGQ4SXc" width="350"/> 


The wheels were then mounted to the servos. We noticed that some of the wheels available to us did not have have a mating part that fit well on the servo. This should be something we think about when designing our own wheels down the line.

<img src="https://docs.google.com/uc?id=1eazASuumQB5sKiFpqPIOJ5tm4pvzpyMf9w" height="350">


The battery mount, Arduino, and front-wheel were also assembled as follows.

<img src="https://docs.google.com/uc?id=0B1r9QYTd8YNrZkRPRGNJLWdRVGs" width="350"/> <img src="https://docs.google.com/uc?id=1Y8ZkJ1QISOGYjLM5azNQpE-2pwf03tD-eA" width="350" height="467"/> 


The next thing we had to think about was how we would power the necessary peripherals. To power two servos from our 5V battery pack, we spliced a USB cable such that we were able to get the individual power and ground wires. Since there is only a single 5V port on the Arduino, this served us well as a temporary hack so both servos could access 5V through connection on a breadboard. The Arduino was powered through a 9V battery, that we placed beneath the Arduino mount on the robot. With everything assembled, our robot was ready to be programmed.

<img src="https://docs.google.com/uc?id=1Bg2puudVcm_6CI4zmGxcJnTOrO3zEnfg6Q" width="350"/> <img src="https://docs.google.com/uc?id=1XYp7w0-QsNITrHD-juAAXtCYlJIOh87FHg" width="350"/> 


## Driving the Robot

After we finished assembling our robot, we wrote code for our robot to move straight in a line and turn around in a circle. We recorded the robot moving in a circle. One of the servos was moving faster than the other one, so our robot appears to be moving around a stationary point.
```cpp
void loop() {
  servoRight.write(180);
  servoLeft.write(180);
  delay(15);                       
}
```

[<img src="https://docs.google.com/uc?id=1Xzk2YGG8ga-V-myhpGhhaubiew9y8vnGVg" width="640" height="480">](https://drive.google.com/file/d/1Xzk2YGG8ga-V-myhpGhhaubiew9y8vnGVg/view?usp=sharing)



## Work Distribution

*   Joo Yeon: internal LED blink
*   Jacob: reading the potentiometer
*   Emily: assembling the robot
