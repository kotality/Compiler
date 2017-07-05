To compile and run this program, type:

gcc -o <runfile> <sourcefile.c>

and then

./<runfile>

The default input file is named: 

vminput_recursivefactorial.txt

...and is stored in a variable called input_file on line 36 of the source code. That is the input file
used to test everyone's homework and it has a logical error in it that occurs on line 6, "2 0 12". The
program will produce an output of "1" for the calculation of 5! as a result.

The corrected input file is named:

vminput_corrected.txt

...and on line 6, the input is "2 0 8" instead - an equality check. This eliminates the logic error and 
the program will produce the correct output.

The default output file is named:

vmoutput.txt

...and is stored in a variable called output_file on line 37 of the source code

For formatting, the tab size in the output file is assumed to be 8. If viewing via the vi
text editor in Eustis, make sure to resize the window to ensure all of the text file is viewable. 
It cuts off in small form but when the window is maximized or half-maximized, the file displays as 
intended.

***Please run the program with both input files to see the difference and confirm the logic error in the original***