#ifndef variables
#define variables

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constant values
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 32768
#define MAX_LEXI_LEVELS 3
#define MAX_NUMBER_LENGTH 5
#define MAX_IDENTIFIER_LENGTH 15
#define MAX_SYMBOL_TABLE_SIZE 2000

int errorFlag;      // To know if hit an error

//Internal representation stuff
int nulsym = 1, identsym = 2, numbersym = 3, plussym = 4,
minussym = 5, multsym = 6, slashsym = 7, oddsym = 8, eqlsym = 9,
neqsym = 10, lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14,
lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
periodsym = 19, becomesym = 20, beginsym = 21, endsym = 22, ifsym = 23,
thensym = 24, whilesym = 25, dosym = 26, /*callsym = 27,*/ constsym = 28,
varsym = 29, writesym = 31, readsym = 32;

// Struct pMachine
typedef struct
{
    int OP;
    int L;
    int M;
}instruction;

// Data structure for symbol
// const: kind, name, value
// var:   kind, name, L, M
typedef struct
{
    int kind;                               // const = 1, var = 2, proc = 3
    char name[MAX_IDENTIFIER_LENGTH];       // name up to 11 characters
    int val;                                // number (ASCII value)
    int level;                              // L level
    int addr;                               // M address
}symbol;

// Token struct
typedef struct 
{
    int type;
    char name[MAX_IDENTIFIER_LENGTH];
    char symbol[MAX_IDENTIFIER_LENGTH];
}tokenType;

#endif
