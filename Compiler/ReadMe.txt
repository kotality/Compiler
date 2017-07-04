Kenia Castro
COP 3402 - Summer 2017
HW #2 Parser - Code Generator
ReadMe File

================================================================================
								| File Contents |

1.  compiler.c
2.  pMachine.h
3.  lexAnalyzer.h
4.  parser.h

================================================================================
								| Instructions |

1. Compile the file "compiler.c" at the command line by typing:
	gcc compiler.c -o compile


2. There are three supported compiler directives:
	-l: print the list of lexemes/tokens (scanner output) to the screen
	-a: print the generated assembly code (parser/codegen output) to the 
            screen
	-v: print virtual machine execution trace (virtual machine output) to 
            the screen

   After compiling, you can use the compiler directives by typing:
	./compile [compiler directive]

   For example:
	./compule -l -a -v
	./compile -v
	./compile


3. Some text editors don't properly display the formatting for the output 
   files and create weird spacing. My specific output file was tested on Visual
   Studio Code and Microsoft WordPad (click View -> Word wrap -> No wrap)

================================================================================