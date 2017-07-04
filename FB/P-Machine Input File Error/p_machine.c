// Travis Bashor
// Systems Software
// 6-2-17

#include <stdio.h>
#include <string.h>
#define MAX_STACK_HEIGHT 2000
#define MAX_CODE_LENGTH 500
#define MAX_LEXI_LEVEL 3

// program instruction
typedef struct {
	int op_code, level, modifier;
} Instruction;

// prototypes
void perform_operation(int *pc, int *sp, int *bp, int mod); /* select operation, given a mod value */
int base(int levels, int base);                             /* find the base of the caller of an AR*/
void print_code(FILE* ofp, int code_length);                /* print the assembly code pulled from the input file */
void print_stack(FILE* ofp, int base, int top);             /* recursive function to print all ARs in the stack */

// global variables
Instruction code[MAX_CODE_LENGTH];  /* used to hold the program */
int stack[MAX_STACK_HEIGHT];        /* used to hold the activation records */

int main() {
  // variables for the registers, as well as some input/output locations
  Instruction ir;
  int pc = 0;
  int sp = 0;
  int bp = 1;
  char input_file[50];
  char output_file[50];
  int run;
  int input;
  strcpy(input_file, "vminput_recursivefactorial.txt"); // change this input file for testing
  strcpy(output_file, "vmoutput.txt");

  // read the assembly code from the input file
  FILE *ifp;
  ifp = fopen(input_file, "r");
  if (ifp == NULL) {
    fprintf(stderr, "Can't open input file.\n");
    return 1;
  }

  // prepare the output file for writing
  FILE *ofp;
  ofp = fopen(output_file, "w");
  if (ofp == NULL) {
    fprintf(stderr, "Can't open output file.\n");
    return 1;
  }

  // load the code array with instructions
  int n = 0;
  int code_length;
  int op_code, level, modifier;
  while (fscanf(ifp, "%d %d %d", &op_code, &level, &modifier) != EOF) {
    code[n].op_code = op_code;
    code[n].level = level;
    code[n].modifier = modifier;
    n++;
  }
  code_length = n;
  fclose(ifp);

  // initialize all stack elements to zero
  int i;
  for (i = 0; i < MAX_STACK_HEIGHT; i++) {
    stack[i] = 0;
  }

  // print the assembly code that corresponds to the numerical inputs
  print_code(ofp, code_length);

  // title the output columns
  fprintf(ofp, "\n\t\t\t\tpc\tbp\tsp\tstack\n");
  fprintf(ofp, "Initial values\t\t\t%d\t%d\t%d\n", pc, bp, sp);

  // run the program
  run = 1;

  // initiate the fetch/execute cycle
  while(run==1) {

    // fetch
    ir = code[pc];
    fprintf(ofp, "%d\t", pc);
    pc++;

    // execute
    switch(ir.op_code) {
      int input;
  		case 1: // LIT
        // push a literal value onto the stack
        fprintf(ofp, "lit\t");
        sp++;
        stack[sp] = ir.modifier;
      break;

      case 2: // OPR
        // choose an operation to be performed on the data on top of the stack
        fprintf(ofp, "opr\t");
        perform_operation(&pc, &sp, &bp, ir.modifier);
      break;

      case 3: // LOD
        // load a value to the top of the stack from a location specified by level and modifier
        fprintf(ofp, "lod\t");
        sp++;
        stack[sp] = stack[base(ir.level, bp) + ir.modifier];
      break;

      case 4: // STO
        // store a value from the top of the stack at a location specified by level and modifier
        fprintf(ofp, "sto\t");
        stack[base(ir.level, bp) + ir.modifier] = stack[sp];
        sp--;
      break;

      case 5: // CAL
        // call a procedure at code index specified by modifier
        fprintf(ofp, "cal\t");

        // assign the standard 4 values at the beginning of the new activation record
        stack[sp + 1] = 0;                            /* functional return value */
        stack[sp + 2] = base(ir.level, bp);           /* static link */
        stack[sp + 3] = bp;                           /* dynamic link */
        stack[sp + 4] = pc;                           /* return address */

        // adjust the base pointer to reflect the active stack frame
        bp = sp + 1;

        // visit the instruction at code index "modifier"
        pc = ir.modifier;
      break;

      case 6: // INC
        // allocate a number of locals on the stack, given by modifier
        fprintf(ofp, "inc\t");
        sp += ir.modifier;
      break;

      case 7: // JMP
        // jump to instruction at index given by modifier
        fprintf(ofp, "jmp\t");
        pc = ir.modifier;
      break;

      case 8: // JPC
        // jump to modifier index if the top of the stack is equal to zero
        fprintf(ofp, "jpc\t");
        if(stack[sp] == 0) {
          pc = ir.modifier;
        }
      break;

      case 9: // SIO
        // write/read/halt, depending on the modifier
        fprintf(ofp, "sio\t");
        switch(ir.modifier) {
          // output the value at the top of the stack to the screen
          case 1:
            printf("The value at the top of the stack is:\n%d\n", stack[sp]);
          break;

          // read input from user and store it on top of the stack
          case 2:
            printf("Please input a value:\n");
            scanf("%d", &input);
            sp++;
            stack[sp] = input;
          break;

          // halt the program
          case 3:
            pc = 0;
            bp = 0;
            sp = 0;
            run = 0;
          break;
        }
      break;

      default:
        printf("Input error.\n");
    }

    // print register values after line is executed
    fprintf(ofp, "%d\t%d\t%d\t%d\t%d\t", ir.level, ir.modifier, pc, bp, sp);

    // print the stack, with activation records separated by bars
    print_stack(ofp, bp, sp);
    fprintf(ofp, "\n");
  }
	return 0;
}

