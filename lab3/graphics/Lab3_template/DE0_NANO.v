//=======================================================
// ECE3400 Fall 2017
// Lab 3: Template top-level module
//
// Top-level skeleton from Terasic
// Modified by Claire Chen for ECE3400 Fall 2017
//=======================================================

`define ONE_SEC 25000000


`define BLACK 8'b000_000_00
`define RED 8'b111_000_00
`define GREEN 8'b000_111_00
`define BLUE 8'b000_000_11


module DE0_NANO(

	//////////// CLOCK //////////
	CLOCK_50,

	//////////// LED //////////
	LED,

	//////////// KEY //////////
	KEY,

	//////////// SW //////////
	SW,

	//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	GPIO_0_D,
	GPIO_0_IN,

	//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	GPIO_1_D,
	GPIO_1_IN,
);

	 //=======================================================
	 //  PARAMETER declarations
	 //=======================================================

	 localparam ONE_SEC = 25000000; // one second in 25MHz clock cycles
	 
	 //=======================================================
	 //  PORT declarations
	 //=======================================================

	 //////////// CLOCK //////////
	 input 		          		CLOCK_50;

	 //////////// LED //////////
	 output		     [7:0]		LED;

	 /////////// KEY //////////
	 input 		     [1:0]		KEY;

	 //////////// SW //////////
	 input 		     [3:0]		SW;

	 //////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_0_D;
	 input 		     [1:0]		GPIO_0_IN;

	 //////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
	 inout 		    [33:0]		GPIO_1_D;
	 input 		     [1:0]		GPIO_1_IN;

    //=======================================================
    //  REG/WIRE declarations
    //=======================================================
    reg         CLOCK_25;
    wire        reset; // active high reset signal 

    wire [9:0]  PIXEL_COORD_X; // current x-coord from VGA driver
    wire [9:0]  PIXEL_COORD_Y; // current y-coord from VGA driver
    reg  [7:0]  PIXEL_COLOR;   // input 8-bit pixel color for current coords
	 
	 reg [24:0] led_counter; // timer to keep track of when to toggle LED
	 reg 			led_state;   // 1 is on, 0 is off
	 
    // Module outputs coordinates of next pixel to be written onto screen
    VGA_DRIVER driver(
		  .RESET(reset),
        .CLOCK(CLOCK_25),
        .PIXEL_COLOR_IN(PIXEL_COLOR),
        .PIXEL_X(PIXEL_COORD_X),
        .PIXEL_Y(PIXEL_COORD_Y),
        .PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
        .H_SYNC_NEG(GPIO_0_D[7]),
        .V_SYNC_NEG(GPIO_0_D[5])
    );
	 
	 assign reset = ~KEY[0]; // reset when KEY0 is pressed
	 
	 //assign PIXEL_COLOR = 8'b000_111_00; // Green
	 //assign LED[0] = led_state;
	 
	 /*
	 Main_Module dope (
		.PIXEL_X(PIXEL_COORD_X),
		.PIXEL_Y(PIXEL_COORD_Y),
		.PIXEL_COLOR_OUT(PIXEL_COLOR)
	 );
	 */
	 
	 
	// LAB 3 CODE
	
	//2-by-2 array of bits
	reg  [7:0] grid_array [1:0][1:0]; //[rows][columns]
	wire [1:0] grid_coord_x; //Index x into the array
	wire [1:0] grid_coord_y; //Index y into the array

	// current highlighted square
	wire highlighted_x;
	wire highlighted_y;	 
	 
	//Switch input through GPIO pins
	assign highlighted_x = GPIO_1_D[33];
	assign highlighted_y = GPIO_1_D[31];
	
	// LED mappings
	assign LED[0] = grid_array[0][0];
   assign LED[1] = grid_array[0][1];
   assign LED[2] = grid_array[1][0];
   assign LED[3] = grid_array[1][1];	
	
	
	// Draw square
	always @ (*) begin 
			// hard code array colors
		
		grid_array[0][0] <= `GREEN;
		grid_array[0][1] <= `GREEN;
		grid_array[1][0] <= `GREEN;
		grid_array[1][1] <= `GREEN;
		if (PIXEL_COORD_X[9:8] ==2'b0 && PIXEL_COORD_Y[9:8] == 2'b0) begin
			if (PIXEL_COORD_X[7] == highlighted_x && PIXEL_COORD_Y[7] == highlighted_y) begin
				PIXEL_COLOR <= `RED;
			end
			else begin
				PIXEL_COLOR <= grid_array[PIXEL_COORD_X[7]][PIXEL_COORD_Y[7]];		 
			end
		end
		else begin
			PIXEL_COLOR <= `BLACK; 
		end
		
	end	 
	 
	 
	 
    //=======================================================
    //  Structural coding
    //=======================================================
 
	 // Generate 25MHz clock for VGA, FPGA has 50 MHz clock
    always @ (posedge CLOCK_50) begin
        CLOCK_25 <= ~CLOCK_25; 
    end // always @ (posedge CLOCK_50)
	 
/*	
	 // Simple state machine to toggle LED0 every one second
	 always @ (posedge CLOCK_25) begin
		  if (reset) begin
				led_state   <= 1'b0;
				led_counter <= 25'b0;
		  end
		  
		  if (led_counter == ONE_SEC) begin
				led_state   <= ~led_state;
				led_counter <= 25'b0;
		  end
		  else begin	
				led_state   <= led_state;
				led_counter <= led_counter + 25'b1;
		  end // always @ (posedge CLOCK_25)
	 end
*/	 

endmodule
