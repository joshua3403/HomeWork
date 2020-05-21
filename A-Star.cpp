#include "A-Star.h"


AStarPathFinding::AStarPathFinding()
{
	m_bInitializedStartGoal = false;
	m_bFoundGoal = false;
	InitialArray();
}

AStarPathFinding::~AStarPathFinding()
{
	DeleteObject(MyBrushYellow);
	DeleteObject(MyPenYellow);
}

void AStarPathFinding::FindPath(int current_x, int current_y, int end_x, int end_y)
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
		printf("%d, %d\n", m_vOpenList.size(), m_vCloseList.size());
	}
}

POSITION AStarPathFinding::NextPathPos()
{
	int index = 1;

	POSITION nextPos;
	nextPos.X = m_vPathToGoal[m_vPathToGoal.size() - index]->X;
	nextPos.Y = m_vPathToGoal[m_vPathToGoal.size() - index]->Y;

	return nextPos;
}

void AStarPathFinding::SetPointBlock(int x, int y)
{
	m_NodeArray[y][x].isBlock = !m_NodeArray[y][x].isBlock;
}

void AStarPathFinding::SetPointBlock(int x, int y, bool flag)
{
	m_NodeArray[y][x].isBlock = flag;

}

void AStarPathFinding::SetPointStart(int x, int y, bool flag)
{
	m_NodeArray[y][x].isStart = flag;
}

void AStarPathFinding::SetPointEnd(int x, int y, bool flag)
{
	m_NodeArray[y][x].isEnd = flag;
}

void AStarPathFinding::PrintBlock(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyBrushGray);
	OldBrush = (HBRUSH)SelectObject(hdc, MyPenGray);

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if (m_NodeArray[i][j].isBlock)
				Rectangle(hdc, m_NodeArray[i][j].x * 10 + 1, m_NodeArray[i][j].y * 10 + 1, m_NodeArray[i][j].x * 10 + 10, m_NodeArray[i][j].y * 10 + 10);

		}
	}

	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);
}

void AStarPathFinding::SetStartAndGoal(NODE start, NODE goal)
{
	m_pStartNode = new NODE(start.x, start.y);
	m_pGoalNode = new NODE(goal.x, goal.y);

	m_pStartNode->G = 0;
	m_pStartNode->H = m_pStartNode->ManHattanDistance(m_pGoalNode);
	m_pStartNode->Parent = nullptr;

	m_vOpenList.push_back(m_pStartNode);
}

void AStarPathFinding::PathOpened(int x, int y, float newG, NODE* parent)
{

	if (m_NodeArray[y][x].isBlock)
		return;

	int id = x * CHECK + y;
	for (int i = 0; i < m_vCloseList.size(); i++)
	{
		if (id == m_vCloseList[i]->ID)
		{
			return;
		}
	}

	if (x < 0 || x >= 100 || y < 0 || y >= 100)
		return;

	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(m_hdc, MyPenYellow);
	OldBrush = (HBRUSH)SelectObject(m_hdc, MyBrushYellow);

	NODE* newChild = new NODE(x, y, parent);
	newChild->G = newG;
	newChild->H = parent->ManHattanDistance(m_pGoalNode);

	for (int i = 0; i < m_vOpenList.size(); i++)
	{
		if (id == m_vOpenList[i]->ID)
		{
			float newF = newChild->G + newG + m_vOpenList[i]->H;

			if (m_vOpenList[i]->GetF() > newF)
			{
				m_vOpenList[i]->G = newChild->G + newG;
				m_vOpenList[i]->Parent = newChild;
			}
			else
			{
				delete newChild;
				return;
			}
		}
	}

	if (newChild != nullptr)
	{
		Rectangle(m_hdc, x * 10 + 1, y * 10 + 1, x * 10 + 10, y * 10 + 10);

	}
	SelectObject(m_hdc, MyPenYellow);
	SelectObject(m_hdc, MyBrushYellow);

	m_vOpenList.push_back(newChild);
}

