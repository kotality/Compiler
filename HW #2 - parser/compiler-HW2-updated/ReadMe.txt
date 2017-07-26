Kenia Castro
COP 3402 - Summer 2017
HW #2 Parser - Parser & Code Generator
ReadMe File

================================================================================
								| File Contents |

1.  compiler.c
2.  pMachine.h
3.  lexAnalyzer.h
4.  parser.h
5.  header.h

6.  parseOuput.txt
7.  vmoutput.txt

8.  lexInput.txt
9.  symLexListOut.txt
10. lexListOut.txt

11. tokenUpdated.txt
12. errors.txt


================================================================================
								| Instructions |

1.  All the file names are hard-coded. Please make sure the input file names 
    match. The starting point should be the lexical analyzer, so please make 
    sure your input file is called "lexInput.txt"

    Sample lexInput.txt file:
        var a, b, answer;
        begin
            read a;
            read b;
            answer := a-b;
            write answer;
        end.

2.  The order of input files:
        Lexical Analyzer: 
            - Input:  lexInput.txt
        Parser/Code Gen: 
            - Input:  symLexListOut.txt (created from lexAnalyzer.h)
        Virtual Machine:
            - Input:  parseOutput.txt (created from parser.h)    


2.  Compile the file "compiler.c" at the command line by typing:
	    gcc compiler.c -o compile


3.  There are three supported compiler directives:
        -l: print the list of lexemes/tokens (scanner output) to the screen
        -a: print the generated assembly code (parser/codegen output) to the 
                screen
        -v: print virtual machine execution trace (virtual machine output) to 
                the screen

    After compiling, you can use the compiler directives by typing:
	    ./compile [compiler directive(s)]

    For example:
        ./compile -l
        ./compile -l -a -v
        ./compile


4.  Some text editors don't properly display the formatting for the output 
    files and create weird spacing. My specific output file was tested on Visual
    Studio Code and Microsoft WordPad (click View -> Word wrap -> No wrap)

================================================================================