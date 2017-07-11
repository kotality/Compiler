// Kenia Castro
// COP 3402 - Summer 2017
// HW #2 - Parser and Code Generator

#include "header.h"

// Data structure for symbol
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
int relOp = 0;
int cx = 0;             // code counter
FILE *inFile;
FILE *outFile;

// Function prototypes
void program();
void block();
void statement();
void condtition();
void expression();
void term();
void factor();
int getNextToken();
void printTokens(int, int);
void emit(int, int, int);
void errorMessage();

// Gets the next token <-------------- Not DONE
int getNextToken()
{
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
    {
        errorMessage(9);    // Period expected
        printf(" - PROCEDURE\n");
    }
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

            // store in the symbol table and increment the counter
			symbol_table[symVal] = newConst;
			symbol_table[symVal].addr = stackCounter;
            symbol_table[symVal].level= currentLevel;
			symVal++;
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
    
    // while (token == procsym)
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
    // ident expression
    if (token == identsym)
    {
        token = getNextToken();
        if (token != becomesym)
            errorMessage(19);           // Incorrect symbol following statement

        token = getNextToken();
        expression(); 
    }
    // Call ------- not in EBNF
    // else if (token == callsym)
    // {
    //     token = getNextToken();
    //     if (token != identsym)
    //         errorMessage(14);           // call must be followed by an identifier

    //     token = getNextToken();
    // }
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
    else if (token == ifsym)
    {
        token = getNextToken();
        condtition();

        if (token != thensym)
            errorMessage(16);           // then expected

        token = getNextToken();
        statement();
    }
    // While
    else if (token == whilesym)
    {
        token = getNextToken();
        condtition();

        if (token != dosym)
            errorMessage(18);           // do expected
        
        token = getNextToken();
        statement();
    }
    // Read
    else if (token == readsym)
    {
        token = getNextToken();

        if (token != identsym)
            errorMessage(14);            // call must be followed by an identifier
        
		token = getNextToken();
        
		// emit(9, 0,2);
        // token = getNextToken();
    }
    // Write
    else if (token == writesym)
    {
        token = getNextToken();

        if (token != identsym)
            errorMessage(14);            // call must be followed by an identifier
        
        token = getNextToken();

        // emit(9,0,1);
		// token = getNextToken();
    }
    // Empty String <---------------- need to check
    else
    {
        token = getNextToken();
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

        switch (token) // <--------------------- NEED TO CHECK THIS
        {
            case 9:
                relOp = eqlsym;
                break;
            case 10:
                relOp = neqsym;
                break;
            case 11:
                relOp = lessym;
                break;
            case 12:
                relOp = lessym;
                break;
            case 13:
                relOp = gtrsym;
                break;
            case 14:
                relOp = geqsym;
                break;
        }

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

// For code generation
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

//Gets token int value and prints it as a string
void printTokens(int token, int flag)
{
    if (token == nulsym)
    {
        if (flag == 1)
            printf("nulsymn \t");
        fprintf(outFile,"nulsym \t");
    }
    else if (token == identsym)
    {
        if (flag == 1)
            printf("identsym \t");
        fprintf(outFile,"identsym \t");
    }
    else if (token == numbersym)
    {
        if (flag == 1)
            printf("numbersym \t");
        fprintf(outFile,"numbersym \t");
    }
    else if (token == plussym)
    {
        if (flag == 1)
            printf("plussym \t");
        fprintf(outFile,"plussym \t");
    }
    else if (token == minussym)
    {
        if (flag == 1)
            printf("minussym \t");
        fprintf(outFile,"minussym \t");
    }
    else if (token == multsym)
    {
        if (flag == 1)
            printf("multsym \t");
        fprintf(outFile,"multsym \t");
    }
    else if (token == slashsym)
    {
        if (flag == 1)
            printf("slashsym \t");
        fprintf(outFile,"slashsym \t");
    }
    else if (token == oddsym)
    {
        if (flag == 1)
            printf("oddsym \t");
        fprintf(outFile,"oddsym \t");
    }
    else if (token == eqlsym)
    {
        if (flag == 1)
            printf("eqlsym \t");
        fprintf(outFile,"eqlsym \t");
    }
    else if (token == neqsym)
    {
        if (flag == 1)
            printf("neqsym \t");
        fprintf(outFile,"neqsym \t");
    }
    else if (token == lessym)
    {
        if (flag == 1)
            printf("lessym \t");
        fprintf(outFile,"lessym \t");
    }
    else if (token == leqsym)
    {
        if (flag == 1)
            printf("leqsym \t");
        fprintf(outFile,"leqsym \t");
    }
    else if (token == gtrsym)
    {
        if (flag == 1)
            printf("gtrsym \t");
        fprintf(outFile,"gtrsym \t");
    }
    else if (token == geqsym)
    {
        if (flag == 1)
            printf("geqsym \t");
        fprintf(outFile,"geqsym \t");
    }
    else if (token == lparentsym)
    {
        if (flag == 1)
            printf("lparentsym \t");
        fprintf(outFile,"lparentsym \t");
    }
    else if (token == rparentsym)
    {
        if (flag == 1)
            printf("rparentsym \t");
        fprintf(outFile,"rparentsym \t");
    }   
    else if (token == commasym)
    {
        if (flag == 1)
            printf("commasym \t");
        fprintf(outFile,"commasym \t");
    }
    else if (token == semicolonsym)
    {
        if (flag == 1)
            printf("semicolonsym \t");
        fprintf(outFile,"semicolonsym \t");
    }
    else if (token == periodsym)
    {
        if (flag == 1)
            printf("periodsym \t");
        fprintf(outFile,"periodsym \t");
    }
    else if (token == becomesym)
    {
        if (flag == 1)
            printf("becomesym \t");
        fprintf(outFile,"becomesym \t");
    }
    else if (token == beginsym)
    {
        if (flag == 1)
            printf("beginsym \t");
        fprintf(outFile,"beginsym \t");
    }
    else if (token == endsym)
    {
        if (flag == 1)
            printf("endsym \t");
        fprintf(outFile,"endsym \t");
    }
    else if (token == ifsym)
    {
        if (flag == 1)
            printf("ifsym \t");
        fprintf(outFile,"ifsym \t");
    }
    else if (token == thensym)
    {
        if (flag == 1)
            printf("thensym \t");
        fprintf(outFile,"thensym \t");
    }
    else if (token == whilesym)
    {
        if (flag == 1)
            printf("whilesym \t");
        fprintf(outFile,"whilesym \t");
    }
    else if (token == dosym)
    {
        if (flag == 1)
            printf("dosym \t");
        fprintf(outFile,"dosym \t");
    }
    else if (token == callsym)
    {
        if (flag == 1)
            printf("callsym \t");
        fprintf(outFile,"callsym \t");
    }
    else if (token == constsym)
    {
        if (flag == 1)
            printf("constsym \t");
        fprintf(outFile,"constsym \t");
    }
    else if (token == varsym)
    {
        if (flag == 1)
            printf("varsym \t");
        fprintf(outFile,"varsym \t");
    }
    else if (token == writesym)
    {
        if (flag == 1)
            printf("writesym \t");
        fprintf(outFile,"writesym \t");
    }
    else if (token == readsym)
    {
        if (flag == 1)
            printf("readsym \t");
        fprintf(outFile,"readsym \t");
    }
    else
    {
        if (flag == 1)
            printf("Not a valid token\n");
        fprintf(outFile,"Not a valid token\n");
    }
}

int parser(int flag)
{
    inFile = fopen("lexOutput.txt", "r");
    outFile = fopen("parseOutput.txt", "w");
    
    if (inFile == NULL)
        printf("Couldn't open input file. Make sure it's called 'parseInput.txt'\n");
    if (outFile == NULL)
        printf("Couldn't open output file\n");

    if (flag == 1)
        printf("============================ parser/Code-Gen Outout ============================ \n");

	program();

	if (flag == 1)
        printf("\n================================================================================ \n");

    fclose(inFile);
    fclose(outFile);

    return 0;
}