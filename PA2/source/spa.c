#include <stdio.h>
#include <nds.h>
#include <PA_lib.h>
u8 T_BG_L = 0;
u8 S_BG_L = 0;

#define PA_UPDATEPAD(pad, var) do{ \
        Pad.pad.A = (var & KEY_A) != 0; \
        Pad.pad.B = (var & KEY_B) != 0; \
        Pad.pad.X = (var & KEY_X) != 0; \
        Pad.pad.Y = (var & KEY_Y) != 0; \
        Pad.pad.L = (var & KEY_L) != 0; \
        Pad.pad.R = (var & KEY_R) != 0; \
        Pad.pad.Select = (var & KEY_SELECT) != 0; \
        Pad.pad.Start = (var & KEY_START) != 0; \
        Pad.pad.Up = (var & KEY_UP) != 0; \
        Pad.pad.Down = (var & KEY_DOWN) != 0; \
        Pad.pad.Left = (var & KEY_LEFT) != 0; \
        Pad.pad.Right = (var & KEY_RIGHT) != 0; \
        Pad.pad.Anykey = (var & 0xFFF) != 0; \
    }while(0)

//SPA_Init Function
void PA_Init(){
        consoleDemoInit(); //initializes the text
        
        PA_SetVideoMode(0, 0); 
        PA_SetVideoMode(1, 0);
        PA_SetRoot("NITROFS"); //Sets root in NITROFS
        PA_InitBgBuffers(); //inits bg buffers
        PA_InitBg(0); //inits bg on bottom screen
        PA_InitBg(1); //inits bg on bottom screen
        PA_InitSpriteBuffers(); //inits sprite buffers
        PA_InitSprite(0); //inits sprite on bottom screen
        PA_InitSprite(1);
        PA_InitMapBuffers(); //inits map buffers
        PA_InitSoundBuffers(); //inits sound buffers
}
void PA_InitVBL(){
    //... Do nothing because PA_InitVBL(); is useless. I may give a use to this after all lol.
}
void PA_InitText(u8 screen, u8 layer){
    switch(screen){
    case 0:
    if(T_BG_L != 0) while(1) swiWaitForVBlank(); //if the thing is reinitialized then hang forever.
    NF_InitText(1);
    PA_LoadFont("default", "default", 256, 256, 1);
    PA_CreateTextLayer(1, layer, 0, "default");
    T_BG_L = layer;
    break;
    case 1:
    if(S_BG_L != 0) while(1) swiWaitForVBlank(); //if the thing is reinitialized then hang forever.
    S_BG_L = layer;
    NF_InitText(0);
    PA_LoadFont("default", "default", 256, 256, 0);
    PA_CreateTextLayer(0, layer, 0, "default");
    break;
    }
}
void PA_ClearTextBg(u8 screen){
    switch(screen){
        case 1: PA_ClearTextBgSys(0,T_BG_L); break;
        case 0: PA_ClearTextBgSys(1,S_BG_L); break;
    }
}
void PA_OutputText(u8 screen, int x, int y, const char *text){
    switch(screen){
        case 0: PA_WriteText(1,T_BG_L,x,y,text);
        break;
        case 1: PA_WriteText(0,S_BG_L,x,y,text);
        break;
    }
}
void PA_OutputSimpleText(u8 screen, int x, int y, const char *text){
    switch(screen){
        case 0: PA_WriteText(1,T_BG_L,x,y,text);
        break;
        case 1: PA_WriteText(0,S_BG_L,x,y,text);
        break;
    }
}
void PA_SetTextCol(u8 screen, u16 r, u16 g, u16 b){
        switch(screen){
                case 0: PA_DefineTextColor(1,T_BG_L,0,r,g,b);
                        PA_SetTextColor(1,T_BG_L,0);
                        break;
                case 1:
                        PA_DefineTextColor(0,S_BG_L,0,r,g,b);
                        PA_SetTextColor(0,S_BG_L,0);
                        break;
        }
}
//SPA_WaitFor Function
void PA_WaitFor(int sec){
        int i = 0; //inits a variable.
        //run a loop for 100 frames
        for (i = 0; i < sec * 100; i++) swiWaitForVBlank(); //Waits for 60 frames...
}
//SPA_LoadSprite function
//    void PA_LoadSprite(int screen, int id, int width, int height, const char *dir, bool transflag) {
void PA_LoadSprite(u8 screen, int id, int size, const char *dir){
        switch(size){
            case 88: 
        PA_LoadSpriteGfx(dir, id, 8, 8); //loads sprite gfx
        PA_LoadSpritePal(dir, id); //loads sprite pal
        PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
        PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
        break;
            case 816:
            PA_LoadSpriteGfx(dir, id, 8, 16); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 832:
            PA_LoadSpriteGfx(dir, id, 8, 32); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 168:
            PA_LoadSpriteGfx(dir, id, 16, 8); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 1616:
            PA_LoadSpriteGfx(dir, id, 16, 16); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 1632:
            PA_LoadSpriteGfx(dir, id, 8, 32); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 328:
            PA_LoadSpriteGfx(dir, id, 32, 8); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 3216:
            PA_LoadSpriteGfx(dir, id, 32, 16); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 3232:
            PA_LoadSpriteGfx(dir, id, 32, 32); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 3264:
            PA_LoadSpriteGfx(dir, id, 32, 64); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 6432:
            PA_LoadSpriteGfx(dir, id, 64, 32); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case 6464:
            PA_LoadSpriteGfx(dir, id, 64, 64); //loads sprite gfx
            PA_LoadSpritePal(dir, id); //loads sprite pal
            PA_VramSpriteGfx(screen, id, id, false); //loads sprite gfx on VRAM
            PA_VramSpritePal(screen, id, id); //loads sprite pal on VRAM
            break;
            case default:
            consoleDemoInit();
            consoleClear();
            iprintf("DO NOT MESS WITH THE SPRITE SIZE. Error, Hang forever");
            while(1) swiWaitForVBlank();
            break;
        }
}
//SPA_Flush
void PA_WaitForVBL() {
    scanKeys();
    PA_SpriteOamSet(1);//set sprite oam to 1
    PA_SpriteOamSet(0);//set sprite oam to 0
    swiWaitForVBlank(); //waits for vblank
    PA_UpdateTextLayers();//update text layers
    oamUpdate(&oamSub);//updates oam
    oamUpdate(&oamMain);//update oam
    PA_UPDATEPAD(Held, keysHeld());
    PA_UPDATEPAD(Newpress, keysDown());
    PA_UPDATEPAD(Released, keysUp());
}
