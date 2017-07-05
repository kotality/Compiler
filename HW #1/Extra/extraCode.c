// Kenia Castro
// COP 3402 - Summer 2017
// HW #1 - pMachine

#include <stdlib.h>
#include <stdio.h>

// Constant values
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVELS 3

// Struct
typedef struct
{
    int OP;
    int L;
    int M;
}instruction;

// Initial values for PM/0 
int SP = 0;  
int BP = 1;  
int PC = 0; 
int IR = 0;
int stack[MAX_STACK_HEIGHT] = {0};
instruction code[MAX_CODE_LENGTH];

// Function prototypes
int ISA();
void print_out(int);
void print_stack();
void fetch(instruction[], count);

// An instruction is fetched from the “code” store and placed in the IR register. 
// Afterwards, the PC is incremented by 1 to point to the next instruction to be 
// executed 
void fetch(instruction code[], int count)
{
    IR = code[PC].M;
    PC = PC + 1; 

    for (int i = 0; i < count; i++)
        ISA(code[i].OP, code[i].M);
}

// The instruction that was fetched is executed by the VM. The OP component that 
// is stored in the IR register (IR.OP) indicates the operation to be executed. 
// For example, if IR.OP is the ISA instruction OPR (IR.OP = 02), then the M 
// component of the instruction in the IR register (IR.M) is used to identify 
// the operator and execute the appropriate arithmetic or logical instruction.
// void execute(instruction code[], int count)
// {
//     // for (i = 0; i < count; i++)
//     // {
//     //     ISA(code[i]);
//     // }
// }

// Find a variable in a different Activation Record some L levels down
int base(int l, int base)
{
    int b1 = base;

    while (l > 0)
    {
        b1 = stack[b1 + 1];
        l--;
    }

    return b1;
}

// Prints the output to the screen in interpreted assembly language with line 
// numbers
void print_out(int count)
{
    int i;

    printf("Line\t OP\t L\t M\n");

    // Prints line numbers
    // for (i = 0; i < num_instr; i++)
    //     lines[i] = ;

}

// Prints out the execution of the program in the virtual machine, showing the 
// stack and registers pc, bp, and sp
// void print_stack()
// {

// }

// The instruction set architecture
int ISA(int opcode, int M)
{
    int readVal;

    switch(opcode)
    {
        // LIT
        case 1:
            SP = SP + 1;
            stack[SP] = code[i].M;
            break;
        // OPR
        case 2:
            switch(code[i].M)
            {
                // RET
                case 0:
                    SP = BP - 1;
                    PC = stack[SP + 4];
                    BP = stack[SP + 3];
                    break;
                // NEG
                case 1:
                    stack[SP] = stack[SP] * -1;
                    break;
                // ADD
                case 2:
                    SP = SP - 1;
                    stack[SP] = stack[SP] + stack[SP + 1];
                    break;
                // SUB
                case 3:
                    SP = SP - 1;
                    stack[SP] = stack[SP] - stack[SP + 1];
                    break;
                // MUL
                case 4:
                    SP = SP - 1;
                    stack[SP] = stack[SP] * stack[SP + 1];
                    break;
                // DIV
                case 5:
                    SP = SP - 1;
                    stack[SP] = stack[SP] / stack[SP + 1];
                    break;
                // ODD
                case 6:
                    stack[SP] = stack[SP] % 2;
                    // or ord(odd(stack[SP]))??
                    break;
                // MOD
                case 7:
                    SP = SP - 1;
                    stack[SP] = stack[SP] % stack[SP + 1];
                    break;
                // EQL
                case 8:
                    SP = SP - 1;
                    if (stack[SP] == stack[SP + 1])
                        stack[SP] = 1;
                    else
                        stack[SP] = 0;
                    break;
                // NEQ
                case 9:
                    SP = SP - 1;
                    if (stack[SP] != stack[SP + 1])
                        stack[SP] = 1;
                    else
                        stack[SP] = 0;
                    break;
                // LSS
                case 10:
                    SP = SP - 1;
                    if (stack[SP] < stack[SP + 1])
                        stack[SP] = 1;
                    else
                        stack[SP] = 0;
                    break;
                // LEQ
                case 11:
                    SP = SP - 1;
                    if (stack[SP] <= stack[SP + 1])
                        stack[SP] = 1;
                    else
                        stack[SP] = 0;
                    break;
                // GTR
                case 12:
                    SP = SP - 1;
                    if (stack[SP] > stack[SP + 1])
                        stack[SP] = 1;
                    else
                        stack[SP] = 0;
                    break;
                // GEQ
                case 13:
                    SP = SP - 1;
                    if (stack[SP] >= stack[SP + 1])
                        stack[SP] = 1;
                    else
                        stack[SP] = 0;
                    break;
                default:
                    printf("Error. Invalid M for OPR\n");
            }
            break;
        // LOD
        case 3:
            SP = SP + 1;
            stack[SP] = stack[base(code[i].L, BP) + code[i].M];
            break;
        // STO
        case 4:
            stack[base(code[i].L, BP) + code[i].M] = stack[SP];
            SP = SP - 1;
            break;
        // CAL
        case 5:
            stack[SP + 1] = 0;                      // Space to return value
            stack[SP + 2] = base(code[i].L, BP);    // Static link (SL)
            stack[SP + 3] = BP;                     // Dynamic link (DL)
            stack[SP + 4] = PC;                     // Return address (RA)
            BP = SP + 1;
            PC = code[i].M;
            break;
        // INC
        case 6:
            SP = SP + code[i].M;
            break;
        // JMP
        case 7:
            PC = code[i].M;
            break;
        // JPC
        case 8:
            if (stack[SP] == 0)
                PC = code[i].M;
            SP = SP - 1;
            break;
        // SIO
        case 9:
            if (code[i].M == 1)                 //Print what's on the stack
            {
                printf("%d", stack[SP]);
                SP = SP - 1;
            }
            else if (code[i].M == 2)            // Read input to on top of stack
            {
                SP = SP + 1;
                printf("Input a value to add into the stack: ");
                scanf("%d", &readVal);
                stack[SP] = readVal;
            }
            else if (code[i].M == 3)            // Exit program
            {
                PC = 0;
                BP = 0;
                SP = 0;
                exit(0);
            }
            break;
        default:
            printf("Error. Invalid Operation\n");
    }
}

int main()
{
    int count = 0;
    FILE *ifp = fopen("vminput.txt", "r");
    
    if (ifp == NULL)
        printf("Error. Couldn't open file. Check that it's called 'vmpinput.txt'\n");

    // while (ifp != EOF)
    // {
    //     fscanf(ifp, "%d %d %d", code[count].OP, code[count].L, code[count].M);
    //     count++;
    // }

    print_out(count);

    return 0;
}

//============================================================
// The instruction that was fetched is executed by the VM. The OP component that 
// is stored in the IR register (IR.OP) indicates the operation to be executed. 
// For example, if IR.OP is the ISA instruction OPR (IR.OP = 02), then the M 
// component of the instruction in the IR register (IR.M) is used to identify 
// the operator and execute the appropriate arithmetic or logical instruction.
// void execute(instruction code[], int count)
// {
    
// }

//=============================================================
//
// fscanf(inFile, "%d %d %d", &code[count].OP, &code[count].L, &code[count].M);

    // Read the input file values for OP, L, M
    // while (!feof(inFile))
    // {
    //     printf("%d %d %d\n", code[count].OP, code[count].L, code[count].M);
    //     fscanf(inFile, "%d %d %d", &code[count].OP, &code[count].L, &code[count].M);
    //     count++;
    // }