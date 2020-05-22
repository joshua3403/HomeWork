#include "JPS.h"

CJPS::CJPS()
{
	m_bInitializedStartGoal = false;
	m_bFoundGoal = false;
	InitialArray();
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

		SetStartAndGoal(start, goal);
		m_bInitializedStartGoal = true;
	}
	else
	{
		ContinueFindingPath();
		m_bInitializedStartGoal = false;
	}

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

bool CJPS::CheckTile(int x, int y)
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

	if (x < 0 || x >= 100 || y < 0 || y >= 100)
		return false;

	return true;
}

void CJPS::MakeNode(int current_x, int current_y, NODE* nowNode, e_Direction dir)
{
	
	int newNodeX = 0;
	int newNodeY = 0;
	float newNodeG = 0;

	if (!CheckTile(current_x, current_y))
		return;

	if (JumpCheck(current_x, current_y, &newNodeX, &newNodeY, nowNode->G, &newNodeG, dir))
	{
		NODE* newNode = new NODE(newNodeX, newNodeY, nowNode);
		newNode->H = newNode->ManHattanDistance(m_pGoalNode);
		newNode->G = newNodeG;
	}
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

			if (JumpCheck(current_x - 1, current_y, newX, newY, currentG + 1, newG, dir) || JumpCheck(current_x, current_y + 1, newX, newY, currentG + 1, newG, dir))
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


			if (JumpCheck(current_x - 1, current_y, newX, newY, currentG + 1, newG, dir) || JumpCheck(current_x, current_y - 1, newX, newY, currentG + 1, newG, dir))
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

			if (JumpCheck(current_x + 1, current_y, newX, newY, currentG + 1, newG, dir) || JumpCheck(current_x, current_y - 1, newX, newY, currentG + 1, newG, dir))
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
			if (JumpCheck(current_x + 1, current_y, newX, newY, currentG + 1, newG, dir) || JumpCheck(current_x, current_y + 1, newX, newY, currentG + 1, newG, dir))
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

void CJPS::ContinueFindingPath()
{
	while (!m_vOpenList.empty())
	{
		NODE* currentNode = GetNextNode();

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

			switch (currentNode->Dir)
			{
				// 방향이 없는 부모노드의 경우
			case e_Direction::NONE:
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
				break;

			// 아래
			case e_Direction::DOWN:
				if (CheckTile(currentNode->x, currentNode->y + 1))
				{
					MakeNode(currentNode->x, currentNode->y + 1, currentNode, e_Direction::DOWN);
				}
				break;

			// 왼쪽 아래
			case e_Direction::DOWNLEFT:
				break;

			// 왼쪽
			case e_Direction::LEFT:
				break;

			// 왼쪽 위
			case e_Direction::LEFTUP:
				break;

			// 위
			case e_Direction::UP:
				break;

			// 오른쪽 위
			case e_Direction::UPRIGHT:
				break;

			// 오른쪽
			case e_Direction::RIGHT:
				break;

			// 오른쪽 아래
			case e_Direction::RIGHTDOWN:
				break;

			
			}
		}
	}
}

