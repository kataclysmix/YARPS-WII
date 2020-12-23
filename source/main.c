#include <stdio.h>
#include <stdlib.h>
#include <gccore.h>
#include <wiiuse/wpad.h>

// Object Identification
#define ROCK 1
#define PAPER 2
#define SCISSORS 3

#define HOME WPAD_BUTTON_HOME
#define PLUS WPAD_BUTTON_PLUS

// D-Pad buttons
#define LEFT    WPAD_BUTTON_LEFT
#define UP        WPAD_BUTTON_UP
#define RIGHT  WPAD_BUTTON_RIGHT
#define DOWN    WPAD_BUTTON_DOWN

// Font Colors
#define WHITEFONT   "\x1B[37m"
#define CYANFONT    "\x1B[36m"

// Background Colors
#define BLACKBG     "\x1B[40m"
#define BLUEBG      "\x1B[44m"

static void *xfb = NULL;
static GXRModeObj *rmode = NULL;

void clear() {
	
	printf("\x1b[2;0H");
	printf("\x1b[2J");
	printf("\x1b[2;0H");

	printf(BLUEBG WHITEFONT "YARPS Coded by Cataclysmix || v1.0\n\n" BLACKBG);


}

int rng(int min, int max) {

	int result = rand() % max + min;
	return result;

}

void stop() {
	
	printf("\x1b[2;0H");
	printf("\x1b[2J");
	printf("\x1b[2;0H");

	printf(BLUEBG WHITEFONT "YARPS Coded By Cataclysmix || v1.0\n\n");

	printf(WHITEFONT BLACKBG "Exiting to loader...\n\n");

	exit(0);

}

//---------------------------------------------------------------------------------
int main(int argc, char **argv) {
//---------------------------------------------------------------------------------

	// Initialise the video system
	VIDEO_Init();

	// This function initialises the attached controllers
	WPAD_Init();

	// Obtain the preferred video mode from the system
	// This will correspond to the settings in the Wii menu
	rmode = VIDEO_GetPreferredMode(NULL);

	// Allocate memory for the display in the uncached region
	xfb = MEM_K0_TO_K1(SYS_AllocateFramebuffer(rmode));

	// Initialise the console, required for printf
	console_init(xfb,20,20,rmode->fbWidth,rmode->xfbHeight,rmode->fbWidth*VI_DISPLAY_PIX_SZ);

	// Set up the video registers with the chosen mode
	VIDEO_Configure(rmode);

	// Tell the video hardware where our display memory is
	VIDEO_SetNextFramebuffer(xfb);

	// Make the display visible
	VIDEO_SetBlack(FALSE);

	// Flush the video register changes to the hardware
	VIDEO_Flush();

	// Wait for Video setup to complete
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();


	// The console understands VT terminal escape codes
	// This positions the cursor on row 2, column 0
	// we can use variables for this with format codes too
	// e.g. printf ("\x1b[%d;%dH", row, column );
	printf("\x1b[2;0H");
	
	clear();

	int start = 1;

	while(1) {

		// Call WPAD_ScanPads each loop, this reads the latest controller states
		WPAD_ScanPads();

		// WPAD_ButtonsDown tells us which buttons were pressed in this loop
		// this is a "one shot" state which will not fire again until the button has been released
		u32 pressed = WPAD_ButtonsDown(0);

		// We return to the launcher application via exit
		if ( pressed & HOME ) stop();
		if ( pressed & PLUS ) start = 1;

		// game code here
		if ( start == 1 ) {

			clear();

			int playerAction;
			int cpuAction;

			start = 0;

			int choosing = 1;

			printf("Press any of the following buttons to call an object\n");
			printf("Left  - Rock\n");
			printf("Up    - Paper\n");
			printf("Right - Scissors\n\n");
			printf("HOME  - Exit\n");

			while(choosing == 1) {

				// Call WPAD_ScanPads each loop, this reads the latest controller states
				WPAD_ScanPads();

				// WPAD_ButtonsDown tells us which buttons were pressed in this loop
				// this is a "one shot" state which will not fire again until the button has been released
				u32 pressed = WPAD_ButtonsDown(0);

				// Action select

				if ( pressed & LEFT ) {

					playerAction = ROCK;
					choosing = 0;

				}

				if ( pressed & UP ) {

					playerAction = PAPER;
					choosing = 0;

				}

				if ( pressed & RIGHT ) {

					playerAction = SCISSORS;
					choosing = 0;


				}

				if ( pressed & HOME ) stop();

			}
			
			clear();
			
			if (playerAction == 1) printf("You chose Rock.\n\n");
			if (playerAction == 2) printf("You chose Paper.\n\n");
			if (playerAction == 3) printf("You chose Scissors\n\n");

			// Computer turn
			cpuAction = rng(1, 3);

			if (cpuAction == 1) printf("The computer chose Rock.\n\n");
			if (cpuAction == 2) printf("The computer chose Paper.\n\n");
			if (cpuAction == 3) printf("The computer chose Scissors\n\n");

			// Decide who won
			if (playerAction == cpuAction) printf("Draw!");

			if (playerAction == ROCK & cpuAction == PAPER) printf("You lost the game!");
			if (playerAction == ROCK & cpuAction == SCISSORS) printf("You won the game!");

			if (playerAction == PAPER & cpuAction == ROCK) printf("You won the game!");
			if (playerAction == PAPER & cpuAction == SCISSORS) printf("You lost the game!");

			if (playerAction == SCISSORS & cpuAction == ROCK) printf("You lost the game!");
			if (playerAction == SCISSORS & cpuAction == PAPER) printf("You won the game!");

			printf("\nPress (+) to play again.");

		}

		// Wait for the next frame
		VIDEO_WaitVSync();
	}

	return 0;
}
