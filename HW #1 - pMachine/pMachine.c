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
instruction IR;
int stack[MAX_STACK_HEIGHT] = {0};
instruction code[MAX_CODE_LENGTH];
FILE *inFile;
FILE *outFile;

// Function prototypes
void fetch();                          
void execute();                      
int base(int, int);                     
void opNamePrint(int);                  
void printProgram();                     
void printStack(int*, int, int);
int ISA(int, int, int);                 

// An instruction is fetched from the “code” store and placed in the IR register. 
// Afterwards, the PC is incremented by 1 to point to the next instruction to be 
// executed 
void fetch()
{
    IR = code[PC];
    PC = PC + 1;   
}

// Prints out the execution of the program in the virtual machine, showing the 
// stack and registers pc, bp, and sp
void execute()
{
    int i, check = 0;

    fprintf(outFile, "\n\n");
    fprintf(outFile, "\t\t\t\t\t\t PC\t BP\t SP\t Stack\n");
    fprintf(outFile, "Initial Values\t\t\t %d\t %d\t %d\n", PC, BP, SP);
    
    while (check != 1)
    {
        fprintf(outFile, "%d\t ", PC);                           
        opNamePrint(code[PC].OP);                                   
        fprintf(outFile, "%d\t %d\t\t ",code[PC].L, code[PC].M);

        // Checks if instruction is SIO 03, which ends the program
        if ((code[PC].OP == 9) && (code[PC].M == 3))
            check = 1;

        fetch();
        
        ISA(code[PC - 1].OP, code[PC - 1].L, code[PC - 1].M);
        fprintf(outFile, "%d\t %d\t %d\t ", PC, BP, SP);

        printStack(stack, SP, BP);

        fprintf(outFile, "\n");
    }
}

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

//Gets opcode and prints the operation name
void opNamePrint(int op)
{
    switch (op)
    {
        case 1:
            fprintf(outFile,"lit \t");
            break;
        case 2:
            fprintf(outFile,"opr \t");
            break;
        case 3:
            fprintf(outFile,"lod \t");
            break;
        case 4:
            fprintf(outFile,"sto \t");
            break;
        case 5:
            fprintf(outFile,"cal \t");
            break;
        case 6:
            fprintf(outFile,"inc \t");
            break;
        case 7:
            fprintf(outFile,"jmp \t");
            break;
        case 8:
            fprintf(outFile,"jpc \t");
            break;
        case 9:
            fprintf(outFile,"sio \t");
            break;
        default:
            fprintf(outFile,"Not a valid opcode\n");
    }
}

// Prints the output to the screen in interpreted assembly language with line 
// numbers
void printProgram()
{
    int count = 0;

    fprintf(outFile, "Line OP\t\tL\t M\n");

    while (fscanf(inFile, "%d %d %d", &code[count].OP, &code[count].L, &code[count].M) != EOF) 
    {   
        fprintf(outFile, "%d\t ", count);                               // Line
        opNamePrint(code[count].OP);                                    // OP
        fprintf(outFile, "%d\t %d\n",code[count].L, code[count].M);     // L, M                                        
        count++;
    }  
}

// Prints the stack with a bracket '|' to separate each Activation Record
void printStack(int *stack, int SP, int BP)
{
    int i, j;

    if (BP == 1)
    {
        for (i = 1; i <= SP; i++)
            fprintf(outFile, "%d ", stack[i]); 
    }

    if (BP > 1)
    {
        printStack(stack, BP-1, stack[BP+2]);

        // To print the stack when instruction CAL
        if (SP < BP) 
        {
            fprintf(outFile, "| ");
            
            fprintf(outFile, "%d ", stack[SP + 1]);
            fprintf(outFile, "%d ", stack[SP + 2]);
            fprintf(outFile, "%d ", stack[SP + 3]);
            fprintf(outFile, "%d ", stack[SP + 4]);
        }
        else
        {
            fprintf(outFile, "| ");

            for (j = BP; j <= SP; j++)
                fprintf(outFile, "%d ", stack[j]);
        }
    } 
}

// The instruction set architecture
int ISA(int opcode, int L, int M)
{
    int readVal;

    switch(opcode)
    {
        // LIT
        case 1:
            SP = SP + 1;
            stack[SP] = M;
            break;
        // OPR
        case 2:
            switch(M)
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
                    printf("Error. Invalid M for OPR opcode.\n");
            }
            break;
        // LOD
        case 3:
            SP = SP + 1;
            stack[SP] = stack[base(L, BP) + M];
            break;
        // STO
        case 4:
            stack[base(L, BP) + M] = stack[SP];
            SP = SP - 1;
            break;
        // CAL
        case 5:
            stack[SP + 1] = 0;                      // Space to return value
            stack[SP + 2] = base(L, BP);            // Static link (SL)
            stack[SP + 3] = BP;                     // Dynamic link (DL)
            stack[SP + 4] = PC;                     // Return address (RA)
            BP = SP + 1;
            PC = M;
            break;
        // INC
        case 6:
            SP = SP + M;
            break;
        // JMP
        case 7:
            PC = M;
            break;
        // JPC
        case 8:
            if (stack[SP] == 0)
                PC = M;
            SP = SP - 1;
            break;
        // SIO
        case 9:
            if (M == 1)                 //Print what's on the stack
            {
                printf("%d\n", stack[SP]);
                SP = SP - 1;
            }
            else if (M == 2)            // Read input value to stack
            {
                SP = SP + 1;
                printf("Input a value to add onto the stack: ");
                scanf("%d", &readVal);
                stack[SP] = readVal;
            }
            else if (M == 3)            // Exit program
            {
                PC = 0;
                BP = 0;
                SP = 0;
            }
            break;
        default:
            printf("Error. Invalid opcode.\n");
    }
}

int main()
{
    int count;
    inFile = fopen("vminput.txt", "r");
    outFile = fopen("vmoutput.txt", "w");
    
    if (inFile == NULL)
        printf("Couldn't open input file. Make sure it's called 'vminput.txt'\n");
    if (outFile == NULL)
        printf("Couldn't open output file\n");

    printProgram();

    execute();

    fclose(inFile);
    fclose(outFile);

    return 0;
}