#include "JPS.h"

CJPS::CJPS()
{
	m_bInitializedStartGoal = false;
	m_bFoundGoal = false;
	m_bEndClicked = false;
	m_bStartClicked = false;
	m_iStartX = 0;
	m_iStartY = 0;
	m_iEndX = 0;
	m_iEndY = 0;
	m_iBrushAndPenIndex = 0;
	CreateBrushAndPen();
	srand((unsigned int)time(NULL));
	InitialArray();
}

CJPS::~CJPS()
{
	DeleteObject(MyBrushGray);
	DeleteObject(MyPenGray);
	DeleteObject(MyBrushGreen);
	DeleteObject(MyPenGreen);
	DeleteObject(MyBrushRed);
	DeleteObject(MyPenRed);
	DeleteObject(MyBrushYellow);
	DeleteObject(MyPenYellow);
	DeleteObject(MyBrushBrown);
	DeleteObject(MyPenBrown);
	DeleteObject(MyPenBlue);

}

void CJPS::FindPath(int start_x, int start_y, int end_x, int end_y)
{
	if (!m_bInitializedStartGoal)
	{
		ClearVector();

		// 알고리즘 시작 전 초기화
		NODE start;
		start.x = start_x;
		start.y = start_y;

		NODE goal;
		goal.x = end_x;
		goal.y = end_y;

		CreateBrush();


		SetStartAndGoal(start, goal);
		m_bInitializedStartGoal = true;
	}
	
	if(m_bInitializedStartGoal)
	{
		ContinueFindingPath();
		m_bInitializedStartGoal = false;
		DestroyBrushAndPen();
		m_iBrushAndPenIndex = 0;
	}

}

void CJPS::PrintGrid(HDC hdc)
{
	int postX, postY;
	postX = postY = 0;

	for (postX = 10; postX <= 1000; postX += 10)
	{
		MoveToEx(hdc, postX, 0, NULL);
		LineTo(hdc, postX, 1000);
	}

	for (postY = 10; postY <= 1000; postY += 10)
	{
		MoveToEx(hdc, 0, postY, NULL);
		LineTo(hdc, 1000, postY);
	}
}

void CJPS::PrintBlock(HDC hdc)
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

void CJPS::PrintStart(HDC hdc)
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

void CJPS::PrintEnd(HDC hdc)
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

void CJPS::PrintNode(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyPenYellow);
	OldBrush = (HBRUSH)SelectObject(hdc, MyBrushYellow);

	if (m_vOpenList.size() <= 0)
		return;

	for (int i = 0; i < m_vOpenList.size(); i++)
	{
		Rectangle(hdc, m_vOpenList[i]->x * 10 + 1, m_vOpenList[i]->y * 10 + 1, m_vOpenList[i]->x * 10 + 10, m_vOpenList[i]->y * 10 + 10);
	}
	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);
}


void CJPS::PrintRoute(HDC hdc)
{
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyPenRed);

	if (m_vPathToGoal.size() <= 0)
		return;
	for (int i = 0; i < m_vPathToGoal.size() - 1; i++)
	{
		MoveToEx(hdc, m_vPathToGoal[i + 1]->X * 10 + 5, m_vPathToGoal[i + 1]->Y * 10 + 5, NULL);
		LineTo(hdc, m_vPathToGoal[i]->X * 10 + 5, m_vPathToGoal[i]->Y * 10 + 5);
	}

	MoveToEx(hdc, m_iStartX * 10 + 5, m_iStartY * 10 + 5, NULL);
	LineTo(hdc, (*(m_vPathToGoal.end() - 1))->X * 10 + 5, (*(m_vPathToGoal.end() - 1))->Y * 10 + 5);

	SelectObject(hdc, OldPen);
}

void CJPS::SetHDC(HDC hdc)
{
	m_Hdc = hdc;
}

void CJPS::ResetPoint()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			m_NodeArray[i][j].isBlock = false;
		}
	}
}

void CJPS::PointBlockPoint(int x, int y)
{
	m_NodeArray[y][x].isBlock = !m_NodeArray[y][x].isBlock;
}

