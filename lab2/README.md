# Lab 2: Digital Signal Processing

## Objectives
* Stuff  

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
TODO




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
