# Milestone 4:

## Treasure Hardware

While the simple treasure detection circuit from Lab 2 worked well, it was only good for detecting the various treasure frequencies at very small distance ranges. Thus, the simple circuit made finding treasures in the maze while the robot was moving much more difficult. As a result, we have added a non-inverting op-amp to to better detect the treasures at farther distances with greater accuracy. 

<img src="https://docs.google.com/uc?id=1OHDrc9akSwBA_-C12_yFAmcMwubLBZNh"  width="400">

We first prototyped our circuit on a breadboard with a gain of 10, and tested to see that it worked well with the range of frequencies (7 kHz, 12 kHz, 17 kHz). Once our testing confirmed that the hardware was robust, we then protoboarded three treasure detection circuits. The idea is that each of the treasure detection circuit will be responsible for detecting a specific frequency. An ISR will toggle between the three treasure detection input pins on the Arduino. Using the FFT library, the robot will then be able to send a signal to the base station if a treasure at a certain frequency is detected. The breadboard assembly and final non-inverting ampilifier breakouts for the treasure detection circuitry is shown below. 

<img src="https://docs.google.com/uc?id=1CS5IXJnBlW2MbFpP2Np_VRaIfRsvI7GE"  width="400">


## Treasure Software

For milestone 2, we have implemented a singel treasure detector that detected a treasure and identified the frequency. For the competition, the robot should be able to detect a treasure from any wall around it. Therefore, we decided to have three treasure sensors: on the front, right, and left side. We used ADC0, ADC2, and ADC4 for the sensors, and we had to switch between them. In order to do so, We set the corresponding bits in the Adc setup and added another ISR that switched the ADCMUX that selects which ADC to use. 

```cpp
void fastAdcSetup(int pin) {
  DIDR0 = 0x3F; // digital inputs disabled
  ADMUX = 0x40; // measuring on ADC0, use the internal 1.1 reference
  ADCSRA = 0xAC; // AD-converter on, interrupt enabled, prescaler = 16
  ADCSRB = 0x40; // AD channels MUX on, free running mode
  bitWrite(ADCSRA, 6, 1); // Start the conversion by setting bit 6 (=ADSC) in ADCSRA
  sei(); // set interrupt flag
}
```

```cpp
ISR(ADC_vect) {
  int port;
    if (ADMUX == 0x40) {
      port = 0;
      ADMUX = 0x41;
    }else if(ADMUX == 0x41) {
      port = 1;
      ADMUX = 0x42;
    }else if(ADMUX == 0x42) {
       port = 2;
       ADMUX = 0x44;
    } 
     
    int val = ADCL; // store lower byte ADC
    val += ADCH << 8; // store higher bytes ADC
   
}
```
For integration, we had to reduce the memory of the code and deal with the timer. The arduino ran out of memory when we first added the code. We got rid of all the serial codes and the serial library and it fixed the problem. When we simply added the code, the robot never moved because the arduino was always dealing with the timer interrupt. Therefore we had to disable the specific timer's interrupt in the setup and enable it in the intersection. We also had to enable the timer compare interrupt in the timer ISR and then disable at the end of it. We used Timer2 for FFT since enabling and disabling Timer0 affected other functionalities of the robot.


## Base Station

For this assignment, we had to modify the base station to display walls and treasures, and to indicate when the robot is done. For the walls, we modified the FPGA code to display white bars along the sides of the squares. We represented treasures as smaller squares displayed in the center of each grid square, and used different colors to distinguish the types of treasures. For the done signal, we wanted a way to signal that the robot was done, without removing the solved maze from the screen. The simplest way to do that was to change the wall color from white to purple when the robot finished. Once the robot finished, we also enabled our sound module from milestone 3, which caused it to play a C scale.

Here is a picture of the base station displaying every possible type of square, with all the possible treasures, and all the possible wall arrangements:

<img src="https://docs.google.com/uc?id=1Z-kXtSD_p57yaE_aZIT3XiW-nkVjkkhb"  width="400">

For squares, we used the following color scheme:
* Black: unvisited
* Green: visited
* Blue: current position
* Red: unreachable

For treasures, we used the following color scheme:
* 7 kHz: orange
* 12 kHz: white
* 17 kHz: purple

For walls, we used the following color scheme:
* White: robot still exploring the maze
* Purple: robot done, maze final

## Radio Transmission

To implement radio transmission, code was scavanged from Lab 4. A method which utilizes the necessary radio writing process was placed into a new method, transmit:
```cpp
void transmit(unsigned short state){
      // First, stop listening so we can talk.
   radio.stopListening();

    bool update; 
    update = false; 
    while (!update) {
    update = radio.write( &state, sizeof(state) );
    };
  }```
  
  This method could now be called to transmit an unsigned short with information about each aspect of an intersection including treasures, walls, location, and its state. Altogether this would require 14 bits. This is how they are sent.
  
  ill finish this soon

## Demo

Here is a video of the final system that displays walls/treasures in a maze as the robot finds them, and signals on screen/plays the done sound when the maze is fully mapped: 


<iframe src="https://drive.google.com/file/d/1jSv64BG5JyUz64KameMMnVCxGJmFDhmi/preview" width="640" height="480"></iframe>


## Work Distribution

*   Ayomi: Fixing mux and sensors
*   Drew: Fixing mux and sensors
*   Emily: Treasure detection hardware and software
*   Eric: Radio code
*   Jacob: Base station FPGA code
*   Joo Yeon: Treasure detection hardware and software
