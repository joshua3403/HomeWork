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

} POSITIONNODE;

typedef struct tag_ST_PointNode
{
	int X;
	int Y;
	tag_ST_PointNode* Parent;
	// G + H
	int F;
	// 출발점으로 부터의 이동 거리
	int G;
	// 현재 포인트에서부터 목적지까지의 거리 (맨하탄 방식)
	int H;
	tag_ST_PointNode(int x, int y)
	{
		X = x;
		Y = y;
		Parent = nullptr;
		F = 0;
		G = 0;
		H = 0;
		printf("create %d %d\n", X, Y);

	}
	~tag_ST_PointNode()
	{
		printf("delete %d %d\n", X, Y);
	}
} POINTNODE;

HWND g_hWnd;
int g_iWindowWidth = GetSystemMetrics(SM_CXSCREEN);
int g_iWindowHeight = GetSystemMetrics(SM_CYSCREEN);

int g_iCountX = 100;
int g_iCountY = 100;


POSITIONNODE* g_NodeStart = nullptr;
POSITIONNODE* g_NodeEnd = nullptr;
POSITIONNODE* g_NodeBlock = nullptr;
POSITIONNODE g_NodeArray[100][100];
POINTNODE* g_PointStart = nullptr;
POINTNODE* g_PointEnd = nullptr;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void InitialArray();
void PrintGrid(HDC hdc);
void PrintStartAndEnd(HDC hdc);

struct cmp
{
	bool operator()(POINTNODE t, POINTNODE u)
	{
		return t.F > u.F;
	}
};

