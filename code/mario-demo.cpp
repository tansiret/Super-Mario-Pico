#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "st7735/ST7735_TFT.hpp"
#include "st7735/ST7735_TFT_Bitmap_Data.hpp"

// Globals
ST7735_TFT myTFT;

// Peripherals
#define BTN_1 12
#define BTN_2 13
#define BTN_3 14
#define BTN_4 15
#define BUZZ 1

// Graphics
#define BACKGR 0x0337

// Map
#define MAPH 7
#define MAPW 40

// Musical note frequencies (Hz)
const int B0  = 31;
const int C1  = 33;
const int CS1 = 35;
const int D1  = 37;
const int DS1 = 39;
const int E1  = 41;
const int F1  = 44;
const int FS1 = 46;
const int G1  = 49;
const int GS1 = 52;
const int A1  = 55;
const int AS1 = 58;
const int B1  = 62;
const int C2  = 65;
const int CS2 = 69;
const int D2  = 73;
const int DS2 = 77;
const int E2  = 82;
const int F2  = 87;
const int FS2 = 93;
const int G2  = 98;
const int GS2 = 104;
const int A2  = 110;
const int AS2 = 117;
const int B2  = 123;
const int C3  = 131;
const int CS3 = 139;
const int D3  = 147;
const int DS3 = 156;
const int E3  = 165;
const int F3  = 175;
const int FS3 = 185;
const int G3  = 196;
const int GS3 = 208;
const int A3  = 220;
const int AS3 = 233;
const int B3  = 247;
const int C4  = 262;
const int CS4 = 277;
const int D4  = 294;
const int DS4 = 311;
const int E4  = 330;
const int F4  = 349;
const int FS4 = 370;
const int G4  = 392;
const int GS4 = 415;
const int A4  = 440;
const int AS4 = 466;
const int B4  = 494;
const int C5  = 523;
const int CS5 = 554;
const int D5  = 587;
const int DS5 = 622;
const int E5  = 659;
const int F5  = 698;
const int FS5 = 740;
const int G5  = 784;
const int GS5 = 831;
const int A5  = 880;
const int AS5 = 932;
const int B5  = 988;
const int C6  = 1047;
const int CS6 = 1109;
const int D6  = 1175;
const int DS6 = 1245;
const int E6  = 1319;
const int F6  = 1397;
const int FS6 = 1480;
const int G6  = 1568;
const int GS6 = 1661;
const int A6  = 1760;
const int AS6 = 1865;
const int B6  = 1976;
const int C7  = 2093;
const int CS7 = 2217;
const int D7  = 2349;
const int DS7 = 2489;
const int E7  = 2637;
const int F7  = 2794;
const int FS7 = 2960;
const int G7  = 3136;
const int GS7 = 3322;
const int A7  = 3520;
const int AS7 = 3729;
const int B7  = 3951;

// Super Mario Bros theme melody
const int melody[] = {

E7, E7, 0, E7,
0, C7, E7, 0,
G7, 0, 0, 0,
G6, 0, 0, 0,
C7, 0, 0, G6,
0, 0, E6, 0,
0, A6, 0, B6,
0, AS6, A6, 0,
G6, E7, G7,
A7, 0, F7, G7,
0, E7, 0, C7,
D7, B6, 0, 0,
C7, 0, 0, G6,
0, 0, E6, 0,
0, A6, 0, B6,
0, AS6, A6, 0,
G6, E7, G7,
A7, 0, F7, G7,
0, E7, 0, C7,
D7, B6, 0, 0,

0, G7, FS7, F7,
DS7, E7, 0, GS6,
A6, C6, 0, A6,
C7, D7, 0, DS6,
0, D7, C7, 0,
};

const int noteDurations[] = {

120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
90, 90, 90,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
90, 90, 90,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,

120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 120, 120,
120, 120, 240, 120,
240, 240, 120,
}; 


void play_tone(uint slice_num, int frequency, int duration) {
    if (frequency == 0) {
        // Silence
        pwm_set_gpio_level(BUZZ, 0);
        sleep_ms(duration);
        return;
    }

    // Calculate the period and wrap value
    float sysclock = 125000000.0f; // System clock frequency
    float pwm_clock = sysclock / 4.0f; // Using clock divider of 4
    float period = pwm_clock / frequency;
    uint16_t wrap = (uint16_t)period;
    
    // Configure PWM for this frequency
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, 4.f);
    pwm_config_set_wrap(&config, wrap);
    pwm_init(slice_num, &config, true);
    
    // Set duty cycle to 50%
    pwm_set_gpio_level(BUZZ, wrap / 2);
    
    // Play the note for specified duration
    sleep_ms(duration);
    
    // Stop the tone
    pwm_set_gpio_level(BUZZ, 0);
}

