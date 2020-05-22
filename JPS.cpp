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

		// �˰��� ���� �� �ʱ�ȭ
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
			// �Ʒ�
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

			// ���� �Ʒ�
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

			// ����
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

			// ���� ��
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

			// ��
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

			// ������ ��
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

			// ������
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

			// ������ �Ʒ�
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
		// �Ʒ�
	case e_Direction::DOWN:
		// ������ �����ְ� ���� �Ʒ��� �շ��ְų�, �������� �����ְ� ������ �Ʒ��� �շ������� �ڳ� ���� ����
		if ((CheckTile(current_x - 1, current_y + 1) && m_NodeArray[current_y][current_x - 1].isBlock && m_NodeArray[current_y + 1][current_x - 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y][current_x + 1].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
			return true;

		break;

		// ���� �Ʒ�
	case e_Direction::DOWNLEFT:
		// ���� �����ְ� ���� ���� �շ��ְų�, �������� �����ְ� ������ �Ʒ��� �շ������� �ڳ� ���� ����
		if ((CheckTile(current_x - 1, current_y - 1) && m_NodeArray[current_y - 1][current_x].isBlock && m_NodeArray[current_y - 1][current_x - 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y][current_x + 1].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
			return true;

		break;

		// ����
	case e_Direction::LEFT:
		// ���� �����ְ� ���� ���� �շ��ְų�, �Ʒ��� �����ְ� ���� �Ʒ��� �շ������� �ڳ� ���� ����
		if ((CheckTile(current_x - 1, current_y - 1) && m_NodeArray[current_y - 1][current_x].isBlock && m_NodeArray[current_y - 1][current_x - 1].isBlock == false) || (CheckTile(current_x - 1, current_y + 1) && m_NodeArray[current_y + 1][current_x].isBlock && m_NodeArray[current_y + 1][current_x - 1].isBlock == false))
			return true;

		break;

		// ���� ��
	case e_Direction::LEFTUP:
		// �Ʒ��� �����ְ� ���� �Ʒ��� �շ��ְų�, �������� �����ְ� ������ ���� �շ������� �ڳ� ���� ����
		if ((CheckTile(current_x - 1, current_y + 1) && m_NodeArray[current_y + 1][current_x].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y][current_x + 1].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
			return true;
		break;

		// ��
	case e_Direction::UP:
		// ������ �����ְ� ���� ���� �շ��ְų�, �������̸����ְ� ������ ���� �շ������� �ڳ� ���� ����
		if ((CheckTile(current_x - 1, current_y - 1) && m_NodeArray[current_y][current_x - 1].isBlock && m_NodeArray[current_y - 1][current_x - 1].isBlock == false) || (CheckTile(current_x + 1, current_y - 1) && m_NodeArray[current_y][current_x + 1].isBlock && m_NodeArray[current_y - 1][current_x + 1].isBlock == false))
			return true;

		break;

		// ������ ��
	case e_Direction::UPRIGHT:
		// ������ �����ְ� ���� ���� �շ��ְų�, �Ʒ��� �����ְ� ������ �Ʒ��� �շ������� �ڳ� ���� ����
		if ((CheckTile(current_x - 1, current_y - 1) && m_NodeArray[current_y][current_x - 1].isBlock && m_NodeArray[current_y - 1][current_x - 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y + 1][current_x].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
			return true;
		break;

		// ������
	case e_Direction::RIGHT:
		// ���� �����ְ� ������ ���� �շ��ְų�, �Ʒ��� �����ְ� ������ �Ʒ��� �շ������� �ڳ� ���� ����
		if ((CheckTile(current_x + 1, current_y - 1) && m_NodeArray[current_y - 1][current_x].isBlock && m_NodeArray[current_y - 1][current_x + 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y + 1][current_x].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
			return true;
		break;

		// ������ �Ʒ�
	case e_Direction::RIGHTDOWN:
		// ���� �����ְ� ������ ���� �շ��ְų�, ������ �����ְ� ���� �Ʒ��� �շ������� �ڳ� ���� ����
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
				// ������ ���� �θ����� ���
			case e_Direction::NONE:
				// 8�������� ã�Ƴ�����.
				// �Ʒ�
				if (CheckTile(currentNode->x, currentNode->y + 1))
				{
					MakeNode(currentNode->x, currentNode->y + 1, currentNode, e_Direction::DOWN);
				}
				// ���� �Ʒ�
				if (CheckTile(currentNode->x - 1, currentNode->y + 1))
				{
					MakeNode(currentNode->x - 1, currentNode->y + 1, currentNode, e_Direction::DOWNLEFT);

				}
				// ����
				if (CheckTile(currentNode->x - 1, currentNode->y))
				{
					MakeNode(currentNode->x - 1, currentNode->y, currentNode, e_Direction::LEFT);
				}
				// ���� ��
				if (CheckTile(currentNode->x - 1, currentNode->y - 1))
				{
					MakeNode(currentNode->x - 1, currentNode->y - 1, currentNode, e_Direction::LEFTUP);
				}
				// ��
				if (CheckTile(currentNode->x, currentNode->y - 1))
				{
					MakeNode(currentNode->x, currentNode->y - 1, currentNode, e_Direction::UP);
				}
				// ������ ��
				if (CheckTile(currentNode->x + 1, currentNode->y - 1))
				{
					MakeNode(currentNode->x + 1, currentNode->y - 1, currentNode, e_Direction::UPRIGHT);
				}
				// ������
				if (CheckTile(currentNode->x + 1, currentNode->y))
				{
					MakeNode(currentNode->x + 1, currentNode->y, currentNode, e_Direction::RIGHT);
				}
				// ������ �Ʒ�
				if (CheckTile(currentNode->x + 1, currentNode->y + 1))
				{
					MakeNode(currentNode->x + 1, currentNode->y + 1, currentNode, e_Direction::RIGHTDOWN);
				}
				break;

			// �Ʒ�
			case e_Direction::DOWN:
				if (CheckTile(currentNode->x, currentNode->y + 1))
				{
					MakeNode(currentNode->x, currentNode->y + 1, currentNode, e_Direction::DOWN);
				}
				break;

			// ���� �Ʒ�
			case e_Direction::DOWNLEFT:
				break;

			// ����
			case e_Direction::LEFT:
				break;

			// ���� ��
			case e_Direction::LEFTUP:
				break;

			// ��
			case e_Direction::UP:
				break;

			// ������ ��
			case e_Direction::UPRIGHT:
				break;

			// ������
			case e_Direction::RIGHT:
				break;

			// ������ �Ʒ�
			case e_Direction::RIGHTDOWN:
				break;

			
			}
		}
	}
}

