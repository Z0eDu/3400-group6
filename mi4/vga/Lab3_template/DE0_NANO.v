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
`define PURPLE 8'b111_000_11
`define GREEN 8'b000_111_00
`define BLUE 8'b000_000_11
`define ORANGE 8'b110_100_00
`define WHITE 8'b111_111_11
`define CYAN 8'b000_111_11

`define SQUARE_EDGE_0 120
`define SQUARE_EDGE_1 240
`define SQUARE_EDGE_2 360
`define SQUARE_EDGE_3 480
`define SQUARE_EDGE_4 600
`define WALL_SIZE 20

`define TREASURE_MAX 70
`define TREASURE_MIN 50


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
	 
	 reg done;

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

	wire [8:0] arduino_in;
	//                  |DATA                                  |CLK          | ADDR                                                          |
	assign arduino_in = {GPIO_1_D[25],GPIO_1_D[23],GPIO_1_D[21], GPIO_1_D[19], GPIO_1_D[17],GPIO_1_D[15],GPIO_1_D[13],GPIO_1_D[11],GPIO_1_D[9]};
	//                  |7           | 6           | 5         | 4           | 3           | 2          | A0         | A1         | A2
	wire [2:0] arduino_data;
	assign arduino_data = arduino_in[8:6];
  wire [4:0] arduino_waddr = arduino_in[4:0];
  assign arduino_waddr = arduino_in[4:0];
  wire       arduino_clock = arduino_in[5];

  reg  [2:0] input_pos;
  reg [4:0] last_arduino_waddr;
  
  reg  [8:0] vga_ram_write;
  reg  [8:0] temp_vga_ram_write;
  
  always @(posedge arduino_clock) begin
	 if (arduino_waddr == 5'b11111) input_pos <= 3'd4;
	 else if (input_pos == 3'd4) input_pos <= 3'd0;
	 else input_pos <= input_pos + 3'b1;
	 
	 if (arduino_waddr == 5'd30) done <= 1'b1;
	 else if (arduino_waddr == 5'd31) done <=done;
	 else done <= 1'b0;
	 
	 last_arduino_waddr <= arduino_waddr;
	 vga_ram_write <= temp_vga_ram_write;
  end

	reg  [4:0] vga_ram_raddr;
	wire [4:0] vga_ram_waddr;
	wire       vga_ram_we;
	wire [8:0] vga_ram_rsp;

	reg [6:0] grid_rel_x;
	reg [6:0] grid_rel_y;

  assign vga_ram_we = (input_pos == 3'd3);
  assign vga_ram_waddr = last_arduino_waddr;

  reg [7:0] base_color;
  reg mark_wall;
  reg [7:0] treasure_color;

	always @(*) begin
		case (input_pos)
			3'd0: temp_vga_ram_write = {vga_ram_write[8:6], vga_ram_write[5:3] , arduino_data};
			3'd1: temp_vga_ram_write = {vga_ram_write[8:6], arduino_data       , vga_ram_write[2:0]};
			3'd2: temp_vga_ram_write = {arduino_data,       vga_ram_write[5:3] , vga_ram_write[2:0]};
      default: temp_vga_ram_write = vga_ram_write;
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

    case (vga_ram_rsp[8:6])
      3'd0: base_color = `BLACK; // unvisited
      3'd1: base_color = `CYAN; // visited
      3'd2: base_color = `ORANGE; // unreachable
      3'd3: base_color = `PURPLE; //robot ^
      3'd4: base_color = `PURPLE; //robot >
      3'd5: base_color = `PURPLE; //robot v
      3'd6: base_color = `PURPLE; //robot <
      default: base_color = `ORANGE;
    endcase

    if ((grid_rel_y > `SQUARE_EDGE_0 - `WALL_SIZE) && vga_ram_rsp[5]) mark_wall = 1'b1;
    else if ((grid_rel_x > `SQUARE_EDGE_0 - `WALL_SIZE) && vga_ram_rsp[4]) mark_wall = 1'b1;
    else if ((grid_rel_y < `WALL_SIZE) && vga_ram_rsp[3]) mark_wall = 1'b1;
    else if ((grid_rel_x < `WALL_SIZE) && vga_ram_rsp[2]) mark_wall = 1'b1;
    else mark_wall = 1'b0;

    if ((grid_rel_x > `TREASURE_MIN) && (grid_rel_x < `TREASURE_MAX) && (grid_rel_y > `TREASURE_MIN) && (grid_rel_y < `TREASURE_MAX)) begin
    case (vga_ram_rsp[1:0])
      2'd1: treasure_color = `RED; // 7 kHz
      2'd2: treasure_color = `GREEN; // 12 kHz
      2'd3: treasure_color = `BLUE; // 17 kHz
      default: treasure_color = `BLACK;
    endcase
    end else treasure_color = `BLACK;

	 if (mark_wall) PIXEL_COLOR = `WHITE;
    else if (treasure_color != `BLACK) PIXEL_COLOR = treasure_color;
    else PIXEL_COLOR = base_color;
	 
	end

	//always @(posedge CLOCK_25) begin
//	end

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

	// GND
	assign GPIO_1_D[24] = 1'b0;
	
   DDS_DRIVER dds(
		  .RESET(~done),
		  .CLOCK(CLOCK_25),
		  .DAC_OUT({GPIO_1_D[22], GPIO_1_D[20], GPIO_1_D[18], GPIO_1_D[16], GPIO_1_D[14], GPIO_1_D[12], GPIO_1_D[10], GPIO_1_D[08]})
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
