#include "stdafx.h"

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

typedef struct tag_ST_PointNode
{
	int X;
	int Y;
	tag_ST_PointNode* Parent;
	// G + H
	double F;
	// 출발점으로 부터의 이동 거리
	double G;
	// 현재 포인트에서부터 목적지까지의 거리 (맨하탄 방식)
	double H;
	tag_ST_PointNode(int x, int y)
	{
		X = x;
		Y = y;
		Parent = nullptr;
		F = 0;
		G = 0;
		H = 0;
		//printf("create %d %d\n", X, Y);

	}
	~tag_ST_PointNode()
	{
		//printf("delete %d %d\n", X, Y);
	}
} POINTNODE;

HWND g_hWnd;
int g_iWindowWidth = GetSystemMetrics(SM_CXSCREEN);
int g_iWindowHeight = GetSystemMetrics(SM_CYSCREEN);

int g_iCountX = 100;
int g_iCountY = 100;

bool g_bIsStartDeleted = false;

bool SortingList(const POINTNODE* lhs, const POINTNODE* rhs)
{
	return lhs->F < rhs->F;
}

std::list<POINTNODE*> OpenNode;	
std::map<const std::pair<int, int>, POINTNODE*> CloseNode;

bool g_bEndClicked = false;
bool g_bStartClicked = false;
int g_iEndX = 0, g_iEndY = 0, g_iStartX = 0, g_iStartY = 0;
POSITIONNODE* g_NodeBlock = nullptr;
POSITIONNODE g_NodeArray[100][100];
POINTNODE* g_startNode = nullptr;
bool g_bAlgorithmStart = false;

HBRUSH MyBrushYellow = CreateSolidBrush(RGB(255, 255, 0));
HPEN MyPenYellow = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
HBRUSH MyBrushGreen = CreateSolidBrush(RGB(0, 255, 0));
HPEN MyPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
HBRUSH MyBrushRed = CreateSolidBrush(RGB(255, 0, 0));
HPEN MyPenRed = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
HBRUSH MyBrushGray = CreateSolidBrush(RGB(125, 125, 125));
HPEN MyPenGray = CreatePen(PS_SOLID, 1, RGB(125, 125, 125));
HBRUSH MyBrushBlue = CreateSolidBrush(RGB(0, 0, 255));
HPEN MyPenBlue = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void InitialArray();
void PrintGrid(HDC hdc);
void PrintStart(HDC hdc);
void PrintEnd(HDC hdc);
void PrintBlock(HDC hdc);
void PrintOpen(HDC hdc);
void CloseListAndMap();
bool IsAvailableToMove(int x, int y);
POINTNODE* IsInOpen(int x, int y);
POINTNODE* IsInClose(int x, int y);
POINTNODE* AStarAlgorithm(int start_x, int start_y, int end_x, int end_y, HDC hdc);
bool MakeChildNode(POINTNODE* node, HDC hdc);
void ExtendNode(POINTNODE* node, int current_x, int current_y, int dest_x, int dest_y, bool isDiagonal, HDC hdc);
void PrintRoute(HDC hdc, POINTNODE* best);

