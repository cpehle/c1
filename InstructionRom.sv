module InstructionRom(input clk, input [7:0] address, output bit [31:0] instruction);
   bit [31:0] memory [255:0];
   initial begin
      $display("Instruction ROM: Reading file.");
      $readmemh("instructions.bin", memory);
   end
   always_ff @(posedge clk) begin
      instruction <= memory[address];
   end
endmodule
