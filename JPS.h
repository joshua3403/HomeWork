#pragma once
#include "stdafx.h"
#define CHECK 10000

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

enum e_Direction
{
	NONE = 0,
	UP = 1,
	UPRIGHT,
	RIGHT,
	RIGHTDOWN,
	DOWN,
	DOWNLEFT,
	LEFT,
	LEFTUP
};

typedef struct st_NODE
{
	int x, y;
	st_NODE* Parent;
	float ID;
	float G;
	float H;
	float F;
	e_Direction Dir;
	st_NODE() : Parent(nullptr) {}
	st_NODE(int current_x, int current_y, st_NODE* _parent = nullptr, e_Direction dir = e_Direction::NONE) : x(current_x), y(current_y), Parent(_parent), G(0), H(0), ID(x* CHECK + y), Dir(dir) {};

	float GetF() { return G + H; };
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

class CJPS
{
private:
	CJPS();
	~CJPS();

public:
	void FindPath(int start_x, int start_y, int end_x, int end_y);

private:
	void SetStartAndGoal(NODE start, NODE goal);
	void InitialArray();
	// 오픈 리스트와, 클로즈 리스트, 루트 리스트 모두 초기화
	void ClearVector();
	NODE* GetNextNode();
	// 특정 위치에 노드를 생성할 수 있는지 확인하는 함수
	bool CheckTile(int x, int y);
	// 오픈 리스트에서 노드를 뽑아 목표와 같은지 확인하고 새로 노드를 만들어 오픈 리스트에 넣는 함수
	void MakeNode(int current_x, int current_y, NODE* nowNode, e_Direction dir);
	// 실질적인 서치 함수
	bool JumpCheck(int current_x, int current_y, int* newX, int* newY, float currentG, float* newG, e_Direction dir);
	bool CornerCheck(int current_x, int current_y, e_Direction dir);


	void ContinueFindingPath();

	POSITIONNODE m_NodeArray[100][100];

	bool m_bInitializedStartGoal;
	bool m_bFoundGoal;
	NODE* m_pStartNode;
	NODE* m_pGoalNode;
	std::vector<NODE*> m_vOpenList;
	std::vector<NODE*> m_vCloseList;
	std::vector<POSITION*> m_vPathToGoal;
};

// 첫노드(부모가 없으면 8방향)