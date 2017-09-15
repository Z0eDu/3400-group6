# Milestone 1: Line Following and Figure Eight

## Robot Design

We mounted 4 line sensors on the front of our robot:
<img src="https://docs.google.com/uc?id=0ByCM4xElwbIeUkpPaGQtN01ITWs" height="400">

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

## Figure Eight

## Work Distribution

*   Ayomi:
*   Drew:
*   Emily:
*   Eric:
*   Jacob:
*   Joo Yeon:
