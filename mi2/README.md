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
From the conversion above, we were able to make the robot move while continually checking the wall sensor to see how far away it was from the wall. For the purposes of Milestone 2, we had the robot move closer to the wall until it read that it was 7 cm away. To accomplish this, we created a helper function:
```cpp
void stopAtWall() {
  while (getDistance(1) > 7)  //drive while the sensor at A1 is farther than 7 cm away from the wall
    drive(10, 10);
    
  drive(0,0);             //stop
}
```
Below is a video of our robot moving toward, and stopping at, a wall. In order to show that the robot stopped autonomously, we moved the wall farther away from the robot, and the robot moved toward the wall again and stopped.

<iframe src="https://drive.google.com/file/d/0ByCM4xElwbIeX2ZreEtQaWdUelk/preview" width="640" height="480"></iframe>




## Treasure Classification

### Initial Detecting Treasures at Different Frequencies
Using our code from Lab 2, we wanted to first make sure that we were able to detect the treasures at 7kHz, 12kHz, and 17kHz. To do this, we first tuned the frequency of the treasure to obtain the desired frequencies using the oscilloscpe, as shown below.  

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoY1ltR0toYmJDYzQ" width="275"><img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoUkxULUZYeEl0b2c" width="275"><img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoeXFOVDcyXzRWdDg" width="275">

We then created a sine wave with the frequency generator and ran our FFT code to find the log magnitudes for each bin for both 12kHz and 17kHz (7 kHz was done in Lab 2). 

<img src="https://docs.google.com/uc?id=0B1QvEdmy23tjRTRMTXpqUGFGMDA" width="400"> <img src="https://docs.google.com/uc?id=0B0-yVGdr0EwobldOS3dCRVQwVXM" width="425" height="300">

<img src="https://docs.google.com/uc?id=0B1QvEdmy23tjMFZ6MENBSFJXTjQ" width="400"> <img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoRjU3d09LZmdVdjQ" width="425" height="300">

We then held the treasure tuned to the correct frequency close to the phototransistor, and checked to ensure that the peaks for the treasure data lined up with that from the frequency generator. As seen from the graphs, the peak for 7KHz is around the 48th bin, around the 82nd bin for 12kHz (see Signal Filtering section), and around the 115th bin for 17kHz.  
<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoeVhLWVJ0dmloUHc" width="350">  <img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoaGQ1OGxUNnFHbTA" width="350">

With this data, we can digitally process the bins numbers and detect treasures at different frequencies.

### Signal Amplification
Since filter circuits affected the peak, we decided to filter software wise instead. Using the data above, we amplified only the outputs at the peak range bins: 7kHz 20-60th bin, 12kHz 70-90th bin, and 17kHz 100-120th bin. This is the amplifying code.

```cpp
for (int i = 0 ; i < 128 ; i += 1) {  
   if (i > 30 && i < 60) { 
      Serial.println(100*fft_log_out[i]);
    }
    else if (i > 70 && i < 90){
      Serial.println(100*fft_log_out[i]);
    }
    else if (i > 100 && i < 120){
      Serial.println(100*fft_log_out[i]);
    }
    else {
      Serial.println(fft_log_out[i]);
    }
 ```

The following graphs are from the amplified data.

<img src="https://docs.google.com/uc?id=0B1QvEdmy23tjU183Qjc4SW9MTFU" width="300"> <img src="https://docs.google.com/uc?id=0B1QvEdmy23tjMzYzcjVXWmt5UW8" width="300">

<img src="https://docs.google.com/uc?id=0B1QvEdmy23tjdU9YRUhwSm5OUDg" width="300"> <img src="https://docs.google.com/uc?id=0B1QvEdmy23tjenNwbzc2bG1maTA" width="300">

The logarithmic amplitude went up to about 6000 at a peak. We were able to figure out from the data if there was a peak, and what the frequency of the treasure was by determining which bin range was the point with logarithmic amlitude of around 6000 in.


### Proximity of Treasure to Phototransistor
One issue during this procedure resulted from the intensity of the signal falling off over the distance from the sensor to the reciever. Within 2.5cm, each signal produced a detectable peak in its respective bin. However, it was observed that passed 8cm, the noise level would surpass the peak generated by the signal. The differences between the two ranges depicted below:

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwodHVUS1h1M2JIdE0" width="275">
<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoMXhyWFRhMWhQQkU" width="275">

This raised the issue that attempting to amplify the signal to make the peak more discernable would also raise the noise level. Because of this, the decision to mount the treasure sensor as near the walls as possible was decided in order avoid the effects of noise.

### Filters and Future Improvements 

The two things we want to improve are the distance our robot picks up the treasure signals, and being able to better separate the signals from the rest. We know that we need to be able to detect signals only at 7kHz, 12kHz, 17kHz. Any other signals are irrelevant, so we want to design a filter that will ignore the other signals or amplify the ones we need. For improving the distance, we do not have a set plan yet, but we are deciding where is the best spot on the robot for it. 

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoRjU3d09LZmdVdjQ" width="350"> <img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoaGQ1OGxUNnFHbTA" width="350">

For now, we are able to distinguish between the different treasures, but in the weeks before the competition we will work on improving the range in which we can detect the treasures.


## Work Distribution

*   Ayomi: Treasure detection
*   Drew: Treasure detection
*   Emily: Treasure detection
*   Eric: Wall detection
*   Jacob: Wall detection
*   Joo Yeon: Treasure detection
