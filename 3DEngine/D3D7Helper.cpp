#include "stdafx.h"
#include "D3D7Helper.h"

void D3D7Helper::gameInit() {
	RECT window_rect = { 0,0, width - 1, height- 1 };

	AdjustWindowRectEx(&window_rect,
		(DWORD)GetWindowLong(hwnd, GWL_STYLE),
		GetMenu(hwnd) != NULL,
		(DWORD)GetWindowLong(hwnd, GWL_EXSTYLE));

	window_client_x0 = -window_rect.left;
	window_client_y0 = -window_rect.top;

	if (FAILED(DirectDrawCreateEx(NULL, (void **)&lpdd, IID_IDirectDraw7, NULL)))
		return;

	if (FAILED(lpdd->SetCooperativeLevel(hwnd, DDSCL_NORMAL)))
	{
		return;
	}

	DDSURFACEDESC2 ddsd;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	ddsd.dwFlags = DDSD_CAPS;
	ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;

	ddsd.dwBackBufferCount = 0;

	if (FAILED(lpdd->CreateSurface(&ddsd, &lpddsprimary, NULL)))
	{
		return;
	}

	lpddsback = createSurface(width, height);

	fillSurface(lpddsback, 0);

	RECT screen_rect = {0, 0, width, height};
	lpddclipper = attachClipper(lpddsback, 1, &screen_rect);

	if (FAILED(lpdd->CreateClipper(0, &lpddclipperwin, NULL)))
		return;

	if (FAILED(lpddclipperwin->SetHWnd(0, hwnd)))
		return;

	if (FAILED(lpddsprimary->SetClipper(lpddclipperwin)))
		return;
}

void D3D7Helper::gameMain(int frame) {
	DDSURFACEDESC2 ddsd;

	memset(&ddsd, 0, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);

	if (FAILED(lpddsback->Lock(NULL, &ddsd,
		DDLOCK_SURFACEMEMORYPTR | DDLOCK_WAIT,
		NULL)))
	{
		return;
	}

	GameLogic g;
	g.run(this, (int)ddsd.lPitch, (UCHAR *)ddsd.lpSurface);

	if (FAILED(lpddsback->Unlock(NULL)))
		return;

	flip();

	Sleep(1000.0/frame);
}

void D3D7Helper::gameShutdown() {
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
}

LPDIRECTDRAWSURFACE7 D3D7Helper::createSurface(int width, int height) {
	DDSURFACEDESC2 ddsd;
	LPDIRECTDRAWSURFACE7 lpdds;

	initDesc(ddsd);
	ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
	ddsd.dwWidth = width;
	ddsd.dwHeight = height;

	if (FAILED(lpdd->CreateSurface(&ddsd, &lpdds, NULL)))
		return(NULL);

	return(lpdds);
}

void D3D7Helper::fillSurface(LPDIRECTDRAWSURFACE7 lpdds, USHORT color, RECT *client)
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
}

LPDIRECTDRAWCLIPPER D3D7Helper::attachClipper(LPDIRECTDRAWSURFACE7 lpdds, int num_rects, LPRECT clip_list)
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

void D3D7Helper::flip()
{
	RECT    dest_rect;

	GetWindowRect(hwnd, &dest_rect);

	dest_rect.left += window_client_x0 + 20;
	dest_rect.top += window_client_y0 + 50;

	dest_rect.right = dest_rect.left + width;
	dest_rect.bottom = dest_rect.top + height;

	lpddsprimary->Blt(&dest_rect, lpddsback, NULL, DDBLT_WAIT, NULL);
}