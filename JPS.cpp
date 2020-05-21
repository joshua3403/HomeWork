#include "JPS.h"

CJPS::CJPS()
{
	m_bInitializedStartGoal = false;
	m_bFoundGoal = false;
	InitialArray();
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

bool CJPS::CheckJumpNode(int current_x, int current_y, e_Direction dir)
{
	int check = false;
	switch (dir)
	{
		// 위
		case e_Direction::UP:
			// 왼쪽이 막혀있고, 왼쪽 위가 뚫렸는지 확인. 오른쪽이 막혀있고, 오른쪽 위가 뚫렸는지 확인.
			if ((CheckTile(current_x - 1, current_y - 1) && m_NodeArray[current_y][current_x - 1].isBlock && m_NodeArray[current_y - 1][current_x - 1].isBlock == false) || (CheckTile(current_x + 1, current_y - 1) && m_NodeArray[current_y][current_x + 1].isBlock && m_NodeArray[current_y - 1][current_x + 1].isBlock == false))
				check = true;
			break;

		// 오른쪽 위
		case e_Direction::UPRIGHT:
			// 왼쪽이 막혀있고, 왼쪽 위가 뚫려있는지 확인. 아래가 막혀있고, 오른쪽 아래가 뚫렸는지 확인.
			if ((CheckTile(current_x + 1, current_y - 1) && m_NodeArray[current_y][current_x - 1].isBlock && m_NodeArray[current_y - 1][current_x - 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y + 1][current_x].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
				check = true;
			break;

			// 오른쪽
		case e_Direction::RIGHT:
			// 위가 막혀있고, 오른쪽 위가 뚫렸는지 확인. 아래가 막혀있고, 오른쪽 아래가 뚫렸는지 확인.
			if ((CheckTile(current_x + 1, current_y - 1) && m_NodeArray[current_y - 1][current_x].isBlock && m_NodeArray[current_y - 1][current_x + 1].isBlock == false) || (CheckTile(current_x + 1, current_y + 1) && m_NodeArray[current_y + 1][current_x].isBlock && m_NodeArray[current_y + 1][current_x + 1].isBlock == false))
				check = true;
			break;
	}
}
