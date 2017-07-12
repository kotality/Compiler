Read input file and print contents
void printParserIn(int flag)
{
    int input;

    while (fscanf(inFile, "%d", &input) != EOF)
    {
        tokenArray[tokenVal] = input;
        tokenVal++;

        // Check if there's an identifier
        if (input == identsym)
        {
            fscanf(inFile, "%d", &input);
            tokenArray[tokenVal] = input;
            tokenVal++;

            if (flag == 1)
                printf("identym %d ", input);
            fprintf(outFile, "identym %d ", input);
        }
        // Get number
        else if (input == numbersym)
        {
            fscanf(inFile, "%d", &input);
            tokenArray[tokenVal] = input;
            tokenVal++;

            if (flag == 1)
                printf("numbersym %d ", input);
            fprintf(outFile, "numbersym %d ", input);
        }
        printTokens(input, flag);
    }
}

==============================================================
//Gets token int value and prints it as a string
void printTokens(int token, int flag)
{
    if (token.type == nulsym)
    {
        if (flag == 1)
            printf("nulsymn \t");
        fprintf(outFile,"nulsym \t");
    }
    else if (token.type == identsym)
    {
        if (flag == 1)
            printf("identsym \t");
        fprintf(outFile,"identsym \t");
    }
    else if (token.type == numbersym)
    {
        if (flag == 1)
            printf("numbersym \t");
        fprintf(outFile,"numbersym \t");
    }
    else if (token.type == plussym)
    {
        if (flag == 1)
            printf("plussym \t");
        fprintf(outFile,"plussym \t");
    }
    else if (token.type == minussym)
    {
        if (flag == 1)
            printf("minussym \t");
        fprintf(outFile,"minussym \t");
    }
    else if (token.type == multsym)
    {
        if (flag == 1)
            printf("multsym \t");
        fprintf(outFile,"multsym \t");
    }
    else if (token.type == slashsym)
    {
        if (flag == 1)
            printf("slashsym \t");
        fprintf(outFile,"slashsym \t");
    }
    else if (token.type == oddsym)
    {
        if (flag == 1)
            printf("oddsym \t");
        fprintf(outFile,"oddsym \t");
    }
    else if (token.type == eqlsym)
    {
        if (flag == 1)
            printf("eqlsym \t");
        fprintf(outFile,"eqlsym \t");
    }
    else if (token.type == neqsym)
    {
        if (flag == 1)
            printf("neqsym \t");
        fprintf(outFile,"neqsym \t");
    }
    else if (token.type == lessym)
    {
        if (flag == 1)
            printf("lessym \t");
        fprintf(outFile,"lessym \t");
    }
    else if (token.type == leqsym)
    {
        if (flag == 1)
            printf("leqsym \t");
        fprintf(outFile,"leqsym \t");
    }
    else if (token.type == gtrsym)
    {
        if (flag == 1)
            printf("gtrsym \t");
        fprintf(outFile,"gtrsym \t");
    }
    else if (token.type == geqsym)
    {
        if (flag == 1)
            printf("geqsym \t");
        fprintf(outFile,"geqsym \t");
    }
    else if (token.type == lparentsym)
    {
        if (flag == 1)
            printf("lparentsym \t");
        fprintf(outFile,"lparentsym \t");
    }
    else if (token.type == rparentsym)
    {
        if (flag == 1)
            printf("rparentsym \t");
        fprintf(outFile,"rparentsym \t");
    }   
    else if (token.type == commasym)
    {
        if (flag == 1)
            printf("commasym \t");
        fprintf(outFile,"commasym \t");
    }
    else if (token.type == semicolonsym)
    {
        if (flag == 1)
            printf("semicolonsym \t");
        fprintf(outFile,"semicolonsym \t");
    }
    else if (token.type == periodsym)
    {
        if (flag == 1)
            printf("periodsym \t");
        fprintf(outFile,"periodsym \t");
    }
    else if (token.type == becomesym)
    {
        if (flag == 1)
            printf("becomesym \t");
        fprintf(outFile,"becomesym \t");
    }
    else if (token.type == beginsym)
    {
        if (flag == 1)
            printf("beginsym \t");
        fprintf(outFile,"beginsym \t");
    }
    else if (token.type == endsym)
    {
        if (flag == 1)
            printf("endsym \t");
        fprintf(outFile,"endsym \t");
    }
    else if (token.type == ifsym)
    {
        if (flag == 1)
            printf("ifsym \t");
        fprintf(outFile,"ifsym \t");
    }
    else if (token.type == thensym)
    {
        if (flag == 1)
            printf("thensym \t");
        fprintf(outFile,"thensym \t");
    }
    else if (token.type == whilesym)
    {
        if (flag == 1)
            printf("whilesym \t");
        fprintf(outFile,"whilesym \t");
    }
    else if (token.type == dosym)
    {
        if (flag == 1)
            printf("dosym \t");
        fprintf(outFile,"dosym \t");
    }
    else if (token.type == constsym)
    {
        if (flag == 1)
            printf("constsym \t");
        fprintf(outFile,"constsym \t");
    }
    else if (token.type == varsym)
    {
        if (flag == 1)
            printf("varsym \t");
        fprintf(outFile,"varsym \t");
    }
    else if (token.type == writesym)
    {
        if (flag == 1)
            printf("writesym \t");
        fprintf(outFile,"writesym \t");
    }
    else if (token.type == readsym)
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

==============================================================
printf("count: %d\n", count);
int i;
for(i = 0; i < count; i++)
{
    printf("%s ",tokenArray[i].name);
    printf("%d", tokenArray[i].type);
    printf("\n");
}
==============================================================