#include "stdafx.h"
#include "GameLogic.h"

void GameLogic::run(D3D7Helper* const helper, int pitch, UCHAR* video_buffer) {
	Line line(0, 0, 300, 300);
	line.drawClipLine(100, 100, 300, 300, 0xFFFF0000, video_buffer, pitch);
}