std::priority_queue<POINTNODE, std::vector<POINTNODE>, cmp> g_OpenNode;

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
		PrintStartAndEnd(hdc);
		// A Star Algoritm start



		if (g_PointStart != nullptr && g_PointEnd != nullptr)
		{
			printf("A Star start\n");

			g_OpenNode.push(*g_PointStart);

			printf("%d ", g_OpenNode.top().F);


		}
		EndPaint(hWnd, &ps);
		break;

	case WM_RBUTTONDBLCLK:
		positionY = HIWORD(lParam) / 10;
		positionX = LOWORD(lParam) / 10;
		if (positionY < 100 && positionX < 100)
		{
			if (g_NodeEnd != nullptr)
			{
				if ((*g_NodeEnd).x == positionX && (*g_NodeEnd).y == positionY)
				{
					(*g_NodeEnd).isEnd = false;
					g_NodeEnd = nullptr;
					if (g_PointEnd != nullptr)
					{
						delete g_PointEnd;
						g_PointEnd = nullptr;
					}
					//printf("prev : %d %d %d\n", (*g_NodeStart).x, (*g_NodeStart).y, (*g_NodeStart).isStart);
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				}
				else
				{
					(*g_NodeEnd).isEnd = false;
					g_NodeEnd = nullptr;
					if (g_PointEnd != nullptr)
					{
						delete g_PointEnd;
						g_PointEnd = nullptr;
					}
					//printf("prev : %d %d %d\n", (*g_NodeStart).x, (*g_NodeStart).y, (*g_NodeStart).isStart);
				}
			}
			g_NodeEnd = &g_NodeArray[positionY][positionX];
			g_PointEnd = new POINTNODE(positionX, positionY);
			(*g_NodeEnd).isEnd = !(*g_NodeEnd).isEnd;
			//printf("next : %d %d %d\n", (*g_NodeStart).x, (*g_NodeStart).y, (*g_NodeStart).isStart);

		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDBLCLK:
		positionY = HIWORD(lParam) / 10;
		positionX = LOWORD(lParam) / 10;
		if (positionY < 100 && positionX < 100)
		{
			if (g_NodeStart != nullptr)
			{
				if ((*g_NodeStart).x == positionX && (*g_NodeStart).y == positionY)
				{
					(*g_NodeStart).isStart = false;
					(*g_NodeStart).isBlock = false;
					g_NodeStart = nullptr;
					if (g_PointStart != nullptr)
					{
						delete g_PointStart;
						g_PointStart = nullptr;
					}
					//printf("prev : %d %d %d\n", (*g_NodeStart).x, (*g_NodeStart).y, (*g_NodeStart).isStart);
					InvalidateRect(hWnd, NULL, TRUE);
					break;
				}
				else
				{
					(*g_NodeStart).isBlock = false;
					(*g_NodeStart).isStart = false;
					g_NodeStart = nullptr;
					if (g_PointStart != nullptr)
					{
						delete g_PointStart;
						g_PointStart = nullptr;
					}
					//printf("prev : %d %d %d\n", (*g_NodeStart).x, (*g_NodeStart).y, (*g_NodeStart).isStart);
				}

			}
			g_NodeStart = &g_NodeArray[positionY][positionX];
			g_PointStart = new POINTNODE(positionX, positionY);
			(*g_NodeStart).isStart = !(*g_NodeStart).isStart;
			(*g_NodeStart).isBlock = false;
			//printf("next : %d %d %d\n", (*g_NodeStart).x, (*g_NodeStart).y, (*g_NodeStart).isStart);
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
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
	int postX, postY, nextX, nextY;
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

void DeleteArray()
{
	for (int i = 0; i < 100; i++)
	{
		free(g_NodeArray[i]);
	}
	free(g_NodeArray);
}

void PrintStartAndEnd(HDC hdc)
{
	HBRUSH MyBrushGreen, MyBrushRed, MyBrushGray, OldBrush;
	HPEN MyPenGreen,MyPenRed, MyPenGray, OldPen;

	MyBrushGreen = CreateSolidBrush(RGB(0, 255, 0));
	MyPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));

	MyBrushRed = CreateSolidBrush(RGB(255, 0, 0));
	MyPenRed = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));

	MyBrushGray = CreateSolidBrush(RGB(125, 125, 125));
	MyPenGray = CreatePen(PS_SOLID, 1, RGB(125, 125, 125));

	OldPen = (HPEN)SelectObject(hdc, MyPenGreen);
	OldBrush = (HBRUSH)SelectObject(hdc, MyBrushGreen);
	if (g_NodeStart != nullptr)
	{
		if ((*g_NodeStart).isStart)
		{
			Rectangle(hdc, (*g_NodeStart).x*10+1, (*g_NodeStart).y*10+1, (*g_NodeStart).x*10 + 10, (*g_NodeStart).y*10 + 10);
		}
	}

	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);

	OldPen = (HPEN)SelectObject(hdc, MyPenRed);
	OldBrush = (HBRUSH)SelectObject(hdc, MyBrushRed);

	if (g_NodeEnd != nullptr)
	{
		if ((*g_NodeEnd).isEnd)
		{
			Rectangle(hdc, (*g_NodeEnd).x * 10 + 1, (*g_NodeEnd).y * 10 + 1, (*g_NodeEnd).x * 10 + 10, (*g_NodeEnd).y * 10 + 10);
		}
	}
	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);

	OldPen = (HPEN)SelectObject(hdc, MyBrushGray);
	OldBrush = (HBRUSH)SelectObject(hdc, MyPenGray);

	//if (g_NodeBlock != nullptr)
	//{
	//	if ((*g_NodeBlock).isBlock)
	//	{
	//		Rectangle(hdc, (*g_NodeBlock).x * 10 + 1, (*g_NodeBlock).y * 10 + 1, (*g_NodeBlock).x * 10 + 10, (*g_NodeBlock).y * 10 + 10);
	//	}
	//}

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

	DeleteObject(MyPenGreen);
	DeleteObject(MyBrushGreen);

	DeleteObject(MyBrushRed);
	DeleteObject(MyPenRed);

	DeleteObject(MyBrushGray);
	DeleteObject(MyPenGray);
}

void AStarAlgorithm(int x, int y)
{
	if ((*g_PointStart).X == (*g_PointEnd).X && (*g_PointStart).Y == (*g_PointEnd).Y)
		return;

	if (x < 0)
		return;
	if (x >= 100)
		return;
	if (y < 0)
		return;
	if (y >= 100)
		return;
}

//  