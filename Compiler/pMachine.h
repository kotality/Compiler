// Kenia Castro
// COP 3402 - Summer 2017
// HW #1 - pMachine

#include "header.h"

// Initial values for PM/0
int SP = 0;  
int BP = 1;  
int PC = 0; 
int out = 0;
instruction IR;
int stack[MAX_STACK_HEIGHT] = {0};
int stackOut[MAX_STACK_HEIGHT] = {0};
instruction code[MAX_CODE_LENGTH];
FILE *inFile;
FILE *outFile;

// Function prototypes
void fetch();                          
void execute(int);                      
int base(int, int);                     
void opNamePrint(int, int);                  
void printProgram(int);                     
void printStack(int*, int, int, int);
int ISA(int, int, int, int);                 

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
void execute(int flag)
{
    int i, check = 0;

    if (flag == 1)
    {
        printf("\n\n");
        printf("\t\t\t\t\t\t PC\t BP\t SP\t Stack\n");
        printf("Initial Values\t\t\t %d\t %d\t %d\n", PC, BP, SP);
    }
    
        fprintf(outFile, "\n\n");
        fprintf(outFile, "\t\t\t\t\t\t PC\t BP\t SP\t Stack\n");
        fprintf(outFile, "Initial Values\t\t\t %d\t %d\t %d\n", PC, BP, SP);
    
    while (check != 1)
    {
        if (flag == 1)
            printf("%d\t ", PC);   
        fprintf(outFile, "%d\t ", PC); 

        opNamePrint(code[PC].OP, flag);   

        if (flag == 1)                                
            printf("%d\t %d\t\t ",code[PC].L, code[PC].M);                                  
        fprintf(outFile, "%d\t %d\t\t ",code[PC].L, code[PC].M);

        // Checks if instruction is SIO 03, which ends the program
        if ((code[PC].OP == 9) && (code[PC].M == 3))
            check = 1;

        fetch();
        
        ISA(code[PC - 1].OP, code[PC - 1].L, code[PC - 1].M, flag);

        if (flag == 1)
            printf("%d\t %d\t %d\t ", PC, BP, SP);
        fprintf(outFile, "%d\t %d\t %d\t ", PC, BP, SP);

        printStack(stack, SP, BP, flag);

        if (flag == 1)
            printf("\n");
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
void opNamePrint(int op, int flag)
{
    switch (op)
    {
        case 1:
            if (flag == 1)
                printf("lit \t");
            fprintf(outFile,"lit \t");
            break;
        case 2:
            if (flag == 1)
                printf("opr \t");
            fprintf(outFile,"opr \t");
            break;
        case 3:
            if (flag == 1)
                printf("lod \t");
            fprintf(outFile,"lod \t");
            break;
        case 4:
            if (flag == 1)
                printf("sto \t");
            fprintf(outFile,"sto \t");
            break;
        case 5:
            if (flag == 1)
                printf("cal \t");
            fprintf(outFile,"cal \t");
            break;
        case 6:
            if (flag == 1)
                printf("inc \t");
            fprintf(outFile,"inc \t");
            break;
        case 7:
            if (flag == 1)
                printf("jmp \t");
            fprintf(outFile,"jmp \t");
            break;
        case 8:
            if (flag == 1)
                printf("jpc \t");
            fprintf(outFile,"jpc \t");
            break;
        case 9:
            if (flag == 1)
                printf("sio \t");
            fprintf(outFile,"sio \t");
            break;
        default:
            if (flag == 1)
                printf("Not a valid opcode\n");
            fprintf(outFile,"Not a valid opcode\n");
    }
}

// Prints the output to the screen in interpreted assembly language with line 
// numbers
void printProgram(int flag)
{
    int count = 0;

    if (flag == 1)
        printf("Line OP\t\tL\t M\n");
    fprintf(outFile, "Line OP\t\tL\t M\n");

    while (fscanf(inFile, "%d %d %d", &code[count].OP, &code[count].L, &code[count].M) != EOF) 
    {   
        if (flag == 1)
            printf("%d\t ", count);  
        fprintf(outFile, "%d\t ", count);  

        opNamePrint(code[count].OP, flag);  

        if (flag == 1)                     
            printf("%d\t %d\n",code[count].L, code[count].M);                                               
        fprintf(outFile, "%d\t %d\n",code[count].L, code[count].M);

        count++;
    }
}

// Prints the stack with a bracket '|' to separate each Activation Record
void printStack(int *stack, int SP, int BP, int flag)
{
    int i, j;
    
    if (BP == 1)
    {
        for (i = 1; i <= SP; i++)
        {
            if (flag == 1)
                printf("%d ", stack[i]);
            fprintf(outFile, "%d ", stack[i]); 
        }
            
    }

    if (BP > 1)
    {
        printStack(stack, BP-1, stack[BP+2], flag);

        // To print the stack when instruction CAL
        if (SP < BP) 
        {
            if (flag == 1)
            {
                printf("| ");
            
                printf("%d ", stack[SP + 1]);
                printf("%d ", stack[SP + 2]);
                printf("%d ", stack[SP + 3]);
                printf("%d ", stack[SP + 4]);
            }
            
            fprintf(outFile, "| ");
            
            fprintf(outFile, "%d ", stack[SP + 1]);
            fprintf(outFile, "%d ", stack[SP + 2]);
            fprintf(outFile, "%d ", stack[SP + 3]);
            fprintf(outFile, "%d ", stack[SP + 4]);
        }
        else
        {
            if (flag == 1)
                printf("| ");
            fprintf(outFile, "| ");

            for (j = BP; j <= SP; j++)
            {
                if (flag == 1)
                    printf("%d ", stack[j]);
                fprintf(outFile, "%d ", stack[j]);
            }      
        }
    } 
}

// The instruction set architecture
int ISA(int opcode, int L, int M, int flag)
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
                stackOut[out] = stack[SP];
                SP = SP - 1;
                out++;
            }
            else if (M == 2)            // Read input value to stack
            {
                SP = SP + 1;
                printf("\nInsert input: ");
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

int pMachine(int flag)
{
    int count, i;
    inFile = fopen("parseOutput.txt", "r");
    outFile = fopen("vmoutput.txt", "w");
    
    if (inFile == NULL)
        printf("Couldn't open input file. Make sure it's called 'parseOutput.txt'\n");
    if (outFile == NULL)
        printf("Couldn't open output file\n");

    // printf("Flag: %d\n", flag);

    if (flag == 1)
    {
        printf("============================================================================== \n");
        printf("|                               pMachine Output                              | \n");
        printf("============================================================================== \n");
    }

    printProgram(flag); 

    execute(flag);

    for (i = 0; i < out; i++)
        printf("Stack output: %d\n", stackOut[i]);

    fclose(inFile);
    fclose(outFile);

    return 0;
}