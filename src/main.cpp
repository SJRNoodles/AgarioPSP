#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <gfx.hpp>
#include <img.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include <glib2d.h>


PSP_MODULE_INFO("agario", 0, 1, 0);

int mass = 10;
int massSize = mass;
int dir = 1;

int x;
int y;

bool collision(int x1,int y1,int x2,int y2,int w,int h,int w1, int h1){
	return (x1 >= x2 && x1 <= x2 + w) && (y1 >= y2 && y1 <= y2 + h) || (x2 >= x1 && x2 <= x1 + w1) && (y2 >= y1 && y2 <= y1 + h1);
}

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

struct AI {
public:
	    int pX = 10;
		int pY = 10;
		int score = 10;
		int color = 10;
		int dir = 1;
		int tick = 1;
};


auto main() -> int {

	Food dots[240];
	int loop = 1;
	while (loop < 240) {
		dots[loop].foodX = rand() % 1000 - 90;
		dots[loop].foodY = rand() % 1000 + 10;
		dots[loop].color = rand() % 5;
		loop++;
	}
	
	AI plrs[100];
	loop = 1;
	while (loop < 24) {
		plrs[loop].pX = rand() % 1000 - 90;
		plrs[loop].pY = rand() % 1000 + 10;
		plrs[loop].score = rand() % 50 + 10;
		plrs[loop].tick = rand() % 20 + 1;
		plrs[loop].dir = rand() % 3 + 1;
		plrs[loop].color = rand() % 5;
		loop++;
	}

	Virus spots[10];
	loop = 1;
	while (loop < 1) {
		spots[loop].virX = rand() % 480 - 90;
		spots[loop].virY = rand() % 272 + 10;
		loop++;
	}
	SceCtrlData ctrlData;
	while (1) {
		g2dClear(WHITE);
		
		//food
		loop = 1;
		while (loop < 240) {
			g2dBeginRects(NULL);
			g2dSetColor(BLUE);
			g2dSetScaleWH(4,4);
			g2dSetCoordXY((dots[loop].foodX - x) / (mass / 40) + 240,(dots[loop].foodY - y) / (mass / 40) + 132);
			g2dAdd();




			// collision detection for player
			if (collision(240 + (massSize / 2) ,132 + (massSize / 2) , (dots[loop].foodX - x) / (mass / 40) + 240 , (dots[loop].foodY - y) / (mass / 40) + 132 ,massSize,massSize,4,4)) {
				mass+= 10 / (massSize / 4);
				dots[loop].foodX = rand() % 1000 - 90;
				dots[loop].foodY = rand() % 1000 + 10;
			}


			loop++;
		}
		
		//ai players
		loop = 1;
		while (loop < 10) {
			g2dBeginRects(NULL);
			g2dSetColor(ORANGE);
			g2dSetScaleWH(plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40));
			g2dSetCoordXY((plrs[loop].pX - x) / (mass / 40) + 240,(plrs[loop].pY - y) / (mass / 40) + 132);
			g2dAdd();
			plrs[loop].tick++;
			if(plrs[loop].tick >= 100){
				plrs[loop].tick = 0;
				plrs[loop].dir = rand() % 3 + 1;
			}
			
			if (plrs[loop].dir == 1) {
				plrs[loop].pX = plrs[loop].pX - (4 / (plrs[loop].score / 2));
				plrs[loop].pX = plrs[loop].pX - 1;
			}
			if (plrs[loop].dir == 2) {
				plrs[loop].pX = plrs[loop].pX + (4 / (plrs[loop].score / 2));
				plrs[loop].pX = plrs[loop].pX + 1;
			}
			if (plrs[loop].dir == 3) {
				plrs[loop].pY = plrs[loop].pY - (4 / (plrs[loop].score / 2));
				plrs[loop].pY = plrs[loop].pY - 1;
			}
			if (plrs[loop].dir == 4) {
				plrs[loop].pY = plrs[loop].pY + (4 / (plrs[loop].score / 2));
				plrs[loop].pY = plrs[loop].pY + 1;
			}
			
			// collision detection for player
			if (collision(240 + (massSize / 2) ,132 + (massSize / 2) , (plrs[loop].pX - x) / (mass / 40) + 240 , (plrs[loop].pY - y) / (mass / 40) + 132 ,massSize,massSize,plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40))) {
				if (mass > plrs[loop].score + 10) {
					mass += (plrs[loop].score / 4);
					plrs[loop].score = 10;
					plrs[loop].pX = rand() % 1000 - 90;
					plrs[loop].pY = rand() % 1000 + 10;
				}
			}
			loop++;
		}
	
		// spiky things
		loop = 1;
		while (loop < 1) {
			// todo
			if (spots[loop].virX > 232 - y) {

			}
			else {
				if (spots[loop].virY > 232 + y) {

				}
				else {
					g2dBeginRects(NULL);
					g2dSetColor(GREEN);
					g2dSetScaleWH(25,25);
					g2dSetCoordXY(spots[loop].virX - x,spots[loop].virY - y);
					g2dAdd();
				}
			}
			loop++;
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
			g2dBeginRects(NULL);
			g2dSetColor(RED);
			g2dSetScaleWH(massSize / (mass / 40),massSize / (mass / 40));
			g2dSetCoordXY((480 - massSize / (mass / 40)) / 2 ,(272 - massSize / (mass / 40)) / 2);
			massSize = mass / 2;

			g2dAdd();
			g2dEnd();

			g2dFlip(G2D_VSYNC);

		}
		
		sceKernelExitGame();
		return 0;

	}

