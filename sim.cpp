#include "Vc1.h"
#include "verilated.h"
#include "verilated_vcd_c.h"
#include <fstream>

vluint64_t main_time = 0;

typedef enum {
        Add = 0,
        Mul,
        Or,
        Xor,
        And
} Aluop;

int main(int argc, char ** argv) {
        int i;
        int clk;

        Aluop op[] = {Add, Mul, Or, Xor, And};
        unsigned int instructions[255];
        for (int i = 0; i < 255; i++) {
                instructions[i] = op[i%5];
        }

        std::fstream instructionrom("instructions.bin", std::ios::out);
        for (int i = 0; i < 255; i++) {
                instructionrom << op[i%5] << std::endl;
        }

        Verilated::commandArgs(argc, argv);
        VC1* top = new VC1("top");
        Verilated::traceEverOn(true);
        VerilatedVcdC* tfp = new VerilatedVcdC;
        top->trace(tfp, 99);
        tfp->open("c1.vcd");
        top->clk = 1;
        top->rst = 1;
        top->a = 3;
        top->b = 5;

        for (i=0; i<20; i++) {
                top->rst = (i < 2);
                // top->op = op[i%5];
                // dump variables into VCD file and toggle clock
                for (clk=0; clk<2; clk++) {
                        tfp->dump (2*i+clk);
                        top->clk = !top->clk;
                        top->eval ();
                }
                if (Verilated::gotFinish())  exit(0);
        }

        tfp->close();
        instructionrom.close();
        exit(0);
}