void main2() {
    // Initialize GPIO for PWM
    gpio_set_function(BUZZ, GPIO_FUNC_PWM);
    uint slice_num = pwm_gpio_to_slice_num(BUZZ);

    // Play the Super Mario Bros theme
    while (1) {
        for (int i = 0; i < sizeof(melody)/sizeof(melody[0]); i++) {
            play_tone(slice_num, melody[i], noteDurations[i]);
            
            // Small delay between notes
            sleep_ms(20);
        }
        
        // Pause before repeating
        sleep_ms(1000);
    }
}

int main() {
    stdio_init_all(); // Initialize chosen serial port
	gpio_init(BTN_1);
    gpio_set_dir(BTN_1, GPIO_IN);
	gpio_init(BTN_2);
    gpio_set_dir(BTN_2, GPIO_IN);
	gpio_init(BTN_3);
    gpio_set_dir(BTN_3, GPIO_IN);
	gpio_init(BTN_4);
    gpio_set_dir(BTN_4, GPIO_IN);

    //*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = false; // true for hardware spi,

	if (bhardwareSPI == true)
	{								   // hw spi
		uint32_t TFT_SCLK_FREQ = 20000; // Spi freq in KiloHertz , 1000 = 1Mhz , max 62500
		myTFT.TFTInitSPIType(TFT_SCLK_FREQ, spi0);
	}
	else
	{								 // sw spi
		uint16_t SWSPICommDelay = 0; // optional SW SPI GPIO delay in uS
		myTFT.TFTInitSPIType(SWSPICommDelay);
	}
	//**********************************************************

	// ******** USER OPTION 1 GPIO *********
	// NOTE if using Hardware SPI clock and data pins will be tied to
	// the chosen interface (eg Spi0 CLK=18 DIN=19)
	int8_t SDIN_TFT = 19;
	int8_t SCLK_TFT = 18;
	int8_t DC_TFT = 3;
	int8_t CS_TFT = 2;
	int8_t RST_TFT = 17;
	myTFT.TFTSetupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	//**********************************************************

	// ****** USER OPTION 2 Screen Setup ******
	uint8_t OFFSET_COL = 0;	   // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = -16;	   // 3, with manufacture tolerance/defects
	uint16_t TFT_WIDTH = 128;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 192; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************

	// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735S_Black); // pass enum,4 choices,see README
												 //**********************************************************

    const uint8_t *Mario;
    int pos[2] = {32,81};
    int vel[2] = {0,0};
    int acc[2] = {0,10};
    int y = 81;
    int x = 32;
    int xd1 = x;
    int yd2 = 96;
    int xd2 = 0;
    int xdk = 0;
    int spr = 1;
    int t = 0; //time
    int s = 0; //score
    int f = 0;
    int j = 0;
    char teststr1[] = " 2025 Tan S. AKINCI ";
    char teststr2[] = "Mario is a trademark";
    char teststr3[] = " of Nintendo - 1991 ";
    char name [] = "MARIO";
    char world[] = "WORLD";
    char time[] = "TIME";
    char timer[20];


    //Map
    int map[MAPH][MAPW] = { 
        {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        {00, 21, 00, 00, 00, 20, 21, 20, 21, 20, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00},
        {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 32, 33, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 12, 00, 00, 12, 12, 00, 00, 00, 00},
        {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 32, 33, 00, 00, 30, 31, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 12, 12, 00, 00, 12, 12, 12, 00, 00, 00},
        {00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 00, 30, 31, 00, 00, 30, 31, 00, 00, 00, 00, 00, 00, 00, 00, 00, 12, 12, 12, 12, 00, 00, 12, 12, 12, 12, 00, 00},
        {11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 00, 00, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11, 11},
        {10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 00, 00, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10} 
    };
    

    //myTFT.TFTdrawBitmap16Data(0, 0, (uint8_t *)pSuperMarioWorldTitle, 128, 160);
    myTFT.TFTsetRotation(myTFT.TFT_Degrees_270);
    myTFT.TFTdrawBitmap16Data(16, 0, (uint8_t *)pSuperMarioWorldTitle, 160, 128);
    myTFT.TFTdrawText(37, 80, teststr1, ST7735_WHITE, BACKGR, 1);
    myTFT.TFTdrawText(37, 90, teststr2, ST7735_WHITE, BACKGR, 1);
    myTFT.TFTdrawText(37, 100, teststr3, ST7735_WHITE, BACKGR, 1);
    TFT_MILLISEC_DELAY(3000);
    myTFT.TFTfillScreen(BACKGR);
    multicore_launch_core1(main2);
    while(1) {
        ++t;
        sprintf(timer,"%d",t);
        //myTFT.TFTfillScreen(BACKGR);
        //Button actions

        //Right
        if(gpio_get(BTN_2)){
            x+=3;
            if(f==0){
                if(t % 2){
                    spr = 3;
                }
                else {
                    spr = 1;
                }
            }
        }

        //Left
        else if(gpio_get(BTN_3)) {
            x-=3;
            if(f==0){
                if(t % 2){
                    spr = 4;
                }
                else {
                    spr = 2;
                }
            }
        }
        else {
            if(f==0){
                if(spr==4||spr==6) {
                    spr = 2;
                }
                else if(spr==3||spr==5) {
                    spr = 1;
                }
            }
        }

        //Jumping sequence
        if(gpio_get(BTN_1)){
            if (f==0){
                f = 1;
            }
        }
        if(f==1||f==2) {
            if(spr==2||spr==4||gpio_get(BTN_3)){
                spr = 6;
            }
            else if(spr==1||spr==3||gpio_get(BTN_2)){
                spr = 5;
            }
        }

        if(f==1){
            ++j;
            y-=5;
            if(j==7) {
                f = 2;
            }
        }
        else if(f==2){
            --j;
            y+=5;
            if(j==0) {
                f = 0;
            }
        }

        //Spritezzz
        switch (spr) {

            case 1:
                Mario = pMarioIdleS1;
                break;

            case 2:
                Mario = pMarioIdleS2;
                break;

            case 3:
                Mario = pMarioWalkS1;
                break;

            case 4:
                Mario = pMarioWalkS2;
                break;

            case 5:
                Mario = pMarioJumpS1;
                break;
            case 6:
                Mario = pMarioJumpS2;
                break;
        }

        /*
        if (x>80){
            xd2 = -x;
            x = 80;
        }
        
        else if (x<=80){
            xd1 = x;
        }
        */

        if (x>96){
            xd1 = 96;
            xd2 = -x+96;
        }
        else if (x<17){
            x = 18;
            xd2 = 0;
        }
        else{
            xd1 = x;
            xd2 = 0;
        }


        for (int j = 0; j < MAPH; j++) {
            for (int k = 0; k < MAPW; k++) {
                int jm = j * 16 + 24;
                int km = k * 16;
                xdk = xd2 + km;
                
                // Only draw tiles that are visible on screen (0-160 horizontal range)
                if (xdk >= -16 && xdk < 176) {
                    if (map[j][k] == 0) {
                        myTFT.TFTfillRect(xdk, jm, 16, 16, BACKGR);
                    } else if (map[j][k] == 10) {
                        myTFT.TFTdrawBitmap16Data(xdk, jm, (uint8_t *)pDirt, 16, 16);
                    } else if (map[j][k] == 11) {
                        myTFT.TFTdrawBitmap16Data(xdk, jm, (uint8_t *)pGrass, 16, 16);
                    } else if (map[j][k] == 12) {
                        myTFT.TFTdrawBitmap16Data(xdk, jm, (uint8_t *)pWoodBlock, 16, 16);
                    } else if (map[j][k] == 20) {
                        myTFT.TFTdrawBitmap16Data(xdk, jm, (uint8_t *)pRoBlock1, 16, 16);
                    } else if (map[j][k] == 21) {
                        myTFT.TFTdrawBitmap16Data(xdk, jm, (uint8_t *)pLuckBlock1, 16, 16);
                    } else if (map[j][k] == 30) {
                        myTFT.TFTdrawBitmap16Data(xdk, jm, (uint8_t *)pPipeL, 16, 16);
                    } else if (map[j][k] == 31) {
                        myTFT.TFTdrawBitmap16Data(xdk, jm, (uint8_t *)pPipeR, 16, 16);
                    } else if (map[j][k] == 32) {
                        myTFT.TFTdrawBitmap16Data(xdk, jm, (uint8_t *)pPipeTL, 16, 16);
                    } else if (map[j][k] == 33) {
                        myTFT.TFTdrawBitmap16Data(xdk, jm, (uint8_t *)pPipeTR, 16, 16);
                    }
                }
            }
        }

        myTFT.TFTdrawSpriteData(xd2+68, 78, (uint8_t *)pKoopaIdle1, 16, 27, BACKGR);
        myTFT.TFTdrawSpriteData(xd1, y, (uint8_t *)Mario, 16, 24, BACKGR);
        myTFT.TFTdrawText(20, 0, name, ST7735_WHITE, BACKGR, 1);
        myTFT.TFTdrawText(92, 0, world, ST7735_WHITE, BACKGR, 1);
        myTFT.TFTdrawText(140, 0, time, ST7735_WHITE, BACKGR, 1);
        myTFT.TFTdrawText(140, 8, timer, ST7735_WHITE, BACKGR, 1);
        TFT_MILLISEC_DELAY(10);

        //myTFT.TFTdrawBitmap16Data(y, xd1, (uint8_t *)Mario, 24, 16);
    }
}