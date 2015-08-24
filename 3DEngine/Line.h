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

class Line {
    public:
		Line(int x0, int y0, int x1, int y1): x0(x0), y0(y0), x1(x1), y1(y1) {};
		~Line() = default;

		void drawClipLine(int min_clip_x, int min_clip_y, int max_clip_x, int max_clip_y, DWORD color, UCHAR* surface_start, int lpitch);
    private:
		void drawLine(DWORD color, UCHAR* surface_start, int lpitch);
		void clipLine(int min_clip_x, int min_clip_y, int max_clip_x, int max_clip_y);
    private:
		int x0, x1, y0, y1;
		int drawX0 = 0, drawX1 = 0, drawY0 = 0, drawY1 = 0;
};