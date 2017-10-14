# Lab 3: FPGA Video Controller and Sound Generation

### Objectives
* Take at least two external inputs to the FPGA and display them on a screen
* React to an external input to the FPGA of their choice and generate a short ‘tune’ consisting of at least three tones to a speaker via an 8-bit DAC

#### Grading Criteria
1 point: Reading external inputs to FPGA  
1 point: Correctly updating a 4-bit array dependent on the inputs  
1 point: Drawing one box on the screen  
1 point: Description of how the DAC on the provided VGA connectors works and how the resistor values were chosen.  
4 points: Mapping external inputs to four different outputs on the screen  
1 point: Connecting the FPGA output to the speaker  
2 points: Outputting a square wave to the speaker  
4 points: Outputting a tune of at least three different frequencies to the speaker via an 8-bit DAC  

## Teams
**Team 1 (Graphics)**: Ayomi, Emily, Eric

**Team 2 (Acoustic)**: Drew, Jacob, Joo Yeon


## Graphics
### Setup

For our setup, we had to connect the FPGA to the monitor. To do this, we used a VGA connector that connects to GPIO pins xxx on the FPGA, and then a VGA cable to connect the FPGA to the VGA switch. The VGA switch allows us to go between the computer and the FPGA input. 

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoaTVfRHVFYTdCUU0" width="800">

Since the VGA cable is composed of three analog wires and the FPGA produces a 3.3V digital output, a resistive 8-bit Digital-to-Analog Converter (DAC) was provided. Using the DAC, the 8-bits for each color signal (3 red, 3 green, 2 blue) from the FPGA are converted into the three different RGB color 0-1V analog signals. 

The resistor values chosen for the VGA connector are based upon the R-2R DAC resistor network which directly converts the  parallel FPGA digital signal into an analog output. 

<img src="https://sub.allaboutcircuits.com/images/quiz/03999x01.png">

(Credit: https://www.allaboutcircuits.com/worksheets/digital-to-analog-conversion/)

For red and green, since the signals are 3 bits wide, the resistor values chosen were approximately 270 Ohm, 550 Ohm, and 1200 Ohm. This is because 1200 Ohm is approximately twice 550 Ohm, and 550 Ohm is approximately twice 270 Ohm. Likewise, for the blue, since the signal is 2 bits wide, the respective values are 270 Ohm and 550 Ohm.


### Simple Drawings

The first thing we wanted to work on after setup was to draw a simple image. From the template code for the VGA driver, we figured that the two synchronizing clock signals HSYNC and VYSNC would drive the output on the screen and output the x and y coordinates for the next pixel. The color of the corresponding pixel would be determined by the three RGB analog signals. 

The block diagram below demonstrates how the FPGA code interacts with the VGA display. 

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwoQlpTX3ZOUmg2eHM" width="600">

(Credit: ECE 3400 course website)

Using what we had learned, we were able to split our screen in half and set them as two different colors, and also draw a basic square in the top left corner of the screen as shown. 

<img src="https://docs.google.com/uc?id=0B4-ue266N8b0b25pRmlQeW1Pc0k" width="350">   <img src="https://docs.google.com/uc?id=0B4-ue266N8b0N0psVlBvb2o2dU0" width="350">

### Memory System for Blocks in Grid

Once we were able to draw a single square, we then needed to implement an array in order to store color values for the inclusion of multiple squares. For this, the 2x2 8-bit `grid_array` was used to store 4 different colors. Instead of using if-statements to determine which color in `grid_array` to select, we used the binary properties of the X and Y coordinates. The 7th index of our position variables is 0 within 0-127, 1 within 128-255, 0 again within 256-383, and so on . These binary values were to selected indexes in `grid_array`, assigning the color at each pixel and creating a repeating pattern of 128x128 pixel squares. 

```verilog
PIXEL_COLOR <= grid_array[PIXEL_COORD_X[7]][PIXEL_COORD_Y[7]];
```

Later, a conditional statment so that values larger than the 255 would be black, therefore eliminating the repeating pattern. The result image can be seen below.

<img src="https://docs.google.com/uc?id=0B4-ue266N8b0VVVFcEhKWEFMNGs" width="350">   <img src="https://docs.google.com/uc?id=0B4-ue266N8b0N0psVlBvb2o2dU0" width="350">


### Communcation method between Arduino and FPGA

### Voltage Divider


## Acoustics

### Sine Table Generation

We generated a table of a single period of a sine wave, with 256 points ranging from 0 to 255 using python:

```python
for i in range(256):
    print(hex(int(127*sin(2*pi*i / 256) + 128))[2:].zfill(2))

```

We saved the output to a file, which synthesized into a Verilog ROM:
```verilog
module sine_rom
#(parameter DATA_WIDTH=8, parameter ADDR_WIDTH=8)
(
	input [(ADDR_WIDTH-1):0] addr,
	input clk,
	output reg [(DATA_WIDTH-1):0] q
);

	// Declare the ROM variable
	reg [DATA_WIDTH-1:0] rom[2**ADDR_WIDTH-1:0];

	// Initialize the ROM with $readmemb.  Put the memory contents
	// in the file single_port_rom_init.txt.  Without this file,
	// this design will not compile.

	// See Verilog LRM 1364-2001 Section 17.2.8 for details on the
	// format of this file, or see the "Using $readmemb and $readmemh"
	// template later in this section.

	initial
	begin
		$readmemh("sin256.txt", rom);
	end

	always @ (posedge clk)
	begin
		q <= rom[addr];
	end

endmodule
```

### Scale Generation

Once we got pure sine generation working, we added the capability to play arbitrary frequencies in sequence. For that, we used another python script:

```
#! /usr/bin/env python3

import sys

if __name__ == '__main__':
    with open(sys.argv[1]) as f:
        for line in f.readlines():
            print(hex(int(2**32*float(line)/25e6))[2:].zfill(8))
```

We ran this script on an input file consisting of the frequencies of the notes in a C scale, and it produced a file containing the DDS increments in hex. Then, we synthesized that into another Verilog ROM, just as above. The DDS module then incremented the index into the increment ROM every second, causing the output frequencies to change to the desired frequencies. With this, we played a C scale:

<iframe src="https://drive.google.com/file/d/0B1QvEdmy23tjRHNLdEx6V1U1RFk/preview" width="640" height="480"></iframe>

### Playing Arbitrary Songs

While we didn't have time for this lab, we can easily generalize the scale approach above to play (at significantly reduced quality) any song. To do so, take the song, and break the samples into groups of some fixed length, probably 0.1 s. Take the Fourier transform of each section, and select the dominant 8 frequencies. Assemble these into a file with 1 frequency per line (8 lines of frequencies that occur at the same time, then the next 8, and so on). Run the increment generation script on the file. Then, modify the Verilog code to instantiate 8 DDS modules, each one reading from a different initial index in the increment file, and incrementing the index by 8 each each 0.1 s. Average the outputs from all 8 (which is not as not that bad: sign extend all to 11 bits, add them, and then bit shift the result right by 3 bits). Output this to the DAC.


## Work Distribution

*   Ayomi:
*   Drew:
*   Emily:
*   Eric:
*   Jacob:
*   Joo Yeon:
