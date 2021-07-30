#include <pspkernel.h>
#include <pspdebug.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <gfx.hpp>
#include <img.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include <pspgu.h>
#include <glib2d.h>


PSP_MODULE_INFO("agario", 0, 1, 0);

int mass = 10;
int massSize = mass;
int dir = 1;

int loop1 = 1;
int loop2 = 1;
int gridOff = 512;
int gridOff2 = 0;
int state = 0;
int running = 1;

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

	g2dTexture* bg = g2dTexLoad("grid.png",G2D_SWIZZLE);
	g2dTexture* title = g2dTexLoad("title.png",G2D_SWIZZLE);
	g2dTexture* tex = g2dTexLoad("cell.png",G2D_SWIZZLE);
	
	g2dColors colors[7] = {RED,ORANGE,YELLOW,GREEN,BLUE,VIOLET,ROSE};
	
	Food dots[240];
	int loop = 1;
	while (loop < 240) {
		dots[loop].foodX = rand() % 2000 - 90;
		dots[loop].foodY = rand() % 2000 + 10;
		dots[loop].color = rand() % 7;
		loop++;
	}
	
	AI plrs[50];
	loop = 1;
	while (loop < 50) {
		plrs[loop].pX = rand() % 5000 - 90;
		plrs[loop].pY = rand() % 5000 + 10;
		plrs[loop].score = rand() % 200 + 10;
		plrs[loop].tick = rand() % 20 + 1;
		plrs[loop].dir = rand() % 3 + 1;
		plrs[loop].color = rand() % 7;
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
	while (running == 1) {
		if (state == 1) {
		g2dClear(WHITE);
		// background
		// round (plr x pos / 64 * 64)
		loop2 = 1;
		gridOff=-1024;
		gridOff2=1;
		while (loop2 <= 16) {
			g2dSetTexLinear(false);
			g2dBeginRects(bg);
			g2dSetColor(WHITE);
			g2dSetScaleWH(512,512);
			g2dSetCoordXY(((round(x / 128) * 128) - x - (gridOff)) / (mass / 40),((round(y / 128) * 128) - y - (gridOff2))/ (mass / 40));
			g2dAdd();
			gridOff+=512;
			loop2++;
		}
		g2dEnd();
		
		//food
		loop = 1;
		while (loop < 240) {
			g2dBeginRects(tex);
			g2dSetColor(colors[dots[loop].color]);
			g2dSetScaleWH(4,4);
			g2dSetCoordXY((dots[loop].foodX - x) / (mass / 40) + 240,(dots[loop].foodY - y) / (mass / 40) + 132);
			g2dAdd();




			// collision detection for player
			if (collision(240 + (massSize / 2) ,132 + (massSize / 2) , (dots[loop].foodX - x) / (mass / 40) + 240 , (dots[loop].foodY - y) / (mass / 40) + 132 ,massSize,massSize,4,4)) {
				mass+= 10 / (massSize / 4);
				dots[loop].foodX = rand() % 2000 - 90;
				dots[loop].foodY = rand() % 2000 + 10;
			}
			

			loop++;
		}
		
		//ai players
		loop = 1;
		while (loop < 50) {
			loop1 = 1;
			g2dBeginRects(tex);
			g2dSetColor(colors[plrs[loop].color]);
			g2dSetScaleWH(plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40));
			g2dSetCoordXY((plrs[loop].pX - x) / (mass / 40) + 240,(plrs[loop].pY - y) / (mass / 40) + 132);
			g2dAdd();
			plrs[loop].tick++;
			if(plrs[loop].tick >= plrs[loop].score * 2){
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
			
			// ai eats pellets
			while (loop1 <= 240) {
				if (collision((plrs[loop].pX - x) / (mass / 40) + 240 ,(plrs[loop].pY - y) / (mass / 40) + 132 , (dots[loop1].foodX - x) / (mass / 40) + 240 , (dots[loop1].foodY - y) / (mass / 40) + 132 ,plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40),4,4)) {
					plrs[loop].score+= 10 / (plrs[loop].score / 4);
					dots[loop1].foodX = rand() % 2000 - 90;
					dots[loop1].foodY = rand() % 2000 + 10;
				}
				loop1++;
			}
			
			loop1 = 1;
			
			while (loop1 <= 100) {
				// ai eat ai
				if (collision((plrs[loop].pX - x) / (mass / 40) + 240 ,(plrs[loop].pY - y) / (mass / 40) + 132 , (plrs[loop1].pX - x) / (mass / 40) + 240 , (plrs[loop1].pY - y) / (mass / 40) + 132 ,plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40),plrs[loop1].score / (mass / 40),plrs[loop1].score / (mass / 40))) {
					if (plrs[loop].score > plrs[loop1].score + 30) {
						plrs[loop].score += (plrs[loop1].score / 4);
						plrs[loop1].score = 10;
						plrs[loop1].pX = rand() % 1000 - 90;
						plrs[loop1].pY = rand() % 1000 + 10;
					}else if (plrs[loop1].score > plrs[loop].score + 30) {
						plrs[loop1].score += (plrs[loop1].score / 4);
						plrs[loop].score = 10;
						plrs[loop].pX = rand() % 1000 - 90;
						plrs[loop].pY = rand() % 1000 + 10;
					}
				}
				loop1++;
			}
			
			// collision detection for player
			if (collision(240 + (massSize / 2) ,132 + (massSize / 2) , (plrs[loop].pX - x) / (mass / 40) + 240 , (plrs[loop].pY - y) / (mass / 40) + 132 ,massSize,massSize,plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40))) {
				if (mass > plrs[loop].score + 30) {
					mass += (plrs[loop].score / 4);
					plrs[loop].score = 10;
					plrs[loop].pX = rand() % 1000 - 90;
					plrs[loop].pY = rand() % 1000 + 10;
				}
				if (plrs[loop].score > mass) {
					x = 0;
					y = 0;
					mass = 10;
					
					// reset positions of all players
					
					loop2 = 1;
					while (loop2 < 100) {
						plrs[loop].pX = rand() % 1000 - 90;
						plrs[loop].pY = rand() % 1000 + 10;
						plrs[loop].score = rand() % 200 + 10;
						plrs[loop].tick = rand() % 20 + 1;
						plrs[loop].dir = rand() % 3 + 1;
						plrs[loop].color = rand() % 7;
						loop2++;
					}
					
					state = 0;
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
				x = x - (40 / (mass / 4));
				x = x - 1;
			}
			if (dir == 2) {
				x = x + (40 / (mass / 4));
				x = x + 1;
			}
			if (dir == 3) {
				y = y - (40 / (mass / 4));
				y = y - 1;
			}
			if (dir == 4) {
				y = y + (40 / (mass / 4));
				y = y + 1;
			}



			//cell
			g2dBeginRects(tex);
			g2dSetColor(RED);
			g2dSetScaleWH(massSize / (mass / 40),massSize / (mass / 40));
			g2dSetCoordXY((480 - massSize / (mass / 40)) / 2 ,(272 - massSize / (mass / 40)) / 2);
			massSize = mass / 2;

			g2dAdd();
			g2dEnd();

			g2dFlip(G2D_VSYNC);
			
		}
		if(state == 0){
			g2dClear(WHITE);
			g2dBeginRects(title);
			g2dSetColor(WHITE);
			g2dSetScaleWH(480,272);
			g2dSetCoordXY(0,0);
			g2dAdd();
			g2dEnd();
			
			sceCtrlReadBufferPositive(&ctrlData, 1);
			if (ctrlData.Buttons & PSP_CTRL_START) {
				state = 1;
			}
			if (ctrlData.Buttons & PSP_CTRL_SELECT) {
				running = 0;
			}
			
			g2dFlip(G2D_VSYNC);
		}

		}
		
		sceKernelExitGame();
		return 0;

	}

