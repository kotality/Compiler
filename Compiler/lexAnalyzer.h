/*
	Lexical Analyzer
*/

#include "header.h"

//Returns the index in reserved of the string pointed to by [identifier].
int reservedIndex(char * identifier)
{
	int i;

	for(i = 0; i < 14; i++)
	{
		if (strcmp(reserved[i], identifier) == 0)
		{
			return i;
		}
	}
	return -1;
}

//Maps an index in reserved to the proper internal representation...
int mapReserved(int spotInReserved)
{
	if (spotInReserved == 0)
		return constsym;
	if (spotInReserved == 1)
		return varsym;
	if (spotInReserved == 3)
		return callsym;
	if (spotInReserved == 4)
		return beginsym;
	if (spotInReserved == 5)
		return endsym;
	if (spotInReserved == 6)
		return ifsym;
	if (spotInReserved == 7)
		return thensym;
	if (spotInReserved == 9)
		return whilesym;
	if (spotInReserved == 10)
		return dosym;
	if (spotInReserved == 11)
		return readsym;
	if (spotInReserved == 12)
		return writesym;
	if (spotInReserved == 13)
		return oddsym;
    return -1;
}

//Maps a symbol from character to internal representation...
int mapSymbol(char * symbol)
{
	if (strcmp(symbol, "+") == 0)
		return plussym;
	if (strcmp(symbol, "-") == 0)
		return minussym;
	if (strcmp(symbol, "*") == 0)
		return multsym;
	if (strcmp(symbol, "/") == 0)
		return slashsym;
	if (strcmp(symbol, "(") == 0)
		return lparentsym;
	if (strcmp(symbol, ")") == 0)
		return rparentsym;
	if (strcmp(symbol, "=") == 0)
		return eqlsym;
	if (strcmp(symbol, ",") == 0)
		return commasym;
	if (strcmp(symbol, ".") == 0)
		return periodsym;
	if (strcmp(symbol, "<") == 0)
		return lessym;
	if (strcmp(symbol, ">") == 0)
		return gtrsym;
	if (strcmp(symbol, ";") == 0)
		return semicolonsym;

	if (strcmp(symbol, "<>") == 0)
		return neqsym;
	if (strcmp(symbol, "<=") == 0)
		return leqsym;
	if (strcmp(symbol, ">=") == 0)
		return geqsym;
	if (strcmp(symbol, ":=") == 0)
		return becomesym;
    return -1;
}

//~~~Character Manipulation Methods~~~

//Returns 1 iff [theChar] is a uppercase letter, 0 otherwise.
int isUpper(char theChar)
{
	return theChar >= 65 && theChar <= 90;
}

//Returns 1 iff [theChar] is a lowercase letter, 0 otherwise.
int isLower(char theChar)
{
	return theChar >= 97 && theChar <= 122;
}

//Returns 1 iff [theChar] is an alphabetic letter, 0 otherwise.
int isAlpha(char theChar)
{
	return isLower(theChar) || isUpper(theChar);
}

//Returns 1 iff [theChar] is a digit 0-9, 0 otherwise.
int isDigit(char theChar)
{
	return theChar >= 48 && theChar <= 57;
}

//Returns 1 iff [theChar] is either a digit or an alphabetic letter, 0 otherwise.
int isAlphanumeric(char theChar)
{
	return isAlpha(theChar) || isDigit(theChar);
}

//Returns 1 iff [theChar] is a valid whitespace or invisible character, 0 otherwise.
int isInvisible(char theChar)
{
	return theChar == 9 || theChar == 10 || theChar == 13 || theChar == 32;
}

//Returns 1 iff [theChar] is a valid symbol, 0 otherwise.
int isSymbol(char theChar)
{
	int i;

	for(i = 0; i < 13; i++)
	{
		if (symbols[i] == theChar)
			return 1;
	}
	return 0;
}

//Returns 1 iff [theChar] is a valid character used for scanning, 0 otherwise.
int isValid(char theChar)
{
	return isAlphanumeric(theChar) || isSymbol(theChar) || isInvisible(theChar) || theChar == '\0';
}

//~~~Error state stuff~~~

//If called, this makes a text file called "ef" and places the error message into it.
void throwError(char * message)
{
	FILE * errorFile = fopen("ef", "w");
	printf("An error occurred while running lexical analysis: %s\n", message);
	fprintf(errorFile, "An error occurred while running lexical analysis: %s\n", message);
	fclose(errorFile);
	exit(0);
}

