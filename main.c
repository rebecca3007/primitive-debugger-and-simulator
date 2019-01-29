/*
 * the main.c file is responsible for reading in the name of the executable file 
 * as well as accepting and processing inputs from the user. For descriptions of the
 * other .c file see their respective .h files
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "features.h"
#include "reader.h"
extern void run(char*);

void main(int argc, char* argv []) {
    run(argv[1]);
}

void run(char* filename) {
    readFile(filename);
    
    char buffer[10];
    char input;
    help();

    while((input != 'X' && input != 'x') && endofprog!=1) {
        printf("type in a command: ");
        fgets(buffer, 10, stdin);
        input = buffer[0]; 
        
        if (input == 's' || input == 'S') { 
            step();
        } else if (input == 'p' || input == 'P') {
            print();
        } else if (input == 'c' || input == 'C') {
            char location[4]; //pulls the location to continue to from the buffer
            location[0] = buffer[2];
            location[1] = buffer[3]; 
            location[2] = buffer[4]; 
            location[3] = buffer[5];  
            cont(location);
        } else if (input == 'e' || input == 'E') {
            char location[4]; //pulls the location to examinea to from the buffer
            location[0] = buffer[2];
            location[1] = buffer[3]; 
            location[2] = buffer[4]; 
            location[3] = buffer[5];
            examine(location);
        } else if (input == 'h' || input == 'H') {
            help();
        } else if (input == 'm' || input == 'M') {
            char location[4]; //pulls the location to modify from the buffer
            location[0] = buffer[2];
            location[1] = buffer[3]; 
            location[2] = buffer[4]; 
            location[3] = buffer[5];
            char value[4]; //pulls the location the value to modify to from the buffer
            value[0] = buffer[7];
            value[1] = buffer[8];
            value[2] = buffer[9];
            value[3] = buffer[10];
            modify(location, value);
        } else if (input != 'x' && input != 'X') {
            printf("invalid command. try again\n");
        }
    }
}

