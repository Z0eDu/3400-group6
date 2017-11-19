module DDS_DRIVER(
	RESET,
	CLOCK,
	DAC_OUT
);

	input RESET;
	input CLOCK;
	output [7:0] DAC_OUT;
	
	reg [31:0] counter; 
	
	reg [7:0] OUT;
	
//	always @(posedge CLOCK) begin
//		if (RESET) begin
//			OUT <= 8'b0;
//			counter <= 32'b0;
//		end
//		
//		else begin
//			if (counter == 32'd1000) begin
//				counter <= 32'b0;
//				OUT <= ~OUT;
//			end
//			else counter <= counter + 1;
//		end
//	end

   // increment = 2^32 * f / f_s
	// f_s = 25 MHz
	// increment     f
	// 75591        440 Hz
	// 171799        1 kHz
	// 151182       880 Hz
	// 7576322		 44.1 kHz  (.wav sampling freq)
//	localparam [31:0] increment = 32'd7576322;
	
	reg [7:0] freq_counter;
	wire [31:0] increment;
	reg [31:0] ticks;

	always@(posedge CLOCK) begin
		if (RESET) begin
			counter <= 0;
			ticks <= 0;
			freq_counter <= 0;
		end
		// increment phase accumulator
		else begin
			counter <= counter + increment;
			if (ticks == 32'd25000000) begin
				if (freq_counter == 8'd14) freq_counter <= 8'd0;
				else freq_counter <= freq_counter + 1;
				ticks <= 0;
			end
			else begin 
				freq_counter <= freq_counter;
				ticks <= ticks + 1;
			end
		end
	end
	
	// link the accumulator to the sine lookup table
	sine_rom #(8, 8) sineTable(counter[31:24], CLOCK, DAC_OUT);
	incr_rom #(32, 8) incrRom(freq_counter, CLOCK, increment);

endmodule