// recursive function to print all activation records in the stack
void print_stack(FILE* ofp, int base, int top) {
  // base case 1: we are at the end of the program
  if (base == 0) {
    // print an empty line and exit
    fprintf(ofp, "");
    return;
  }

  // find the height of the current AR, and the dynamic link to the caller of this AR
  int record_height = top - base + 1;
  int dynamic_link = stack[base + 2];

  // recursion case: use the dynamic link to find the caller of this AR
  // print the contents of the AR that called this one, then its own contents
  if (base != 1) {
    // dynamic link is the base of the caller, and base - 1 is the highest location in the caller
    print_stack(ofp, dynamic_link, base - 1);
    fprintf(ofp, "| ");

    // check for the unique case of cal
    if(base > top) {
      // show the first 4 values of the new activation record
      record_height = 4;
    }
  }

  // base case 2: we are at the first activation record
  // print this AR's contents
  int i;
  for(i = 0; i < record_height; i++) {
    fprintf(ofp, "%d ", stack[base + i]);
  }
}

// identify and perform the proper operation on the data on top of the stack
void perform_operation(int* pc, int* sp, int* bp, int modifier) {
  switch(modifier) {
    // RET
    case 0:
      // go back to the stack frame of the caller
      *sp = *bp - 1;
      *pc = stack[*sp + 4];
      *bp = stack[*sp + 3];
    break;

    // NEG
    case 1:
      // store the additive inverse of stack[sp] at the top of the stack
      stack[*sp] = 0 - stack[*sp];
    break;

    // ADD
    case 2:
      // store the sum of stack[sp] and stack[sp+1] at the top of the stack
      *sp = *sp - 1;
      stack[*sp] += stack[*sp + 1];
    break;

    // SUB
    case 3:
      // store the difference of stack[sp] and stack[sp+1] at the top of the stack
      *sp = *sp - 1;
      stack[*sp] -= stack[*sp + 1];
    break;

    // MUL
    case 4:
      // store product of stack[sp] and stack[sp+1] at the top of the stack
      *sp = *sp - 1;
      stack[*sp] *= stack[*sp + 1];
    break;

    // DIV
    case 5:
      // store quotient of stack[sp] and stack[sp + 1] at the top of the stack
      *sp = *sp - 1;
      stack[*sp] /= stack[*sp + 1];
    break;

    // ODD
    case 6:
      // store 0 at the top of the stack if stack[sp] is an odd number
      if (stack[*sp] % 2 == 1) {
        stack[*sp] = 0;
      }
      // store 1 otherwise
      else {
        stack[*sp] = 1;
      }
    break;

    // MOD
    case 7:
      *sp = *sp - 1;
      // store modulus of stack[sp] and stack[sp+1] at the top of the stack
      stack[*sp] %= stack[*sp + 1];
    break;

    // EQL
    case 8:
      *sp = *sp - 1;
      // store 0 at the top of the stack if equal
      if (stack[*sp] == stack[*sp + 1]) {
        stack[*sp] = 0;
      }
      // store 1 otherwise
      else {
        stack[*sp] = 1;
      }
    break;

    // NEQ
    case 9:
      *sp = *sp - 1;
      // store 0 at the top of the stack if unequal
      if (stack[*sp] != stack[*sp + 1]) {
        stack[*sp] = 0;
      }
      // store 1 otherwise
      else {
        stack[*sp] = 1;
      }
    break;

    // LSS
    case 10:
      *sp = *sp - 1;
      // store 0 at the top of the stack if less than
      if (stack[*sp] < stack[*sp + 1]) {
        stack[*sp] = 0;
      }
      // store 1 otherwise
      else {
        stack[*sp] = 1;
      }
    break;

    // LEQ
    case 11:
      *sp = *sp - 1;
      // store 0 at the top of the stack if less than or equal to
      if (stack[*sp] <= stack[*sp + 1]) {
        stack[*sp] = 0;
      }
      // store 1 otherwise
      else {
        stack[*sp] = 1;
      }
    break;

    // GTR
    case 12:
      *sp = *sp - 1;
      // store 0 at the top of the stack if greater than
      if (stack[*sp] > stack[*sp + 1]) {
        stack[*sp] = 0;
      }
      // store 1 otherwise
      else {
        stack[*sp] = 1;
      }
    break;

    // GEQ
    case 13:
      *sp = *sp - 1;
      // store 0 at the top of the stack if greater than or equal to
      if (stack[*sp] >= stack[*sp + 1]) {
        stack[*sp] = 0;
      }
      // store 1 otherwise
      else {
        stack[*sp] = 1;
      }
    break;

    default:
      printf("Modifier out of range.");
  }
}

