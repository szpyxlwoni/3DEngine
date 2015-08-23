#pragma once

enum CLIP_CODE {
	CENTER = 0,
	NORTH = 8,
	EAST = 2,
	WEST = 1,
	SOUTH = 4,
	NW = 9,
	NE = 10,
	SE = 6,
	SW = 5
};

int Draw_Line(int x0, int y0, int x1, int y1, DWORD color, UCHAR* surface_start, int lpitch);

int Clip_Line(int& x0, int& y0, int& x1, int& y1, int max_clip_x, int min_clip_x, int max_clip_y, int min_clip_y);