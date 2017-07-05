#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Constant values
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 32768
#define MAX_LEXI_LEVELS 3
#define MAX_NUMBER_LENGTH 5
#define MAX_IDENTIFIER_LENGTH 11
#define MAX_SYMBOL_TABLE_SIZE 2000

// Globals for input and output files
FILE *VMinFile;
FILE *VMoutFile;
FILE *leXinFile;
FILE *leXoutFile;
FILE *PinFile;
FILE *PoutFile;

// const: kind, name, value         // REMOVE THIS LATER AND PUT STRUCT
// var:   kind, name, L, M
// proc:  kind, name, L, M
typedef struct
{
    int kind;           // const = 1, var = 2, proc = 3
    char name[10];      // name up to 11 characters
    int val;            // number (ASCII value)
    int level;          // L level
    int addr;           // M address
}symbol;

//Internal representation stuff
int nulsym = 1, identsym = 2, numbersym = 3, plussym = 4,
minussym = 5, multsym = 6, slashsym = 7, oddsym = 8, eqlsym = 9,
neqsym = 10, lessym = 11, leqsym = 12, gtrsym = 13, geqsym = 14,
lparentsym = 15, rparentsym = 16, commasym = 17, semicolonsym = 18,
periodsym = 19, becomesym = 20, beginsym = 21, endsym = 22, ifsym = 23,
thensym = 24, whilesym = 25, dosym = 26, callsym = 27, constsym = 28,
varsym = 29, writesym = 31, readsym = 32;

//Internal representation mapping, from integer to string.
char IRMapping[34][64] = {
"ZERO",
"nulsym",
"identsym",
"numbersym",
"plussym",
"minussym",
"multsym",
"slashsym",
"oddsym",
"eqlsym",
"neqsym",
"lessym",
"leqsym",
"gtrsym",
"geqsym",
"lparentsym",
"rparentsym",
"commasym",
"semicolonsym",
"periodsym",
"becomesym",
"beginsym",
"endsym",
"ifsym",
"thensym",
"whilesym",
"dosym",
"callsym",
"constsym",
"varsym",
"?",
"writesym",
"readsym",
"?",

};

//List of symbols allowed
char symbols[] = {'+', '-', '*', '/', '(', ')', '=', ',', '.', '<', '>', ';', ':'};
char reserved[14][32] = {
"const",
"var",
"?",
"call",
"begin",
"end",
"if",
"then",
"?",
"while",
"do",
"read",
"write",
"odd"
};