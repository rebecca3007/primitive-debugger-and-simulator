#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "features.h"
#include "reader.h"

/*
 * help - prints out all valid commands
 * params - none
 * return - void
 */
void help() {
    printf("valid commands: \n");
    printf("x - exit the program\n");
    printf("s - single step through one instruction\n");
    printf("p - print the values of the program counter and registers X, Y and A\n");
    printf("c - continue until a specified location (16-bit hex #)\n");
    printf("e - examine the value at a specified location (16-bit hex)\n");
    printf("m - modify the value (8-bit hex #) at a specified location (16-bit hex #)\n");
    printf("h - print out all valid commands\n");
    printf("(no hex value should include '0x'\nin case a hex number does not reach the specified bit number, pad with zeros)\n");
}

/*
 * step - single steps through one instruction
 * params - none
 * return - void
 */
void step() {
    instructions(mem[pc]);
}

/*
 * print - prints the values of the program counter, the X and Y registers,
 *         and the current location  
 * params - none
 * return - void
 */
void print() {
    printRegisters();
}

/*
 * cont - continues until specified location
 * params char* - memory address you want to continue to
 * return - void
 */
void cont(char* loc) {
    short addr;
    addr = strtoul(loc, 0, 16); 
    while (pc != addr) {
        step();
    }
    pc++;
}

/*
 * examine - prints out 1 byte at the given location
 * params char* - memory address to examine
 * return - void
 */
void examine(char* loc) {
    short addr;
    addr = strtoul(loc, 0, 16);
    char byte = mem[addr];
    printf("%X\n", byte);
}

/*
 * modify - modifies the value of a specified memory location
 * params char* - memory address to modify
 * params char - value to change to
 * return - void
 */
void modify(char* loc, char* val) {
    short addr;
    addr = strtoul(loc, 0, 16);
    unsigned char value = atoi(val);
    mem[addr] = value;
}