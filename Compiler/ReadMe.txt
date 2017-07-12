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

6.  vminput.txt
7.  vmoutput.txt

8.  lexInput.txt
9.  symLexListOut.txt
10. lexListOut.txt

11. scannerOut.txt
12. parseOutput.txt


================================================================================
								| Instructions |

1. The input file names are hard-coded. Please make sure the input files are 
   named as follows:

    Virtual Machine: vminput.txt
    Lexical Analyzer: lexInput.txt
    Parser/Code Gen: scannerOut.txt


2. Compile the file "compiler.c" at the command line by typing:
	gcc compiler.c -o compile


3. There are three supported compiler directives:
	-l: print the list of lexemes/tokens (scanner output) to the screen
	-a: print the generated assembly code (parser/codegen output) to the 
            screen
	-v: print virtual machine execution trace (virtual machine output) to 
            the screen

   After compiling, you can use the compiler directives by typing:
	./compile [compiler directive(s)]

   For example:
	./compile -l -a -v
	./compile -v
	./compile


4. Some text editors don't properly display the formatting for the output 
   files and create weird spacing. My specific output file was tested on Visual
   Studio Code and Microsoft WordPad (click View -> Word wrap -> No wrap)

================================================================================