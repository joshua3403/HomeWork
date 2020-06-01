 #include "A-Star.h"


AStarPathFinding::AStarPathFinding()
{
	m_bInitializedStartGoal = false;
	m_bFoundGoal = false;
	m_bEndClicked = false;
	m_bStartClicked = false;
	m_iStartX = 0;
	m_iStartY = 0;
	m_iEndX = 0;
	m_iEndY = 0;
	InitialArray();
	//printf("%d", sizeof(NODE));
}

AStarPathFinding::~AStarPathFinding()
{
	DeleteObject(MyBrushYellow);
	DeleteObject(MyPenYellow);
	DeleteObject(MyBrushGray);
	DeleteObject(MyPenGray);
	DeleteObject(MyBrushGreen);
	DeleteObject(MyPenGreen);
	DeleteObject(MyBrushRed);
	DeleteObject(MyPenRed);
	DeleteObject(MyBrushBlue);
	DeleteObject(MyPenBlue);
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
	if (flag)
	{
		m_iStartX = x;
		m_iStartY = y;
	}
	m_NodeArray[y][x].isStart = flag;
}

void AStarPathFinding::SetPointEnd(int x, int y, bool flag)
{
	if (flag)
	{
		m_iEndX = x;
		m_iEndY = y;
	}
	m_NodeArray[y][x].isEnd = flag;
}

void AStarPathFinding::SetEndIsClicked(bool flag)
{
	m_bEndClicked = flag;
}

void AStarPathFinding::SetStartIsClicked(bool flag)
{
	m_bStartClicked = flag;
}

bool AStarPathFinding::GetStartIsClicked()
{
	return m_bStartClicked;
}

bool AStarPathFinding::GetEndIsClicked()
{
	return m_bEndClicked;
}

void AStarPathFinding::ResetPoint()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			m_NodeArray[i][j].isBlock = false;
		}
	}
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

void AStarPathFinding::PrintStart(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyPenGreen);
	OldBrush = (HBRUSH)SelectObject(hdc, MyBrushGreen);

	if (m_bStartClicked)
	{
		Rectangle(hdc, m_iStartX * 10 + 1, m_iStartY * 10 + 1, m_iStartX * 10 + 10, m_iStartY * 10 + 10);
	}

	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);
}


void AStarPathFinding::PrintOpen(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;
	

	OldPen = (HPEN)SelectObject(hdc, MyBrushBlue);
	OldBrush = (HBRUSH)SelectObject(hdc, MyPenBlue);
	for (std::vector<NODE*>::iterator itor = m_vOpenList.begin(); itor != m_vOpenList.end(); itor++)
	{
		Rectangle(hdc, (*itor)->x * 10 + 1, (*itor)->y * 10 + 1, (*itor)->x * 10 + 10, (*itor)->y * 10 + 10);
	}
	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);
}

void AStarPathFinding::PrintRoute(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyPenRed);
	OldBrush = (HBRUSH)SelectObject(hdc, MyBrushRed);

	if (m_vPathToGoal.size() <= 0)
		return;
	for (int i = 0; i < m_vPathToGoal.size() - 1; i++)
	{
		MoveToEx(hdc, m_vPathToGoal[i + 1]->X * 10 + 5, m_vPathToGoal[i + 1]->Y * 10 + 5, NULL);
		LineTo(hdc, m_vPathToGoal[i]->X * 10 + 5, m_vPathToGoal[i]->Y * 10 + 5);
	}

	MoveToEx(hdc, m_pStartNode->x * 10 + 5, m_pStartNode->y * 10 + 5, NULL);
	LineTo(hdc, (*(m_vPathToGoal.end() - 1))->X * 10 + 5, (*(m_vPathToGoal.end() - 1))->Y * 10 + 5);

	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);
}

void AStarPathFinding::PrintEnd(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;


	OldPen = (HPEN)SelectObject(hdc, MyPenRed);
	OldBrush = (HBRUSH)SelectObject(hdc, MyBrushRed);

	if (m_bEndClicked)
	{
		Rectangle(hdc, m_iEndX * 10 + 1, m_iEndY * 10 + 1, m_iEndX * 10 + 10, m_iEndY * 10 + 10);
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
	newChild->H = newChild->ManHattanDistance(m_pGoalNode);

	//std::list<NODE*>::iterator itor = std::find(m_vOpenList.begin(), m_vOpenList, )

	for (int i = 0; i < m_vOpenList.size(); i++)
	{
		if (id == m_vOpenList[i]->ID)
		{
			float newF = newChild->G + m_vOpenList[i]->H;

			if (m_vOpenList[i]->GetF() > newF)
			{
				m_vOpenList[i]->G = newChild->G + newG;
				m_vOpenList[i]->Parent = newChild;
				//ChangeParent(x, y, newChild);
			}
			else
			{
				delete newChild;
				return;
			}
		}
	}

	for (int i = 0; i < m_vCloseList.size(); i++)
	{
		if (id == m_vCloseList[i]->ID)
		{
			delete newChild;
			return;
		}
	}


	Rectangle(m_hdc, x * 10 + 1, y * 10 + 1, x * 10 + 10, y * 10 + 10);

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
			PathOpened(currentNode->x + 1, currentNode->y + 1, currentNode->G + 1.4f, currentNode);

			// 아래
			PathOpened(currentNode->x, currentNode->y + 1, currentNode->G + 1, currentNode);

			// 왼쪽 아래
			PathOpened(currentNode->x - 1, currentNode->y + 1, currentNode->G + 1.4f, currentNode);

			// 왼쪽
			PathOpened(currentNode->x - 1, currentNode->y, currentNode->G + 1, currentNode);

			// 왼쪽 위
			PathOpened(currentNode->x - 1, currentNode->y - 1, currentNode->G + 1.4f, currentNode);

			// 위
			PathOpened(currentNode->x, currentNode->y - 1, currentNode->G + 1, currentNode);

			// 오른쪽 위
			PathOpened(currentNode->x + 1, currentNode->y - 1, currentNode->G + 1.4f, currentNode);

			for (int i = 0; i < m_vOpenList.size(); i++)
			{
				if (currentNode->ID == m_vOpenList[i]->ID)
				{
					m_vOpenList.erase(m_vOpenList.begin() + i);
				}
			}

		}

		ProcessWindowMessage();
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

void	AStarPathFinding::ProcessWindowMessage()
{
	MSG	msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
}


void AStarPathFinding::SaveMap()
{

	FILE* file = nullptr;
	file = fopen("Map.txt", "w");
	for (int y = 0; y < 100; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			fprintf(file, "%d", m_NodeArray[y][x].isBlock);
		}
		fprintf(file, "\n");
	}
	fclose(file);

}

void AStarPathFinding::LoadMap(const char* temp)
{
	FILE* file = nullptr;
	file = fopen(temp, "r");
	char* buffer = (char*)malloc(sizeof(char) * 10100);

	fread(buffer, 1, 10100, file);

	int i = 0;

	for (int y = 0; y < 100; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			if (buffer[i] == '0')
				m_NodeArray[y][x].isBlock = false;
			else
				m_NodeArray[y][x].isBlock = true;

			i++;
		}
		i++;
	}
	free(buffer);
	fclose(file);
}

void AStarPathFinding::RandomMap()
{
	for (int i = 0; i < 3000; i++)
	{
		int x = rand() % 100;
		int y = rand() % 100;
		m_NodeArray[y][x].isBlock = true;
	}
}
