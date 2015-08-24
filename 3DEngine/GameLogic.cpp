#include "stdafx.h"
#include "GameLogic.h"
#include "Line.h"
#include "Polygon2D.h"

void GameLogic::run(D3D7Helper* const helper, int pitch, UCHAR* video_buffer) {
	//Line line(0, 0, 300, 300);
	//line.drawClipLine(100, 100, 300, 300, 0xFFFF0000, video_buffer, pitch);

	int asteroid_vertices[16] = { 33,-3, 9,-18, -12,-9, -21,-12, -9,6, -15,15, -3,27, 21,21 };
	vector<Vertex2D> vlist;

	for (int index = 0; index < 16; index += 2) {
		vlist.push_back(Vertex2D(asteroid_vertices[index], asteroid_vertices[index + 1]));
	}

	Polygon2D p(1, 8, 100, 100, 0xFFFF0000, vlist);

	p.draw(pitch, video_buffer);
}