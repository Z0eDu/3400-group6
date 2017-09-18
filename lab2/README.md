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

Amplifier values:  
peak to peak: 20 mV  
Ri = 1.18k  
Rf = 118k  
Gain = Rf/Ri = 100 gain => output should be ~2 V  



Low pass filter:

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwockktY3V2UmtnMDA" width="400">


High pass filter:

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoRWRibUhjSHk4NXM" width="400">

Band pass filter

<img src="https://docs.google.com/uc?id=0B1QvEdmy23tjNlY3YzFfelppQVU" width="400">


#### Code and Results
Our code was modified from the sample code to use the Arduino's internal ADC.  We used the internal ADC rather than analogRead() because the latter has a maximum sampling frequency of about 9 kHz, which is not nearly enough to sample even 7 kHz signals.  

We set up the ADC like so:
```cpp
  TIMSK0 = 0; // turn off timer0 for lower jitter - delay() and millis() killed
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
```
Then we took 512 samples from the output of pin A0 to be processed.  We had to manipulate the output of the ADC, which returns two bytes of data in two separate variables, to be a 16 bit signed integer.  We took each sample and placed the real data into the even bins of the fft_input[] array, setting the odd (imaginary) bins to 0.
```cpp
  fft_input[i] = k; // put real data into even bins
  fft_input[i+1] = 0; // set odd bins to 0
```
Finally, we called fft functions from the FFT library to reorder the data to be processed, run the data, and then take the log of the magnitudes for each bin to facilitate analysis.
```cpp
  fft_window(); // window the data for better frequency response
  fft_reorder(); // reorder the data before doing the fft
  fft_run(); // process the data in the fft
  fft_mag_log(); // take the output of the fft
```
For now, we just displayed all of the data points for analysis and graphed them as shown below.  For later parts of the lab, we will process the data, eliminating peaks far away from where we want to look and only check for the treasure frequencies we need to detect.

Below are the graphs for a 7 kHz sine wave from a function generator and for the 7 kHz treasure held close to the sensor.  While slightly less sharp than the function generator, the peak around the 48th bin for the treasure data is still clear.  We can digitally process this data and detect treasures at different frequencies.

<img src="https://docs.google.com/uc?id=0ByCM4xElwbIeQnBmUGZ1UGdiZ1k" height="400">

<img src="https://docs.google.com/uc?id=0ByCM4xElwbIeUjRjSHBSaW90Slk" height="400">




## Work Distribution

*   Ayomi: Soldering header pins onto microphone breakout board, audio FFT analysis and plots, threshold detection
*   Drew: Code and results for treasure frequencies
*   Emily: Treasure detection circuit assembly and initial testing
*   Eric: Audio ISR code, threshold detection code
*   Jacob: Audio ISR code
*   Joo Yeon: Amplifier and filtering circuits for phototransistor output
