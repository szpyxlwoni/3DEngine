#include "stdafx.h"
#include "2DHelper.h"

int Draw_Line(int x0, int y0, int x1, int y1, DWORD color, UCHAR* surface_start, int lpitch) {
	int dx,
		dy,
		dx2,
		dy2,
		x_inc,
		y_inc,
		error_test,
		index;

	surface_start += x0 + y0 * lpitch;

	dx = x1 - x0;
	dy = y1 - y0;

	if (dx >= 0) {
		x_inc = 4;
	}
	else {
		x_inc = -4;
		dx = -dx;
	}

	if (dy >= 0) {
		y_inc = lpitch;
	}
	else {
		y_inc = -lpitch;
		dy = -dy;
	}

	dx2 = dx << 1;
	dy2 = dy << 1;

	if (dx > dy) {
		error_test = dy2 - dx;

		for (index = 0; index <= dx; index++) {
			*((DWORD *)surface_start) = color;

			if (error_test >= 0) {
				error_test -= dx2;

				surface_start += y_inc;
			}

			error_test += dy2;

			surface_start += x_inc;
		}
	}
	else {
		error_test = dx2 - dy;

		for (index = 0; index <= dy; index++) {
			*((DWORD *)surface_start) = color;

			if (error_test >= 0) {
				error_test -= dy2;

				surface_start += x_inc;
			}

			error_test += dx2;

			surface_start += y_inc;
		}
	}

	return(1);
}

int Clip_Line(int& x0, int& y0, int& x1, int& y1, int min_clip_x, int min_clip_y, int max_clip_x, int max_clip_y) {
	int cc = 0, xc0 = 0, yc0 = 0, xc1 = 0, yc1 = 0;
	if (x0 > max_clip_x) {
		cc += EAST;
	}
	else if (x0 < min_clip_x) {
		cc += WEST;
	}
	if (y0 > max_clip_y) {
		cc += SOUTH;
	}
	else if (y0 < min_clip_y) {
		cc += NORTH;
	}

	switch(cc) {
	case CENTER:
		break;
	case NORTH:
		yc0 = min_clip_y;
		xc0 = x0 + 0.5 + (min_clip_y - y0) * (x1 - x0) / (y1 - y0);
		break;
	case SOUTH:
		yc0 = max_clip_y;
		xc0 = x0 + 0.5 + (max_clip_y - y0) * (x1 - x0) / (y1 - y0);
		break;
	case WEST:
		yc0 = y0 + 0.5 + (min_clip_x - 0) * (y1 - y0) / (x1 - x0);
		xc0 = min_clip_x;
		break;
	case EAST:
		yc0 = y0 + 0.5 + (max_clip_x - x0) * (y1 - y0) / (x1 - x0);
		xc0 = max_clip_x;
		break;
	case SW:
		yc0 = max_clip_y;
		xc0 = x0 + 0.5 + (max_clip_y - y0) * (x1 - x0) / (y1 - y0);

		if (xc0 < min_clip_x || xc0 > max_clip_x) {
			yc0 = y0 + 0.5 + (min_clip_x - x0) * (y1 - y0) / (x1 - x0);
			xc0 = min_clip_x;
		}
		break;
	case SE:
		yc0 = max_clip_y;
		xc0 = x0 + 0.5 + (max_clip_y - y0) * (x1 - x0) / (y1 - y0);

		if (xc0 < min_clip_x || xc0 > max_clip_x) {
			yc0 = y0 + 0.5 + (max_clip_x - x0) * (y1 - y0) / (x1 - x0);
			xc0 = max_clip_x;
		}
		break;
	case NE:
		yc0 = min_clip_y;
		xc0 = x0 + 0.5 + (min_clip_y - y0) * (x1 - x0) / (y1 - y0);

		if (xc0 < min_clip_x || xc0 > max_clip_x) {
			yc0 = y0 + 0.5 + (max_clip_x - x0) * (y1 - y0) / (x1 - x0);
			xc0 = max_clip_x;
		}
		break;
	case NW:
		yc0 = min_clip_y;
		xc0 = x0 + 0.5 + (min_clip_y - y0) * (x1 - x0) / (y1 - y0);

		if (xc0 < min_clip_x || xc0 > max_clip_x) {
			yc0 = y0 + 0.5 + (min_clip_x - x0) * (y1 - y0) / (x1 - x0);
			xc0 = min_clip_x;
		}
		break;
	default:
		break;
	}

	cc = 0;
	if (x1 > max_clip_x) {
		cc += EAST;
	}
	else if (x1 < min_clip_x) {
		cc += WEST;
	}
	if (y1 > max_clip_y) {
		cc += SOUTH;
	}
	else if (y1 < min_clip_y) {
		cc += NORTH;
	}

	switch (cc) {
	case CENTER:
		break;
	case NORTH:
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1) * (x1 - x0) / (y1 - y0);
		break;
	case SOUTH:
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1) * (x1 - x0) / (y1 - y0);
		break;
	case WEST:
		yc1 = y1 + 0.5 + (min_clip_x - x1) * (y1 - y0) / (x1 - x0);
		xc1 = min_clip_x;
		break;
	case EAST:
		yc1 = y1 + 0.5 + (max_clip_x - x1) * (y1 - y0) / (x1 - x0);
		xc1 = max_clip_x;
		break;
	case SW:
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1) * (x1 - x0) / (y1 - y0);

		if (xc1 < min_clip_x || xc1 > max_clip_x) {
			yc1 = y1 + 0.5 + (min_clip_x - x1) * (y1 - y0) / (x1 - x0);
			xc1 = min_clip_x;
		}
		break;
	case SE:
		yc1 = max_clip_y;
		xc1 = x1 + 0.5 + (max_clip_y - y1) * (x1 - x0) / (y1 - y0);

		if (xc1 < min_clip_x || xc1 > max_clip_x) {
			yc1 = y1 + 0.5 + (max_clip_x - x1) * (y1 - y0) / (x1 - x0);
			xc1 = max_clip_x;
		}
		break;
	case NE:
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1) * (x1 - x0) / (y1 - y0);

		if (xc1 < min_clip_x || xc1 > max_clip_x) {
			yc1 = y1 + 0.5 + (max_clip_x - x1) * (y1 - y0) / (x1 - x0);
			xc1 = max_clip_x;
		}
		break;
	case NW:
		yc1 = min_clip_y;
		xc1 = x1 + 0.5 + (min_clip_y - y1) * (x1 - x0) / (y1 - y0);

		if (xc1 < min_clip_x || xc1 > max_clip_x) {
			yc1 = y1 + 0.5 + (min_clip_x - x1) * (y1 - y0) / (x1 - x0);
			xc1 = min_clip_x;
		}
		break;
	default:
		break;
	}

	x0 = xc0;
	x1 = xc1;
	y0 = yc0;
	y1 = yc1;

	return(1);
}