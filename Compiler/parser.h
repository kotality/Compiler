// Kenia Castro
// COP 3402 - Summer 2017
// HW #2 - Parser and Code Generator

#include "header.h"

// Some initial values and global variables
symbol symbolTable[MAX_SYMBOL_TABLE_SIZE];
instruction code[MAX_CODE_LENGTH];
tokenType tokenArray[MAX_CODE_LENGTH];

tokenType token;
int symVal = 0;         // symbolTable[]
int codeVal = 0;        // code[]
int tokenVal = 0;       // tokenArray[]
int count = 0;          // removeID_Num function
int relOp = 0;          // condition switch
int cx = 0;             // code counter emit function
int printCount = 0;     // helper to print generated code
int spaceOffset = 4;    // startin point for symbol table 
int lexLevel = 0 ;      // keep track of current lex level-- 
                        // --increase when enter Block and decrease when exit it

FILE *inFile;
FILE *outFile;
FILE *tokenUpdated;

// Function prototypes
void program();
void block();
void statement();
void condtition();
void expression();
void term();
void factor();
void getNextToken();
void printTokens(int, int);
void emit(int, int, int);
void errorMessage();
void convert();
void getTokenType(char*, int);
void printCode();

// Gets the next token
void getNextToken()
{
    token = tokenArray[tokenVal];
    // printf("NAME: %s \tTYPE: %d \tSYMBOL: %s \tCOUNT: %d \n",tokenArray[tokenVal].name, tokenArray[tokenVal].type, tokenArray[tokenVal].symbol, tokenVal);
	tokenVal++;
}

void program()
{
    getNextToken();
    block();

    // Period
    if (token.type != periodsym)
    {
        errorMessage(9);    // Period expected
        errorFlag = 1;
    }
    else
        emit(9,0,3);      // End program
}

void block()            
{ 
    int jmpaddr = cx;
    int varCounter = 0;
    emit(7,0,0);
    
    // Const
    if (token.type == constsym)
    {
        do
        {
            getNextToken();
            if (token.type != identsym)
            {
                errorMessage(4);        // const, var, procedure must be followed by identifier
                errorFlag = 1;
            }
            
            symbolTable[symVal].kind = 1;
            strcpy(symbolTable[symVal].name, token.symbol);
            getNextToken();

            if (token.type != eqlsym)
            {
                errorMessage(3);        // Identifier must be followed by =
                errorFlag = 1;
            }

            getNextToken();
            if (token.type != numbersym) 
            {
                errorMessage(2);        // = must be followed by a number
                errorFlag = 1;
            }

            int val = atoi(token.symbol);
            symbolTable[symVal].val = val;
            symbolTable[symVal].level = lexLevel;
            symbolTable[symVal].addr = cx;

            symVal++;
            getNextToken();
        }while (token.type == commasym);

        if (token.type != semicolonsym)
        {
            errorMessage(5);            // Semicolon or comma missing
            errorFlag = 1;
        }

        getNextToken();
    }

    // Var
    if (token.type == varsym)
    {
        do
        {
            symbolTable[symVal].kind = 2;
            symbolTable[symVal].level = 0;
            symbolTable[symVal].addr = spaceOffset + varCounter;

            getNextToken();
            if (token.type != identsym)
            {
                errorMessage(4);        // const, var, procedure must be followed by identifier
                errorFlag = 1;
            }
            
            strcpy(symbolTable[symVal].name, token.symbol);
            
            symVal++;
            varCounter++;
            getNextToken();
        }while (token.type == commasym);

        if (token.type != semicolonsym)
        {
            errorMessage(5);            // Semicolon or comma missing
            errorFlag = 1;
        }
        
        getNextToken();
    }

    // printf("space: %d\n", space);
    // ADDED THIS
    code[jmpaddr].M = cx;       //<===========================
    emit(6,0,spaceOffset + varCounter);

    statement();

    // ADDED THIS
    // emit(2,0,0); 
    lexLevel--;
}

