// Kenia Castro
// COP 3402 - Summer 2017
// HW #2 - Parser and Code Generator

#include "header.h"

// Some initial values and global variables
symbol symbol_table[MAX_SYMBOL_TABLE_SIZE];
int tokenArray[MAX_CODE_LENGTH];
int token;
int tokenVal = 0;
int symVal = 0;

// Function prototypes
void program();
void block();
void statement();
void condtition();
void expression();
void term();
void factor();
void getNextToken();
void errorMessage();

void getNextToken()
{
    token = tokenArray[tokenVal];
	tokenVal++;
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

    // Int
    if (token == intsym)
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
    
    // Procedure
    while (token == procsym)                                // where is procsym?
    {
        token = getNextToken();
        if (token != identsym)
            errorMessage(4);            //const, var, procedure must be followed by identifier.
        
        token = getNextToken();
        if (token != semicolonsym)
             errorMessage(5);           // Semicolon or comma missing.

        token = getNextToken();
        block();
        if (token != semicolonsym)
            errorMessage(5);            // Semicolon or comma missing.

        token = getNextToken();
    }

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
        default:
            printf("General Error. Need to make an error message for this.\n");
    }
}

int parser()
{
    inFile = fopen("pInput.txt", "r");
    outFile = fopen("pOutput.txt", "w");
    
    if (inFile == NULL)
        printf("Couldn't open input file. Make sure it's called 'pInput.txt'\n");
    if (outFile == NULL)
        printf("Couldn't open output file\n");

    fclose(inFile);
    fclose(outFile);

    return 0;
}