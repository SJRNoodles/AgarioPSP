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


//i coded this all on my own
PSP_MODULE_INFO("agario", 0, 1, 0);
PSP_HEAP_SIZE_KB(-256);

int mass = 45;
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
		int type = 0;
};

struct AI {
public:
	    int pX = 10;
		int pY = 10;
		int score = 45;
		int color = 10;
		int dir = 1;
		int tick = 1;
		int skin = 0;
		g2dTexture* chosenSkin = g2dTexLoad("cell.png",G2D_SWIZZLE);
};

auto main() -> int {

	SceCtrlLatch latchData;
	
	g2dTexture* bg = g2dTexLoad("grid.png",G2D_SWIZZLE);
	g2dTexture* title = g2dTexLoad("title.png",G2D_SWIZZLE);
	g2dTexture* tex = g2dTexLoad("cell.png",G2D_SWIZZLE);
	g2dTexture* vir = g2dTexLoad("vir.png",G2D_SWIZZLE);
	
	g2dColors color[7] = {RED,ORANGE,YELLOW,GREEN,BLUE,VIOLET,ROSE};
	
	char* skins[5] = {"default","agarskin_cia.png","agarskin_facepunch.png","agarskin_tiktok.png","agarskin_sussy.png"};
	int mySkin = 1;
	
	g2dTexture* plr_skin = g2dTexLoad(skins[mySkin],G2D_SWIZZLE);
	
	Virus spikes[25];
	for (int i = 0; i < 25; i++) {
		spikes[i].virX = rand() % 2000 - 90;
		spikes[i].virY = rand() % 2000 + 10;
	}
	
	Food dots[250];
	for (int i = 0; i < 250; i++) {
		dots[i].foodX = rand() % 2000 - 90;
		dots[i].foodY = rand() % 2000 + 10;
		dots[i].color = rand() % 7;
	}
	
	AI plrs[60];
	for (int i = 0; i < 60; i++) {
		plrs[i].pX = rand() % 2000 - 90;
		plrs[i].pY = rand() % 2000 + 10;
		plrs[i].score = rand() % 300 + 60;
		plrs[i].tick = rand() % 20 + 1;
		plrs[i].dir = rand() % 3 + 1;
		plrs[i].color = rand() % 7;
		plrs[i].skin = rand() % 4;
		if (plrs[i].skin != 0){
		plrs[i].chosenSkin = g2dTexLoad(skins[plrs[i].skin],G2D_SWIZZLE);
		}
	}

	SceCtrlData ctrlData;
	while (running == 1) {
		if (state == 1) {
		g2dClear(WHITE);
		//background
		//round (plr x pos / 64 * 64)
		loop2 = 1;
		gridOff=-1024;
		gridOff2=1;
		while (loop2 <= 4) {
			g2dBeginRects(bg);
			g2dSetColor(WHITE);
			g2dSetScaleWH(512,512);
			g2dSetCoordXY(((round(x / 128) * 128) - x - (gridOff)) / (mass / 40),((round(y / 128) * 128) - y - (gridOff2))/ (mass / 40));
			g2dAdd();
			g2dEnd();
			gridOff+=512;
			loop2++;
		}
		
		//food
		for (int i = 0; i < 250; i++) {
			g2dBeginRects(NULL);
			g2dSetColor(color[dots[i].color]);
			g2dSetScaleWH(4,4);
			g2dSetCoordXY((dots[i].foodX - x) / (mass / 40) + 240,(dots[i].foodY - y) / (mass / 40) + 132);
			g2dAdd();
			g2dEnd();

			// collision detection for player
			if (collision(240 + (massSize / 2) ,132 + (massSize / 2) , (dots[i].foodX - x) / (mass / 40) + 240 , (dots[i].foodY - y) / (mass / 40) + 132 ,massSize,massSize,4,4)) {
				mass+= 10 / (massSize / 4);
				dots[i].foodX = rand() % 2000 - 90;
				dots[i].foodY = rand() % 2000 + 10;
			}
		}
		
		//ai players
		for (int loop = 0; loop < 60; loop++) {
			g2dBeginRects(tex);
			g2dSetColor(color[plrs[loop].color]);
			g2dSetScaleWH(plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40));
			g2dSetCoordXY((plrs[loop].pX - x) / (mass / 40) + 240,(plrs[loop].pY - y) / (mass / 40) + 132);
			g2dAdd();
			g2dEnd();
			if(plrs[loop].skin != 0){
				// render ai skin!
				g2dSetColor(WHITE);
				g2dBeginRects(plrs[loop].chosenSkin);
				g2dSetScaleWH(plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40));
				g2dSetCoordXY((plrs[loop].pX - x) / (mass / 40) + 240,(plrs[loop].pY - y) / (mass / 40) + 132);
				g2dAdd();
				g2dEnd();
			}
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
			for (int i2 = 0; i2 < 240; i2++) {
				if (collision((plrs[loop].pX - x) / (mass / 40) + 240 + plrs[loop].score ,(plrs[loop].pY - y) / (mass / 40) + 132 + plrs[loop].score , (dots[i2].foodX - x) / (mass / 40) + 240 , (dots[i2].foodY - y) / (mass / 40) + 132 ,plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40),4,4)) {
					plrs[loop].score+= 10 / (plrs[loop].score / 4);
					dots[i2].foodX = rand() % 2000 - 90;
					dots[i2].foodY = rand() % 2000 + 10;
				}
			}

			// ai collide with virus
			for (int i2 = 0; i2 < 25; i2++) {
				if (collision((plrs[loop].pX - x) / (mass / 40) + 240 + plrs[loop].score ,(plrs[loop].pY - y) / (mass / 40) + 132 + plrs[loop].score , (spikes[i2].virX - x) / (mass / 40) + 240 , (spikes[i2].virY - y) / (mass / 40) + 132 ,plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40),50 / (mass / 40),50 / (mass / 40))) {
					if (plrs[loop].score > 80) {
						plrs[loop].score - 25;
						spikes[i2].virX = rand() % 2000 - 90;
						spikes[i2].virY = rand() % 2000 + 10;
					}
				}
			}
			
			for (int i3 = 0; i3 < 60; i3++) {
				// ai eat ai
				if (collision((plrs[loop].pX - x) / (mass / 40) + 240 + plrs[loop].score ,(plrs[loop].pY - y) / (mass / 40) + 132 + plrs[loop].score , (plrs[i3].pX - x) / (mass / 40) + 240+ plrs[i3].score , (plrs[i3].pY - y) / (mass / 40) + 132 + plrs[i3].score ,plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40),plrs[i3].score / (mass / 40),plrs[i3].score / (mass / 40))) {
					if (plrs[loop].score > plrs[loop1].score + 30) {
						plrs[loop].score += (plrs[loop1].score / 4);
						plrs[i3].score = rand() % 100 + 10;
						plrs[i3].pX = rand() % 2000 - 90;
						plrs[i3].pY = rand() % 2000 + 10;
					}else if (plrs[loop1].score > plrs[loop].score + 30) {
						plrs[i3].score += (plrs[loop1].score / 4);
						plrs[loop].score = rand() % 100 + 10;
						plrs[loop].pX = rand() % 2000 - 90;
						plrs[loop].pY = rand() % 2000 + 10;
					}
				}
			}
			
			// collision detection for player
			if (collision(240 + (massSize / 2) ,132 + (massSize / 2) , (plrs[loop].pX - x) / (mass / 40) + 240 + plrs[loop].score , (plrs[loop].pY - y) / (mass / 40) + 132 + plrs[loop].score ,massSize,massSize,plrs[loop].score / (mass / 40),plrs[loop].score / (mass / 40))) {
				if (mass > plrs[loop].score + 30) {
					mass += (plrs[loop].score / 4);
					plrs[loop].score = 10;
					plrs[loop].pX = rand() % 2000 - 90;
					plrs[loop].pY = rand() % 2000 + 10;
				} else if (plrs[loop].score > mass - 15) {
				
					x = 0;
					y = 0;
					mass = 45;
					
					// reset positions of all players
					for (int i4 = 0; i4 < 100; i4++) {
						plrs[i4].pX = rand() % 2000 - 90;
						plrs[i4].pY = rand() % 2000 + 10;
						plrs[loop].score = rand() % 100 + 10;
						plrs[i4].tick = rand() % 20 + 1;
						plrs[i4].dir = rand() % 3 + 1;
						plrs[i4].color = rand() % 7;
					}
					
					state = 0;
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
			if (ctrlData.Buttons & PSP_CTRL_CIRCLE) {
				x = 0;
					y = 0;
					mass = 45;
					
					// reset positions of all players
					for (int i4 = 0; i4 < 100; i4++) {
						plrs[i4].pX = rand() % 2000 - 90;
						plrs[i4].pY = rand() % 2000 + 10;
						plrs[i4].score = rand() % 100 + 10;
						plrs[i4].tick = rand() % 20 + 1;
						plrs[i4].dir = rand() % 3 + 1;
						plrs[i4].color = rand() % 7;
					}
				state=0;
			}

			//cell

			// player is rendered before virus so it looks like player can hide behind them!
			g2dBeginRects(tex);
			g2dSetColor(RED);
			g2dSetScaleWH(massSize / (mass / 40),massSize / (mass / 40));
			g2dSetCoordXY((480 - massSize / (mass / 40)) / 2 ,(272 - massSize / (mass / 40)) / 2);
			massSize = mass / 2;

			g2dAdd();
			g2dEnd();

			// skin (added onto cell if skin is chosen)
			if(mySkin != 0){
				// apply the skin!
				g2dBeginRects(plr_skin);
				g2dSetColor(WHITE);
				g2dSetScaleWH(massSize / (mass / 40),massSize / (mass / 40));
				g2dSetCoordXY((480 - massSize / (mass / 40)) / 2 ,(272 - massSize / (mass / 40)) / 2);

				g2dAdd();
				g2dEnd();
			}

			// viruses
			for (int i = 0; i < 25; i++) {
				g2dBeginRects(vir);
				g2dSetColor(GREEN);
				if (spikes[i].type = 1) {
					// experimental cell
					g2dSetColor(RED);
				}
				g2dSetScaleWH(50 / (mass / 40),50 / (mass / 40));
				g2dSetCoordXY((spikes[i].virX - x) / (mass / 40) + 240,(spikes[i].virY - y) / (mass / 40) + 132);
				g2dAdd();
				g2dEnd();

				// collision detection for player
				if (collision(240 + (massSize / 2) ,132 + (massSize / 2) , (spikes[i].virX - x) / (mass / 40) + 240 , (spikes[i].virY - y) / (mass / 40) + 132 ,massSize,massSize,50 / (mass / 40),50 / (mass / 40))) {
					if (mass > 80) {
						if (mass - 25 > 40) {
							mass -= 25;
							spikes[i].virX = rand() % 2000 - 90;
							spikes[i].virY = rand() % 2000 + 10;
						}
					}
				}
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
			
			sceCtrlReadLatch(&latchData);
			
			sceCtrlReadBufferPositive(&ctrlData, 1);
			if (ctrlData.Buttons & PSP_CTRL_START) {
				state = 1;
			}
			if (ctrlData.Buttons & PSP_CTRL_SELECT) {
				running = 0;
			}
			
			if (latchData.uiBreak & PSP_CTRL_TRIANGLE){
				if (mySkin + 1 > (sizeof(skins)/sizeof(*skins))-1) {
					mySkin = 0;
				} else {
					mySkin++;
				}
				if(mySkin != 0){
					plr_skin = g2dTexLoad(skins[mySkin],G2D_SWIZZLE);
				}
			}
			
			//cell
			g2dBeginRects(tex);
			g2dSetColor(RED);
			g2dSetScaleWH(50,50);
			g2dSetCoordXY(370,132);
			massSize = mass / 2;

			g2dAdd();
			g2dEnd();
			
			// skin (added onto cell if skin is chosen)
			if(mySkin != 0){
				// apply the skin!
				g2dBeginRects(plr_skin);
				g2dSetColor(WHITE);
				g2dSetScaleWH(50,50);
				g2dSetCoordXY(370,132);

				g2dAdd();
				g2dEnd();
			}

			
			g2dFlip(G2D_VSYNC);
		}

		}
		
		sceKernelExitGame();
		return 0;

	}