void statement()        
{
    int i, j = 0;
    // Identifier
    if (token.type == identsym)
    {
        for (i = 0; i < symVal; i++)
        {
            // printf("SYM Name: %s TOK Sym: %s\n",symbolTable[i].name,token.symbol);
            if (strcmp(symbolTable[i].name,token.symbol) == 0 && symbolTable[i].kind == 2)
            {
                j = i;
            }
            // printf("i value: %d j value: %d\n", i, j);
        }

        getNextToken();

        if (token.type != becomesym)
        {
            errorMessage(19);           // Incorrect symbol following statement
            errorFlag = 1;
        }

        getNextToken();
        expression(); 

        // THIS ADDED IN
        // printf("j value2: %d\n", j);
        // printf("EMIT(4,0,%d)\n", symbolTable[j].addr);
        emit(4,0,symbolTable[j].addr);
    }
    // Begin 
    else if (token.type == beginsym)
    {
        getNextToken();
        statement();

        while (token.type == semicolonsym)
        {
            getNextToken();
            statement();                
        }

        if (token.type != endsym)
        {
            errorMessage(26);           // end expected
            errorFlag = 1;
        }

        getNextToken();
    }
    // If
    else if (token.type == ifsym)
    {
        int ctemp;
        getNextToken();
        condtition();

        if (token.type != thensym)
        {
            errorMessage(16);           // then expected
            errorFlag = 1;
        }
        
        getNextToken();
        
        ctemp = cx;
        emit(8,0,0);

        statement();

        // printf("cx if: %d\n", cx);
        code[ctemp].M = cx;
    }
    // While
    else if (token.type == whilesym)
    {
        int cx1 = cx;

        getNextToken();
        condtition();

        int cx2 = cx;
        emit(8,0,0);

        if (token.type != dosym)
        {
            errorMessage(18);           // do expected
            errorFlag = 1;
        }
        
        getNextToken();

        // printf("cx1: %d\n", cx1);
        statement();
        emit(7, 0, cx1);

        // printf("cx: %d\n", cx);
        // printf("cx2: %d\n", cx2);
        code[cx2].M = cx;
    }
    // Read
    else if (token.type == readsym)
    {
        getNextToken();

        if (token.type != identsym)
        {
            errorMessage(14);            // call must be followed by an identifier
            errorFlag = 1;
        }
        
		emit(9,0,2);
        getNextToken();
    }
    // Write
    else if (token.type == writesym)
    {
        getNextToken();

        if (token.type != identsym)
        {
            errorMessage(14);            // call must be followed by an identifier
            errorFlag = 1;
        }

        expression();
        emit(9,0,1);

        // if (token.type != identsym)
        // {
        //     errorMessage(14);            // call must be followed by an identifier
        //     errorFlag = 1;
        // }

		// getNextToken();
    }
}

void condtition()
{
    // Odd
    if (token.type == oddsym)
    {
        getNextToken();
        expression();
        // emit(2,0,6);        // ODD <===================== CHECK
    }
    else
    {
        expression();

        if ((token.type != eqlsym) && (token.type != neqsym) && (token.type != lessym) && 
            (token.type != leqsym) && (token.type != gtrsym) && (token.type != geqsym))
        {
            errorMessage(20);           // Relational operator expected
            errorFlag = 1;
        }

        relOp = token.type;

        getNextToken();
        expression();
        
        switch (token.type)
        {
            case 8:
                relOp = eqlsym;
                break;
            case 9:
                relOp = neqsym;
                break;
            case 10:
                relOp = lessym;
                break;
            case 11:
                relOp = lessym;
                break;
            case 12:
                relOp = gtrsym;
                break;
            case 13:
                relOp = geqsym;
                break;
        }

        emit(2,0,relOp - 1);
    }
}

void expression()
{
    int addop;

    // Plus and Minus
    if (token.type == plussym || token.type == minussym)
    {
        addop = token.type;
        getNextToken();
        term();

        if(addop == minussym)
            emit(2, 0, 1);  // negate
    }
    else
        term();

    while (token.type == plussym || token.type == minussym)
    {
        addop = token.type;
        getNextToken();
        term();

        if (addop == plussym)
            emit(2, 0, 2);  // addition
        else
            emit(2, 0, 3);  // subtraction
    }
}

