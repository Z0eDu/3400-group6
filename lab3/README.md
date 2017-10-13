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

### Simple Drawings

### Memory System for Blocks in Grid

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