void CJPS::PointBlockPoint(int x, int y, bool flag)
{
	m_NodeArray[y][x].isBlock = flag;
}

void CJPS::SetPointEnd(int x, int y, bool flag)
{
	if (flag)
	{
		m_iEndX = x;
		m_iEndY = y;
	}

	m_NodeArray[y][x].isEnd = flag;
}

void CJPS::SetPointStart(int x, int y, bool flag)
{
	if (flag)
	{
		m_iStartX = x;
		m_iStartY = y;
	}

	m_NodeArray[y][x].isStart = flag;
}

void CJPS::SetEndIsClicked(bool flag)
{
	m_bEndClicked = flag;
}

void CJPS::SetStartIsClicked(bool flag)
{
	m_bStartClicked = flag;
}

bool CJPS::GetStartIsClicked()
{
	return m_bStartClicked;
}

bool CJPS::GetEndIsClicked()
{
	return m_bEndClicked;
}

void CJPS::SetStartAndGoal(NODE start, NODE goal)
{
	m_pStartNode = new NODE(start.x, start.y);
	m_pGoalNode = new NODE(goal.x, goal.y);

	m_pStartNode->G = 0;
	m_pStartNode->H = m_pStartNode->ManHattanDistance(m_pGoalNode);

	m_vOpenList.push_back(m_pStartNode);
}

void CJPS::InitialArray()
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

void CJPS::ClearVector()
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

void CJPS::PrintRouteBresenham(HDC hdc)
{
	int index = 0;
	int now = 0;
	HPEN OldPen;


	OldPen = (HPEN)SelectObject(hdc, MyPenBlue);
	if (m_vPathToGoal.size() <= 0)
		return;
	while (true)
	{
		if (index == m_vPathToGoal.size() - 1)
			break;

		bool temp = BresenHam(m_vPathToGoal[now]->X, m_vPathToGoal[now]->Y, m_vPathToGoal[index + 1]->X, m_vPathToGoal[index + 1]->Y);
		if (!temp)
		{
			MoveToEx(hdc, m_vPathToGoal[index]->X * 10 + 5, m_vPathToGoal[index]->Y * 10 + 5, NULL);
			LineTo(hdc, m_vPathToGoal[now]->X * 10 + 5, m_vPathToGoal[now]->Y * 10 + 5);
			now = index;
		}
		index++;
	}
	if (BresenHam(m_vPathToGoal[now]->X, m_vPathToGoal[now]->Y, m_pStartNode->x, m_pStartNode->y))
	{
		MoveToEx(hdc, m_pStartNode->x * 10 + 5, m_pStartNode->y * 10 + 5, NULL);
		LineTo(hdc, m_vPathToGoal[now]->X * 10 + 5, m_vPathToGoal[now]->Y * 10 + 5);
	}
	SelectObject(hdc, OldPen);

}

void CJPS::SaveMap()
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

void CJPS::LoadMap()
{
	FILE* file = nullptr;
	file = fopen("Map.txt", "r");
	char* temp = (char*)malloc(sizeof(char)*10100);

	fread(temp, 1, 10100, file);

	int i = 0;

	for (int y = 0; y < 100; y++)
	{
		for (int x = 0; x < 100; x++)
		{
			if(temp[i] == '0')
				m_NodeArray[y][x].isBlock = false;
			else
				m_NodeArray[y][x].isBlock = true;

			i++;
		}
		i++;
	}
	free(temp);
	fclose(file);
}

NODE* CJPS::GetNextNode()
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

inline bool CJPS::CheckTile(int x, int y)
{
	if (m_NodeArray[y][x].isBlock)
		return false;

	int id = x * CHECK + y;
	for (int i = 0; i < m_vCloseList.size(); i++)
	{
		if (id == m_vCloseList[i]->ID)
		{
			return false;
		}
	}

	if(EXCEEDNUM(x,y))
		return false;

	return true;
}

