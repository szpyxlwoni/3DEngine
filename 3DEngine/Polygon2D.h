#pragma once
#include "stdafx.h"
#include <vector>

using std::vector;

class Vertex2D {
	public:
		Vertex2D(float x, float y) :x(x), y(y) {};
		float x, y;
};

class Polygon2D {
    public:
		Polygon2D(int state, int num_verts, int x0, int y0, DWORD color, vector<Vertex2D> vlist)
			:state(state), num_verts(num_verts), x0(x0), y0(y0), color(color), vlist(vlist) {};

		void addVertex(Vertex2D vert) {
			vlist.push_back(vert);
		};

		void draw(int pitch, UCHAR* video_buffer);
	private:
		int state;
		int num_verts;
		int x0, y0;
		int xv = 0, yv = 0;
		DWORD color;
		vector<Vertex2D> vlist;
};
