// Kenia Castro
// COP 3402 - Summer 2017
// HW #2 - Parser and Code Generator

#include "header.h"

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

// Some initial values and global variables
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
instruction code[MAX_CODE_LENGTH];
int tokenArray[MAX_CODE_LENGTH];
int token;
int tokenVal = 0;
int symVal = 0;
int counter = 0;
int cx = 0;         // code counter
FILE *inFileParser;
FILE *outFileParser;

// Function prototypes
void program();
void block();
void statement();
void condtition();
void expression();
void term();
void factor();
int getNextToken();
void emit(int, int, int);
void errorMessage();

int getNextToken()
{
    int token;

    token = tokenArray[tokenVal];
	tokenVal++;

    return token;
}

void program()
{
    token = getNextToken();
    block();

    // Period
    if (token != periodsym)
        errorMessage(9);    // Period expected
}

void block()            
{
    // Const
    if (token == constsym)
    {
        while (token == commasym)
        {
            token = getNextToken();
            if (token != identsym)
                errorMessage(4);        // const, var, procedure must be followed by identifier
            
            token = getNextToken();
            if (token != eqlsym)
                errorMessage(3);        // Identifier must be followed by =
            
            token = getNextToken();
            if (token != numbersym) 
                errorMessage(2);        // = must be followed by a number

            token = getNextToken();
        }

        if (token != semicolonsym)
            errorMessage(5);            // Semicolon or comma missing
        
        token = getNextToken();
    }

    // Int should read as var??? <-------------
    if (token == varsym)
    {
        while (token == commasym)
        {
            token = getNextToken();
            if (token != identsym)
                errorMessage(4);        // const, var, procedure must be followed by identifier
            
            token = getNextToken();
        }

        if (token != semicolonsym)
            errorMessage(5);            // Semicolon or comma missing
        
        token = getNextToken();
    }
    
    // Procedure should read as???
    // while (token == procsym)                                // where is procsym?
    // {
    //     token = getNextToken();
    //     if (token != identsym)
    //         errorMessage(4);            //const, var, procedure must be followed by identifier.
        
    //     token = getNextToken();
    //     if (token != semicolonsym)
    //          errorMessage(5);           // Semicolon or comma missing.

    //     token = getNextToken();
    //     block();
    //     if (token != semicolonsym)
    //         errorMessage(5);            // Semicolon or comma missing.

    //     token = getNextToken();
    // }

    statement(); 
}

void statement()        
{
    // COMMENT COMMENT
    if (token == identsym)
    {
        token = getNextToken();
        if (token != becomesym)
            errorMessage(19);           // Incorrect symbol following statement

        token = getNextToken();
        expression(); 
    }
    // Call
    else if (token == callsym)
    {
        token = getNextToken();
        if (token != identsym)
            errorMessage(14);           // call must be followed by an identifier

        token = getNextToken();
    }
    // Begin 
    else if (token == beginsym)
    {
        token = getNextToken();
        statement();

        while (token == semicolonsym)
        {
            token = getNextToken();
            statement();
        }

        if (token != endsym)
            errorMessage(26);           // end expected

        token = getNextToken();
    }
    // If
    else if(token == ifsym)
    {
        token = getNextToken();
        condtition();

        if (token != thensym)
            errorMessage(16);           // then expected

        token = getNextToken();
        statement();
    }
    // While
    else if(token == whilesym)
    {
        token = getNextToken();
        condtition();

        if (token != dosym)
            errorMessage(18);           // do expected
        
        token = getNextToken();
        statement();
    }
}

void condtition()
{
    // Odd
    if (token == oddsym)
    {
        token = getNextToken();
        expression();
    }
    else
    {
        expression();

        if ((token != eqlsym) && (token != neqsym) && (token != lessym) && 
            (token != leqsym) && (token != gtrsym) && (token != geqsym))
            errorMessage(20);           // Relational operator expected

        token = getNextToken();
        expression();
    }
}

void expression()
{
    // Plus and Minus
    if (token == plussym || token == minussym)
        token = getNextToken();

    term();

    while (token == plussym || token == minussym)
    {
        token = getNextToken();
        term();
    }
}

void term()
{
    factor();

    // Multiply and Divide
    while (token == multsym || token == slashsym)
    {
        token = getNextToken();
        factor();
    }
}

void factor()
{
    if (token == identsym)
        token = getNextToken();
    else if (token = numbersym)
        token = getNextToken();
    else if (token == lparentsym)
    {
        token = getNextToken();
        expression();

        if (token != rparentsym)
            errorMessage(22);           // Right parenthesis missing
        
        token = getNextToken();
    }
    else
        errorMessage(27);               // Invalid factor
}

void emit(int op, int l, int m)
{
    if (cx > MAX_CODE_LENGTH)
        errorMessage(28);
    else
    {
        code[cx].OP = op; 	    // opcode
        code[cx].L = l;	        // lex level
        code[cx].M = m;         // modifier
        cx++;
    }
}

//Error messages for the PL/0 Parser
void errorMessage(int error)
{
    switch(error)
    {
        case 1:
            printf("Use = instead of :=.\n");
            break;
        case 2:
            printf("= must be followed by a number.\n");
            break;
        case 3:
            printf("Identifier must be followed by =.\n");
            break;
        case 4:
            printf("const, var, procedure must be followed by identifier.\n");
            break;
        case 5:
            printf("Semicolon or comma missing.\n");
            break;
        case 6:
            printf("Incorrect symbol after procedure declaration.\n");
            break;
        case 7:
            printf("Statement expected.\n");
            break;
        case 8:
            printf("Incorrect symbol after statement part in block.\n");
            break;
        case 9:
            printf("Period expected.\n");
            break;
        case 10:
            printf("Semicolon between statements missing.\n");
            break;
        case 11:
            printf("Undeclared identifier.\n");
            break;
        case 12:
            printf("Assignment to constant or procedure is not allowed.\n");
            break;
        case 13:
            printf("Assignment operator expected.\n");
            break;
        case 14:
            printf("call must be followed by an identifier.\n");
            break;
        case 15:
            printf("Call of a constant or variable is meaningless.\n");
            break;
        case 16:
            printf("then expected.\n");
            break;
        case 17:
            printf("Semicolon or } expected.\n");
        case 18:
            printf("do expected.\n");
            break;
        case 19:
            printf("Incorrect symbol following statement.\n");
            break;
        case 20:
            printf("Relational operator expected.\n");
            break;
        case 21:
            printf("Expression must not contain a procedure identifier.\n");
            break;
        case 22:
            printf("Right parenthesis missing.\n");
            break;
        case 23: 
            printf("The preceding factor cannot begin with this symbol.\n");
            break;
        case 24:
            printf("An expression cannot begin with this symbol.\n");
            break;
        case 25: 
            printf("This number is too large.\n");
            break;
        case 26:
            printf("end expected.\n");
            break;
        case 27:
            printf("Invalid factor.\n");
            break;
        case 28:
            printf("Problem with code generation overflow. \n");
            break;
        default:
            printf("General Error. Need to make an error message for this.\n");
    }
}

int parser(int flag)
{
    int value;

    inFileParser = fopen("count.txt", "r");
    outFileParser = fopen("parseOutput.txt", "w");
    
    if (inFile == NULL)
        printf("Couldn't open input file. Make sure it's called 'input.txt'\n");
    if (outFile == NULL)
        printf("Couldn't open output file\n");

    while (fscanf(inFileParser, "%d", &value) != EOF)
    {
        tokenArray[counter] = value;
        counter++;
    }

    program();

    fclose(inFile);
    fclose(outFile);

    return 0;
}