bool CJPS::MakeNode(int current_x, int current_y, NODE* nowNode, e_Direction dir)
{
	
	int newNodeX = 0;
	int newNodeY = 0;
	float newNodeG = 0;

	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(m_Hdc, MyPenYellow);
	OldBrush = (HBRUSH)SelectObject(m_Hdc, MyBrushYellow);

	if (!CheckTile(current_x, current_y))
		return false;

	if (JumpCheck(current_x, current_y, &newNodeX, &newNodeY, nowNode->G, &newNodeG, dir))
	{
		NODE* newNode = new NODE(newNodeX, newNodeY, nowNode);
		newNode->G = newNodeG;
		newNode->x = newNodeX;
		newNode->y = newNodeY;
		newNode->H = newNode->ManHattanDistance(m_pGoalNode);
		newNode->Parent = nowNode;
		newNode->GetF();
		int id = current_x * CHECK + current_y;
		for (int i = 0; i < m_vOpenList.size(); i++)
		{
			if (id == m_vOpenList[i]->ID)
			{
				float newF = newNode->G + m_vOpenList[i]->H;

				if (m_vOpenList[i]->GetF() > newF)
				{
					m_vOpenList[i]->G = newNode->G + newNodeG;
					m_vOpenList[i]->Parent = newNode;
					//ChangeParent(x, y, newChild);
				}
				else
				{
					delete newNode;
					return true;
				}
			}
		}

		for (int i = 0; i < m_vCloseList.size(); i++)
		{
			if (id == m_vCloseList[i]->ID)
			{
				delete newNode;
				return true;
			}
		}
		Rectangle(m_Hdc, newNode->x * 10 + 1, newNode->y * 10 + 1, newNode->x * 10 + 10, newNode->y * 10 + 10);

		m_vOpenList.push_back(newNode);

		SelectObject(m_Hdc, OldPen);
		SelectObject(m_Hdc, OldBrush);
		return true;

	}

	return false;
}

bool CJPS::JumpCheck(int current_x, int current_y, int* newX, int* newY, float currentG, float* newG, e_Direction dir)
{
	int tempG = 0;
	if(!CheckTile(current_x, current_y))
		return false;
	if (dir == e_Direction::DOWN || dir == e_Direction::UP || dir == e_Direction::RIGHT || dir == e_Direction::LEFT)
		tempG = currentG + 1;
	else
		tempG = currentG + 1.4f;


	
	HBRUSH OldBrush;
	HPEN OldPen;
	int t = rand() % 255;


	OldPen = (HPEN)SelectObject(m_Hdc, m_vPenHandles[m_iBrushAndPenIndex]);
	OldBrush = (HBRUSH)SelectObject(m_Hdc, m_vBrushHandles[m_iBrushAndPenIndex]);
	
	Rectangle(m_Hdc, current_x * 10 + 1, current_y * 10 + 1, current_x * 10 + 10, current_y * 10 + 10);
	
	SelectObject(m_Hdc, OldPen);
	SelectObject(m_Hdc, OldBrush);
	if (current_x == m_pGoalNode->x && current_y == m_pGoalNode->y)
	{
		*newX = current_x;
		*newY = current_y;
		*newG = currentG;
		return true;
	}
	else
	{

		switch (dir)
		{
			// 아래
		case e_Direction::DOWN:
			if (CornerCheck(current_x, current_y, dir))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}

			return JumpCheck(current_x, current_y + 1, newX, newY, currentG + 1, newG, dir);
			break;

			// 왼쪽 아래
		case e_Direction::DOWNLEFT:
			if (CornerCheck(current_x, current_y, dir))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}

			if (JumpCheck(current_x - 1, current_y, newX, newY, currentG + 1, newG, e_Direction::LEFT) || JumpCheck(current_x, current_y + 1, newX, newY, currentG + 1, newG, e_Direction::DOWN))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}

			return JumpCheck(current_x - 1, current_y + 1, newX, newY, currentG + 1.4f, newG, dir);
			break;

			// 왼쪽
		case e_Direction::LEFT:
			if (CornerCheck(current_x, current_y, dir))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}

			return JumpCheck(current_x - 1, current_y, newX, newY, currentG + 1, newG, dir);
			break;

			// 왼쪽 위
		case e_Direction::LEFTUP:
			if (CornerCheck(current_x, current_y, dir))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}


			if (JumpCheck(current_x - 1, current_y, newX, newY, currentG + 1, newG, e_Direction::LEFT) || JumpCheck(current_x, current_y - 1, newX, newY, currentG + 1, newG, e_Direction::UP))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}

			return  JumpCheck(current_x - 1, current_y - 1, newX, newY, currentG + 1.4f, newG, dir);
			break;

			// 위
		case e_Direction::UP:
			if (CornerCheck(current_x, current_y, dir))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}
			return JumpCheck(current_x, current_y - 1, newX, newY, currentG + 1, newG, dir);
			break;

			// 오른쪽 위
		case e_Direction::UPRIGHT:
			if (CornerCheck(current_x, current_y, dir))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}

			if (JumpCheck(current_x + 1, current_y, newX, newY, currentG + 1, newG, e_Direction::RIGHT) || JumpCheck(current_x, current_y - 1, newX, newY, currentG + 1, newG, e_Direction::UP))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}

			return  JumpCheck(current_x + 1, current_y - 1, newX, newY, currentG + 1.4f, newG, dir);
			break;

			// 오른쪽
		case e_Direction::RIGHT:
			if (CornerCheck(current_x, current_y, dir))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}
			return JumpCheck(current_x + 1, current_y, newX, newY, currentG + 1, newG, dir);

			break;

			// 오른쪽 아래
		case e_Direction::RIGHTDOWN:
			if (CornerCheck(current_x, current_y, dir))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}
			if (JumpCheck(current_x + 1, current_y, newX, newY, currentG + 1, newG, e_Direction::RIGHT) || JumpCheck(current_x, current_y + 1, newX, newY, currentG + 1, newG, e_Direction::DOWN))
			{
				*newX = current_x;
				*newY = current_y;
				*newG = currentG;
				return true;
			}
			return  JumpCheck(current_x + 1, current_y + 1, newX, newY, currentG + 1.4f, newG, dir);

			break;
		}

	}
}

