# Lab 4: Radio Communication and Map Drawing

### Objectives
* Extend work from Lab 3 to use the FPGA draw a full maze on VGA monitor
* Update VGA monitor with information received from radio
* Get robot and video controller to talk to each other to simulate actual maze information


#### Grading
2 points: Sending information wirelessly between Arduino’s  
2 points: Sending the entire maze wirelessly  
3 points: Updating the maze array, dependent only on the updated robot information  
1 point: Displaying a full 4-by-5 grid array on the screen  
3 points: Communicating maze information from the Arduino to the FPGA  
2 points: Display the robot location on the screen  
2 points: Distinguish what sites have been visited and which haven’t on the screen  

## Teams
**Team 1 (Radio)**: Ayomi, Emily, Eric

**Team 2 (FPGA)**: Drew, Jacob, Joo Yeon


## Radio

### Setup

### Sending Entire Maze

### Sending New Information Only

First, We create a 8-bit unsigned char then created two int values one called location and the other called state. We set new_data equal to the pos left-shifted the three bits and OR-ed with the state. This way the most significant 5-bits are the position, and the last 3 are the state.

```cpp
unsigned char new_data;
unsigned char pos = 4;
unsigned char state = 0;
int start = 0;

new_data =  pos << 3 | state;
```

We changed the new radio.write so that it sends the new data.

```cpp
    printf("Now sending new map data\n");
    bool update = radio.write( &new_data, sizeof(unsigned char) );

     if (update)
      printf("ok update...");
    else
      printf("failed update.\n\r");
```

Lastly, we changed the while loop, so that it would send the new information, and make the changes to the maze. Using the information we got from got_data, we figured out the location and changed that positon in the maze. Since location was only a single value, we broke it up to represent the row and column. We set the maze position equal to the decimal value of position, to confirm that it was receiving the right information.

 ```cpp
 doneU = radio.read( &got_data, sizeof(unsigned char) );

 unsigned char location = got_data >> 3;

 int y = location / 5;
 int x = location % 5;

 printf("Location: ");
 printf("%d" , location );
 printf("\n");

 got_maze[y][x] = location;
 ```

Here is what the serial monitor of the receiver printed out.

<img src="https://docs.google.com/uc?id=0B4-ue266N8b0ODRVeDNCaTBEN0E"  width="640" height="480">

Here is a video showing that we were sending the information wirelessly.

<iframe src="https://drive.google.com/file/d/18rVIUlMtZgDl6QTX3NCYy8VCqUEpkdbZGw/preview" width="640" height="480"></iframe>


### Sending Robot Position

Instead of sending (x,y) coordinates to determine the position of the robot in the 4 x 5 maze, we decided that we would send integer positions to denote robot position. Each square of the maze will be numbered as follows:

<img src="https://docs.google.com/uc?id=0B0-yVGdr0Ewod3hvaVg2UWw5X1E" width="300">

Each message that Arduino A transmits to Arduino B is a 8-bit unsigned char that denotes robot position, state, and a clock bit. The 8-bit message is broken down as follows. The lower two least-significant bits differentiates between three states: current robot position, explored, and unexplored -- with decimal values 0, 1, and 2 respectively. The third bit is a clock line that we lower as the Arduino is writing all the bits to the FPGA, and then brought high once all the bits are written. The least most-significant bits indicate the position of the grid, with decimal values 0 to 19 as shown in our grid above.

<img src="https://docs.google.com/uc?id=0B0-yVGdr0EwocnlMbHItclEwR1k" width="600">

In communicating to the FPGA, we first set 8 output pins on the Arduino, one for each bit in the message. The idea is such that there is a separate line for each bit and the FPGA will read if the bit is either a 0 or 1 and update the map/robot position and states accordingly. Since digital pin 0 and 1 are used for serial communication on the Arduino, we mapped bit 0 and bit 1 of our message to digital pin 8, and analog pin 1 respectively.

```cpp

 // Output pin setup
  pinMode(8, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

```

#### Transmit Code  

Two nested for loops are used to send our updated robot position. In this example, we iterate through the entire 4 x 5 maze and have the robot visit each grid. The message containing the location and state is packed so it can be sent in a single payload. Since the five most-significant bits contain the location, it is left-shifted three bits and then OR-ed with the state (```new_data =  i << 3 | state```, where ```i``` is the location).

