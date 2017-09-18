# Lab 2: Digital Signal Processing

### Objectives
* Learn about Open Music Labs FFT
* Detect a 660Hz whistle blow signifying the beginning of your maze mapping
* Be able to detect a 7kHz IR beacon with an Arduino using the FFT library

<!-- Grading Criteria  
2 points: Correct FFT analysis  
2 points: Working amplifier circuit  
3 points: Distinguish a 660Hz tone (from tones at 585Hz and 735Hz)  
2 points: Working IR sensor circuit  
3 points: Distinguish an IR treasure blinking at 7 KHz from one blinking at 12KHz.  
3 points: Nicely merged code   -->

## Teams
**Team 1 (acoustic)**: Ayomi, Eric, Jacob

**Team 2 (optical)**: Drew, Emily, Joo Yeon


## Tone Detection

### Circuit
We connected the microphone power and ground to +5 V and GND on the Arduino, respectively, and attached the signal line to A0. The Adafruit Electret Microphone Amplifier breakout board (product ID 1063) we used already had an amplifier on it, so there was no need to further amplify the signal.

### FFT

Team Alpha and the instructions spent lots of time experimenting with wrapping `analogRead` in a loop and measuring the sampling frequency by playing known tones and checking which bins of the FFT picked them up. This seemed like a complicated and time consuming process. Instead, we configured an 8 kHz interrupt. The interrupt fired at exactly 8 kHz, so we knew the exact sampling frequency. We chose 8kHz to satisfy the Nyquist sampling condition, which requires the sampling frequency to be double the highest frequency in our system, which would be human voice at 4 kHz.

```cpp
cli();
// put your setup code here, to run once:
TCCR2A = 0;
TCCR2B = 0;
TCNT2  = 0; // Counter starts at 0
// compare: arduino clock frequency / (desired frequency * prescaler) - 11
OCR2A = 249;// = (16*10^6) / (8000*8) - 1 (must be <256)
// Clear Timer On Compare (CTC mode)
TCCR2A |= (1 << WGM21);
// Set CS21 bit for 8 prescaler
TCCR2B |= (1 << CS21);   
// Enable timer compare interrupt
TIMSK2 |= (1 << OCIE2A);

sei();
```

We toggled a digital output with the ISR to make sure it was working properly, and got a square wave at 4 kHz as expected (because it ran at 8 kHz and changed state each time):

<img src = "https://docs.google.com/uc?id=0B-CRHiExsEzBNXZPZzZ4RmxrRmM">

We tested our code to see if it could initially pick up a 660 Hz tone using a function generator and a tone generator. We processed the FFT by converting bins to frequencies.  We printed out “start”, so we could easily read the serial monitor, and take out the bins to convert them. Using excel, we converted the bins to frequencies. The formula we used was bins * 8000/ 256. We used that data to create two separate graphs for Function generator(left) and tone generator(right)

<img src = "https://docs.google.com/uc?id=0B4-ue266N8b0LVRKV1k4MWE3Mms"><img src = "https://docs.google.com/uc?id=0B4-ue266N8b0S0RjRDdyYXFlemM">

We found the spike were between bins 20 and 22, which would be frequencies 625 and 687.5. We changed our code so that we would keep track if we are hearing a tone in that range. We estimated the tone should occur for 15 data collection and FFT cycles before the robot started moving. In this case we just lit up the LED on the Arduino.

<iframe src="https://drive.google.com/file/d/0B4-ue266N8b0TWV5cG9sXzRNX0U/preview" width="640" height="480"></iframe>


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

We tried amplifier and filter circuits to cut out noise and make the treasure signal clear.
The schematics, photos, and specifics are included here.

Amplifier circuit:

<img src="https://docs.google.com/uc?id=0B1QvEdmy23tjQ0NpRjJIeXhZWm8" width="400">   <img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoVndobUdDX0lza3M" width="400">

peak to peak: 20 mV  
Ri = 1.18k  
Rf = 118k  
Gain = Rf/Ri = 100 gain => output should be ~2 V  


Low pass filter:

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwockktY3V2UmtnMDA" width="400">  <img src="https://docs.google.com/uc?id=0B1QvEdmy23tjbTM4LVp4ZkdXVEk" width="400">


High pass filter:

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoRWRibUhjSHk4NXM" width="400">


Band pass filter

<img src="https://docs.google.com/uc?id=0B1QvEdmy23tjNlY3YzFfelppQVU" width="400">


The amplifier amplified the overall response. However, this made the distinction between the treasure signal and noise smaller. Each filter dampened the desired range of frequency, but filters also dampened the treasure signal somewhat. Additional circuits made the peak unclear, so we decided to process the data software wise.


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
