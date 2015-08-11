/* -----\/----- EXCLUDED -----\/-----
interface c1(input clk, input rst);
   bit req, gnt;
   bit [7:0] addr, data;
   bit [1:0] mode;
   bit       start, rdy;
endinterface: c1
 -----/\----- EXCLUDED -----/\----- */

module C1(
          input wire         clk,
          input wire         rst,
          input wire [63:0]  a,
          input wire [63:0]  b,
          output wire [63:0] c
);
   wire [31:0]  /* verilator lint_off UNUSED */ instruction;
   // C1CPU c1cpu(.clk(clk), .rst(rst), )
   InstructionRom instructionrom(.clk(clk), .address(pc[7:0]), .instruction(instruction));

   typedef enum [4:0] {
                       MDefault
                       } Memop;

   typedef struct packed {
      // Aluop op,
      bit         wben;
      Memop memop;
   } Decoderom;

   typedef struct packed {
      Decoderom func;
      bit [4:0]   rs, rt, rd;
      bit [27:0]  address;
      bit [4:0]   shamt;
      bit [5:0]   funct;
   } Decoded;



   // program counter
   bit [63:0]     pc;
   bit [63:0]     pcN;

   //bit [63:0]     l1reg [63:0];
   //bit [63:0]     l3reg [63:0];

   // organise functional units in pipelines which have two inputs and
   // outputs sorted by latency
   Alu alu(.clk(clk), .instruction(instruction[4:0]), .a(a), .b(b), .l1r(c));

   always_comb begin
      if (rst) begin
         pcN = pc;
      end else begin
         pcN = pc + 1;
      end
   end

   always_ff @(posedge clk) begin
      if (rst) begin
         pc <= 0;
      end else begin
         pc <= pcN;
      end
   end
endmodule
