#pragma once
#include "stdafx.h"
#define CHECK 10000

typedef struct tag_ST_POSITIONNODESTAR
{
	int x;
	int y;
	bool isStart;
	bool isEnd;
	bool isBlock;
	tag_ST_POSITIONNODESTAR()
	{
		x = 0;
		y = 0;
		isStart = isEnd = isBlock = false;
	}
	~tag_ST_POSITIONNODESTAR()
	{

	}
} POSITIONNODE;

typedef struct st_NODE
{
	int x, y;
	st_NODE* Parent;
	float ID;
	float G;
	float H;
	float F;

	st_NODE() : Parent(nullptr) {}
	st_NODE(int current_x, int current_y, st_NODE* _parent = nullptr) : x(current_x), y(current_y), Parent(_parent), G(0), H(0), ID(x* CHECK + y) {};

	float GetF()
	{
		F = G + H;
		return G + H;
	};
	float ManHattanDistance(st_NODE* endNode)
	{
		float x = (float)abs(endNode->x - this->x);
		float y = (float)abs(endNode->y - this->y);

		return x + y;
	}

} NODE;

typedef struct st_POSITION
{
	int X;
	int Y;
	st_POSITION() : X(0), Y(0) {}
	st_POSITION(int x, int y) : X(x), Y(y) {}
} POSITION;