// find base a specified number of levels up the static chain
int base(int levels, int base) {
  int b1;
  b1 = base;
  while (levels > 0) {
    b1 = stack[b1 + 1];
    levels--;
  }
  return b1;
}

// print the code loaded from the file
void print_code(FILE* ofp, int code_length) {
  
  // print column headers in file
  fprintf(ofp, "Line\tOP\tL\tM\n");
  
  // create variables to use while reading in instructions
  int pc;
  Instruction ir;

  // go through the instructions, loading them into the instruction register one at a time
  for(pc = 0; pc < code_length; pc++) {
    
    // load the next instruction into ir
    ir = code[pc];

    // print the current line number
    fprintf(ofp, "%d\t", pc);
    
    // print three-letter op_code
    switch(ir.op_code) {
      case 1:
        fprintf(ofp, "lit\t");
      break;
      case 2:
        fprintf(ofp, "opr\t");
      break;
      case 3:
        fprintf(ofp, "lod\t");
      break;
      case 4:
        fprintf(ofp, "sto\t");
      break;
      case 5:
        fprintf(ofp, "cal\t");
      break;
      case 6:
        fprintf(ofp, "inc\t");
      break;
      case 7:
        fprintf(ofp, "jmp\t");
      break;
      case 8:
        fprintf(ofp, "jpc\t");
      break;
      case 9:
        fprintf(ofp, "sio\t");
      break;
      default:
        fprintf(ofp, "error\t");
    }

    // print level and modifier
    fprintf(ofp, "%d\t%d\n", ir.level, ir.modifier);
  }
}