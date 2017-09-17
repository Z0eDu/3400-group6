# Lab 2: Digital Signal Processing

### Objectives
* Add sensors make analog and digital filters to interface with Arduino
* Detect a 660Hz whistle blow signifying the beginning of your maze mapping 
* Be able to detect a 7kHz IR beacon with an Arduino using the FFT library

Grading Criteria  
2 points: Correct FFT analysis  
2 points: Working amplifier circuit  
3 points: Distinguish a 660Hz tone (from tones at 585Hz and 735Hz)  
2 points: Working IR sensor circuit  
3 points: Distinguish an IR treasure blinking at 7 KHz from one blinking at 12KHz.  
3 points: Nicely merged code  

## Teams
**Team 1 (acoustic)**: Ayomi, Eric, Jacob

**Team 2 (optical)**: Drew, Emily, Joo Yeon


## Tone Detection
TODO

## Treasure Detection

We first assembled the IR circuit according to the suggested schematic for Lab 2, as shown below.

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoTm1OeHpqU1VTWlE" width="200">

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoaGRGRjl3MWpkbjA" width="400">

Since a phototransistor acts as a switch under light, we wanted to see how the brightness of the room would affect the frequency detected by the phototransistor. To do this, we probed the output of the phototransistor with an oscilloscope to examine it's it's voltage. The video below shows voltage fluctuations as we covered the phototransistor with our hand. As seen from the video of the oscilloscope output, there was a bit of noise, so we decided to build a simple amplifier circuit to see if that would provide us a better signal.

<iframe src="https://drive.google.com/file/d/0B1QvEdmy23tjbzR6SC1KUTAwTEU/preview" width="600" height="480"></iframe>



< Insert Schematic of Amplifier Circuit >

< Low Pass/High pass filter>

In order to detect the 7 kHz frequency, we took FFT data using the Arduino's ADC hooked up to a function generator with a 7 kHz sine wave and compared that to the FFT from holding the treasure close to the sensor.  Below are the results of the FFT for both cases.  While slightly less sharp than the function generator, the peak around the 48th bin for the treasure data is still clear.  We can digitally process this data and detect treasures at different frequencies.

<img src="https://docs.google.com/uc?id=0ByCM4xElwbIeQnBmUGZ1UGdiZ1k" height="400">

<img src="https://docs.google.com/uc?id=0ByCM4xElwbIeUjRjSHBSaW90Slk" height="400">




## Work Distribution

*   Ayomi:
*   Drew:
*   Emily:
*   Eric:
*   Jacob:
*   Joo Yeon:
