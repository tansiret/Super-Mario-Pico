#include "pico/stdlib.h"
#include "pico/time.h"
#include "pico/multicore.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "displaylib_16/st7735.hpp"
#include "displaylib_16/Bitmap_TEST_Data.hpp"

// Globals
ST7735_TFT myTFT;
// Peripherals
#define BTN_JUMP 28
#define BTN_RIGHT 27
#define BTN_LEFT 26
#define BTN_4 25
#define BUZZ 1
// Graphics
#define LBLUE 0x0337
// Map
#define MAPH 7
#define MAPW 40

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

void SetupHW(void)
{   
	stdio_init_all(); // Initialize chosen serial port
	sleep_ms(500);
	printf("Game Start\r\n");
	gpio_init(BTN_JUMP);
	gpio_set_dir(BTN_JUMP, GPIO_IN);
	gpio_init(BTN_RIGHT);
	gpio_set_dir(BTN_RIGHT, GPIO_IN);
	gpio_init(BTN_LEFT);
	gpio_set_dir(BTN_LEFT, GPIO_IN);
	gpio_init(BTN_4);
	gpio_set_dir(BTN_4, GPIO_IN);

	gpio_pull_down(BTN_JUMP);
	gpio_pull_down(BTN_RIGHT);
	gpio_pull_down(BTN_LEFT);
	gpio_pull_down(BTN_4);

	//*************** USER OPTION 0 SPI_SPEED + TYPE ***********
	bool bhardwareSPI = false; // true for hardware spi,

	if (bhardwareSPI == true)
	{								   // hw spi
		uint32_t TFT_SCLK_FREQ = 62500; // Spi freq in KiloHertz , 1000 = 1Mhz , max 62500
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
	int8_t RST_TFT = 4;
	myTFT.setupGPIO(RST_TFT, DC_TFT, CS_TFT, SCLK_TFT, SDIN_TFT);
	//**********************************************************
	// ****** USER OPTION 2 Screen Setup ******
	uint8_t OFFSET_COL = 0;	   // 2, These offsets can be adjusted for any issues->
	uint8_t OFFSET_ROW = 0;	   // 3, with manufacture tolerance/defects
	uint16_t TFT_WIDTH = 128;  // Screen width in pixels
	uint16_t TFT_HEIGHT = 160; // Screen height in pixels
	myTFT.TFTInitScreenSize(OFFSET_COL, OFFSET_ROW, TFT_WIDTH, TFT_HEIGHT);
	// ******************************************
	// ******** USER OPTION 3 PCB_TYPE  **************************
	myTFT.TFTInitPCBType(myTFT.TFT_ST7735S_Black); // pass enum,4 choices
	//**********************************************************
	myTFT.setBuffer();
	myTFT.setTextCharPixelOrBuffer(true);
}

void DisplayTitle()
{
	myTFT.setTextColor(myTFT.C_WHITE, LBLUE);
	char teststr1[] = "2025 Tan AKINCI";
	char teststr2[] = "Mario trademark";
	char teststr3[] = "Nintendo - 1991";
	myTFT.setRotation(myTFT.Degrees_270);
	myTFT.drawBitmap16Data(0, 0, pSuperMarioWorldTitle, 160, 128);
	myTFT.writeCharString(17, 70, teststr1);
	myTFT.writeCharString(17, 80, teststr2);
	myTFT.writeCharString(17, 90, teststr3);
	myTFT.writeBuffer();
	MILLISEC_DELAY(3000);
	myTFT.fillScreen(LBLUE);
	myTFT.clearBuffer();
}


void DisplayGameOver()
{
	MILLISEC_DELAY(500);
	myTFT.setTextColor(myTFT.C_WHITE, LBLUE);
	char teststr1[] = "GAME OVER";
	myTFT.setRotation(myTFT.Degrees_270);
	myTFT.drawBitmap16Data(0, 0, pSuperMarioWorldTitle, 160, 128);
	myTFT.writeCharString(30, 70, teststr1);
	myTFT.writeBuffer();
	MILLISEC_DELAY(3000);
	myTFT.fillScreen(LBLUE);
	myTFT.clearBuffer();
}

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
			sleep_ms(20);
		}
		sleep_ms(1000);
	}
}

void GameLoop(int timeLimit)
{
	// Game variables
	std::span<const uint8_t> Mario;
    int pos[2] = {32,81}; //xy
    float velf[2] = {0,0};
    int vel[2] = {0,0};
    float acc[2] = {0,0};
    float fric = 0; //friction
    float g = 1;
    float nf = -g;
    int y = 81;
    int x = 32;
    int xd1 = x;
    int yd2 = 96;
    int xd2 = 0;
    int xdk = 0;
    int spr = 1;
    int time = 0; //time
    int s = 0; //score
    int jumpFlag = 0;
    int jumpCounter = 0;
    char teststr1[] = " 2025 Tan S. AKINCI ";
    char teststr2[] = "Mario is a trademark";
    char teststr3[] = " of Nintendo - 1991 ";
    char name [] = "MARIO";
    char world[] = "WORLD";
    char timeText[] = "TIME";
    char timer[20];
    char position[30];
    char level[3];

	// Start second core
	multicore_launch_core1(main2);
	myTFT.fillRect(0,0, 128, 160, LBLUE+1);

	while (time < timeLimit)
	{
		++time;
		sprintf(timer,"%d",time);
        sprintf(level,"%d",jumpFlag);
        sprintf(position,"X:%d,Y:%d ",pos[0],pos[1]);
		
		//Right
        if(gpio_get(BTN_RIGHT)){
            acc[0]=1;
        }

        //Left
        else if(gpio_get(BTN_LEFT)) {
            acc[0]=-1;
        }
        else {
            acc[0]=0;
        }

        //Physics lecture

        if(velf[0]>=4.0f||velf[0]<=-4.0f) {
            acc[0] = 0;
        }
        
        if(vel[0]==0){
            fric = 0;
            if(jumpFlag==0){
                if(spr==4||spr==6) {
                    spr = 2;
                }
                else if(spr==3||spr==5) {
                    spr = 1;
                }
                pos[1]=81;
            }
        }
        else if(velf[0]>0.0f) {
            fric = -0.5;
            if(jumpFlag==0){
                if(time % 3){
                    spr = 3;
                    pos[1]=80;
                }
                else {
                    spr = 1;
                    pos[1]=81;
                }
            }
        }
        else if(velf[0]<0.0f) {
            fric = 0.5;
            if(jumpFlag==0){
                if(time % 3){
                    spr = 4;
                    pos[1]=80;
                }
                else {
                    spr = 2;
                    pos[1]=81;
                }
            }
        }

        velf[0] = velf[0] + acc[0] + fric;
        /*
        if(fabsf(floorf(velf[0]) - velf[0]) <= 0.01f){ //Rounding int control with float calc errors in mind
            vel[0] = floor(velf[0]);
        }
        */
        vel[0] = round(velf[0]);
        pos[0] += vel[0];
        
        //Jumping sequence
        if(gpio_get(BTN_JUMP)){
            if (jumpFlag==0){
                jumpFlag = 1;
            }
        }
        if(jumpFlag==1||jumpFlag==2) {
            if(spr==2||spr==4||gpio_get(BTN_LEFT)){
                spr = 6;
            }
            else if(spr==1||spr==3||gpio_get(BTN_RIGHT)){
                spr = 5;
            }
        }

        if(jumpFlag==1){
            ++jumpCounter;
            acc[1]=-3;
            nf = -g;
            if(jumpCounter==4) { //Jump velocity limit
                jumpFlag = 2; //Fall indicator
                jumpCounter = 0;
            }
        }
        if(jumpFlag==2){
            acc[1]=0;
            nf = 0;
            if(pos[1]>=80) {
                jumpFlag = 0;
            }
        }

        if(jumpFlag==0){
            velf[1]=0;
            nf = -g;
        }

        velf[1] = velf[1] + acc[1] + g + nf;
        vel[1] = floor(velf[1]);
        pos[1] += vel[1];

		// Sprite selection
		switch (spr) {
			case 1: Mario = pMarioIdleS1; break;
			case 2: Mario = pMarioIdleS2; break;
			case 3: Mario = pMarioWalkS1; break;
			case 4: Mario = pMarioWalkS2; break;
			case 5: Mario = pMarioJumpS1; break;
			case 6: Mario = pMarioJumpS2; break;
		}
		if (pos[0] > 96) {
			xd1 = 96;
			xd2 = -pos[0] + 96;
		} else if (pos[0] < 17) {
			pos[0] = 18;
			xd2 = 0;
		} else {
			xd1 = pos[0];
			xd2 = 0;
		}
		for (int j = 0; j < MAPH; j++) {
			for (int k = 0; k < MAPW; k++) {
				int jm = j * 16 + 24;
				int km = k * 16;
				int xdk = xd2 + km;
				// Ensure tiles at edges are pre-rendered
				if (xdk >= -16 && xdk <= 176 ){
					switch (map[j][k]) {
						//case 0:  myTFT.fillRect(xdk, jm, 16, 16, LBLUE); break;
						case 0:  myTFT.drawBitmap16Data(xdk, jm, pSky, 16, 16); break;
						case 10: myTFT.drawBitmap16Data(xdk, jm, pDirt,16, 16); break;
						case 11: myTFT.drawBitmap16Data(xdk, jm, pGrass,16, 16); break;
						case 12: myTFT.drawBitmap16Data(xdk, jm, pWoodBlock,16, 16); break;
						case 20: myTFT.drawBitmap16Data(xdk, jm, pRoBlock1,16, 16); break;
						case 21: myTFT.drawBitmap16Data(xdk, jm, pLuckBlock1,16, 16); break;
						case 30: myTFT.drawBitmap16Data(xdk, jm, pPipeL,16, 16); break;
						case 31: myTFT.drawBitmap16Data(xdk, jm, pPipeR,16, 16); break;
						case 32: myTFT.drawBitmap16Data(xdk, jm, pPipeTL,16, 16); break;
						case 33: myTFT.drawBitmap16Data(xdk, jm, pPipeTR,16, 16); break;
					}
				}
			}
		}
		// **SPRITE RENDERING**
		myTFT.drawSpriteData(xd2 + 68, 78, pKoopaIdle1, 16, 27, LBLUE, false);
		myTFT.drawSpriteData(xd1, pos[1], Mario, 16, 24, LBLUE, false);
		// **HUD Display**
		myTFT.writeCharString(5, 0, name);
		myTFT.writeCharString(5, 8, position);
		myTFT.writeCharString(74, 0, world);
		myTFT.writeCharString(122, 0, timeText);
		myTFT.writeCharString(122, 8, timer);
		myTFT.writeBuffer();
		myTFT.clearBuffer(LBLUE+1);
		MILLISEC_DELAY(20);
		if (pos[0] > 650) {
			// Game over condition
			break;
		}
	}
}

void EndGame(void)
{
	gpio_deinit(BTN_JUMP);
	gpio_deinit(BTN_RIGHT);
	gpio_deinit(BTN_LEFT);
	gpio_deinit(BTN_4);
	gpio_deinit(BUZZ);
	DisplayGameOver();
	myTFT.destroyBuffer();
	myTFT.TFTPowerDown(); 
	printf("Game Over");
}

int main() 
{
	SetupHW();
	DisplayTitle();
	GameLoop(2000);
	EndGame();
	return 0;	
}
