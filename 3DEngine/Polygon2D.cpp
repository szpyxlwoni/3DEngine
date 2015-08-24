#include "stdafx.h"
#include "Polygon2D.h"
#include "Line.h"

void Polygon2D::draw(int pitch, UCHAR* video_buffer) {
	for (int index = 0; index < num_verts - 1; index++) {
		Line l(x0 + vlist[index].x, y0 + vlist[index].y, x0 + vlist[index + 1].x, y0 + vlist[index + 1].y);
		l.drawClipLine(0, 0, 400, 400, 0xFFFF0000, video_buffer, pitch);
	}

	Line l(x0 + vlist[num_verts - 1].x, y0 + vlist[num_verts - 1].y, x0 + vlist[0].x, y0 + vlist[0].y);
	l.drawClipLine(0, 0, 400, 400, 0xFFFF0000, video_buffer, pitch);
}