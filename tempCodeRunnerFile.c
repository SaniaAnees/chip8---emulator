#include<stdio.h>
#include<stdint.h>
#include<string.h>

//CHIP-8 STRUCT
typedef struct {
uint8_t memory[4096];   
uint8_t V[16];         //here each register is only of one byte 
uint16_t PC;
uint16_t I;
uint16_t stack[16];  //here stack is used to call subroutines , it can be nested so to limit it we keep size of stack as 16 so it can hold only addresses of 16 subroutines
uint8_t sound;
uint8_t delay;
uint8_t SP;
uint8_t display[64*32]; //here we can also represent it as a matrix like unit8_t[64][32] but it would increase the loops in DXYN
uint8_t KP[16];    //Keypad 0-F
}chip8;   //here chip8 is structure data type

// MAIN FUNCTION
int main(){
chip8 c8;
memset(&c8,0,sizeof(c8));  //memset fills a block of memory with a value you provided in second argument  
c8.PC=0x200;
return 0;
}