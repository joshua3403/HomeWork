#pragma once
typedef struct st_POSITION
{
	int X;
	int Y;
	st_POSITION() : X(0), Y(0) {}
	st_POSITION(int x, int y) : X(x), Y(y) {}
} POSITION;

class CBresenham
{
public:
	POSITION LowLine(POSITION* start, POSITION* end);
	POSITION HighLine(POSITION* start, POSITION* end);


};