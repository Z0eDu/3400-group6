# Milestone 1: Line Following and Figure Eight

## Robot Design

We mounted 4 line sensors on the front of our robot:

<img src="https://docs.google.com/drawings/d/e/2PACX-1vTnUPFIO_hQl_hhROnfM4NYv3XP-baZnS-AEOdejTgAUpANg4RfpMMHAJY5JZtBBANSvGLjsKtdm3mF/pub?w=387&h=688" height="400">

The inner 2 sensors are used for line following, and the outer 2 sensors are used to detect intersections:

![foobar](https://docs.google.com/drawings/d/e/2PACX-1vRF6c4o8zFln_yPjxUNZebqH3a0HgL3D8mK2dRO821P812rKY34zp4H12wJCZgIeUxLSrVgDQRGiksS/pub?w=960&h=720)

## Line Following

### Sensor Calibration

We read the sensors using the Arduino's 10 bit ADC, so the reading for each sensor was between 0 and 1023. In order to make line detection and subsequent math easier, we scaled the raw values such that 0 corresponded to black, and 255 corresponded to white. To do so, we measured the raw sensor values for black and white, and found that they were about 900 and 500, respectively. Then, using the `constrain` and `map` functions, we scaled the raw value, `raw`:
```cpp
255 - map(constrain(raw, LINE_WHITE, LINE_BLACK), LINE_WHITE, LINE_BLACK, 0, 255)
```

### Error Measurement

Once the sensors were calibrated, we then implemented line following using a differential method. Because we position 2 sensors on opposite sides of the line, the _error_ while line following is simply the difference between the left and right normalized values! This approach reduces noise, because it removes the common mode from the signals.

### Regulation Loop

We used a simple proportional controller for line following. We took the error, multiplied it by a constant, used that value to control the curvature of our path. We wrote a method called `drive(int dir)` which caused the robot to drive at a given curvature.


### Video

This simple proportional controller worked very well, and was able to consistently follow both left and right curving lines:

<iframe src="https://drive.google.com/file/d/0BzwUNPRyfTQHME9vWGwtVHR6WWM/preview" width="640" height="480"></iframe>

In order to make the line following faster and more robust, we will use a full PID controller for the final project, likely with [this library](https://github.com/br3ttb/Arduino-PID-Library).

## Figure Eight

### Intersection Detection
Now that the robot was able to consistently follow a curved black line, we had the confidence it could handle worst case scenarios in the maze. With this confidence, the detection of intersections was implemented. The circumstance that would determine if there was a intersection was if both outer line sensors fell below a certain threshold simultaneously (indicating a black line). The code that corresponds is as follows:
```cpp
if (left < LINE_THRESHOLD && right < LINE_THRESHOLD) {
	    return LINE_FOLLOW_STOP;
```
After reaching these intersections, the function for line following is ended.

### Turning
Then, the method `rotate90(int dir)` was implemented. This function's intention was to place the robot near 90 degrees, any variations would then be caught by `lineFollow`. In this function the robot drives forward for 2 seconds to nearly center its wheels around the intersection. It then turns around its center by having both wheels turn at the same speed in opposite directions. The direction each wheel turns in is given by `dir`, an argument that is set to either 1 or -1, the speed is given by a constant. This turn is carried out for 1.7 seconds, an empirically determined value. Then the program stops the robot.

### Putting it Together
Finally, the method `figureEight()` was constructed to be called in `loop()`. Inside this method is a for loop with an variable, `i`, that counts from 0 to 7. Line follow is called, and does not end until it reaches an intersection. Once it is finished, if `i` is below 4, a right turn is instructed, otherwise a left turn is instructed. Then this method repeats, giving the figure eight pattern.

<iframe src="https://drive.google.com/file/d/0B1r9QYTd8YNrUDF1QnNyRUhGYTA/preview" width="640" height="480"></iframe>

## Work Distribution

*   Ayomi: Soldering header pins into the line sensors, rebuilding the robot to accommodate a breadboard
*   Drew: Robot building
*   Emily: Mounting line sensors to the robot
*   Eric: Driving and turning code, tuning
*   Jacob: Initial line following code
*   Joo Yeon: Robot building
