#include "stdafx.h"
#include "D3D7Helper.h"
#include "2DHelper.h"

extern HWND hwnd; // save the window handle
extern HINSTANCE hinst; // save the instance

//game var
LPDIRECTDRAW7         lpdd = nullptr;
LPDIRECTDRAWSURFACE7  lpddsprimary = nullptr;
LPDIRECTDRAWSURFACE7  lpddsback = nullptr;
LPDIRECTDRAWCLIPPER   lpddclipper = nullptr;
LPDIRECTDRAWCLIPPER   lpddclipperwin = nullptr;

int min_clip_x = 0,
max_clip_x = (SCREEN_WIDTH - 1),
min_clip_y = 0,
max_clip_y = (SCREEN_HEIGHT - 1);

int screen_width = SCREEN_WIDTH,
screen_height = SCREEN_HEIGHT,
screen_bpp = SCREEN_BPP;

int window_client_x0 = 0;
int window_client_y0 = 0;

int Game_Init() {
	RECT window_rect = { 0,0,SCREEN_WIDTH - 1,SCREEN_HEIGHT - 1 };

	AdjustWindowRectEx(&window_rect,
		(DWORD)GetWindowLong(hwnd, GWL_STYLE),
		GetMenu(hwnd) != NULL,
		(DWORD)GetWindowLong(hwnd, GWL_EXSTYLE));

	window_client_x0 = -window_rect.left;
	window_client_y0 = -window_rect.top;

	DDraw_Init(SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP);

	return 1;
}

int DDraw_Init(int width, int height, int bpp) {
	if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL)))
		return(0);

	if (FAILED(lpdd->SetCooperativeLevel(hwnd, DDSCL_NORMAL)))
	{
		return(0);
	}

	screen_height = height;
	screen_width = width;
	screen_bpp = bpp;

	DDSURFACEDESC2 ddsd;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	ddsd.dwBackBufferCount = 0;

	if (FAILED(lpdd->CreateSurface(&ddsd, &lpddsprimary, NULL)))
	{
		return(0);
	}

	lpddsback = DDraw_Create_Surface(width, height);

	DDraw_Fill_Surface(lpddsback, 0);

	RECT screen_rect = { 0,0,screen_width,screen_height };
	lpddclipper = DDraw_Attach_Clipper(lpddsback, 1, &screen_rect);

	if (FAILED(lpdd->CreateClipper(0, &lpddclipperwin, NULL)))
		return(0);

	if (FAILED(lpddclipperwin->SetHWnd(0, hwnd)))
		return(0);

	if (FAILED(lpddsprimary->SetClipper(lpddclipperwin)))
		return(0);

	return(1);
}

LPDIRECTDRAWSURFACE7 DDraw_Create_Surface(int width, int height)
{
	DDSURFACEDESC2 ddsd;
	LPDIRECTDRAWSURFACE7 lpdds;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;

	ddsd.dwWidth = width;
	ddsd.dwHeight = height;

	if (FAILED(lpdd->CreateSurface(&ddsd, &lpdds, NULL)))
		return(NULL);

	return(lpdds);
}

int DDraw_Fill_Surface(LPDIRECTDRAWSURFACE7 lpdds, USHORT color, RECT *client)
{
	DDBLTFX ddbltfx;

	memset(&ddbltfx, 0, sizeof(ddbltfx));
	ddbltfx.dwSize = sizeof(ddbltfx);

	ddbltfx.dwFillColor = color;

	lpdds->Blt(client,
		NULL,
		NULL,
		DDBLT_COLORFILL | DDBLT_WAIT,
		&ddbltfx);

	return(1);
}

LPDIRECTDRAWCLIPPER DDraw_Attach_Clipper(LPDIRECTDRAWSURFACE7 lpdds,
	int num_rects,
	LPRECT clip_list)

{
	int index;
	LPDIRECTDRAWCLIPPER lpddclipper;
	LPRGNDATA region_data;

	if (FAILED(lpdd->CreateClipper(0, &lpddclipper, NULL)))
		return(NULL);

	region_data = (LPRGNDATA)malloc(sizeof(RGNDATAHEADER) + num_rects*sizeof(RECT));

	memcpy(region_data->Buffer, clip_list, sizeof(RECT)*num_rects);

	region_data->rdh.dwSize = sizeof(RGNDATAHEADER);
	region_data->rdh.iType = RDH_RECTANGLES;
	region_data->rdh.nCount = num_rects;
	region_data->rdh.nRgnSize = num_rects*sizeof(RECT);

	region_data->rdh.rcBound.left = 64000;
	region_data->rdh.rcBound.top = 64000;
	region_data->rdh.rcBound.right = -64000;
	region_data->rdh.rcBound.bottom = -64000;

	for (index = 0; index<num_rects; index++)
	{
		if (clip_list[index].left < region_data->rdh.rcBound.left)
			region_data->rdh.rcBound.left = clip_list[index].left;

		if (clip_list[index].right > region_data->rdh.rcBound.right)
			region_data->rdh.rcBound.right = clip_list[index].right;

		if (clip_list[index].top < region_data->rdh.rcBound.top)
			region_data->rdh.rcBound.top = clip_list[index].top;

		if (clip_list[index].bottom > region_data->rdh.rcBound.bottom)
			region_data->rdh.rcBound.bottom = clip_list[index].bottom;
	}
	
	if (FAILED(lpddclipper->SetClipList(region_data, 0)))
	{
		free(region_data);
		return(NULL);
	}

	if (FAILED(lpdds->SetClipper(lpddclipper)))
	{
		free(region_data);
		return(NULL);
	}

	free(region_data);
	return(lpddclipper);
}

int Game_Main() {
	DDSURFACEDESC2 ddsd;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	if (FAILED(lpddsback->Lock(NULL, &ddsd,
		DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
		NULL)))
	{
		return(0);
	}

	int mempitch = (int)ddsd.lPitch;
	UCHAR *video_buffer = (UCHAR *)ddsd.lpSurface;
	int x0 = 0;
	int y0= 0;
	int x1 = 500;
	int y1 = 500;

	Clip_Line(x0, y0, x1, y1, 100, 100, 400, 400);
	Draw_Line(x0, y0, x1, y1, 0xFFFF0000, video_buffer, mempitch);

	if (FAILED(lpddsback->Unlock(NULL)))
		return(0);

	DDraw_Flip();

	Sleep(30);

	return 1;
}

int DDraw_Flip(void)
{
	RECT    dest_rect;

	GetWindowRect(hwnd, &dest_rect);

	dest_rect.left += window_client_x0 + 20;
	dest_rect.top += window_client_y0 + 50;

	dest_rect.right = dest_rect.left + screen_width;
	dest_rect.bottom = dest_rect.top + screen_height;

	if (FAILED(lpddsprimary->Blt(&dest_rect, lpddsback, NULL, DDBLT_WAIT, NULL)))
		return(0);

	return(1);

}

int Game_Shutdown() {
	if (lpddclipper)
		lpddclipper->Release();

	if (lpddclipperwin)
		lpddclipperwin->Release();

	if (lpddsback)
		lpddsback->Release();

	if (lpddsprimary)
		lpddsprimary->Release();

	if (lpdd)
		lpdd->Release();

	return 1;
}