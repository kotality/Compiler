This program is a lexical analyzer. It takes as input a text file containing
PL/0 tokens (or a PL/0 program) and produces as output a text file containing
the internal representation information about the tokens in the input file. 

To run the program on a linux machine, you need to first compile it using
a command similar to the following:

gcc -o lexicalAnalyzer lexicalAnalyzer.c

Once compiled, you can then run the program on a linux machine using a command
similar to the following:

./lexicalAnalyzer [input file path/name] [output file path/name]

where [input file path/name] is the relative path to the input file, and [output
file path/name] is the relative path to the output file. After running, there will
be an output file created which contains a symbolic lexeme list followed by a
lexeme list (if no errors occurred). The important portion of this output will be
the last line of the output, which corresponds to the internal representation lexeme
list. 

If errors did occur, then the output file will not be usable, and another file will
be created and filled with the corresponding error message. This error file will be
named "ef" and will be a text file and will reside in the same directory that the
lexical analyzer was run from. Thus, to check for errors, it will be important to
delete the "ef" file before running the lexical analyzer, so you can then simply
check for the existance of the "ef" file to determine if something went wrong with
lexical analysis.

Attached is an example of some input and output for this lexical analyzer, residing
in in.txt and out.txt respectively.