void term()
{
    int mulop;

    factor();

    // Multiply and Divide
    while (token.type == multsym || token.type == slashsym)
    {
        mulop = token.type;
        getNextToken();
        factor();

        if (mulop == multsym)
            emit(2, 0, 4);  // multiplication
        else    
            emit(2, 0, 5);  // division
    }
}

void factor()
{
    if (token.type == identsym)
    {
        int i, j = 0;

        for (i = 0; i < symVal; i++)
        {
            if (strcmp(symbolTable[i].name,token.symbol) == 0)
            {
                j = i;

                // printf("j value: %d\n", j);
                //ADDED THIS
                if (symbolTable[symVal].kind == 1)
                {
                    emit(1,0,symbolTable[j].val);
                    // printf("EMIT(1,0,%d)\n", symbolTable[j].val);
                }
                else
                {
                    emit(3,0,symbolTable[j].addr);
                    // printf("EMIT(3,0,%d)\n", symbolTable[j].addr);
                }
            }
        }

        getNextToken();
    }
    else if (token.type = numbersym)
    {
        int num = atoi(token.symbol);
        // printf("token symbol: %d\n",num);

        emit(1,0,num);
        getNextToken();
    }
    else if (token.type == lparentsym)
    {
        getNextToken();
        expression();

        if (token.type != rparentsym)
        {
            errorMessage(22);           // Right parenthesis missing
            errorFlag = 1;
        }
        
        getNextToken();
    }
    else
    {
        errorMessage(27);               // Invalid factor
        errorFlag = 1;
    }
}

// For code generation
void emit(int op, int l, int m)
{
    if (cx > MAX_CODE_LENGTH)
    {
        errorMessage(28);
        errorFlag = 1;
    }
    else
    {
        code[cx].OP = op; 	    // opcode
        code[cx].L = l;	        // lex level
        code[cx].M = m;         // modifier
        cx++;
    }

    printCount++;
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
        case 29:
            printf("Error. Invalid type for tokenArray. \n");
            break;
        default:
            printf("General Error. Need to make an error message for this.\n");
    }
}

// Converts symbolic lexeme list into tokens
void convert()
{
    char *temp;
    char str[MAX_CODE_LENGTH];
    int compareIdent, compareNum;

    while(fgets(str, sizeof str, inFile) != NULL)
    {
        temp = strtok(str, " ");    
        while (temp != NULL)
        {
            compareIdent = strcmp(temp, "identsym");
            compareNum = strcmp(temp, "numbersym");

            if ((compareIdent == 0) || (compareNum == 0))
            {
                strcpy(tokenArray[count].name, temp);
                getTokenType(temp, count);
                fputs(temp, tokenUpdated);
                fputs("\n", tokenUpdated);

                // if identsym or numbersym add the identifier that follows
                temp = strtok(NULL, " ");
                strcpy(tokenArray[count].symbol, temp);
            }
            else
            {
                strcpy(tokenArray[count].name, temp);
                getTokenType(temp, count);
                fputs(temp, tokenUpdated);
                fputs("\n", tokenUpdated);
            }
            
            temp = strtok(NULL, " ");
            count++;
        }
    }
}

