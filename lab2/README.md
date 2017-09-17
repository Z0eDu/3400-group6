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

#### Assembling and Initial Testing
We first assembled the IR circuit according to the suggested schematic for Lab 2, as shown below.

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoTm1OeHpqU1VTWlE" width="300"><img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoaGRGRjl3MWpkbjA" width="500">


We then adjusted the potentiometer on the treasure to make it flash at 7kHz, and measured the frequency output with an oscilloscope. We were able to tune the treasure to flash at around 7.04 kHz, as shown by the square wave on the oscilloscope.

<img src="https://docs.google.com/uc?id=0B1QvEdmy23tjN0JxWFpabXpPckk" width="460">


The next step was placing the treasure near our circuit to see if the phototransistor would be able to detect the frequency of the treasure. The image below shows us having to place the treasure extremely close to the phototransistor to get any sort of transient waveform on the oscilloscope. 

<img src="https://docs.google.com/uc?id=0B1QvEdmy23tjbHBrX3FEOTBjWkU" width="400">     <img src="https://docs.google.com/uc?id=0B1QvEdmy23tjNHF1N3p4b001X3M" width="400">

Since there was so much noise, we wanted to see how unwanted signals such as ambient light in the room was affecting the phototransistor. As a phototransistor acts as a switch under light, we wanted to see how the brightness of the room would affect the detected frequency. To do this, we probed the output of the phototransistor with an oscilloscope to examine it's it's voltage. The video below shows voltage fluctuations as we covered the phototransistor with our hand. As seen from the video of the oscilloscope output, there was a bit of noise, so we decided to build a simple amplifier circuit and add filtering to see if that would provide us a better signal.

<iframe src="https://drive.google.com/file/d/0B1QvEdmy23tjbzR6SC1KUTAwTEU/preview" width="500" height="400"></iframe>

#### Amplifier and Filters
< Insert Schematic of Amplifier Circuit >


Low pass filter:

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwockktY3V2UmtnMDA" width="400">


High pass filter:

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoRWRibUhjSHk4NXM" width="400">


#### Code and Results
In order to detect the 7 kHz frequency, we took FFT data using the Arduino's ADC hooked up to a function generator with a 7 kHz sine wave and compared that to the FFT from holding the treasure close to the sensor. Below are the results of the FFT for both cases.  While slightly less sharp than the function generator, the peak around the 48th bin for the treasure data is still clear.  We can digitally process this data and detect treasures at different frequencies.

<img src="https://docs.google.com/uc?id=0ByCM4xElwbIeQnBmUGZ1UGdiZ1k" height="400">

<img src="https://docs.google.com/uc?id=0ByCM4xElwbIeUjRjSHBSaW90Slk" height="400">




## Work Distribution

*   Ayomi:
*   Drew: Code and results for treasure frequencies
*   Emily: Treasure detection circuit assembly and initial testing 
*   Eric:
*   Jacob:
*   Joo Yeon: Amplifier and filtering circuits for phototransistor output
