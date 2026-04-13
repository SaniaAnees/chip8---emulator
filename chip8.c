#include<stdio.h>
#include<stdint.h>
#include<string.h>
#include<stdlib.h>
#include<SDL2/SDL.h>
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
uint8_t KP[16];    //Keypad 0-F      //pixel is either 1 or 0 ie is in bits but c cant express in bits so we allocate 1 byte for a pixel- waste of storage but simple
}chip8;   //here chip8 is structure data type
//font
uint8_t font[]={
    0xF0,0x90,0x90,0x90,0xF0, // 0
    0x20,0x60,0x20,0x20,0x70, // 1
    0xF0,0x10,0xF0,0x80,0xF0, // 2
    0xF0,0x10,0xF0,0x10,0xF0, // 3
    0x90,0x90,0xF0,0x10,0x10, // 4
    0xF0,0x80,0xF0,0x10,0xF0, // 5
    0xF0,0x80,0xF0,0x90,0xF0, // 6
    0xF0,0x10,0x20,0x40,0x40, // 7
    0xF0,0x90,0xF0,0x90,0xF0, // 8
    0xF0,0x90,0xF0,0x10,0xF0, // 9
    0xF0,0x90,0xF0,0x90,0x90, // A
    0xE0,0x90,0xE0,0x90,0xE0, // B
    0xF0,0x80,0x80,0x80,0xF0, // C
    0xE0,0x90,0x90,0x90,0xE0, // D
    0xF0,0x80,0xF0,0x80,0xF0, // E
    0xF0,0x80,0xF0,0x80,0x80  // F
};
//init function
void init(chip8*c8){
    memset(c8,0,sizeof(chip8)); //memset fills a block of memory with a value you provided in second argument  
    c8->PC=0x200;
    for(int i=0;i<80;i++){
        c8->memory[0x050+i]=font[i];
    }
}
//load rom
void load_rom(chip8*c8,const char*filename)  //cont char* means the string wont be modified
{   FILE*file;
    file=fopen(filename,"rb");
    fread(c8->memory+0x200,1,3584,file); //c8->memory[0x200] , 4096-512(font data + reserved unused space)=3584
    fclose(file);
}
// MAIN FUNCTION
int main(int argc,char*argv[]){
chip8 c8; //struct variable 
init(&c8);
load_rom(&c8,"Pong [Paul Vervalin, 1990].ch8");
int X,Y;
int add,N;
int xpos,ypos;
uint16_t Add;
int i=0;
SDL_Init(SDL_INIT_VIDEO);                     //starts the SDL system
SDL_Window* window=SDL_CreateWindow("THE SCREEN",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,640,320,0);  //This function creates and opens new window on your screen with title "THE SCREEN",centered,640*320 pixels.
SDL_Renderer* renderer=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);  //Takes your window and creates a drawing surface inside it.
// -1 above means let SDL automatically pick up the best rendering driver available on your system   --SDL_RENDERER_SOFTWARE = force software rendering
while(1){
    SDL_Event event;         //// declare a variable to hold whatever event happened
    while(SDL_PollEvent(&event)){            // keep checking for events; fills 'event' with what happened
    if(event.type == SDL_QUIT){              // if the event was "user clicked X"--close button
        exit(0);                              //// close the program
    }
    if(event.type == SDL_KEYDOWN){
        switch(event.key.keysym.sym){
            case SDLK_x:
            c8.KP[0]=1;
            break;
            case SDLK_1:
            c8.KP[1]=1;
            break;
            case SDLK_2:
            c8.KP[2]=1;
            break;
            case SDLK_3:
            c8.KP[3]=1;
            break;
            case SDLK_q:
            c8.KP[4]=1;
            break;
            case SDLK_w:
            c8.KP[5]=1;
            break;
            case SDLK_e:
            c8.KP[6]=1;
            break;
            case SDLK_r:
            c8.KP[7]=1;
            break;
            case SDLK_a:
            c8.KP[8]=1;
            break;
            case SDLK_s:
            c8.KP[9]=1;
            break;
            case SDLK_d:
            c8.KP[0xA]=1;
            break;
            case SDLK_f:
            c8.KP[0xB]=1;
            break;
            case SDLK_z:
            c8.KP[0xC]=1;
            break;
            case SDLK_c:
            c8.KP[0xD]=1;
            break;
            case SDLK_v:
            c8.KP[0xE]=1;
            break;
            case SDLK_4:
            c8.KP[0xF]=1;
            break;
            }
        }
    if(event.type==SDL_KEYUP){
        switch(event.key.keysym.sym){
            case SDLK_x:
            c8.KP[0]=0;
            break;
            case SDLK_1:
            c8.KP[1]=0;
            break;
            case SDLK_2:
            c8.KP[2]=0;
            break;
            case SDLK_3:
            c8.KP[3]=0;
            break;
            case SDLK_q:
            c8.KP[4]=0;
            break;
            case SDLK_w:
            c8.KP[5]=0;
            break;
            case SDLK_e:
            c8.KP[6]=0;
            break;
            case SDLK_r:
            c8.KP[7]=0;
            break;
            case SDLK_a:
            c8.KP[8]=0;
            break;
            case SDLK_s:
            c8.KP[9]=0;
            break;
            case SDLK_d:
            c8.KP[0xA]=0;
            break;
            case SDLK_f:
            c8.KP[0xB]=0;
            break;
            case SDLK_z:
            c8.KP[0xC]=0;
            break;
            case SDLK_c:
            c8.KP[0xD]=0;
            break;
            case SDLK_v:
            c8.KP[0xE]=0;
            break;
            case SDLK_4:
            c8.KP[0xF]=0;
            break;
        }
    }
    }
    uint16_t opcode=(c8.memory[c8.PC])<<8|c8.memory[c8.PC+1];  //opcode is 16 bits long
    c8.PC+=2;
    switch(opcode & 0xF000){
        case 0x0000:
        if(opcode==0x00EE){ //return from sub routine
            c8.SP--;
            c8.PC=c8.stack[c8.SP];
        }
        if (opcode==0x00E0){  //clear screen
            memset(c8.display,0,sizeof(c8.display));
        };
        break;
        case 0x2000: // call sub routine
        c8.stack[c8.SP]=c8.PC;  // save current PC onto the stack
        c8.SP++;                //move stack pointer up
        c8.PC= opcode & 0x0FFF; //Start executing the subroutine
        break;
        case 0x3000:          //skip if V[X]==NN
        X=(opcode & 0x0F00)>>8;
        add=opcode & 0x00FF;
        if(c8.V[X]==add){
            c8.PC+=2;
        }
        break;               
        case 0x4000:         //skip if V[X]!=NN
        X=(opcode & 0x0F00)>>8;
        add=opcode & 0x00FF;
        if(c8.V[X] != add){
            c8.PC+=2;
        }
        break;   
        case 0x5000:         //skip if V[X]==V[Y]
        X=(opcode & 0x0F00)>>8;
        Y=(opcode & 0x00F0)>>4;
        if(c8.V[X]==c8.V[Y]){
            c8.PC+=2;
        }
        break;
        case 0x9000:    //SKIP IF V[X]!=V[Y]
        X=(opcode & 0x0F00)>>8;
        Y=(opcode & 0x00F0)>>4;
        if(c8.V[X]!=c8.V[Y]){
            c8.PC+=2;
        }
        break;
        case 0x8000:
        X=(opcode & 0x0F00)>>8;
        Y=(opcode & 0x00F0)>>4;
        switch(opcode & 0x000F){
            case 0x0:    //set V[X]=V[Y]
            c8.V[X]=c8.V[Y];
            break;
            case 0x1:   //set V[X] to Bitwise or of V[X] | V[Y]
            c8.V[X]=(c8.V[X]|c8.V[Y]);
            break;
            case 0x2:   //set V[X] to Bitwise and of V[X] & V[Y]
            c8.V[X]=(c8.V[X] & c8.V[Y]);
            break;
            case 0x3:   //set V[X] to Bitwise xor of V[X] ^ V[Y]
            c8.V[X]=(c8.V[X]^c8.V[Y]);
            break;
            case 0x4:   //set V[X]=V[X]+V[Y]
            if((c8.V[X]+c8.V[Y])>255){
                c8.V[0xF]=1;
            }
            else{
                c8.V[0xF]=0;
            }
            Add=c8.V[X]+c8.V[Y];   //If add is >255 only the leftmost 8 bits will be kept and the remaining will be truncated because of uint8_t
            c8.V[X]=Add;
            break;
            case 0x5:   //set V[X]=V[X]-V[Y]
            if(c8.V[X]<c8.V[Y]){
                c8.V[0xF]=0;       // subtraction failed ->VF=0,uint8_t can only hold 0 to 255. When a result goes negative, C wraps it by adding 256.so if v[x]=-192 then uint8_t will hold -192+256=64(wraparound)
            }
            if(c8.V[X]>=c8.V[Y]){
                c8.V[0xF]=1;
            }
            c8.V[X]=c8.V[X]-c8.V[Y];
            break;
            case 0x6:       // V[X]=V[X]>>1
            c8.V[0xF]=c8.V[X] & 0x01;
            c8.V[X]=c8.V[X]>>1;
            break;
            case 0x7:      //V[X]=V[Y]-V[X]
            if(c8.V[X]>c8.V[Y]){
                c8.V[0xF]=0;
            }
            if(c8.V[X]<=c8.V[Y]){
                c8.V[0xF]=1;          //subtraction sucessfull
            }
            c8.V[X]=c8.V[Y]-c8.V[X];
            break;
           case 0xE:     //c8.V[X]=c8.V[X]<<1
            if((c8.V[X] & 0x80)!=0){
            c8.V[0xF]=1;
            }
            else{
                c8.V[0xF]=0;
            }
            c8.V[X]=c8.V[X]<<1;
            break;
        }
        break;
        case 0xB000:         //jump PC to address NNN + whatever value is in register V[0]
        add=opcode & 0x0FFF;
        c8.PC=add+c8.V[0];
        break;
        case 0xC000:         //put any random number in v[x] but wrapped by NN to limit it in 0-255
        add=opcode & 0x00FF;
        X=(opcode & 0x0F00)>>8;
        c8.V[X]=rand() & add;
        break;
        case 0xE000:    // skip if key is pressed or not 
        X=(opcode & 0x0F00)>>8;
        switch(opcode & 0x00FF){
        case 0x009E:    //skip if v[x] is already pressed
        if(c8.KP[c8.V[X]]==1){
            c8.PC+=2;
        }
        break;
        case 0x00A1:    //skip if V[X] is not pressed
        if(c8.KP[c8.V[X]]==0){
            c8.PC+=2;
        }
        break;
        }
        break;
        case 0xF000:
        X=(opcode & 0x0F00)>>8;
        switch(opcode & 0x00FF){
        case 0x07:     //set v[x]=delay //  — used by games to check if a cooldown/wait has finished
        c8.V[X]=c8.delay;
        break;
        case 0x15:     //set delay=v[x]-Set delay timer = VX — starts a countdown, used for timing events like shoot cooldowns
        c8.delay=c8.V[X];
        break;
        case 0x18:   //set sound=v[x]-Set sound timer = VX — buzzer beeps while this counts down, used for sound effects
        c8.sound=c8.V[X];
        break;
        case 0x1E:    //set I=I+V[X]---I += VX — moves I forward in memory, used to walk through sprite or data arrays
        c8.I+=c8.V[X];
        break;
        case 0x29:
        c8.I=0x050+(c8.V[X]*5);   //I= starts at 0x050+ N(HERE WHATEVER IS STORED IN V[X])*5(EACH FONT IS 5 BYTES TALL)--I is a pointer to address---I = font sprite address for digit VX — points to the correct font so games can draw scores
        break;
        case 0x33:  //BCD: splits VX into hundreds/tens/ones digits — used with FX29 to display score numbers
        add=c8.V[X];
        c8.memory[c8.I]=add/100;
        c8.memory[c8.I+1]=(add/10)%10;
        c8.memory[c8.I+2]=add%10;
        break;
        case 0x55:   //Store V0-VX into memory at I — saves register state before subroutine callsv
        for(i=0;i<=X;i++){
            c8.memory[c8.I+i]=c8.V[i];
        }
        break;
        case 0x65:     //Load V0-VX from memory at I — restores register state after subroutine returns
        for(i=0;i<=X;i++){
           c8.V[i]=c8.memory[c8.I+i];
        }
        break;
        case 0x0A:{       //// Wait for keypress — halts execution until a key is pressed, stores key value in VX
        int found=0;
        for(i=0;i<16;i++){
            if(c8.KP[i]==1){
                c8.V[X]=i;
                found=1;
            }
        }
        if(found==0){
            c8.PC-=2;
        }
        break;
        }
        }
        break;
        case 0x1000:          // Jump to address NNN
        c8.PC= opcode & 0x0FFF;        
        break;
        case 0x6000:         // here set register V[X] to address NN
        X= (opcode & 0x0F00)>>8;
        c8.V[X]=opcode & 0x00FF;
        break;
        case 0x7000:        // here NN is added to V[X]
        add = opcode & 0x00FF;
        X=(opcode & 0x0F00)>>8;
        c8.V[X]+=add;
        break;
        case 0xA000:          // Set Register A to addresses NNN
        c8.I=opcode & 0x0FFF;
        break;
        case 0xD000:
        X=(opcode & 0x0F00)>>8;
        Y=(opcode & 0x00F0)>>4;
        N=(opcode & 0x000F);
        xpos=c8.V[X]%64;   
        ypos=c8.V[Y]%32;
        c8.V[0xF]=0;  // sets V[0xF]=0
        for(int i=0;i<N;i++){
            uint8_t sprite_byte = c8.memory[c8.I+i];  //I points to where the sprite data starts in memory. Each row of the sprite is one byte.(c8.I + i) just walks through each row of the sprite data
            for(int j=0;j<8;j++){
                if((sprite_byte & (0x80>>j))!=0){
                    xpos=(c8.V[X]+j)%64;
                    ypos=(c8.V[Y]+i)%32;
                    int index=(ypos*64)+xpos;
                    if(c8.display[index] == 1) {
                    c8.V[0xF] = 1;           //collision detected  ->VF=1
                }
                c8.display[index]^=1;
                }
            }
        }
        break;
        default:printf("unkown opcode : 0x%X\n",opcode);
        break;
    }
   if(c8.delay>0){
    c8.delay--;
   }
   if(c8.sound>0){
    c8.sound--;
   }
   SDL_Delay(16); //pauses instruction for 16 milliseconds-stable to achieve 500fps
   SDL_SetRenderDrawColor(renderer,20,20,60,255);      //sets color toc navy(20,20,60) for the renderer
   SDL_RenderClear(renderer);   //this fills the renderer with navy
   for(int i=0;i<32;i++){
    for(int j=0;j<64;j++){
        if(c8.display[(i*64)+j]==1){
            SDL_SetRenderDrawColor(renderer,200,150,255,255);   //tells renderer -the next thing you draw -use this colour -renderer (the drawing surface),200-150-255 (rgb colour for light purple),255-alpha(opacity)fully visible [Alpha — transparency. 255 = fully visible, 0 = invisible.]
            SDL_Rect rect;
            rect.x=j*10;
            rect.y=i*10;
            rect.w=10;
            rect.h=10;
            SDL_RenderFillRect(renderer,&rect);
        }
    }
   }
   SDL_RenderPresent(renderer);
}
}