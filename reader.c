#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "features.h"
#include "reader.h"

int A = 0x00;
int X = 0x00;
int Y = 0x00;
short pc = 0x0100;
unsigned short sp = 0xFFF0;
unsigned char mem[8000];
unsigned char stack[0xFFF0];
int n; //neg flag
int z; //0 flag
int c; //carry flag
int v; //overflow flag
int instlength;
int endofprog = 0;

/*
 * readFile - reads file line by line until the end of the header
 * params - file name
 * return void
 */ 
void readFile(char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error! Could not open file!\n");
        exit(-1);
    }
    char line[500];
    int j = 0x100;
    fread(line, 1, 500, f);
    char* ptr = strstr(line, "ENDHEADER");
    ptr+=10;
    while(ptr != &line[499]) {
        mem[j] = *ptr;
        ptr++;
        j++;
    }
   
    while (fread(mem[j], 1, 4000, f) == 4000) {
        //weird loop
    }

    fclose(f);
}


/*
 * instructions - this function executes an instruction based on its input
 * params - opcode
 * return void
 */
void instructions(int inst) {
    unsigned char m;
    char s_m;
    short offset;
    unsigned char data;
    unsigned char loc;
    unsigned char byte1;
    short byte2;
    unsigned char zerobit;
    unsigned char temp;
    unsigned char ps;
    switch(inst) {
        case 0x69 : 
            //adc imm a+m+c->a,c
            //n z c v 
            m = imm();
            temp = A;
            A = m+A+c; 
            carryflag(A);
            zeroflag(A);
            negativeflag(A);
            overflowflag(A, m, temp);
            break;
        case 0x65:
            //zp
            m = zp();
            temp = A;
            A = m+A+c; 
            carryflag(A);
            zeroflag(A);
            negativeflag(A);
            overflowflag(A, m, temp);
            break;
        case 0x75:
            //zpx
            m = zpi('X');
            temp = A;
            A = m+A+c; 
            carryflag(A);
            zeroflag(A);
            negativeflag(A);
            overflowflag(A, m, temp);
            break;
        case 0x6D:
            //abs 
            m = abso();
            temp = A;
            A = m+A+c;  
            carryflag(A);
            zeroflag(A);
            negativeflag(A);
            overflowflag(A, m, temp);
            break;
        case 0x7D: 
            //absx
            m = absi('X');
            temp = A;
            A = m+A+c; 
            carryflag(A);
            zeroflag(A);
            negativeflag(A);
            overflowflag(A, m, temp);
            break;
        case 0x79: 
            //absy
            m = absi('Y');
            temp = A;
            A = m+A+c; 
            carryflag(A);
            zeroflag(A);
            negativeflag(A);
            overflowflag(A, m, temp); 
            break;
        case 0x61: 
            //(i,x)
            m = zpi('X');
            temp = A;
            A = m+A+c; 
            carryflag(A);
            zeroflag(A);
            negativeflag(A);
            overflowflag(A, m, temp);
            break;
        case 0x71: 
            //(i),y 
            m = indi();
            temp = A;
            A = m+A+c; 
            carryflag(A);
            zeroflag(A);
            negativeflag(A);
            overflowflag(A, m, temp);
            break;
        case 0x29: 
            //and imm a^m->a
            //n z
            m = imm();
            A = A&m;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0x25: 
            //zp 
            m = zp();
            A= A&m;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0x35: 
            //zpx
            m = zpi('X');
            A = A&m; 
            zeroflag(A);
            negativeflag(A);
            break;
        case 0x2D: 
            //abs
            m = abso();
            A = A&m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x3D: 
            //absx 
            m = absi('X');
            A = A&m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x39: 
            //absy
            m = absi('Y');
            A = A&m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x21: 
            //(i,x) 
            m = zpi('X');
            A = A&m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x31: 
            //(i),y
            m =  indi();
            A = A&m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x06: 
            //asl zp shift left one bit
            //n z c
            instlength = 2;
            pc++;
            data = mem[pc];
            int m = mem[data];
            carryflag(m);
            m = m << 1;
            mem[data] = m; 
            negativeflag(m);
            zeroflag(m);
            break;
        case 0x16: 
            //zpx
            instlength = 2;
            pc++;
            data = mem[pc];
            loc = data + X;
            if (loc>0xFF) {
                loc = loc%0xFF;
            }
            m = mem[loc];
            carryflag(m);
            m = m << 1;
            mem[loc] = m;
            negativeflag(m);
            zeroflag(m);
            break;
        case 0x0E: 
            //abs
            instlength = 3;
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2 = byte2<<8;
            m = mem[byte2];
            carryflag(m);
            m = m << 1;
            mem[byte2] = m;
            negativeflag(m);
            zeroflag(m);
            break;
        case 0x1E: 
            //absx
            instlength = 3;
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2 = byte2<<8;
            loc = mem[byte2] + X;
            m = mem[loc];
            carryflag(m);
            m = m << 1;
            mem[loc] = m;
            negativeflag(m);
            zeroflag(m);
            break;
        case 0x0A: 
            //acc
            instlength = 1;
            carryflag(A);
            A = A << 1;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x90: 
            //bcc rel branch if carry == 0
            s_m = imm();
            offset = branches(s_m);
            if (c == 0) {
                pc = pc + s_m;
            }
            break;
        case 0xB0: 
            //bcs rel branch if carry == 1
            s_m = imm();
            offset = branches(s_m);
            if (c == 1) {
                pc = pc + s_m;
                
            }
            break;
        case 0xF0: 
            //beq rel branch if z == 1
            s_m = imm();
            offset = branches(s_m);
            if (z == 1) {
                pc = pc + s_m;
                
            }
            break;
        case 0x24: 
            //bit zp
            instlength = 2;
            pc++;
            data = mem[pc];
            temp = A&data;
            v = (temp&0x40);
            n = (temp&0x80);
            break;
        case 0x2C: 
            //abs
            instlength = 3;
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2 = byte2<<8;
            byte2 += byte1;
            temp = A&byte2;
            v = (temp&0x40);
            n = (temp&0x40);
            break;
        case 0x30: 
            //bmi rel branch if n == 1
            s_m = imm();
            offset = branches(s_m);
            if (n == 1) {
                pc = pc + s_m; 
                
            }
            break;
        case 0xD0: 
            //bne rel
            s_m = imm();
            offset = branches(s_m);
            if (z == 0) {
                pc = pc + s_m;
                
            }
            break;
        case 0x10:
            //bpl rel
            s_m = imm();
            offset = branches(s_m);
            if (n == 0) {
                pc = pc + s_m;
                
            }
        case 0x50: 
            //bvc rel
            s_m = imm();
            offset = branches(s_m);
            if (v == 0) {
                pc = pc + s_m;
                
            }
            break;
        case 0x70: 
            //bvs rel
            s_m = imm();
            offset = branches(s_m);
            if (v == 1) {
                pc = pc + s_m;
                
            }
            break;
        case 0x18: 
            //clc imp
            instlength = 1;
            c = 0b0;
            break;
        case 0xB8: 
            //clv imp
            instlength = 1;
            v = 0b0;
            break;
        case 0xC9: 
            //cmp imm a-m
            //z c n
            m =  imm();
            comp(A, m);
            negativeflag((A-m));
            break;
        case 0xC5: 
            //zp
            m =  zp();
            comp(A, m);
            negativeflag((A-m));
            break;
        case 0xD5: 
            //zpx
            m =  zpi('X');
            comp(A, m);
            negativeflag((A-m));
            break;
        case 0xCD: 
            //abs
            m =  abso();
            comp(A, m);
            negativeflag((A-m));            
            break;
        case 0xDD: 
            //absx
            m =  absi('X');
            comp(A, m);
            negativeflag((A-m));
            break;
        case 0xD9: 
            //absy
            m =  absi('Y');
            comp(A, m);
            negativeflag((A-m));
            break;
        case 0xC1: 
            //(i,x)
            m =  zpi('X');
            comp(A, m);
            negativeflag((A-m));
            break;
        case 0xD1: 
            //(i),y
            m =  indi();
            comp(A, m);
            negativeflag((A-m));
            break;
        case 0xE0: 
            //cpx imm
            m =  imm();
            comp(X, m);
            negativeflag((X-m));
            break;
        case 0xE4: 
            //zp
            m =  zp();
            comp(X, m);
            negativeflag((X-m));
            break;
        case 0xEC: 
            //abs
            m =  abso();
            comp(X, m);
            negativeflag((X-m));
            break;
        case 0xC0: 
            //cpy imm
            m =  imm();
            comp(Y, m);
            negativeflag((Y-m));
            break;
        case 0xC4: 
            //zp
            m =  zp();
            comp(Y, m);
            negativeflag((Y-m));
            break;
        case 0xCC: 
            //abs
            m =  abso();
            comp(Y, m);
            negativeflag((Y-m));
            break;
        case 0xC6: 
            //dec zp
            m =  zp();
            m = m - 1;
            mem[data] = m;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0xD6: 
            //zpx
            m =  zpi('X');
            m = m - 1;
            mem[loc] = m;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0xCE: 
            //abs
            m =  abso();
            m = m - 1;
            mem[byte2] = m;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0xDE: 
            //absx
            m =  absi('X');
            m = m - 1;
            mem[loc] = m;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0xCA: 
            //dex imp
            instlength = 1;
            X = X - 1;
            zeroflag(X);
            negativeflag(X);
            break;
        case 0x88: 
            //dey imp
            instlength = 1;
            Y = Y - 1;
            zeroflag(Y);
            negativeflag(Y);
            break;
        case 0x49: 
            //eor imm A^M
            //z n
            m =  imm();
            A = A^m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x45: 
            //zp
            m =  zp();
            A = A^m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x55: 
            //zpx
            m =  zpi('X');
            A = A^m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x4D: 
            //abs 
            m =  abso();
            A = A^m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x5D: 
            //absx
            m =  absi('X');
            A = A^m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x59: 
            //absy
            m =  absi('Y');
            A = A^m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x41: 
            //(i,x)
            m =  zpi('X');
            A = A^m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x51: 
            //(i),y
            m =  indi();
            A = A^m;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0xE6: 
            //inc zp
            m =  zp();
            m = m + 1;
            zeroflag(m);
            negativeflag(m);
            mem[data] = m;
            break;
        case 0xF6: 
            //zpx
            m =  zpi('X');
            m = m + 1;
            zeroflag(m);
            negativeflag(m);
            mem[data] = m;
            break;
        case 0xEE: 
            //abs
            m =  abso();
            m = m + 1;
            zeroflag(m);
            negativeflag(m);
            mem[byte2] = m;
            break;
        case 0xFE: 
            //absx
            m =  absi('X');
            m = m + 1;
            zeroflag(m);
            negativeflag(m);
            mem[loc] = m;
            break;
        case 0xE8: 
            //inx imp
            instlength = 1;
            X++;
            zeroflag(X);
            negativeflag(X);
            break;
        case 0xC8: 
            //iny imp
            instlength = 1;
            Y++;
            zeroflag(Y);
            negativeflag(Y);
            break;
        case 0x4C: 
            //jmp abs
            instlength = 3;
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2 = byte2 <<8;
            byte2 += byte1;
            pc = byte2;
            
            break;
        case 0x6C: 
            //jmp ind
            instlength = 3;
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2 = byte2 <<8;
            byte2 += byte1;
            loc = mem[byte2];
            pc = loc;
            
            break;
        case 0x20: 
            //jsr abs
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2= byte2<<8;
            byte2+=byte1;
            pc-=instlength;
            temp = (pc&0xFF00) >> 8; //high order byte
            stack[sp] = temp;
            sp--;
            temp = pc&0x00FF; //low order byte
            stack[sp] = temp;
            pc = byte2; //pc is at opcode
            pc--; //end of instructions = pc++ so pc would be at the byte after the opcode
            instlength = 3;
            break;
        case 0xA9: 
            //lda imm
            m =  imm();
            A = m;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0xA5: 
            //zp
            m =  zp();
            A = m;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0xB5: 
            //zpx
            m =  zpi('X');
            A = m;
            zeroflag(A);
            negativeflag(A);  
            break;      
        case 0xAD: 
            //abs
            m =  abso();
            A = m;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0xBD: 
            //absx
            m =  absi('X');
            A = m;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0xB9: 
            //absy
            m =  absi('Y');
            A = m;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0xA1: 
            //(i,x)
            m =  zpi('X');
            A = m;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0xB1: 
            //(i),y
            m =  indi();
            A = m;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0xA2: 
            //ldx imm
            m =  imm();
            X = m;
            zeroflag(X);
            negativeflag(X);
            break;
        case 0xA6: 
            //zp
            m =  zp();
            X = m;
            zeroflag(X);
            negativeflag(X);
            break;
        case 0xB6: 
            //zpy
            m =  zpi('Y');
            X = m;
            zeroflag(X);
            negativeflag(X);
            break;
        case 0xAE: 
            //abs
            m =  abso();
            X = m;
            zeroflag(X);
            negativeflag(X);
            break;
        case 0xBE: 
            //absy
            m =  absi('Y');
            X = m;
            zeroflag(X);
            negativeflag(X);
            break;
        case 0xA0: 
            //ldy imm
            m =  imm();
            Y = m;
            zeroflag(Y);
            negativeflag(Y);
            break;
        case 0xA4: 
            //zp
            m =  zp();
            Y = m;
            zeroflag(Y);
            negativeflag(Y);
            break;
        case 0xB4: 
            //zpx
            m =  zpi('X');
            Y = m;
            zeroflag(Y);
            negativeflag(Y);
            break;
        case 0xAC: 
            //abs
            m =  abso();
            Y = m;
            zeroflag(Y);
            negativeflag(Y);
            break;
        case 0xBC: 
            //absx
            m =  absi('X');
            Y = m;
            zeroflag(Y);
            negativeflag(Y);
            break;
        case 0x4A: 
            //lsr acc
            instlength = 1;
            zerobit = A&0b1; 
            carryflag(zerobit);
            A = A>>1;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0x46: 
            //zp
            m =  zp();
            zerobit = m&0b1; 
            carryflag(zerobit);
            m = m >> 1;
            mem[data] = m; 
            negativeflag(m);
            zeroflag(m);
            break;
        case 0x56: 
            //zpx
            m =  zpi('X');
            zerobit = m&0b1; 
            carryflag(zerobit);
            m = m >> 1;
            mem[loc] = m;
            negativeflag(m);
            zeroflag(m);
            break;
        case 0x4E: 
            //abs
            m =  abso();
            zerobit = m&0b1;
            carryflag(zerobit);
            m = m >> 1;
            mem[byte2] = m;
            negativeflag(m);
            zeroflag(m);
            break;
        case 0x5E: 
            //absx
            m =  absi('X');
            zerobit = m&0b1;
            carryflag(zerobit);
            m = m >> 1;
            mem[loc] = m;
            negativeflag(m);
            zeroflag(m);
            break;
        case 0xEA: 
            //nop imp
            instlength = 1;
            //nothing
            break;
        case 0x09: 
            //ora imm
            m =  imm();
            A = A|m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x05: 
            //zp
            m =  zp();
            A = A|m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x15: 
            //zpx
            m =  zpi('X');
            A = A|m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x0D: 
            //abs
            m =  abso();
            A = A|m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x1D: 
            //absx
            m =  absi('X');
            A = A|m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x19: 
            //absy
            m =  absi('Y');
            A = A|m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x01: 
            //(i,x)
            m =  zpi('X');
            A = A|m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x11: 
            //(i),y
            m =  indi();
            A = A|m;
            negativeflag(A);
            zeroflag(A);
            break;
        case 0x48: 
            //pha imp
            instlength = 1;
            stack[sp] = A;
            sp--;
            break;
        case 0x08: 
            //php imp
            //order: c z v n 
            instlength = 1;
            ps = c;
            ps = ps << 3;
            temp = z << 2;
            ps += temp;
            temp = v << 1;
            ps += temp;
            ps += n;
            stack[sp] = ps;
            sp--;
            break;
        case 0x68: 
            //pla imp
            instlength = 1;
            sp++;
            A = stack[sp];
            stack[sp] = 0;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0x28:
            //plp imp
            //order: nvzc;
            instlength = 1;
            sp++;
            ps = stack[sp];
            n = ps&0b0001;
            v = (ps&0b0010) >> 1;
            z = (ps&0b0100) >> 2;
            c = (ps&0b1000) >> 3;
            stack[sp] = 0;
            break;
        case 0x2A: 
            //rol acc
            instlength = 1;
            temp = A&0x80;
            A = (A<<1)+c;
            c = temp;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0x26: 
            //zp
            m = zp();
            temp = m&0x80;
            m = (m<<1)+c;
            c = temp;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0x36: 
            //zpx
            m = zpi('X');
            temp = m&0x80;
            m = (m<<1)+c;
            c = temp;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0x2E: 
            //abs
            m = abso();
            temp = m&0x80;
            m = (m<<1)+c;
            c = temp;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0x3E: 
            //absx
            m = absi('X');
            temp = m&0x80;
            m = (m<<1)+c;
            c = temp;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0x6A: 
            //ror acc
            instlength = 1;
            temp = A&0b1;
            c = c << 7;
            A = (A>>1) + c;
            c = temp;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0x66: 
            //zp
            m = zp();
            temp = m&0b1;
            c = c << 7;
            m = (m>>1) + c;
            c = temp;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0x76: 
            //zpx
            m = zpi('X');
            temp = m&0b1;
            c = c << 7;
            m = (m>>1) + c;
            c = temp;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0x6E: 
            //abs
            m = abso();
            temp = m&0b1;
            c = c << 7;
            m = (m>>1) + c;
            c = temp;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0x7E: 
            //absx
            m = absi('X');
            temp = m&0b1;
            c = c << 7;
            m = (m>>1) + c;
            c = temp;
            zeroflag(m);
            negativeflag(m);
            break;
        case 0x60: 
            //rts imp
            instlength = 1;
            if (sp != 0xFFF0) {
                byte1 = stack[sp];
                stack[sp] = 0;
                sp++;
                byte2 = stack[sp];
                stack[sp] = 0;
                byte2 = byte2 << 8;
                byte2+= byte1;
                pc = byte2; //pc is at opcode
                pc--; //end of function moves pc up so it would not be at opcode anymore
            }
            endofprog = 1;
            break;
        case 0xE9: 
            //sbc imm
            m = imm();
            temp = A;
            A = A-m-(~c);
            zeroflag(A);
            negativeflag(A);
            sbcflags(A, m, temp);
            break;
        case 0xE5: 
            //zp
            m = zp();
            temp = A;
            A = A-m-(~c);
            zeroflag(A);
            negativeflag(A);
            break;
        case 0xF5: 
            //zpx
            m = zpi('X');
            temp = A;
            A = A-m-(~c);
            zeroflag(A);
            negativeflag(A);
            sbcflags(A, m, temp);
            break;
        case 0xED: 
            //abs
            m = abso();
            temp = A;
            A = A-m-(~c);
            zeroflag(A);
            negativeflag(A);
            sbcflags(A, m, temp);
            break;
        case 0xFD: 
            //absx
            m = absi('X');
            temp = A;
            A = A-m-(~c);
            zeroflag(A);
            negativeflag(A);
            sbcflags(A, m, temp);
            break;
        case 0xF9: 
            //absy
            m = absi('Y');
            temp = A;
            A = A-m-(~c);
            zeroflag(A);
            negativeflag(A);
            sbcflags(A, m, temp);
            break;
        case 0xE1: 
            //(i,x)
            m = zpi('X');
            temp = A;
            A = A-m-(~c);
            zeroflag(A);
            negativeflag(A);
            sbcflags(A, m, temp);
            break;
        case 0xF1: 
            //(i),y
            m = indi();
            temp = A;
            A = A-m-(~c);
            zeroflag(A);  
            negativeflag(A); 
            sbcflags(A, m, temp);         
            break;
        case 0x38: 
            //sec imp
            instlength = 1;
            c = 1;
            break;
        case 0x85: 
            //sta zp
            instlength = 2;
            pc++;
            data = mem[pc];
            mem[data] = A;
            break;
        case 0x95: 
            //zpx
            instlength = 2;
            pc++;
            data = mem[pc] + X;
            if (data > 0xFF) {
                data = data%0xFF;
            }
            mem[data] = A;
            break;
        case 0x8D: 
            //abs
            instlength = 3;
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2 = byte2<<8;
            byte2+=byte1;
            mem[byte2] = A;
            break;
        case 0x9D: 
            //absx
            instlength = 3;
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2 = byte2<<8;
            byte2+=byte1;
            byte2+=X;
            mem[byte2] = A;
            break;
        case 0x99: 
            //absy
            instlength = 3;
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2 = byte2<<8;
            byte2+=byte1;
            byte2+=Y;
            mem[byte2] = A;
            break;
        case 0x81: 
            //(i,x)
            instlength = 2;
            pc++;
            data = mem[pc] + X;
            if (data>0xFF)  {
                data = data%0xFF;
            }
            mem[data] = A;
            break;
        case 0x91: 
            //(i),y
            instlength = 2;
            pc++;
            data = mem[pc];
            loc = mem[data] + Y;
            mem[loc] = A;
            break;
        case 0x86: 
            //stx zp
            instlength = 2;
            pc++;
            data = mem[pc];
            mem[data] = X;
            break;
        case 0x96: 
            //zpy
            instlength = 2;
            pc++;
            data = mem[pc]+Y;
            if (data>0xFF) {
                data = data%0xFF;
            }
            mem[data] = X;
            break;
        case 0x8E: 
            //abs
            instlength = 3;
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2 = byte2 << 8;
            byte2+=byte1;
            mem[byte2] = X;
            break;
        case 0x84: 
            //sty zp
            instlength = 2;
            pc++;
            data = mem[pc];
            mem[data] = Y;
            break;
        case 0x94: 
            //zpx
            instlength = 2;
            pc++;
            data = mem[pc]+X;
            if (data>0xFF) {
                data = data%0xFF;
            }
            mem[data] = Y;
            break;
        case 0x8C: 
            //abs
            instlength = 3;
            pc++;
            byte1 = mem[pc];
            pc++;
            byte2 = mem[pc];
            byte2 = byte2 << 8;
            byte2+=byte1;
            mem[byte2] = Y;
            break;
        case 0xAA: 
            //tax imp
            instlength = 1;
            X = A;
            zeroflag(X);
            negativeflag(X);
            break;
        case 0xA8: 
            //tay imp
            instlength = 1;
            Y = A;
            zeroflag(Y);
            negativeflag(Y);
            break;
        case 0xBA: 
            //tsx imp
            instlength = 1;
            X = sp;
            zeroflag(X);
            negativeflag(X);
            break;
        case 0x8A: 
            //txa imp
            instlength = 1;
            A = X;
            zeroflag(A);
            negativeflag(A);
            break;
        case 0x9A: 
            //txs imp
            instlength = 1;
            sp = X;  
            break;
        case 0x98: 
            //tya imp
            instlength = 1;
            A = Y;
            zeroflag(A);
            negativeflag(A);
            break; 
    }
    pc++;
}

