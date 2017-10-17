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
`define ORANGE 8'b110_100_00

`define SQUARE_EDGE_0 120
`define SQUARE_EDGE_1 240
`define SQUARE_EDGE_2 360
`define SQUARE_EDGE_3 480
`define SQUARE_EDGE_4 600


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
	reg [4:0] grid_coord_x; //Index x into the array COLUMN
	reg [4:0] grid_coord_y; //Index y into the array ROW
	
	wire [7:0] arduino_in;
	assign arduino_in = {GPIO_1_D[9],GPIO_1_D[11],GPIO_1_D[13],GPIO_1_D[15],GPIO_1_D[17],GPIO_1_D[19],GPIO_1_D[21],GPIO_1_D[23]};
	wire [2:0] arduino_data;
	
	reg  [7:0] vga_ram_write;
	reg  [4:0] vga_ram_raddr;
	wire  [4:0] vga_ram_waddr;
	wire        vga_ram_we;
	wire  [7:0] vga_ram_rsp;
	
	reg [6:0] grid_rel_x;
	reg [6:0] grid_rel_y;
	
	
	assign vga_ram_waddr = arduino_in[4:0];
	assign vga_ram_we = arduino_in[5];
	assign arduino_data = arduino_in[7:6];
	
	always @(*) begin
		case (arduino_data)
			2'd0: vga_ram_write = `GREEN;
			2'd1: vga_ram_write = `RED;
			2'd2: vga_ram_write = `BLUE;
			2'd3: vga_ram_write = `ORANGE;
			default: vga_ram_write = `BLACK;
		endcase
		
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
		
		
		case (grid_coord_x)
			5'd0: grid_rel_x = PIXEL_COORD_X;
			5'd1: grid_rel_x = PIXEL_COORD_X - `SQUARE_EDGE_0;
			5'd2: grid_rel_x = PIXEL_COORD_X - `SQUARE_EDGE_1;
			5'd3: grid_rel_x = PIXEL_COORD_X - `SQUARE_EDGE_2;
			5'd4: grid_rel_x = PIXEL_COORD_X - `SQUARE_EDGE_3;
			default: grid_rel_x = 0;
			
		endcase
		
		
		case (grid_coord_y)
			5'd0: grid_rel_y = PIXEL_COORD_Y;
			5'd1: grid_rel_y = PIXEL_COORD_Y - `SQUARE_EDGE_0;
			5'd2: grid_rel_y = PIXEL_COORD_Y - `SQUARE_EDGE_1;
			5'd3: grid_rel_y = PIXEL_COORD_Y - `SQUARE_EDGE_2;
			default: grid_rel_y = 0;
			
		endcase
		
		
		if      (grid_coord_x == 5'd5)          vga_ram_raddr = 5'd31;
		else if (grid_coord_y == 5'd0)          vga_ram_raddr = grid_coord_x;
		else if (grid_coord_y == 5'd1)          vga_ram_raddr = grid_coord_x + 5;
		else if (grid_coord_y == 5'd2)          vga_ram_raddr = grid_coord_x + 10;
		else if (grid_coord_y == 5'd3)          vga_ram_raddr = grid_coord_x + 15;
		else                                    vga_ram_raddr = 5'd31;
	end
	
	always @(posedge CLOCK_25) begin
		if      (vga_ram_rsp == `GREEN) PIXEL_COLOR <= kirstin_out;
		else if (vga_ram_rsp == `RED)   PIXEL_COLOR <= donald_out;
		else if (vga_ram_rsp == `BLUE) PIXEL_COLOR <= sonic_out;
		else PIXEL_COLOR <= vga_ram_rsp;
	end
	
	VGA_RAM vga_ram (
		.data(vga_ram_write),
	   .read_addr(vga_ram_raddr),
		.write_addr(vga_ram_waddr),
	   .we(vga_ram_we),
		.clk(CLOCK_25),
	   .q(vga_ram_rsp)
	);
	
	wire [7:0] donald_out;
	
	DONALD_DUCK donald_rom (
		.addr({grid_rel_y,grid_rel_x}),
		.clk(CLOCK_25),
		.q(donald_out)
	);
	
	wire [7:0] kirstin_out;
	
	KIRSTIN kirstin (
		.addr({grid_rel_y,grid_rel_x}),
		.clk(CLOCK_25),
		.q(kirstin_out)
	);
	 
	 
	wire [7:0] sonic_out;
	
	SONIC sonic (
		.addr({grid_rel_y,grid_rel_x}),
		.clk(CLOCK_25),
		.q(sonic_out)
	);
	 
	 
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