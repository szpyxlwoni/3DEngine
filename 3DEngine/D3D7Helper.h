#pragma once
#include "ddraw.h"

#define SCREEN_WIDTH    640
#define SCREEN_HEIGHT   480
#define SCREEN_BPP      32

class D3D7Helper {
	public:
		D3D7Helper();
		~D3D7Helper();

		void GameInit();
		void GameMain();
		void GameShutdown();
};

int Game_Init();

int Game_Main();

int Game_Shutdown();

int DDraw_Init(int width, int height, int bpp);

LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width, int height);

int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds, USHORT color, RECT *client = NULL);

LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds, int num_rects, LPRECT clip_list);

int DDraw_Flip(void);

//game var
extern LPDIRECTDRAW7         lpdd;
extern LPDIRECTDRAWSURFACE7  lpddsprimary;
extern LPDIRECTDRAWSURFACE7  lpddsback;
extern LPDIRECTDRAWCLIPPER   lpddclipper;

extern int screen_width, screen_height, screen_bpp;

extern int window_client_x0;
extern int window_client_y0;