#pragma once
#include "D3D7Helper.h"

class GameLogic {
    public:
	    GameLogic() = default;
	    ~GameLogic() = default;
    
		void run(D3D7Helper* const helper, int pitch, UCHAR* video_buffer);
};