int main()
{
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = NULL;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = L"RedBlackTree";

	if (!RegisterClass(&wndclass))
		return 1;


	// 윈도우 생성
	g_hWnd = CreateWindow(L"RedBlackTree", L"RedBlackTree", WS_OVERLAPPEDWINDOW, 0, 0, g_iWindowWidth, g_iWindowHeight, NULL, NULL, NULL, NULL);

	if (g_hWnd == NULL)
		return 1;

	ShowWindow(g_hWnd, SW_SHOWNORMAL);
	UpdateWindow(g_hWnd);
	InitialArray();


	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HBRUSH oldBr;
	HDC hdc;
	int positionY = 0, positionX = 0;
	static int prevPositionY = 0, prevPositionX = 0;
	static BOOL bDrag = FALSE;
	POINTNODE* BestRoute = nullptr;
	switch (msg)
	{
	case WM_LBUTTONDOWN:
			bDrag = TRUE;
	case WM_MOUSEMOVE:
			if (bDrag == TRUE)
			{
				positionY = HIWORD(lParam) / 10;
				positionX = LOWORD(lParam) / 10;
				if (positionY < 100 && positionX < 100)
				{
					if (prevPositionX != positionX || prevPositionY != positionY)
					{
						//printf("%d %d %d %d\n", prevPositionX, prevPositionY, positionX, positionY);
						g_NodeBlock = &g_NodeArray[positionY][positionX];
						(*g_NodeBlock).isBlock = !(*g_NodeBlock).isBlock;
						//printf("prev : %d %d %d\n", (*g_NodeBlock).x, (*g_NodeBlock).y, (*g_NodeBlock).isBlock);
						InvalidateRect(hWnd, NULL, TRUE);
					}
				}
				prevPositionY = positionY;
				prevPositionX = positionX;
			}
			break;
	case WM_LBUTTONUP:
		bDrag = FALSE;
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		PrintGrid(hdc);
		PrintStart(hdc);
		PrintBlock(hdc);
		// A Star Algoritm start
		g_bAlgorithmStart = g_bEndClicked && g_bStartClicked;
		if(g_bAlgorithmStart)
		{
			g_startNode = new POINTNODE(g_iStartX, g_iStartY);
			g_startNode->H = abs(g_iStartX - g_iEndX) + abs(g_iStartY - g_iEndY);
			g_startNode->F = g_startNode->H + g_startNode->G;
			OpenNode.push_back(g_startNode);
			BestRoute = AStarAlgorithm(g_iStartX, g_iStartY, g_iEndX, g_iEndY, hdc);
			PrintRoute(hdc, BestRoute);
		}
		g_bAlgorithmStart = !g_bAlgorithmStart;
		PrintOpen(hdc);

		PrintEnd(hdc);
		EndPaint(hWnd, &ps);
		break;

	case WM_RBUTTONDBLCLK:
		positionY = HIWORD(lParam) / 10;
		positionX = LOWORD(lParam) / 10;
		if (positionY < 100 && positionX < 100)
		{
			// 이미 클릭된 곳이 존재한 상태라면
			if (g_bEndClicked)
			{
				if (g_iEndX == positionX && g_iEndY == positionY)
				{
					g_NodeArray[positionY][positionX].isEnd = false;
					g_bEndClicked = false;
				}
				else
				{
					g_NodeArray[g_iEndY][g_iEndX].isEnd = false;
					g_NodeArray[positionY][positionX].isEnd = true;
					g_bEndClicked = true;
					g_iEndY = positionY;
					g_iEndX = positionX;
				}
			}
			else
			{
				g_NodeArray[positionY][positionX].isEnd = true;
				g_bEndClicked = true;
				g_iEndY = positionY;
				g_iEndX = positionX;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDBLCLK:
		positionY = HIWORD(lParam) / 10;
		positionX = LOWORD(lParam) / 10;
		if (positionY < 100 && positionX < 100)
		{
			// 이미 클릭된 곳이 존재한 상태라면
			if (g_bStartClicked)
			{
				if (g_iStartX == positionX && g_iStartY == positionY)
				{
					g_NodeArray[positionY][positionX].isEnd = false;
					g_NodeArray[positionY][positionX].isBlock = false;
					g_bStartClicked = false;
				}
				else
				{
					g_NodeArray[g_iStartY][g_iStartX].isEnd = false;
					g_NodeArray[positionY][positionX].isEnd = true;
					g_NodeArray[positionY][positionX].isBlock = false;
					g_iStartX = positionX;
					g_iStartY = positionY;
					g_bStartClicked = true;
				}
			}
			else
			{
				g_NodeArray[positionY][positionX].isEnd = true;
				g_NodeArray[positionY][positionX].isBlock = false;
				g_bStartClicked = true;
				g_iStartX = positionX;
				g_iStartY = positionY;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		CloseListAndMap();

		DeleteObject(MyBrushYellow);
		DeleteObject(MyPenYellow);
		DeleteObject(MyPenGreen);
		DeleteObject(MyBrushGreen);
		DeleteObject(MyBrushGray);
		DeleteObject(MyPenGray);
		DeleteObject(MyBrushRed);
		DeleteObject(MyPenRed);

		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void InitialArray()
{
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			g_NodeArray[i][j].x = j;
			g_NodeArray[i][j].y = i;
			g_NodeArray[i][j].isBlock = g_NodeArray[i][j].isEnd = g_NodeArray[i][j].isStart = false;
		}
	}
}

void PrintGrid(HDC hdc)
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

void PrintStart(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyPenGreen);
	OldBrush = (HBRUSH)SelectObject(hdc, MyBrushGreen);
	
	if (g_bStartClicked)
	{
		Rectangle(hdc, g_iStartX * 10 + 1, g_iStartY * 10 + 1, g_iStartX * 10 + 10, g_iStartY * 10 + 10);
	}

	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);


}

void PrintEnd(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;


	OldPen = (HPEN)SelectObject(hdc, MyPenRed);
	OldBrush = (HBRUSH)SelectObject(hdc, MyBrushRed);

	if (g_bEndClicked)
	{
		Rectangle(hdc, g_iEndX * 10 + 1, g_iEndY * 10 + 1, g_iEndX * 10 + 10, g_iEndY * 10 + 10);
	}
	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);
}

void PrintBlock(HDC hdc)
{

	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyBrushGray);
	OldBrush = (HBRUSH)SelectObject(hdc, MyPenGray);

	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 100; j++)
		{
			if (g_NodeArray[i][j].isBlock)
				Rectangle(hdc, g_NodeArray[i][j].x * 10 + 1, g_NodeArray[i][j].y * 10 + 1, g_NodeArray[i][j].x * 10 + 10, g_NodeArray[i][j].y * 10 + 10);

		}
	}

	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);

}