//~~~File handling stuff~~~

//Our input file
int ip = 0;
char inputChars[MAX_CODE_LENGTH];
int inputCharsSize;

//Gets a character from the input; enforces that the character is valid iff ignoreValidity is 0.
char getChar(int ignoreValidity)
{	
	//Make sure this char is even actually existing...
	if (ip > inputCharsSize)
	{
		throwError("Input file ends unexpectedly! (Did you forget to close a comment?)");
	}

	char nextChar = inputChars[ip];
	ip++;
	
	if (!ignoreValidity && !isValid(nextChar))
	{
		throwError("Invalid character encountered!");
	}
	return nextChar;
}

//Go back!!!
void ungetChar()
{
	ip--;
}

//The almighty buffer, and associates
int bp;
char buffer[16];

//Empty out that buffer!
void clearBuffer()
{
	int i;
	bp = 0;
	for(i = 0; i < 16; i++)
	{
		buffer[i] = '\0';
	}
}

//Add [theChar] to the end of the buffer!
void addToBuffer(char theChar)
{
	if (bp < 16)
	{
		buffer[bp] = theChar;
		bp++;
	}
}

//This method opens the input and output files, and also reads in all the data from the input file.
void openFiles(char * inputFile, char * outputFile)
{
	leXinFile = fopen(inputFile, "r");
	leXoutFile = fopen(outputFile, "w");

	fseek(inFile, 0, SEEK_END);
	int inputSize = ftell(inFile);
	inputCharsSize = inputSize;
	fseek(inFile, 0, SEEK_SET);
	int i;
	for(i = 0; i < inputSize; i++)
	{
		fscanf(inFile, "%c", &inputChars[i]);
	}
	inputChars[i] = '\0';
}

//~~~Text processing~~~
char lexemeTable[MAX_CODE_LENGTH];
char lexemeList[MAX_CODE_LENGTH];
char symbolicLexemeList[MAX_CODE_LENGTH];

//Overwrite all data in the lexeme output arrays!
void clearLexemeOutput()
{
	int i;

	for(i = 0; i < MAX_CODE_LENGTH; i++)
	{
		lexemeTable[i] = '\0';
		lexemeList[i] = '\0';
		symbolicLexemeList[i] = '\0';
	}
	strcat(lexemeTable, "Lexeme Table:\nlexeme       token type\n");
	strcat(lexemeList, "Lexeme List:\n");
	strcat(symbolicLexemeList, "Symbolic Lexeme List:\n");
}

//Insert the lexeme [lexeme] of type [tokenType] nicely into the lexeme table.
void insertToLexemeTable(char * lexeme, int tokenType)
{
	char temp[64];
	char spaces[64] = {'\0'};
	while(strlen(lexeme) + strlen(spaces) < 13)
		strcat(spaces, " ");
	sprintf(temp, "%s%s%d\n", lexeme, spaces, tokenType);
	strcat(lexemeTable, temp);
}

//Insert a number into the lexeme list!
void insertIntToLexemeList(int num)
{
	char temp[64];
	sprintf(temp, "%d ", num);
	strcat(lexemeList, temp);
	sprintf(temp, "%s ", IRMapping[num]);
	strcat(symbolicLexemeList, temp);
}

//Insert a string into the lexeme list!
void insertStrToLexemeList(char * identifier)
{
	char temp[64];
	sprintf(temp, "%s ", identifier);
	strcat(lexemeList, temp);
	strcat(symbolicLexemeList, temp);
}

//Take [identifier] and see if it's a reserved word or actually just an identifier...
void processIdentifier(char * identifier)
{
	int index = reservedIndex(identifier);
	if (index > -1)
	{
		//It's reserved!
		int mapping = mapReserved(index);
		insertIntToLexemeList(mapping);
		insertToLexemeTable(identifier, mapping);
	}
	else
	{
		//Not reserved!
		insertIntToLexemeList(2);
		insertStrToLexemeList(identifier);
		insertToLexemeTable(identifier, 2);
	}
}


//Process a number literal represented by the string pointed to by [num]
void processNumber(char * num)
{
	insertToLexemeTable(num, 3);
	insertIntToLexemeList(3);
	insertStrToLexemeList(num);
}

//Process a symbol represented by the string pointed to by [sym]
void processSymbol(char * sym)
{
	insertToLexemeTable(sym, mapSymbol(sym));
	insertIntToLexemeList(mapSymbol(sym));
}

