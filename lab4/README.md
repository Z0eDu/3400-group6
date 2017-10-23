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
To begin, we obtained the six hardware materials we would for communication. This was 2 Arduino Unos, two Nordic nRF24L01+ transceivers, and two 2 radio breakout boards. The breakout boards with transceivers were then connected to pins 9-13, ground, and the 3.3V power supply for both arduinos.

For software setup, the RF24 Arduino library was installed and the supplied GettingStarted.ino files were downloaded. Using the formula given on the website, we found our channnel numbers to be 18 and 19, or 12 and 13 in hex. We then modified our channel numbers in getting started as follows:
```cpp
const uint64_t pipes[2] = { 0x0000000012LL, 0x0000000013LL };
```
We then uploaded GettingStarted to both arduinos and, using the serial monitors, set one to transmit and one to recieve to ensure we had communication.

### Sending Entire Maze

After communication was established, we were ready to send the maze. First we initialized an arbitrary 4x5 maze to send.
```cpp
unsigned char maze[4][5] =
{
  3, 3, 3, 3, 3,
  3, 1, 1, 1, 3,
  3, 2, 0, 1, 2,
  3, 1, 3, 1, 3
};
```
On the transmitter side, the code was added to send this data:
```cpp
      bool ok = radio.write( maze, sizeof(maze) );
      
      if (ok)
        printf("ok...");
      else
        printf("failed.\n\r");
```
Then the radio read was added to the receiver:
```cpp
 while (!done) {
        done = radio.read( got_maze, sizeof(got_maze) );
	}
```
Then to make this data readable, a for-loop was created to read and print through all the values in got_maze. Here is the final product.

<iframe src="https://drive.google.com/file/d/1ugxnGpxkmuRaxVPf9SKYErIXR39YX2M9JQ/preview" width="640" height="480"></iframe>


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

### Communication Protocol
We mapped the display for our 4x5 grid by giving each square a number that we could write using the above 8-bit data sent by the radio. Starting in the upper left corner and going across the row, we numbered each square from 0-19 as in the diagram in the radio section, which we wrote in the 5-bit location section of the data. The write enable bit allowed us to effectively clock our updates to the screen rather than interfering previous and preceding messages.  Before we added this, we would get random intermediate squares to be modified, as if we were changing the inputs slowly by hand. We used the color portion of the data to differentiate how we should modify the square at the given location. Each combination of the 2-bit section was mapped to colors, and then later images (see below), that correspond to the state of each square (current position, explored, and unexplored).

We parsed the input from the Arduino according to the protocol as such.  The vga_ram_waddr corresponds to the square to which we are writing in the RAM. The vga_ram_we bit is the write enable bit that we used for clocking.  Both of these are inputted into our VGA_ROM module to write, and later read from, memory. Arduino_data contains the state of the robot as described above.
```verilog
assign vga_ram_waddr = arduino_in[4:0];
assign vga_ram_we = arduino_in[5];
assign arduino_data = arduino_in[7:6];
```

### Verilog

We separated the maze grid into a full 4x5 grid with each edge 120 pixels long. This was done by defining the pixel numbers of the edges and assigning the correct grid coordinate numbers according to the range that the pixel coordinate numbers are in. Using nested if statements, we were able to determine which square the current pixel is in by finding if it was outside the range of each square edge for both the x and y directions. After determining which square edge the pixel was in, we assigned it a decimal value corresponding to its row or column number (zero-aligned).

```verilog
`define SQUARE_EDGE_0 120
`define SQUARE_EDGE_1 240
`define SQUARE_EDGE_2 360
`define SQUARE_EDGE_3 480
`define SQUARE_EDGE_4 600

always @(*) begin
if      (PIXEL_COORD_X < `SQUARE_EDGE_0) grid_coord_x = 5'd0;
		else if (PIXEL_COORD_X < `SQUARE_EDGE_1) grid_coord_x = 5'd1;
		else if (PIXEL_COORD_X < `SQUARE_EDGE_2) grid_coord_x = 5'd2;
		else if (PIXEL_COORD_X < `SQUARE_EDGE_3) grid_coord_x = 5'd3;
		else if (PIXEL_COORD_X < `SQUARE_EDGE_4) grid_coord_x = 5'd4;
		else                                    grid_coord_x = 5'd5;	
		
if      (PIXEL_COORD_Y < `SQUARE_EDGE_0) grid_coord_y = 5'd0;
		else if (PIXEL_COORD_Y < `SQUARE_EDGE_1) grid_coord_y = 5'd1;
		else if (PIXEL_COORD_Y < `SQUARE_EDGE_2) grid_coord_y = 5'd2;
		else if (PIXEL_COORD_Y < `SQUARE_EDGE_3) grid_coord_y = 5'd3;
		else                                    grid_coord_y = 5'd5;
end
```

From the above, we were able to continue to map the coordinates of each pixel to correspond to the square numbers from 0-19. The vga_ram_raddr holds this number, and it is essentially the column number (x coordinate) offset by 5 times the row number (the y coordinate). Here, we use bogus numbers such as 31 if the x or y coordinates are out of range. This allows us to read the state of this square from the RAM so we can color this pixel.
```verilog
		if      (grid_coord_x == 5'd5)          vga_ram_raddr = 5'd31;
		else if (grid_coord_y == 5'd0)          vga_ram_raddr = grid_coord_x;
		else if (grid_coord_y == 5'd1)          vga_ram_raddr = grid_coord_x + 5;
		else if (grid_coord_y == 5'd2)          vga_ram_raddr = grid_coord_x + 10;
		else if (grid_coord_y == 5'd3)          vga_ram_raddr = grid_coord_x + 15;
		else                                    vga_ram_raddr = 5'd31;
```


#### Dual port RAM

We created a simple Dual Port RAM with separate read/write addresses in order to save and read the state of each grid. The write_addr and read_addr indices in the RAM correspond to a square on the grid and thus hold the values 0-19. Here is where we implemented clocking.  We only write to the RAM if the Arduino sends the write enable bit high. This ensures that nothing gets overwritten when we are sending information to the FPGA. Also note that we are able to read on every positive edge of the clock.

```verilog
module VGA_RAM
#(parameter DATA_WIDTH=8, parameter ADDR_WIDTH=5)
(
	input [(DATA_WIDTH-1):0] data,
	input [(ADDR_WIDTH-1):0] read_addr, write_addr,
	input we, clk,
	output reg [(DATA_WIDTH-1):0] q
);

	// Declare the RAM variable
	reg [DATA_WIDTH-1:0] ram[2**ADDR_WIDTH-1:0];

	always @ (posedge clk)
	begin
		// Write
		if (we)
			ram[write_addr] <= data;

		q <= ram[read_addr];
	end

endmodule
```
The data that we write to the RAM corresponds to a color, which indicates the state for a given square.  This vga_ram_write is what we pass into the VGA_RAM module as the data for each square so we can keep track of the states of all squares in memory.
```verilog
always @(*) begin
		case (arduino_data)
			2'd0: vga_ram_write = `GREEN;
			2'd1: vga_ram_write = `RED;
			2'd2: vga_ram_write = `BLUE;
			2'd3: vga_ram_write = `ORANGE;
			default: vga_ram_write = `BLACK;
		endcase
end	
```

The separated arduino signals were connected to the vga_ram (shown here for reference).
```verilog
VGA_RAM vga_ram (
		.data(vga_ram_write),
	   .read_addr(vga_ram_raddr),
		.write_addr(vga_ram_waddr),
	   .we(vga_ram_we),
		.clk(CLOCK_25),
	   .q(vga_ram_rsp)
	);
```

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