bool CJPS::CornerCheck(int current_x, int current_y, e_Direction dir)
{
	switch (dir)
	{
		// 아래
	case e_Direction::DOWN:
		// 왼쪽이 막혀있고 왼쪽 아래가 뚫려있거나, 오른쪽이 막혀있고 오른쪽 아래가 뚫려있으면 코너 생성 가능
		if ((CheckTile(current_x - 1, current_y + 1) && m_NodeArray[current_y][current_x - 1].isBlock && m_NodeArray[current_y + 1][current_x - 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y][current_x + 1].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
			return true;

		break;

		// 왼쪽 아래
	case e_Direction::DOWNLEFT:
		// 위가 막혀있고 왼쪽 위가 뚫려있거나, 오른쪽이 막혀있고 오른쪽 아래가 뚫려있으면 코너 생성 가능
		if ((CheckTile(current_x - 1, current_y - 1) && m_NodeArray[current_y - 1][current_x].isBlock && m_NodeArray[current_y - 1][current_x - 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y][current_x + 1].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
			return true;

		break;

		// 왼쪽
	case e_Direction::LEFT:
		// 위가 막혀있고 왼쪽 위가 뚫려있거나, 아래가 막혀있고 왼쪽 아래가 뚫려있으면 코너 생성 가능
		if ((CheckTile(current_x - 1, current_y - 1) && m_NodeArray[current_y - 1][current_x].isBlock && m_NodeArray[current_y - 1][current_x - 1].isBlock == false) || (CheckTile(current_x - 1, current_y + 1) && m_NodeArray[current_y + 1][current_x].isBlock && m_NodeArray[current_y + 1][current_x - 1].isBlock == false))
			return true;

		break;

		// 왼쪽 위
	case e_Direction::LEFTUP:
		// 아래가 막혀있고 왼쪽 아래가 뚫려있거나, 오른쪽이 막혀있고 오른쪽 위가 뚫려있으면 코너 생성 가능
		if ((CheckTile(current_x - 1, current_y + 1) && m_NodeArray[current_y + 1][current_x].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y][current_x + 1].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
			return true;
		break;

		// 위
	case e_Direction::UP:
		// 왼쪽이 막혀있고 왼쪽 위가 뚫려있거나, 오른쪽이막혀있고 오른쪽 위가 뚫려있으면 코너 생성 가능
		if ((CheckTile(current_x - 1, current_y - 1) && m_NodeArray[current_y][current_x - 1].isBlock && m_NodeArray[current_y - 1][current_x - 1].isBlock == false) || (CheckTile(current_x + 1, current_y - 1) && m_NodeArray[current_y][current_x + 1].isBlock && m_NodeArray[current_y - 1][current_x + 1].isBlock == false))
			return true;

		break;

		// 오른쪽 위
	case e_Direction::UPRIGHT:
		// 왼쪽이 막혀있고 왼쪽 위가 뚫려있거나, 아래가 막혀있고 오른쪽 아래가 뚫려있으면 코너 생성 가능
		if ((CheckTile(current_x - 1, current_y - 1) && m_NodeArray[current_y][current_x - 1].isBlock && m_NodeArray[current_y - 1][current_x - 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y + 1][current_x].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
			return true;
		break;

		// 오른쪽
	case e_Direction::RIGHT:
		// 위가 막혀있고 오른쪽 위가 뚫려있거나, 아래가 막혀있고 오른쪽 아래가 뚫려있으면 코너 생성 가능
		if ((CheckTile(current_x + 1, current_y - 1) && m_NodeArray[current_y - 1][current_x].isBlock && m_NodeArray[current_y - 1][current_x + 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y + 1][current_x].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
			return true;
		break;

		// 오른쪽 아래
	case e_Direction::RIGHTDOWN:
		// 위가 막혀있고 오른쪽 위가 뚫려있거나, 왼쪽이 막혀있고 왼쪽 아래가 뚫려있으면 코너 생성 가능
		if ((CheckTile(current_x + 1, current_y - 1) && m_NodeArray[current_y - 1][current_x].isBlock && m_NodeArray[current_y - 1][current_x + 1].isBlock == false) || (CheckTile(current_x - 1, current_y + 1) && m_NodeArray[current_y][current_x - 1].isBlock && m_NodeArray[current_y + 1][current_x - 1].isBlock == false))
			return true;
		break;

	}
	return false;
}

bool CJPS::BresenHam(int start_x, int start_y, int end_x, int end_y)
{
	std::vector<POSITION> NodePositions;
	// x 증가량이 y증가량보다 크면
	if (abs(end_x - start_x) > abs(end_y - start_y))
	{
		// 오른쪽으로 움직인 경우
		if (end_x > start_x)
			LineLow(start_x, start_y, end_x, end_y, &NodePositions);
		// 왼쪽
		else
			LineLow(end_x, end_y, start_x, start_y, &NodePositions);
	}
	else
	{
		if (end_y > start_y)
			LineHigh(start_x, start_y, end_x, end_y, &NodePositions);
		else
			LineHigh(end_x, end_y, start_x, start_y, &NodePositions);

	}

	for (int i = 0; i < NodePositions.size(); i++)
	{
		if (m_NodeArray[NodePositions[i].Y][NodePositions[i].X].isBlock)
			return false;
	}

	return true;
}

void CJPS::LineLow(int start_x, int start_y, int end_x, int end_y, std::vector<POSITION>* temp)
{
	int dy = 0;
	int dx = 0;
	int yi = 0;
	int D = 0;
	int newY = start_y * 10 + 5;
	int newX = start_x * 10 + 5;
	POSITION newPos(start_x, start_y);
	//HBRUSH OldBrush;
	//HPEN OldPen;

	//OldPen = (HPEN)SelectObject(m_Hdc, MyPenBrown);
	//OldBrush = (HBRUSH)SelectObject(m_Hdc, MyBrushBrown);

	dx = (end_x) - (start_x);
	dy = (end_y) - (start_y);
	if (dy < 0)
		yi = -1;
	else
		yi = 1;

	if (dy < 0 && (yi == -1))
		dy = -dy;
	D = 2 * dy - dx;
	while (newPos.X < end_x)
	{
		newPos.Y = newY / 10;
		newPos.X = newX / 10;
		(*temp).push_back(newPos);
		//Rectangle(m_Hdc, newPos.X * 10 + 1, newPos.Y * 10 + 1, newPos.X * 10 + 10, newPos.Y * 10 + 10);

		if (D > 0)
		{
			newY += yi;
			D -= 2 * dx;
		}
		D += 2 * dy;
		newX++;
	}

	//SelectObject(m_Hdc, OldPen);
	//SelectObject(m_Hdc, OldBrush);
}

void CJPS::LineHigh(int start_x, int start_y, int end_x, int end_y, std::vector<POSITION>* temp)
{
	int dy = 0;
	int dx = 0;
	int xi = 0;
	int D = 0;
	int newY = start_y * 10 + 5;
	int newX = start_x * 10 + 5;
	POSITION newPos(start_x, start_y);
	//HBRUSH OldBrush;
	//HPEN OldPen;

	//OldPen = (HPEN)SelectObject(m_Hdc, MyPenBrown);
	//OldBrush = (HBRUSH)SelectObject(m_Hdc, MyBrushBrown);

	dx = (end_x * 10 + 5) - (start_x * 10 + 5);
	dy = (end_y * 10 + 5) - (start_y * 10 + 5);
	if (dx < 0)
		xi = -1;
	else
		xi = +1;

	if (dx < 0 && (xi == -1))
		dx = -dx;
	D = 2 * dy - dx;
	while (newPos.Y < end_y)
	{
		newPos.Y = newY / 10;
		newPos.X = newX / 10;
		(*temp).push_back(newPos);
		//Rectangle(m_Hdc, newPos.X * 10 + 1, newPos.Y * 10 + 1, newPos.X * 10 + 10, newPos.Y * 10 + 10);

		if (D > 0)
		{
			newX += xi;
			D -= 2 * dy;
		}
		D += 2 * dx;
		newY++;
	}

	//SelectObject(m_Hdc, OldPen);
	//SelectObject(m_Hdc, OldBrush);
}

void CJPS::ContinueFindingPath()
{
	int iDX = 0;
	int iDY = 0;
	int iCurrentX = 0;
	int iCurrentY = 0;
	while (!m_vOpenList.empty())
	{
		NODE* currentNode = GetNextNode();
		iCurrentX = currentNode->x;
		iCurrentY = currentNode->y;
		m_iBrushAndPenIndex++;
		if (m_iBrushAndPenIndex >= 15)
			m_iBrushAndPenIndex = 0;
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
			// 부모가 없는 최상위 노드일 경우
			if (currentNode->Parent == nullptr)
			{
				// 8방향으로 찾아나간다.
				// 아래
				if (CheckTile(currentNode->x, currentNode->y + 1))
				{
					MakeNode(currentNode->x, currentNode->y + 1, currentNode, e_Direction::DOWN);
				}
				// 왼쪽 아래
				if (CheckTile(currentNode->x - 1, currentNode->y + 1))
				{
					MakeNode(currentNode->x - 1, currentNode->y + 1, currentNode, e_Direction::DOWNLEFT);

				}
				// 왼쪽
				if (CheckTile(currentNode->x - 1, currentNode->y))
				{
					MakeNode(currentNode->x - 1, currentNode->y, currentNode, e_Direction::LEFT);
				}
				// 왼쪽 위
				if (CheckTile(currentNode->x - 1, currentNode->y - 1))
				{
					MakeNode(currentNode->x - 1, currentNode->y - 1, currentNode, e_Direction::LEFTUP);
				}
				// 위
				if (CheckTile(currentNode->x, currentNode->y - 1))
				{
					MakeNode(currentNode->x, currentNode->y - 1, currentNode, e_Direction::UP);
				}
				// 오른쪽 위
				if (CheckTile(currentNode->x + 1, currentNode->y - 1))
				{
					MakeNode(currentNode->x + 1, currentNode->y - 1, currentNode, e_Direction::UPRIGHT);
				}
				// 오른쪽
				if (CheckTile(currentNode->x + 1, currentNode->y))
				{
					MakeNode(currentNode->x + 1, currentNode->y, currentNode, e_Direction::RIGHT);
				}
				// 오른쪽 아래
				if (CheckTile(currentNode->x + 1, currentNode->y + 1))
				{
					MakeNode(currentNode->x + 1, currentNode->y + 1, currentNode, e_Direction::RIGHTDOWN);
				}
			}
			else
			{
				// 부모의 좌표와 현재 노드의 좌표로 방향을 구함
				iDX = currentNode->x - currentNode->Parent->x;
				iDY = currentNode->Parent->y - currentNode->y;

				if (iDX != 0)
					iDX = abs(iDX) / iDX;
				if (iDY != 0)
					iDY = abs(iDY) / iDY;

				// 위
				if (iDX == 0 && iDY == 1)
				{
					MakeNode(iCurrentX, iCurrentY - 1, currentNode, e_Direction::UP);

					// 왼쪽 막힌지 확인하고
					if (!CheckTile(iCurrentX - 1, iCurrentY))
						// 왼쪽위 생성
						MakeNode(iCurrentX - 1, iCurrentY - 1, currentNode, e_Direction::LEFTUP);

					// 오른쪽 막힌지 확인하고
					if (!CheckTile(iCurrentX + 1, iCurrentY))
						// 오른쪽 위 생성
						MakeNode(iCurrentX + 1, iCurrentY - 1, currentNode, e_Direction::UPRIGHT);
				}
				// 오른쪽 위
				else if (iDX == 1 && iDY == 1)
				{
					// 기본
					// 위
					MakeNode(iCurrentX, iCurrentY - 1, currentNode, e_Direction::UP);
					//	오른쪽 위
					MakeNode(iCurrentX + 1, iCurrentY - 1, currentNode, e_Direction::UPRIGHT);
					// 오른쪽
					MakeNode(iCurrentX + 1, iCurrentY, currentNode, e_Direction::RIGHT);

					// 추가 확인
					if (!CheckTile(iCurrentX - 1, iCurrentY))
						MakeNode(iCurrentX - 1, iCurrentY - 1, currentNode, e_Direction::LEFTUP);
					
					if(!CheckTile(iCurrentX, iCurrentY + 1))
						MakeNode(iCurrentX + 1, iCurrentY + 1, currentNode, e_Direction::RIGHTDOWN);

				}
				// 오른쪽
				else if (iDX == 1 && iDY == 0)
				{
					MakeNode(iCurrentX + 1, iCurrentY, currentNode, e_Direction::RIGHT);

					// 위쪽 막힌지 확인하고
					if (!CheckTile(iCurrentX, iCurrentY - 1))
						// 오른쪽 위 생성
						MakeNode(iCurrentX + 1, iCurrentY - 1, currentNode, e_Direction::UPRIGHT);

					// 아래쪽 막힌지 확인하고
					if (!CheckTile(iCurrentX, iCurrentY + 1))
						// 오른쪽 아래 생성
						MakeNode(iCurrentX + 1, iCurrentY + 1, currentNode, e_Direction::RIGHTDOWN);
				}
				// 오른쪽 아래
				else if (iDX == 1 && iDY == -1)
				{
					// 오른쪽
					MakeNode(iCurrentX + 1, iCurrentY, currentNode, e_Direction::RIGHT);
					// 오른쪽 아래
					MakeNode(iCurrentX + 1, iCurrentY + 1, currentNode, e_Direction::RIGHTDOWN);
					// 아래
					MakeNode(iCurrentX, iCurrentY + 1, currentNode, e_Direction::DOWN);

					// 위가 막혀있다면
					if (!CheckTile(iCurrentX, iCurrentY - 1))
						// 오른쪽 위 생성
						MakeNode(iCurrentX + 1, iCurrentY - 1, currentNode, e_Direction::UPRIGHT);


					// 왼쪽이 막혀있다면
					if (!CheckTile(iCurrentX - 1, iCurrentY))
						// 왼쪽 아래 생성
						MakeNode(iCurrentX - 1, iCurrentY + 1, currentNode, e_Direction::DOWNLEFT);

				}
				// 아래
				else if (iDX == 0 && iDY == -1)
				{
					MakeNode(iCurrentX, iCurrentY + 1, currentNode, e_Direction::DOWN);

					// 왼쪽이 막혀있다면
					if(!CheckTile(iCurrentX - 1, iCurrentY))
						MakeNode(iCurrentX - 1, iCurrentY + 1, currentNode, e_Direction::DOWNLEFT);

					// 오른쪽이 막혀있다면
					if(!CheckTile(iCurrentX + 1, iCurrentY))
						MakeNode(iCurrentX + 1, iCurrentY + 1, currentNode, e_Direction::RIGHTDOWN);
				}
				// 왼쪽 아래
				else if (iDX == -1 && iDY == -1)
				{
					// 왼쪽
					MakeNode(iCurrentX - 1, iCurrentY, currentNode, e_Direction::LEFT);
					// 왼쪽 아래
					MakeNode(iCurrentX - 1, iCurrentY + 1, currentNode, e_Direction::DOWNLEFT);
					// 아래
					MakeNode(iCurrentX, iCurrentY + 1, currentNode, e_Direction::DOWN);

					// 위쪽이 막혀있다면
					if(!CheckTile(iCurrentX, iCurrentY - 1))
						MakeNode(iCurrentX -1, iCurrentY - 1, currentNode, e_Direction::LEFTUP);

					// 오른쪽이 막혀있다면
					if(!CheckTile(iCurrentX + 1, iCurrentY))
						MakeNode(iCurrentX + 1, iCurrentY + 1, currentNode, e_Direction::RIGHTDOWN);
				}
				// 왼쪽
				else if (iDX == -1 && iDY == 0)
				{
					MakeNode(iCurrentX - 1, iCurrentY, currentNode, e_Direction::LEFT);
					
					// 위쪽이 막혀있다면
					if(!CheckTile(iCurrentX, iCurrentY - 1))
						MakeNode(iCurrentX - 1, iCurrentY - 1, currentNode, e_Direction::LEFTUP);

					// 아래쪽이 막혀있다면
					if(!CheckTile(iCurrentX, iCurrentY + 1))
						MakeNode(iCurrentX - 1, iCurrentY + 1, currentNode, e_Direction::DOWNLEFT);

				}
				// 왼쪽 위
				else if(iDX == -1 && iDY == 1)
				{
					// 왼쪽
					MakeNode(iCurrentX - 1, iCurrentY, currentNode, e_Direction::LEFT);
					// 왼쪽 위
					MakeNode(iCurrentX - 1, iCurrentY - 1, currentNode, e_Direction::LEFTUP);
					// 위
					MakeNode(iCurrentX, iCurrentY - 1, currentNode, e_Direction::UP);
					
					// 오른쪽이 막혀있다면
					if(!CheckTile(iCurrentX + 1, iCurrentY))
						MakeNode(iCurrentX + 1, iCurrentY - 1, currentNode, e_Direction::UPRIGHT);

					// 아래가 막혀있다면
					if(!CheckTile(iCurrentX, iCurrentY + 1))
						MakeNode(iCurrentX - 1, iCurrentY + 1, currentNode, e_Direction::DOWNLEFT);

				}


			}

			
			ProcessWindowMessage();
		}
	}
}

void CJPS::ProcessWindowMessage()
{
	MSG	msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
}

void CJPS::CreateBrushAndPen()
{
	for (int i = 0; i < 15; i++)
	{
		int r = rand() % 125 + 1;
		int g = rand() % 125 + 1;
		int b = rand() % 125 + 1;
		m_vBrushHandles.push_back(CreateSolidBrush(RGB(r, g, b)));
		m_vPenHandles.push_back(CreatePen(PS_SOLID, 1, RGB(r, g, b)));
	}
}


void CJPS::DestroyBrushAndPen()
{
	for (int i = 0; i < m_vBrushHandles.size(); i++)
	{
		DeleteObject(m_vBrushHandles[i]);
		DeleteObject(m_vPenHandles[i]);
	}
}

void CJPS::CreateBrush()
{
	for (int i = 0; i < 15; i++)
	{
		int r = rand() % 125 + 1;
		int g = rand() % 125 + 1;
		int b = rand() % 125 + 1;
		m_vBrushHandles[i] = CreateSolidBrush(RGB(r, g, b));
		m_vPenHandles[i] = CreatePen(PS_SOLID, 1, RGB(r, g, b));
	}
}

