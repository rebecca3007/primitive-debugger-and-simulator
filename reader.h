/* 
 * the reader.c file reads in the binary file, discards the header, and then saves the program
 * to the mem array, starting at index 0x100 (0-0xFF is zero page). The instructions function 
 * accepts a hex number == opcode and compares it against a list of all the opcodes for the 6502
 * processor. On a match, the function executes the necessary code for that operation. All other 
 * functions in the file aid in this, eliminating repeating code. However, some opcodes work in a 
 * slightly different way than the others, so these functions aren't always used. 
 */ 

extern unsigned char mem[8000]; //memory array
extern short pc; //program counter
extern int endofprog; //boolean value that stores whether the program is finished

extern void readFile(char*);
extern void instructions(int);
extern unsigned char imm();
extern unsigned char zp();
extern unsigned char zpi(unsigned char);
extern unsigned char abso();
extern unsigned char absi(unsigned char);
extern unsigned char indi();
extern void negativeflag(unsigned char);
extern void zeroflag(unsigned char);
extern void carryflag(unsigned char);
extern void overflowflag(unsigned char, unsigned char, unsigned char);
extern void sbcflags(unsigned char, unsigned char, unsigned char);
extern void comp(unsigned char, unsigned char);
extern short branches(short);
extern void printRegisters();