/*
 * imm - this function obtains the content in memory based on the mode immediate
 * param none
 * return unsigned char - value of memory 
 */
unsigned char imm() {
    instlength = 2;
    pc++;
    return mem[pc];
}

/*
 * zp - this function obtains the content in memory based on the mode zero page
 * param none
 * return unsigned char - value of memory 
 */
unsigned char zp() {
    instlength = 2;
    pc++;
    unsigned char data = mem[pc];
    return mem[data];
}

/*
 * zpi - this function obtains the content in memory based on the modes zero page indexed (x and y)
 *  and indexed indirect
 * param unsigned char - is the register x or y?
 * return unsigned char - value of memory 
 */
unsigned char zpi(unsigned char r) {
    instlength = 2;
    pc++;
    unsigned char data = mem[pc];
    unsigned char loc;
    if (r == 'X') {
        loc = data + X;
    } else if (r == 'Y') {
        loc = data + Y;
    }
    if (loc>0xFF) {
        loc = loc%0xFF;
    }
    return mem[loc];
}

/*
 * abso - this function obtains the content in memory based on the mode absolute
 * param none
 * return unsigned char - value of memory 
 */
unsigned char abso() {
    instlength = 3;
    pc++;
    unsigned char byte1 = mem[pc];
    pc++;
    unsigned char byte2 = mem[pc];
    byte2 = byte2 << 8;
    byte2 += byte1;
    return mem[byte2];
}