void PrintOpen(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyBrushBlue);
	OldBrush = (HBRUSH)SelectObject(hdc, MyPenBlue);

	for (std::list<POINTNODE*>::iterator itor = OpenNode.begin(); itor != OpenNode.end(); itor++)
	{
		Rectangle(hdc, (*itor)->X * 10 + 1, (*itor)->Y * 10 + 1, (*itor)->X * 10 + 10, (*itor)->Y * 10 + 10);
	}
	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);
	CloseListAndMap();
}

void CloseListAndMap()
{
	std::list<POINTNODE*>::iterator itor = OpenNode.begin();
	while (true)
	{
		if (itor == OpenNode.end())
			break;
		else
		{
			delete* itor;
			itor = OpenNode.erase(itor);
		}
	}

	std::map<std::pair<int, int>, POINTNODE*>::iterator itor2 = CloseNode.begin();
	while (true)
	{
		if (itor2 == CloseNode.end())
			break;
		else
		{
			POINTNODE* temp = itor2->second;
			delete temp;
			itor2 = CloseNode.erase(CloseNode.find(itor2->first));
		}

	}
	
	g_bIsStartDeleted = true;
}

bool IsAvailableToMove(int x, int y)
{
	bool available = true;
	if (g_NodeArray[y][x].isBlock == true)
		available = false;

	if (x < 0 || x >= 100 || y < 0 || y >= 100)
		available = false;
	return available;
}

POINTNODE* IsInOpen(int x, int y)
{
	POINTNODE* temp = nullptr;
	std::list<POINTNODE*>::iterator itor = OpenNode.begin();
	for (; itor != OpenNode.end(); itor++)
	{
		if ((*itor)->X == x && (*itor)->Y == y)
			temp = (*itor);
	}
	return temp;
}

POINTNODE* IsInClose(int x, int y)
{
	POINTNODE* temp = nullptr;
	std::map<const std::pair<int, int>, POINTNODE*>::iterator itor;
	itor = CloseNode.find(std::make_pair(x, y));
	if (itor == CloseNode.end())
		temp = nullptr;
	else
		temp = itor->second;
	return temp;
}

POINTNODE* AStarAlgorithm(int start_x, int start_y, int end_x, int end_y ,HDC hdc)
{
	// 시작점
	POINTNODE* current = nullptr;
	
	//OpenNode.sort(SortingList);

	current = (*OpenNode.begin());
	//CloseNode.insert(std::make_pair(std::make_pair(current->X, current->Y), current));

	current->G = 0;
	current->H = abs(end_x - start_x) + abs(end_y - start_y);
	current->F = current->G + current->H;

	POINTNODE* best = nullptr;

	while (OpenNode.size() != 0)
	{
		OpenNode.sort(SortingList);

		best = (*OpenNode.begin());
		OpenNode.pop_front();
		CloseNode.insert(std::make_pair(std::make_pair(best->X, best->Y), best));

		if (best->X == end_x && best->Y == end_y)
		{
			return best;
		}

		if (best == nullptr)
			return nullptr;

		if (MakeChildNode(best, hdc) == false)
			return nullptr;

	}

	return best;
}

