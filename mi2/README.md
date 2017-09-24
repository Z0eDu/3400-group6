# Milestone 2: Walls and Treasures

## Wall Detection

### Sensors
We used the Sharp GP2Y0A41SK0F IR distance sensor. With a supply voltage of 5 V, the sensor outputs an analog voltage which is linear with the inverse of distance. Using the [data sheet](https://www.pololu.com/file/0J713/GP2Y0A41SK0F.pdf), we determined the equation which relates output voltage and distance to be:
```
d = (12.9895 - 0.42 * v) / (v + 0.0249221)
```
Where `d` is the distance in centimeters and `v` is the voltage in volts. The Arduino ADC is a 10 bit ADC with a 5 V reference voltage, so given a raw reading `x`, the detected voltage is `5 / 1023 * x`.

We tested our distance calculations by place the sensor in front of a wooden plank, and moving it back and forth while measuring the distance with a ruler. The sensor worked as expcected, and the above formulas produced the correct distance, within 1 cm.

### Code

## Treasure Classification

### Signal Filtering

Since filter circuits affected the peak, we have decided to magnify it software wise instead. To determine the range of bin where the peak should appear, we first connected a function generator that generates 7kHz/ 12kHz/ 17Hz directly as the input and collected data. 


<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoUkFROEMxZXFnZlU" width="500">

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoSG5adlBBX05kaVk" width="200"><img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoSG5adlBBX05kaVk" width="200"><img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoWHdON3U1VHR6N1U" width="200">

Comparing it with the data when there is no treasure, we figured out the ranges of bin for the peak at each frequency: 7kHz 20-60, 12kHz 70-90, and 17kHz ??.


## Work Distribution

*   Ayomi: Treasure detection
*   Drew: Wall detection
*   Emily: Treasure detection
*   Eric: Treasure detection
*   Jacob: Wall detection
*   Joo Yeon: Treasure detection
