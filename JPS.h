#pragma once
#include "stdafx.h"
#include "Node.h"

#define EXCEEDNUM(X , Y) (( X < 0) || (X >= 100) || (Y < 0) || (Y >= 100))


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

class CJPS
{
public:
	CJPS();
	~CJPS();

public:
	void FindPath(int start_x, int start_y, int end_x, int end_y);

	void PrintGrid(HDC hdc);
	void PrintBlock(HDC hdc);
	void PrintStart(HDC hdc);
	void PrintEnd(HDC hdc);
	void PrintNode(HDC hdc);
	void PrintRoute(HDC hdc);
	void SetHDC(HDC hdc);
	void ResetPoint();
	void PointBlockPoint(int x, int y);
	void PointBlockPoint(int x, int y, bool flag);
	void SetPointEnd(int x, int y, bool flag);
	void SetPointStart(int x, int y, bool flag);
	void SetEndIsClicked(bool flag);
	void SetStartIsClicked(bool flag);
	bool GetStartIsClicked();
	bool GetEndIsClicked();
	void ClearVector();
	void PrintRouteBresenham(HDC hdc);
	void SaveMap();
	void LoadMap();
	//bool BresenHam(int start_x, int start_y, int end_x, int end_y);

private:
	void SetStartAndGoal(NODE start, NODE goal);
	void InitialArray();
	// 오픈 리스트와, 클로즈 리스트, 루트 리스트 모두 초기화
	NODE* GetNextNode();
	// 특정 위치에 노드를 생성할 수 있는지 확인하는 함수
	bool CheckTile(int x, int y);
	// 오픈 리스트에서 노드를 뽑아 목표와 같은지 확인하고 새로 노드를 만들어 오픈 리스트에 넣는 함수
	bool MakeNode(int current_x, int current_y, NODE* nowNode, e_Direction dir);
	// 실질적인 서치 함수
	bool JumpCheck(int current_x, int current_y, int* newX, int* newY, float currentG, float* newG, e_Direction dir);
	bool CornerCheck(int current_x, int current_y, e_Direction dir);
	bool BresenHam(int start_x, int start_y, int end_x, int end_y);
	void LineLow(int start_x, int start_y, int end_x, int end_y, std::vector<POSITION>* temp);
	void LineHigh(int start_x, int start_y, int end_x, int end_y, std::vector<POSITION>* temp);


	void ContinueFindingPath();
	void ProcessWindowMessage();
	void CreateBrushAndPen();
	void DestroyBrushAndPen();
	void CreateBrush();

	POSITIONNODE m_NodeArray[100][100];

	bool m_bInitializedStartGoal;
	bool m_bFoundGoal;
	bool m_bEndClicked;
	bool m_bStartClicked;
	int m_iStartX;
	int m_iStartY;
	int m_iEndX;
	int m_iEndY;
	int m_iBrushAndPenIndex;

	HDC m_Hdc;
	NODE* m_pStartNode;
	NODE* m_pGoalNode;
	std::vector<NODE*> m_vOpenList;
	std::vector<NODE*> m_vCloseList;
	std::vector<POSITION*> m_vPathToGoal;
	std::vector<HBRUSH> m_vBrushHandles;
	std::vector<HPEN> m_vPenHandles;

	HBRUSH MyBrushGray = CreateSolidBrush(RGB(60, 60, 60));
	HPEN MyPenGray = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HBRUSH MyBrushGreen = CreateSolidBrush(RGB(0, 255, 0));
	HPEN MyPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HBRUSH MyBrushRed = CreateSolidBrush(RGB(255, 0, 0));
	HPEN MyPenRed = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	HBRUSH MyBrushYellow = CreateSolidBrush(RGB(255, 255, 0));
	HPEN MyPenYellow = CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	HBRUSH MyBrushBrown = CreateSolidBrush(RGB(180, 100, 0));
	HPEN MyPenBrown = CreatePen(PS_SOLID, 1, RGB(180, 100, 0));
	HPEN MyPenBlue = CreatePen(PS_SOLID, 3, RGB(0, 50, 255));

};

// 첫노드(부모가 없으면 8방향)