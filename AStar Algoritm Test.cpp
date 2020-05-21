#include "stdafx.h"
#include "A-Star.h"


HWND g_hWnd;
int g_iWindowWidth = GetSystemMetrics(SM_CXSCREEN);
int g_iWindowHeight = GetSystemMetrics(SM_CYSCREEN);

int g_iCountX = 100;
int g_iCountY = 100;

bool g_bIsStartDeleted = false;



bool g_bEndClicked = false;
bool g_bStartClicked = false;
int g_iEndX = 0, g_iEndY = 0, g_iStartX = 0, g_iStartY = 0;
POSITIONNODE* g_NodeBlock = nullptr;
bool g_bAlgorithmStart = false;


HBRUSH MyBrushGreen = CreateSolidBrush(RGB(0, 255, 0));
HPEN MyPenGreen = CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
HBRUSH MyBrushRed = CreateSolidBrush(RGB(255, 0, 0));
HPEN MyPenRed = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

HBRUSH MyBrushBlue = CreateSolidBrush(RGB(0, 0, 255));
HPEN MyPenBlue = CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void PrintGrid(HDC hdc);
void PrintStart(HDC hdc);
void PrintEnd(HDC hdc);
void PrintOpen(HDC hdc);

void PrintRoute(HDC hdc);
AStarPathFinding AStarPathFind;

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
	HDC hdc;
	int positionY = 0, positionX = 0;
	static int prevPositionY = 0, prevPositionX = 0;
	static BOOL bDrag = FALSE;
	NODE* BestRoute = nullptr;
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
						AStarPathFind.SetPointBlock(positionX, positionY);
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
		AStarPathFind.PrintBlock(hdc);
		// A Star Algoritm start
		g_bAlgorithmStart = g_bEndClicked && g_bStartClicked;
		if(g_bAlgorithmStart)
		{
			AStarPathFind.SetDC(hdc);
			AStarPathFind.FindPath(g_iStartX, g_iStartY, g_iEndX, g_iEndY);
			PrintOpen(hdc);

		}
		else
		{
			AStarPathFind.ClearVector();
		}
		g_bAlgorithmStart = !g_bAlgorithmStart;
		PrintRoute(hdc);

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
					AStarPathFind.SetPointEnd(positionX, positionY, false);
					g_bEndClicked = false;
				}
				else
				{
					AStarPathFind.SetPointEnd(g_iEndX, g_iEndY, false);
					AStarPathFind.SetPointEnd(positionX, positionY, true);
					g_bEndClicked = true;
					g_iEndY = positionY;
					g_iEndX = positionX;
				}
			}
			else
			{
				AStarPathFind.SetPointEnd(positionX, positionY, true);
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
					AStarPathFind.SetPointStart(positionX, positionY, false);
					AStarPathFind.SetPointBlock(positionX, positionY, false);
					g_bStartClicked = false;
				}
				else
				{
					AStarPathFind.SetPointStart(g_iStartX, g_iStartY, false);
					AStarPathFind.SetPointStart(positionX, positionY, true);
					AStarPathFind.SetPointBlock(positionX, positionY, false);
					g_iStartX = positionX;
					g_iStartY = positionY;
					g_bStartClicked = true;
				}
			}
			else
			{
				AStarPathFind.SetPointStart(g_iStartX, g_iStartY, true);
				AStarPathFind.SetPointBlock(positionX, positionY, false);
				g_bStartClicked = true;
				g_iStartX = positionX;
				g_iStartY = positionY;
			}
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:


		DeleteObject(MyPenGreen);
		DeleteObject(MyBrushGreen);
		DeleteObject(MyBrushRed);
		DeleteObject(MyPenRed);

		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
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



void PrintOpen(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyBrushBlue);
	OldBrush = (HBRUSH)SelectObject(hdc, MyPenBlue);
	std::vector<NODE*>temp = AStarPathFind.GetOpenList();
	for (std::vector<NODE*>::iterator itor = temp.begin(); itor != temp.end(); itor++)
	{
		Rectangle(hdc, (*itor)->x * 10 + 1, (*itor)->y * 10 + 1, (*itor)->x * 10 + 10, (*itor)->y * 10 + 10);
	}
	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);
}


void PrintRoute(HDC hdc)
{
	HBRUSH OldBrush;
	HPEN OldPen;

	OldPen = (HPEN)SelectObject(hdc, MyPenRed);
	OldBrush = (HBRUSH)SelectObject(hdc, MyBrushRed);

	std::vector<POSITION*>temp = AStarPathFind.GetRout();
	if (temp.size() <= 0)
		return;
	for (int i = 0; i < temp.size() - 1; i++)
	{
		MoveToEx(hdc, temp[i + 1]->X * 10 + 5, temp[i + 1]->Y * 10 + 5, NULL);
		LineTo(hdc, temp[i]->X * 10 + 5, temp[i]->Y * 10 + 5);
	}
	SelectObject(hdc, OldPen);
	SelectObject(hdc, OldBrush);
}