/*
 * absi - this function obtains the content in memory based on the mode absolute indexed (x and y)
 * param unsigned char - is the register x or y?
 * return unsigned char - value of memory 
 */
unsigned char absi(unsigned char r) {
    instlength = 3;
    pc++;
    unsigned char byte1 = mem[pc];
    pc++; 
    unsigned char byte2 = mem[pc];
    byte2 = byte2 << 8;
    byte2 += byte1;
    unsigned char loc;
    if (r == 'X') {
        loc = X+byte2;
    } else if (r == 'Y') {
        loc = Y+byte2;
    }
    return mem[loc];    
}

/*
 * indi - this function obtains the content in memory based on the mode indirect indexed
 * param none
 * return unsigned char - value of memory 
 */
unsigned char indi() {
    instlength = 2;
    pc++;
    unsigned char data = mem[pc];
    unsigned char loc = mem[data] + Y;
    return mem[loc];
}

/*
 * negativeflag - this function sets or unsets the negative flag based on the input
 * param unsigned char - value that is being checked
 * return void
 */
void negativeflag(unsigned char r) {
    char c = r;
    if (c < 0) {
        n = 0b1;
    } else {
        n = 0b0;
    }
}

/*
 * zeroflag - this function sets or unsets the zero flag based on the input
 * param unsigned char - value that is being checked
 * return void
 */
