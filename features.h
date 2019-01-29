/* 
 * the features.c file contains the functions for the programs "debugger". A user has the 
 * possibility to print out all the commands, along with a little description in case they 
 * forget what all the commands are/what they do. The other features include: a command to 
 * step through a single instruction of the program; a command to print all the registers
 * minus the stack pointer; a command to continue executing the program until a given location;
 * a command to examine (print out) the value (1 byte) at a given location in memory; a command
 * to manually modify the value of a specified location in memory.
 */

extern void help();
extern void step();
extern void print();
extern void cont(char*);
extern void examine(char*);
extern void modify(char*, char*);