```cpp
    unsigned char new_data;
    unsigned char pos;
    unsigned char state;
    bool update;

  for (int j = 0; j < 20; j++) {
    for (int i = 0; i < 20; i++) {

      if (i == j){
        state = 0; // current robot state
      } else if (i < j) {
        state = 1; // explored
      } else {
        state = 2; // unexplored
      }

      new_data =  i << 3 | state;
      printf("Now sending new robot update\n");

      update = false;
      while (!update) {
      update = radio.write( &new_data, sizeof(unsigned char) );
      Serial.println(update);
      }

    }
    delay(100);
  }
  ```


#### Receive Code  
On the receive end, there is a continuos loop in which the radio checks to see if there is a payload that was sent. The data received is stored in ```got_data```, and then each bit is read using ```bitRead(got_data, x)```, where ```x``` is the respective bit. For each bit, we check to see if the bit is a 0 or a 1, and then write to the pin output so it can be read by the FPGA using a ternary operator.

Since the third bit of our message is used as the clock line, we write the clock bit to be low, write all the outputs to the FPGA, and then raise the clock bit high.


```cpp
    // if there is data ready
    if ( radio.available() )
    {
      unsigned char got_data;
      bool doneU = false;

      while (!doneU){
        // Fetch the payload, and see if this was the last one.
        doneU = radio.read( &got_data, sizeof(unsigned char) );
        printf("%d", got_data);

        // Clock low
        digitalWrite(2, LOW);

        // State bits
        bitRead(got_data, 0) ? digitalWrite(8, HIGH) : digitalWrite(8, LOW);
        bitRead(got_data, 1) ? digitalWrite(A1, HIGH) : digitalWrite(A1, LOW);
        // Location bits
        bitRead(got_data, 3) ? digitalWrite(3, HIGH) : digitalWrite(3, LOW);
        bitRead(got_data, 4) ? digitalWrite(4, HIGH) : digitalWrite(4, LOW);
        bitRead(got_data, 5) ? digitalWrite(5, HIGH) : digitalWrite(5, LOW);
        bitRead(got_data, 6) ? digitalWrite(6, HIGH) : digitalWrite(6, LOW);
        bitRead(got_data, 7) ? digitalWrite(7, HIGH) : digitalWrite(7, LOW);

        // Clock high
        digitalWrite(2, HIGH);

      }
```


## FPGA

### Verilog

#### Dual port RAM

### Communication Protocol

### Displaying Images

We added support for displaying arbitrary images be storing image data in a Verilog single port ROM, and reading the data out of the ROM to display it. Each square on our 4 x 5 grid was 120 x 120 pixels, so we used a ROM with 14 bit wide address bus (7 bits for each x and y gives us up to 128 for each coordinate) and an 8 bit wide data bus (colors are 8 bits). We wrote a python script in order to generate the data file which was used to build the Verilog ROM. The script used Pillow, a fork of the Python Image Library (PIL), to manipulate an input image. The script resized the input image to 120 x 120, converted each pixel to 8 bit color, and then outputted each pixel's value in hex. We took the output file, and used it to build an the ROM using the following Verilog code:

```verilog
module DONALD_DUCK
#(parameter DATA_WIDTH=8, parameter ADDR_WIDTH=14)
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
		$readmemh("donaldduck.txt", rom);
	end

	always @ (posedge clk)
	begin
		q <= rom[addr];
	end

endmodule
```

See [`img_to_rom.py`](https://github.com/sunemily/3400-group6/blob/master/lab4/vga/img_to_rom.py) for the python script.

We were able to display Donald Duck, Sonic the Hedgehog (not pictured), and Kirstin Petersen:

<img src="https://docs.google.com/uc?id=0B1r9QYTd8YNrZllET0hCXzhmT00" width="300">

<img src="https://docs.google.com/uc?id=0B1r9QYTd8YNrVWJXNVo0NVVQNkk" width="300">


## Final Result

<iframe src="https://drive.google.com/file/d/0B1r9QYTd8YNrd3luMFFmTWxhdTg/preview" width="640" height="480"></iframe>

Kirstin represents the robot as it explores the map, Sonic represents the unexplored areas, and Donald represents explored areas.


## Work Distribution

*   Ayomi:
*   Drew: Verilog code, voltage dividers
*   Emily:
*   Eric:
*   Jacob: Verilog code, python scripts
*   Joo Yeon: Verilog code