void zeroflag(unsigned char r) {
    if (r == 0) {
        z = 0b1;
    } else {
        z = 0b0;
    }
}

/*
 * carryflag - this function sets or unsets the carry flag based on the input
 * param unsigned char - value that is being checked
 * return void
 */
void carryflag(unsigned char r) {
    if (r >= 255) {
        c = 0b1;
    } else {
        c = 0b0;
    } 
}

/*
 * overflowflag - this function sets or unsets the overflow flag based on the input
 * param unsigned char - value that is being checked
 * return void
 */
void overflowflag(unsigned char r, unsigned char m, unsigned char a) {
    if ((m&0x70 == 0x70 && a&0x70 == 0x70 && r&0x80 == 0x80)) {
        v = 0b1;
    } else {
        v = 0b0;
    }
}

/*
 * sbcflags - this function sets the carry and overflow flags based on the conditions set
 *   by the sbc instruction
 * param result of sbc instruction
 * param value of memory being used in sbc instruction
 * param value of accumulator being in sbc instruction
 * return void
 */
void sbcflags(unsigned char r, unsigned char m, unsigned char a) {
    if (r&0x80==0x80) {
        c = 0b0;
    } 

    if ((r&0x80 != 0x80 && m&0x80 == 0x80 && a&0x80 == 0x80) || (r&0x80 == 0x80 && m&0x80 != 0x80 && a&0x80 != 0x80) || (a >= m && r&0x80 == 0x80) || (m > a && r&0x80 != 0x80)) {
        v = 0b1;
    } else {
        v = 0b0;
    }
}

/*
 * comp - this function sets/unsets the zero and carry flags based on the conditions specified by the
 *   cmp, cpx, and cpy instructions
 * param register being compared to value of memory
 * param value of memory being compared to register
 * return void
 */
void comp(unsigned char r, unsigned char m) {
    if (r == m) {
        z = 0b1;
    } else if (m <= r) {
        c = 0b1;
        z = 0b0;
    } else if (m > r) {
        c = 0b0;
        z = 0b0;
    }
}

short branches(short n) {
    short ans;
    char bit7 = n&0x80;
    if (bit7 == 0x80) {
        ans = 0xFF00 + n;
        return ans;
    } else {
        ans = 0x0000 + n;
        return ans;
    }
}
/*
 * printRegisters - this function sets up a print statement with all the registers for 
 *   the print function in features.c
 * param none
 * return void
 */
void printRegisters() {
    printf("pc: %d\nX: %d\nY: %d\nA: %d\n", pc, X, Y, A);
}