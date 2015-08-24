#pragma once
#include "ddraw.h"
#include "GameLogic.h"

class D3D7Helper {
	public:
		D3D7Helper(HWND& hWnd, int width, int height) : hwnd(hWnd), width(width), height(height), min_clip_x(10),
		    min_clip_y(10), max_clip_x(width - 10), max_clip_y(height - 10){};
		~D3D7Helper() = default;

		void gameInit();
		void gameMain(int frame);
		void gameShutdown();
    private:
		void initDesc(DDSURFACEDESC2& ddsd) {
			memset(&ddsd, 0, sizeof(ddsd));
			ddsd.dwSize = sizeof(ddsd);
		}

		LPDIRECTDRAWSURFACE7 createSurface(int width, int height);
		LPDIRECTDRAWCLIPPER attachClipper(LPDIRECTDRAWSURFACE7 lpdds, int num_rects, LPRECT clip_list);
		void fillSurface(LPDIRECTDRAWSURFACE7 lpdds, USHORT color, RECT *client = nullptr);
		void flip();
    private:
		friend class GameLogic;
		HWND hwnd;
		const int width;
		const int height;
		int window_client_x0 = 0;
		int window_client_y0 = 0;
		int min_clip_x,
			max_clip_x,
			min_clip_y,
			max_clip_y;
		LPDIRECTDRAW7         lpdd = nullptr;
		LPDIRECTDRAWSURFACE7  lpddsprimary = nullptr;
		LPDIRECTDRAWSURFACE7  lpddsback = nullptr;
		LPDIRECTDRAWCLIPPER   lpddclipper = nullptr;
		LPDIRECTDRAWCLIPPER   lpddclipperwin = nullptr;
};