bool MakeChildNode(POINTNODE* node, HDC hdc)
{
	bool result = false;
	int x = node->X;
	int y = node->Y;

	// 왼쪽
	if (IsAvailableToMove(x - 1, y))
	{
		ExtendNode(node, x - 1, y, g_iEndX, g_iEndY, false, hdc);
		result = true;
	}

	// 왼쪽 위
	if (IsAvailableToMove(x - 1, y - 1))
	{
		ExtendNode(node, x - 1, y - 1, g_iEndX, g_iEndY, true, hdc);
		result = true;
	}

	// 위
	if (IsAvailableToMove(x , y - 1))
	{
		ExtendNode(node, x , y - 1, g_iEndX, g_iEndY, false, hdc);
		result = true;
	}

	// 오른쪽 위
	if (IsAvailableToMove(x + 1, y - 1))
	{
		ExtendNode(node, x + 1, y - 1, g_iEndX, g_iEndY, true, hdc);
		result = true;
	}
	
	// 오른쪽
	if (IsAvailableToMove(x + 1, y))
	{
		ExtendNode(node, x + 1, y, g_iEndX, g_iEndY, false, hdc);
		result = true;
	}

	// 오른쪽 아래
	if (IsAvailableToMove(x + 1, y + 1))
	{
		ExtendNode(node, x + 1, y + 1, g_iEndX, g_iEndY, true, hdc);
		result = true;
	}

	// 아래
	if (IsAvailableToMove(x, y + 1))
	{
		ExtendNode(node, x, y + 1, g_iEndX, g_iEndY, false, hdc);
		result = true;
	}

	// 왼쪽 아래
	if (IsAvailableToMove(x - 1, y + 1))
	{
		ExtendNode(node, x - 1, y + 1, g_iEndX, g_iEndY, true, hdc);
		result = true;

	}
	OpenNode.sort(SortingList);
	//printf("-------start-------\n");

	//for (std::list<POINTNODE*>::iterator itor = OpenNode.begin(); itor != OpenNode.end(); itor++)
	//{
	//	printf("node : x = %d, y = %d, F = %f\n", (*itor)->X, (*itor)->Y, (*itor)->F);

	//}
	//printf("-------end-------\n");

	return result;
}

void ExtendNode(POINTNODE* node, int current_x, int current_y, int dest_x, int dest_y, bool isDiagonal, HDC hdc)
{
	HBRUSH OldBrush;
	HPEN  OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyBrushYellow);
	OldBrush = (HBRUSH)SelectObject(hdc, MyPenYellow);
	POINTNODE* old = IsInOpen(current_x, current_y);
	POINTNODE* oldClose = IsInClose(current_x, current_y);
	POINTNODE* child = nullptr;

	// 새로 만들 자리의 노드에 이미 노드가 생성되어 있다면
	if (old != nullptr)
	{
		if (node->F < old->F)
		{
			old->Parent = node;

			if (isDiagonal)
				old->G = old->G + 1.5;
			else
				old->G = old->G + 1;

			old->F = old->G + old->H;

			//printf("oldNode(%d, %d) : F = %f, G = %f, H = %f, Parent = (%d, %d)\n", old->X, old->Y, old->F, old->G, old->H, node->X, node->Y);
			//if (IsInClose(node->X, node->Y) != nullptr)
			//{
			//	OpenNode.push_back(old);
			//	CloseNode.erase(CloseNode.find(std::make_pair(node->X, node->Y)));
			//}

			OpenNode.sort(SortingList);
		}
	}
	// 이미 지나갔었던 자리의 노드에 새로 생성하려는 경우
	else if (oldClose != nullptr)
	{
		//if (node->F < oldClose->F)
		//{
		//	oldClose->Parent = node;
		//	oldClose->G = node->G + 1;
		//	oldClose->F = oldClose->G + oldClose->H;
		//}
		return;
	}
	else
	{
		POINTNODE* newNode = new POINTNODE(current_x, current_y);

		newNode->H = abs(current_x - dest_x) + abs(current_y - dest_y);
		newNode->Parent = node;
		if (isDiagonal)
			newNode->G = newNode->Parent->G + 1.5;
		else
			newNode->G = newNode->Parent->G + 1;
		newNode->F = newNode->G + newNode->H;

		Rectangle(hdc, newNode->X * 10 + 1, newNode->Y * 10 + 1, newNode->X * 10 + 10, newNode->Y * 10 + 10);

		printf("newNode(%d, %d) : F = %f, G = %f, H = %f, Parent = (%d, %d)\n", newNode->X, newNode->Y,newNode->F, newNode->G, newNode->H,newNode->Parent->X, newNode->Parent->Y);

		OpenNode.push_back(newNode);
	}

	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);

}

void PrintRoute(HDC hdc, POINTNODE* best)
{
	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyPenRed);
	OldBrush = (HBRUSH)SelectObject(hdc, MyBrushRed);

	POINTNODE* temp = best;
	while (temp->Parent != nullptr)
	{
		MoveToEx(hdc, (temp->Parent->X * 10)+5, (temp->Parent->Y * 10) + 5 , NULL);
		LineTo(hdc, (temp->X * 10) + 5, (temp->Y * 10) + 5);

		//printf("Best : x = %d, y = %d\n", temp->X, temp->Y);
		temp = temp->Parent;
	}
	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);
}

//  