// Gets tokenArray[].type value from tokenArray[].name value
void getTokenType(char *name, int count)
{
    if (strcmp(name, "nulsym") == 0)
        tokenArray[count].type = nulsym;
    else if (strcmp(name, "identsym") == 0)
        tokenArray[count].type = identsym;
    else if (strcmp(name, "numbersym") == 0)
        tokenArray[count].type = numbersym;
    else if (strcmp(name, "plussym") == 0)
        tokenArray[count].type = plussym;
    else if (strcmp(name, "minussym") == 0)
        tokenArray[count].type = minussym;
    else if (strcmp(name, "multsym") == 0)
        tokenArray[count].type = multsym;
    else if (strcmp(name, "slashsym") == 0)
        tokenArray[count].type = slashsym;
    else if (strcmp(name, "oddsym") == 0)
        tokenArray[count].type = oddsym;
    else if (strcmp(name, "eqlsym") == 0)
        tokenArray[count].type = eqlsym;
    else if (strcmp(name, "neqsym") == 0)
        tokenArray[count].type = neqsym;
    else if (strcmp(name, "lessym") == 0)
        tokenArray[count].type = lessym;
    else if (strcmp(name, "leqsym") == 0)
        tokenArray[count].type = leqsym;
    else if (strcmp(name, "gtrsym") == 0)
        tokenArray[count].type = gtrsym;
    else if (strcmp(name, "geqsym") == 0)
        tokenArray[count].type = geqsym;
    else if (strcmp(name, "lparentsym") == 0)
        tokenArray[count].type = lparentsym;
    else if (strcmp(name, "rparentsym") == 0)
        tokenArray[count].type = rparentsym;
    else if (strcmp(name, "commasym") == 0)
        tokenArray[count].type = commasym;
    else if (strcmp(name, "semicolonsym") == 0)
        tokenArray[count].type = semicolonsym;
    else if (strcmp(name, "periodsym") == 0)
        tokenArray[count].type = periodsym;
    else if (strcmp(name, "becomesym") == 0)
        tokenArray[count].type = becomesym;
    else if (strcmp(name, "beginsym") == 0)
        tokenArray[count].type = beginsym;
    else if (strcmp(name, "endsym") == 0)
        tokenArray[count].type = endsym;
    else if (strcmp(name, "ifsym") == 0)
        tokenArray[count].type = ifsym;
    else if (strcmp(name, "thensym") == 0)
        tokenArray[count].type = thensym;
    else if (strcmp(name, "whilesym") == 0)
        tokenArray[count].type = whilesym;
    else if (strcmp(name, "dosym") == 0)
        tokenArray[count].type = dosym;
    else if (strcmp(name, "constsym") == 0)
        tokenArray[count].type = constsym;
    else if (strcmp(name, "varsym") == 0)
        tokenArray[count].type = varsym;
    else if (strcmp(name, "writesym") == 0)
        tokenArray[count].type = writesym;
    else if (strcmp(name, "readsym") == 0)
        tokenArray[count].type = readsym;
    else 
        errorMessage(29);           // Invalid type for tokenArray
}

// Prints generated code to be used in Virtual Machine
void printCode(int flag)
{
    int i, j;
    
    for (i = 0; i < printCount; i++)
    {
        fprintf(outFile, "%d %d %d\n", code[i].OP, code[i].L, code[i].M);

        if (flag == 1)
            printf("%d %d %d\n", code[i].OP, code[i].L, code[i].M);
    }

    // for (j = 0; j < symVal; j++)
    //     printf("KIND: %d NAME: %s, VAL: %d LEVEL: %d ADDR: %d\n", symbolTable[j].kind, symbolTable[j].name, symbolTable[j].val, symbolTable[j].level, symbolTable[j].addr);
}

int parser(int flag)
{
    inFile = fopen("symLexListOut.txt", "r");
    outFile = fopen("parseOutput.txt", "w");
    tokenUpdated = fopen("tokenUpdated.txt", "w");
    
    if (inFile == NULL)
        printf("Couldn't read input file. Make sure it's called 'symlexListOut.txt'\n");
    if (outFile == NULL)
        printf("Couldn't write to output file. Make sure it's called 'parseOutput.txt'\n");
    if (tokenUpdated == NULL)
        printf("Couldn't write to tokenFile. Make sure it's called 'tokenUpdated.txt'\n");

    if (flag == 1)
    {
        printf("============================================================================== \n");
        printf("|                            Parser/Code-Gen Outout                          | \n");
        printf("============================================================================== \n");
    }

    convert();
	program();
    printCode(flag);

    // if ((flag == 1) && (errorFlag == 0))
    // {
    //     printf("\nNo errors, program is syntactically correct.\n\n");
    //     printf("============================================================================== \n");
    // }

    fclose(inFile);
    fclose(outFile);
    fclose(tokenUpdated);

    return 0;
}