#pragma once
#define CHECK 10000

#include "stdafx.h"

typedef struct tag_ST_POSITIONNODE
{
	int x;
	int y;
	bool isStart;
	bool isEnd;
	bool isBlock;
	tag_ST_POSITIONNODE()
	{
		x = 0;
		y = 0;
		isStart = isEnd = isBlock = false;
	}
	~tag_ST_POSITIONNODE()
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

	st_NODE() : Parent(nullptr){}
	st_NODE(int current_x, int current_y, st_NODE* _parent = nullptr) : x(current_x), y(current_y), Parent(_parent),G(0), H(0), ID(x*CHECK + y) {};

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
	st_POSITION(int x, int y): X(x), Y(y){}
} POSITION;

class AStarPathFinding
{
public:
	AStarPathFinding();
	~AStarPathFinding();

	void FindPath(int current_x, int current_y, int end_x, int end_y);

	void SetDC(HDC hdc);

	std::vector<NODE*> GetOpenList();
	std::vector<NODE*> GetCloseList();
	std::vector<POSITION*> GetRout();
	void ClearVector();
	void ChangeParent(int x, int y, NODE* newNode);

	bool m_bInitializedStartGoal;
	bool m_bFoundGoal;

	void SetPointBlock(int x, int y);
	void SetPointBlock(int x, int y, bool flag);
	void SetPointStart(int x, int y, bool flag);
	void SetPointEnd(int x, int y, bool flag);
	void ResetPoint();

	void PrintBlock(HDC hdc);

private:
	void SetStartAndGoal(NODE start, NODE goal);
	void PathOpened(int x, int y, float newG, NODE* parent);
	NODE* GetNextNode();
	void ContinuePath();

	POSITIONNODE m_NodeArray[100][100];

	HBRUSH MyBrushYellow = CreateSolidBrush(RGB(255, 255, 0));
	HPEN MyPenYellow = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HBRUSH MyBrushGray = CreateSolidBrush(RGB(125, 125, 125));
	HPEN MyPenGray = CreatePen(PS_SOLID, 1, RGB(125, 125, 125));

	HDC m_hdc;
	NODE* m_pStartNode;
	NODE* m_pGoalNode;
	std::vector<NODE*> m_vOpenList;
	std::vector<NODE*> m_vCloseList;
	std::vector<POSITION*> m_vPathToGoal;

	void InitialArray();

	void ProcessWindowMessage();

};