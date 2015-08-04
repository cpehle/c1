/* -----\/----- EXCLUDED -----\/-----
interface c1(input logic clk);
   bit req, gnt;
   bit [7:0] addr, data;
   bit [1:0] mode;
   bit       start, rdy;
endinterface: c1
 -----/\----- EXCLUDED -----/\----- */

module c1(
/* -----\/----- EXCLUDED -----\/-----
          inout [63:0]  north,
          input wire    northre,
          input wire    northwe,
          inout [63:0]  south,
          input wire    southre,
          input wire    southwe,
          inout [63:0]  west,
          input wire    westre,
          input wire    westwe,
          inout [63:0]  east,
          input wire    eastre,
          input wire    eastwe,
 -----/\----- EXCLUDED -----/\----- */
          input wire    clk,
          input wire    rst,
          input wire    wen,
          input wire    cen,
          input [63:0]  dat,
          output [63:0] iodataout
          // output [63:0] ioaddress,
          // output        ioread,
          // output        iowrite
);

   typedef enum         bit [3:0] {
                                   IS,
                                   IT,
                                   IPlus,
                                   IAnd,
                                   IOr,
                                   IXor,
                                   INot,
                                   IEq,
                                   ICmp,
                                   ILshift,
                                   IRshift
                                   // few missing
                                   } Aluop;


   typedef struct packed {
      bit         immediate;
      union packed {
         bit [62:0] immediatevalue;
         struct packed {
            Aluop op;
            bit [58:0] rest;
         } d;
      } alu;
   } Instruction;
   //Instruction ins;


/* -----\/----- EXCLUDED -----\/-----

   typedef struct packed {
      MuInstruction A;
      MuInstruction B;
      MuInstruction C;
      MuInstruction D;
   } Instruction;
 -----/\----- EXCLUDED -----/\----- */

   // program counter
   bit [63:0]     pc;
   bit [63:0]     pc_;

   typedef struct packed {
      // bit [31:0][63:0] stk;
      // bit [31:0][4:0]  tag;
      bit [4:0]        sp;
      // bit [63:0]       s; // first element of stack
      //bit [63:0]       t; // second element of stack
   } Stack;

   Stack datastack;
   bit [4:0]           sp_; // need to figure out how to do
   // bit [63:0]          s_;

   // Compute new value of T
/* -----\/----- EXCLUDED -----\/-----
   always_comb begin
      if (ins.immediate) begin

      end
      else begin
         case (ins.alu.op)
           IS: s_ = datastack.s;
           IT: s_ = datastack.t;
           IPlus: s_ = datastack.s + datastack.t;
           IAnd:
             IOr:
             IXor:
             INot:
             IEq:
             ICmp:
             ILshift:
             IRshift:
             default:
         endcase // case (ins.alu.op)
      end
   end
 -----/\----- EXCLUDED -----/\----- */





/* -----\/----- EXCLUDED -----\/-----








   // return stack
   struct packed {
      reg [31:0][63:0] stk;
      reg [4:0]        sp;
      reg [4:0]        sp_;
      reg [63:0]       r;
   } rstack;
   wire                rsW_;
 -----/\----- EXCLUDED -----/\----- */

   always_comb begin
      if (rst) begin
         pc_ = pc;
         sp_ = datastack.sp;
      end else begin
         pc_ = pc + 1;
      end
     end

   always_ff @(posedge clk) begin
      if (rst) begin
         pc <= 0;
         datastack.sp <= 0;
         iodataout <= 0;
      end else begin
         iodataout <= wen ? dat : iodataout + {{63{1'b0}}, cen};
         datastack.sp <= sp_;
         pc <= pc_;
      end
   end
endmodule // c1
