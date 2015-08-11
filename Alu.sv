module Alu(input clk, input [4:0] instruction, input [63:0] a, input [63:0] b, output [63:0] l1r);

   typedef enum [4:0] {
                       Add,
                       Mul,
                       Or,
                       Xor,
                       And
                       } Aluop;

   always_ff @(posedge clk) begin
      case (instruction[4:0])
        Add: l1r <= a + b;
        Mul: l1r <= a * b;
        Or:  l1r <= a | b;
        Xor: l1r <= a ^ b;
        And: l1r <= a & b;
        default: l1r <= 0;
      endcase
   end

endmodule
