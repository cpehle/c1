#include <sys/mman.h>
#include <immintrin.h>
#include <iostream>

typedef unsigned int u32;

typedef enum {
        Nop = 0,
        Id,
        Right,
        Left,
        Braid,
        Delta,
        Add,
        Sub,
        Mul,
        Xor,
        And,
        Or,
        Max,
        Min,
        ShiftL,
        ShiftR,
        Load,
        Store,
        Cond,
        Jmp,
        Call,
        Ret,
        Halt
} Op;


void parallelprefix(u32* mem) {
        for (int i = 0; i < 16; ++i) {
                *mem++ = Add;
                *mem++ = Id;
        }
        for (int i = 0; i < 8; ++i) {
                *mem++ = Id;
                *mem++ = Braid;
                *mem++ = Nop;
                *mem++ = Id;
        }
        for (int i = 0; i < 8; ++i) {
                *mem++ = Add;
                *mem++ = Id;
                *mem++ = Id;
                *mem++ = Id;
        }
        for (int j = 3; j > 0; --j) {
                for (int i = 0; i < 4; ++i) {
                        mem[(j-3)%8] = Id;
                        mem[(j-2)%8] = Id;
                        mem[(j-1)%8] = Id;
                        mem[j] = Braid;
                        mem[j+1] = Nop;
                        mem[j+2] = Id;
                        mem[j+3] = Id;
                        mem[j+4] = Id;
                        mem  += 8;
                }
        }

}

int main() {

        u32* a = (u32 *)mmap(NULL, 1000*32*sizeof(u32), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);;
        u32* b = (u32 *)mmap(NULL, 1000*32*sizeof(u32), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);;

        u32* mem = (u32 *)mmap(NULL, 1000*32*sizeof(u32), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANON, -1, 0);

        memset(a, 0, 1000*32*sizeof(u32));
        memset(b, 0, 1000*32*sizeof(u32));
        memset(mem, 0, 1000*32*sizeof(u32));
        u32 pc = 0;


        for (int i = 0; i < 32; ++i) {
                a[i] = 500*32+i;
                mem[i] = Load;
                mem[500*32+i] = 1;
        }
        parallelprefix(mem + 32);

        u32 tmp;
        for (;;) {
                std::cin >> tmp;
                u32* t;

                for (int i = 0; i < 32; ++i) {
                        std::cout << a[i] << " ";
                }
                std::cout << std::endl;
                for (int i = 0; i < 32; ++i) {
                        std::cout << b[i] << " ";
                }
                std::cout << std::endl;
                if (mem[pc] == Halt) {
                        std::cout << "Halt.";
                        break;
                }

                for (u32 i = 0; i < 32; ++i) {
                        switch (mem[pc + i]) {
                        case Nop: {
                                break;
                        }
                        case Id: {
                                b[i] = a[i];
                                break;
                        }
                        case Right: {
                                b[(i+1)%32] = a[i];
                                break;
                        }
                        case Left: {
                                b[(i-1)%32] = a[i];
                                break;
                        }
                        case Braid: {
                                b[(i+1)%32] = a[i];
                                b[i] = a[(i+1)%32];
                                break;
                        }
                        case Delta: {
                                b[i] = a[i];
                                b[(i+1)] = a[i];
                        }
                        case Add: {
                                b[i] = a[i] + a[(i+1)%32];
                                break;
                        }
                        case Sub: {
                                b[i] = a[i] - a[(i+1)%32];
                                break;
                        }
                        case Mul: {
                                b[i] = a[i] * a[(i+1)%32];
                                break;
                        }
                        case Xor: {
                                b[i] = a[i] ^ a[(i+1)%32];
                                break;
                        }
                        case And: {
                                b[i] = a[i] & a[(i+1)%32];
                                break;
                        }
                        case Or: {
                                b[i] = a[i] | a[(i+1)%32];
                                break;
                        }
                        case ShiftL: {
                                b[i] = a[i] << a[(i+1)%32];
                                break;
                        }
                        case ShiftR: {
                                b[i] = a[i] >> a[i+1];
                                break;
                        }
                        case Max: {
                                b[i] = a[i] > a[(i+1)%32] ? a[i] : a[(i+1)%32];
                                break;
                        }
                        case Min: {
                                b[i] = a[i] > a[(i+1)%32] ? a[(i+1)%32] : a[i];
                                break;
                        }
                        case Load: {
                                if (a[i] < 1000*32) {
                                        b[i] = mem[a[i]];
                                } else {
                                        b[i] = 0;
                                }
                                break;
                        }
                        case Store: {
                                if (a[i] < 1000*32) {
                                        mem[a[i]] = a[(i+1)%32];
                                }
                                break;
                        }
                        case Jmp: {
                                b[i] = pc;
                                pc = (pc + 32*a[i]) % 500*32;
                                break;
                        }
                        case Call: {
                                b[i] = pc;
                                pc = (pc + 32*a[i]) % 500*32;
                                const u32 n = a[(i+1)%32];
                                b[(i+1)%32] = a[(i+1)%32];
                                for (int j = 0; j < n; ++j) {
                                        b[(i+2+j)%32] = a[(i+2+j)%32];
                                }
                                break;
                        }
                        case Ret: {
                                const u32 n = a[i];
                                for (int j = 0; j < n; ++j) {
                                        b[i+j] = a[(i+1+j)%32];
                                }
                                break;
                        }
                        }
                }
                t = a;
                a = b;
                b = t;
                pc += 32;
        }
}
