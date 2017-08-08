// Kenia Castro
// COP 3402 - Summer 2017
// HW #2 - Parser and Code Generator
// Main file for compiler driver

#include <stdio.h>
#include "pMachine.h"
#include "lexAnalyzer.h"
#include "parser.h"
#include "header.h"

int main(int argc, char ** argv)
{
    int i, lex = 0, parse = 0, vm = 0;
    int errorFlag;

    for (i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "-l") == 0)
            lex = 1;
        
        if (strcmp(argv[i], "-a") == 0)
            parse = 1;
        
        if (strcmp(argv[i], "-v") == 0)
            vm = 1;
    }

    lexAnalyzer(lex);
    parser(parse);
    pMachine(vm);

    if (errorFlag == 0)
    {
        printf("\nNo errors, program is syntactically correct.\n\n");
        printf("============================================================================== \n");
    }
    
    return 0;
}