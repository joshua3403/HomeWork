#pragma once
#define CHECK 10000

#include "stdafx.h"
#include "Node.h"


class AStarPathFinding
{
public:
	AStarPathFinding();
	~AStarPathFinding();

	void FindPath(int current_x, int current_y, int end_x, int end_y)
	{
		if (!m_bInitializedStartGoal)
		{
			ClearVector();

			// 알고리즘 시작 전 초기화
			NODE start;
			start.x = current_x;
			start.y = current_y;

			NODE goal;
			goal.x = end_x;
			goal.y = end_y;

			SetStartAndGoal(start, goal);
			m_bInitializedStartGoal = true;
		}

		if (m_bInitializedStartGoal)
		{
			ContinuePath();
			m_bInitializedStartGoal = !m_bInitializedStartGoal;
		}
	}

	void SetDC(HDC hdc);

	std::vector<NODE*> GetOpenList();
	std::vector<NODE*> GetCloseList();
	std::vector<POSITION*> GetRout();
	void ClearVector();

	bool m_bInitializedStartGoal;
	bool m_bFoundGoal;

	void SetPointBlock(int x, int y);
	void SetPointBlock(int x, int y, bool flag);
	void SetPointStart(int x, int y, bool flag);
	void SetPointEnd(int x, int y, bool flag);
	void SetEndIsClicked(bool flag);
	void SetStartIsClicked(bool flag);
	bool GetStartIsClicked();
	bool GetEndIsClicked();
	void ResetPoint();

	void PrintBlock(HDC hdc);
	void PrintStart(HDC hdc);
	void PrintOpen(HDC hdc);
	void PrintRoute(HDC hdc);
	void PrintEnd(HDC hdc);

	void SaveMap();
	void LoadMap(const char* temp);
	void RandomMap();

private:
	void SetStartAndGoal(NODE start, NODE goal);
	void PathOpened(int x, int y, float newG, NODE* parent);
	NODE* GetNextNode();
	void ContinuePath();

	POSITIONNODE m_NodeArray[100][100];
	bool m_bEndClicked;
	bool m_bStartClicked;
	int m_iStartX;
	int m_iStartY;
	int m_iEndX;
	int m_iEndY;

	HBRUSH MyBrushYellow = CreateSolidBrush(RGB(255, 255, 0));
	HPEN MyPenYellow = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	HBRUSH MyBrushGray = CreateSolidBrush(RGB(60, 60, 60));
	HPEN MyPenGray = CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	HBRUSH MyBrushGreen = CreateSolidBrush(RGB(0, 255, 0));
	HPEN MyPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	HBRUSH MyBrushRed = CreateSolidBrush(RGB(255, 0, 0));
	HPEN MyPenRed = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	HBRUSH MyBrushBlue = CreateSolidBrush(RGB(0, 0, 255));
	HPEN MyPenBlue = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	HDC m_hdc;
	NODE* m_pStartNode;
	NODE* m_pGoalNode;
	std::vector<NODE*> m_vOpenList;
	std::vector<NODE*> m_vCloseList;
	std::vector<POSITION*> m_vPathToGoal;

	void InitialArray();

	void ProcessWindowMessage();

};