//The meat of the program, where the actual fancy important scanning stuff happens!
void processText()
{
	//Clear out the output arrays...
	clearLexemeOutput();

	//Run through the input characters...
	char nextChar = ' ';
	while(nextChar != '\0')
	{
		clearBuffer();
		while(isInvisible(nextChar = getChar(0)))
		{
			//Trash the invisible characters
		}

		//It's not invisible if we are here!
		if (isAlpha(nextChar))
		{
			addToBuffer(nextChar);
			while(isAlphanumeric(nextChar = getChar(0)))
			{
				addToBuffer(nextChar);
				if (strlen(buffer) > MAX_IDENTIFIER_LENGTH)
				{
					//Invalid identifier length
					throwError("Identifier too long!");
				}
			}
			ungetChar();
			//Process identifier in buffer
			processIdentifier(buffer);
		}
		else if (isDigit(nextChar))
		{
			addToBuffer(nextChar);
			while(isDigit(nextChar = getChar(0)))
			{
				addToBuffer(nextChar);
				if (strlen(buffer) > MAX_NUMBER_LENGTH)
				{
					//Invalid number length
					throwError("Number too long!");
				}
			}
			//Was this number followed by a letter?
			if (isAlpha(nextChar))
			{
				throwError("Identifier does not start with letter!");
			}
			//It was not followed by a letter.. so we are okay!
			ungetChar();
			//Process number in buffer.
			processNumber(buffer);
		}
		else if (isSymbol(nextChar))
		{
			addToBuffer(nextChar);
			if (nextChar == '+' || nextChar == '-' || nextChar == '*' || nextChar == '(' || nextChar == ')' || nextChar == '=' || nextChar == ',' || nextChar == '.' || nextChar == ';')
			{
				//Process just that symbol itself!
				processSymbol(buffer);
			}
			else if (nextChar == '/')
			{
				nextChar = getChar(0);
				if (nextChar == '*')
				{
					state5:
					while((nextChar = getChar(1)) != '*')
					{
						//Dump comment...
					}
					while((nextChar = getChar(1)) == '*')
					{
						//Dump comment...
					}
					if (nextChar == '/')
					{
						//Done with comment!
					}
					else
					{
						//Still in comment from * to another char...
						goto state5;
					}
				}
				else
				{
					//Process as a divide symbol...
					ungetChar();
					processSymbol(buffer);
				}
			}
			else if (nextChar == '<')
			{
				nextChar = getChar(0);
				if (nextChar == '=')
				{
					addToBuffer(nextChar);
					processSymbol(buffer);
				}
				else if (nextChar == '>')
				{
					addToBuffer(nextChar);
					processSymbol(buffer);
				}
				else
				{
					//Process less only
					ungetChar();
					processSymbol(buffer);
				}
			}
			else if (nextChar == '>')
			{
				nextChar = getChar(0);
				if (nextChar == '=')
				{
					addToBuffer(nextChar);
					processSymbol(buffer);
				}
				else
				{
					//Process greater than only
					ungetChar();
					processSymbol(buffer);
				}
			}
			else if (nextChar == ':')
			{
				//We must have an equal following it!
				nextChar = getChar(0);
				if (nextChar == '=')
				{
					addToBuffer(nextChar);
					processSymbol(buffer);
				}
				else
				{
					throwError("Invalid symbol!");
				}
			}

		}
		else if (nextChar == '\0')
		{
			break;
		}
		else
		{
			//Invalid state
			throwError("Invalid state!");
		}
	}

	//Print results to output file...
	
	//Uncomment this to print out the lexeme table as well...
	//fprintf(outFile, "%s\n", lexemeTable);
	
	fprintf(outFile, "%s\n\n", symbolicLexemeList);
	fprintf(outFile, "%s", lexemeList);
}

void echoInput()
{
	fprintf(outFile, "Source Program:\n%s\n\n", inputChars);
}

int lexicalAnalyzer(int flag)
{
	if (argc != 3)
	{
		printf("Invalid arguments for lexical analyzer!\nUSAGE: ./lexicalAnalyzer [input file] [output file]\n");
		return 1;
	}
	openFiles(argv[1], argv[2]);
	
	//Uncomment this to print out the input program as well...
	//echoInput();
	
	if (flag == 1)
		processText();

	return 0;
}
