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
	// ���� ����Ʈ��, Ŭ���� ����Ʈ, ��Ʈ ����Ʈ ��� �ʱ�ȭ
	void ClearVector();
	NODE* GetNextNode();
	// Ư�� ��ġ�� ��带 ������ �� �ִ��� Ȯ���ϴ� �Լ�
	bool CheckTile(int x, int y);
	// ���� ����Ʈ���� ��带 �̾� ��ǥ�� ������ Ȯ���ϰ� ���� ��带 ����� ���� ����Ʈ�� �ִ� �Լ�
	void MakeNode(int current_x, int current_y, NODE* nowNode, e_Direction dir);
	// �������� ��ġ �Լ�
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

// ù���(�θ� ������ 8����)