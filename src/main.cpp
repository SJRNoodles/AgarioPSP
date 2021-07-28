#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <gfx.hpp>
#include <img.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


PSP_MODULE_INFO("agario", 0, 1, 0);

int mass = 20;
int massSize = mass;
int dir = 1;

int x;
int y;

struct Food {
public:
	int foodX = 10;
	int foodY = 10;
	int color;
};

struct Virus {
public:
	    int virX = 10;
		int virY = 10;
};


auto main() -> int {

	
	GFX::init();
	sceCtrlSetSamplingCycle(0);
	sceCtrlSetSamplingMode(PSP_CTRL_MODE_ANALOG);

	Food dots[24];
	int loop = 1;
	while (loop < 24) {
		dots[loop].foodX = rand() % 480 - 90;
		dots[loop].foodY = rand() % 272 + 10;
		loop++;
	}

	Virus spots[1];
	loop = 1;
	while (loop < 1) {
		spots[loop].virX = rand() % 480 - 90;
		spots[loop].virY = rand() % 272 + 10;
		loop++;
	}
	SceCtrlData ctrlData;
	while (1) {
		GFX::swapBuffers();
		sceDisplayWaitVblankStart();

		//bkg
		GFX::clear(0xFFFFFFFF);
		GFX::drawRect(240, 136, 480, 272, 0xFFFFFFFF);



		//food
		loop = 1;
		while (loop < 24) {
			// crash prevention by when the food touches edge
			if (dots[loop].foodY > 232 - y) {

			}
			else {
				if (dots[loop].foodY > 232 + y) {

				}
				else {
					GFX::drawRect(dots[loop].foodX - x, dots[loop].foodY - y, 4, 4, 0xFFFF00FF);
				}
			}




			// collision detection for player
			if (dots[loop].foodX > 240 + x) {
				if (dots[loop].foodY > 132 + y) {
					dots[loop].foodX = rand() % 480 - 90;
					if (dots[loop].foodY > 232 - y) {

					}
					else {
						if (dots[loop].foodY > 232 + y) {

						}
						else {
							dots[loop].foodY = rand() % 272 + 10;
						}
					}
					mass++;
					//Size thingy to prevent crashing
					if (mass > 60) {
						massSize = 60;
					}
					else {
						massSize = mass;
					}
				}
			}


			loop++;
		}
	
		// spiky things
		loop = 1;
		while (loop < 1) {
			// crash prevention by when the spike touches edge
			if (spots[loop].virX > 232 - y) {

			}
			else {
				if (spots[loop].virY > 232 + y) {

				}
				else {
					GFX::drawRect(spots[loop].virX - x, spots[loop].virY - y, 25, 25, 0xff00ff00);
				}
			}
		}


			//controls
			sceCtrlReadBufferPositive(&ctrlData, 1);
			if (ctrlData.Buttons & PSP_CTRL_LEFT) {
				dir = 1;
			}
			if (ctrlData.Buttons & PSP_CTRL_RIGHT) {
				dir = 2;
			}
			if (ctrlData.Buttons & PSP_CTRL_UP) {
				dir = 3;
			}
			if (ctrlData.Buttons & PSP_CTRL_DOWN) {
				dir = 4;
			}

			if (dir == 1) {
				x = x - (40 / (mass / 2));
				x = x - 1;
			}
			if (dir == 2) {
				x = x + (40 / (mass / 2));
				x = x + 1;
			}
			if (dir == 3) {
				y = y - (40 / (mass / 2));
				y = y - 1;
			}
			if (dir == 4) {
				y = y + (40 / (mass / 2));
				y = y + 1;
			}



			//cell
			GFX::drawRect(240 - (massSize / 2), 132 - (massSize / 2), massSize, massSize, 0xFFFF0000);

			//Crash prevention
			if (mass > 60) {
				massSize = 60;
			}
			else {
				massSize = mass;
			}



		}
	

	}

