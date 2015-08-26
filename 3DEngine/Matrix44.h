#pragma once

class Matrix44 {
	public:
		Matrix44() = default;
		~Matrix44() = default;
    private:
		float matrix[4][4];
};