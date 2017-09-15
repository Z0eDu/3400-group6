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
[insert schematic]
[insert image]

Since the phototransistor is sensitive to light, we wanted to see how the brightness of the room would affect the frequency detected by the phototransistor. To do this, we probed the output with an oscilloscope to examine it's output. The video below shows magnitude fluctuations as we covered the phototransistor with our hand. We decided that that there was enough noise, so we decided to build a simple amplifier circuit to see if that would provide us a better signal.
[insert video]
[insert schematic]

< Amplifier Circuit >

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