NODE* AStarPathFinding::GetNextNode()
{
	float bestF = 999999.0f;
	int index = -1;
	NODE* nextNode = nullptr;

	for (int i = 0; i < m_vOpenList.size(); i++)
	{
		if (m_vOpenList[i]->GetF() < bestF)
		{
			bestF = m_vOpenList[i]->GetF();
			index = i;
		}
	}

	if (index >= 0)
	{
		nextNode = m_vOpenList[index];
		m_vCloseList.push_back(nextNode);
		m_vOpenList.erase(m_vOpenList.begin() + index);
	}

	return nextNode;
}

void AStarPathFinding::ContinuePath()
{
	//if (m_vOpenList.empty())
	//	return;

	while (!m_vOpenList.empty())
	{
		// OpenList에서 제익 작은 F값을 가진 노드를 가져온다.
		// 가져온 NODE는 CloseList로 넣어준다.
		NODE* currentNode = GetNextNode();

		// OpenList에서 가져온 NODE가 목표 NODE와 일치한다면
		if (currentNode->ID == m_pGoalNode->ID)
		{
			m_pGoalNode->Parent = currentNode->Parent;

			NODE* getPath;
			for (getPath = m_pGoalNode; getPath->Parent != nullptr; getPath = getPath->Parent)
			{
				m_vPathToGoal.push_back(new POSITION(getPath->x, getPath->y));
			}

			m_bFoundGoal = true;
			return;
		}
		else
		{
			// 오른쪽
			PathOpened(currentNode->x + 1, currentNode->y, currentNode->G + 1, currentNode);

			// 오른쪽 아래
			PathOpened(currentNode->x + 1, currentNode->y + 1, currentNode->G + 1.5f, currentNode);

			// 아래
			PathOpened(currentNode->x, currentNode->y + 1, currentNode->G + 1, currentNode);

			// 왼쪽 아래
			PathOpened(currentNode->x - 1, currentNode->y + 1, currentNode->G + 1.5f, currentNode);

			// 왼쪽
			PathOpened(currentNode->x - 1, currentNode->y, currentNode->G + 1, currentNode);

			// 왼쪽 위
			PathOpened(currentNode->x - 1, currentNode->y - 1, currentNode->G + 1.5f, currentNode);

			// 위
			PathOpened(currentNode->x, currentNode->y - 1, currentNode->G + 1, currentNode);

			// 오른쪽 위
			PathOpened(currentNode->x + 1, currentNode->y - 1, currentNode->G + 1.5f, currentNode);

			for (int i = 0; i < m_vOpenList.size(); i++)
			{
				if (currentNode->ID == m_vOpenList[i]->ID)
				{
					m_vOpenList.erase(m_vOpenList.begin() + i);
				}
			}

		}
	}


}

void AStarPathFinding::ClearVector()
{
	for (int i = 0; i < m_vOpenList.size(); i++)
	{
		delete m_vOpenList[i];
	}

	m_vOpenList.clear();

	for (int i = 0; i < m_vCloseList.size(); i++)
	{
		delete m_vCloseList[i];
	}

	m_vCloseList.clear();

	for (int i = 0; i < m_vPathToGoal.size(); i++)
	{
		delete m_vPathToGoal[i];
	}

	m_vPathToGoal.clear();
}

void AStarPathFinding::SetDC(HDC hdc)
{
	m_hdc = hdc;
}

std::vector<NODE*> AStarPathFinding::GetOpenList()
{
	return m_vOpenList;
}

std::vector<NODE*> AStarPathFinding::GetCloseList()
{
	return m_vCloseList;
}

std::vector<POSITION*> AStarPathFinding::GetRout()
{
	return m_vPathToGoal;
}

void AStarPathFinding::InitialArray()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			m_NodeArray[i][j].x = j;
			m_NodeArray[i][j].y = i;
			m_NodeArray[i][j].isBlock = m_NodeArray[i][j].isEnd = m_NodeArray[i][j].isStart = false;
		}
	}
}