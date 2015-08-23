#pragma once
#include "ddraw.h"

class D3D7Helper {
	public:
		D3D7Helper(HWND& hWnd, int width, int height) : hwnd(hWnd), width(width), height(height) {};
		~D3D7Helper() = default;

		void GameInit();
		void GameMain();
		void GameShutdown();
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
		HWND hwnd;
		const int width;
		const int height;
		int window_client_x0 = 0;
		int window_client_y0 = 0;
		LPDIRECTDRAW7         lpdd = nullptr;
		LPDIRECTDRAWSURFACE7  lpddsprimary = nullptr;
		LPDIRECTDRAWSURFACE7  lpddsback = nullptr;
		LPDIRECTDRAWCLIPPER   lpddclipper = nullptr;
		LPDIRECTDRAWCLIPPER   lpddclipperwin = nullptr;
};