#include "stdafx.h"
#include "JPS.h"
#include "A-Star.h"
#include "Profiler.h"

HWND g_hWnd;
int g_iWindowWidth = GetSystemMetrics(SM_CXSCREEN);
int g_iWindowHeight = GetSystemMetrics(SM_CYSCREEN);

int g_iCountX = 100;
int g_iCountY = 100;

bool g_bIsStartDeleted = false;



bool g_bEndClicked = false;
bool g_bStartClicked = false;
int g_iEndX = 0, g_iEndY = 0, g_iStartX = 0, g_iStartY = 0;
bool g_bAlgorithmStart = false;
bool g_bAStar = false;
bool g_bJPS = false;
bool g_bBresenham = false;
HFONT _hFont = CreateFont(20, 0, 0, 0, 5, 0, 0, 0, DEFAULT_CHARSET, 0, 0, ANTIALIASED_QUALITY, 0, L"궁서체");
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

CJPS JPSPathFinding;
AStarPathFinding AStarPathFind;
Joshua::List TimeList;

int main()
{
	srand((unsigned int)time(NULL));

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
	static HWND c1, c2, c3, c4;
	double time = 0;
	TCHAR stringtemp[64] = { '\0' };
	HFONT oldFont;
	char string[64] = { '\0' };

	switch (msg)
	{
	case WM_CREATE:

		CreateWindow(L"button", L"Algorithm", WS_CHILD | WS_VISIBLE |
			BS_GROUPBOX, 1100, 20, 600, 100, hWnd, (HMENU)0, NULL, NULL);
		CreateWindow(L"button", L"Heuristic", WS_CHILD | WS_VISIBLE |
			BS_GROUPBOX, 1100, 150, 600, 100, hWnd, (HMENU)1, NULL, NULL);

		CreateWindow(L"button", L"optimization", WS_CHILD | WS_VISIBLE |
			BS_GROUPBOX, 1100, 280, 600, 100, hWnd, (HMENU)2, NULL, NULL);

		CreateWindow(L"button", L"Save Map", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			1100, 480, 100, 100, hWnd, (HMENU)3, NULL, NULL);

		CreateWindow(L"button", L"Snail", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			1250, 480, 100, 100, hWnd, (HMENU)4, NULL, NULL);

		CreateWindow(L"button", L"Puzzle", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			1400, 480, 100, 100, hWnd, (HMENU)5, NULL, NULL);

		CreateWindow(L"button", L"Random", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			1550, 480, 100, 100, hWnd, (HMENU)6, NULL, NULL);

		CreateWindow(L"button", L"Clear Block", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			1700, 480, 100, 100, hWnd, (HMENU)7, NULL, NULL);

		c1 = CreateWindow(L"button", L"A Star", WS_CHILD | WS_VISIBLE |
			BS_CHECKBOX, 1150, 70, 200, 25, hWnd, (HMENU)101, NULL, NULL);

		c2 = CreateWindow(L"button", L"Jump Point Search", WS_CHILD | WS_VISIBLE |
			BS_CHECKBOX, 1400, 70, 200, 25, hWnd, (HMENU)102, NULL, NULL);

		c3 = CreateWindow(L"button", L"Bresenham", WS_CHILD | WS_VISIBLE |
			BS_CHECKBOX, 1150, 330, 200, 25, hWnd, (HMENU)103, NULL, NULL);

		c4 = CreateWindow(L"button", L"Manhatan", WS_CHILD | WS_VISIBLE |
			BS_CHECKBOX, 1150, 200, 200, 25, hWnd, (HMENU)104, NULL, NULL);

		SendMessage(c4, BM_SETCHECK, BST_CHECKED, 0);

		break;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case 3:
			if (g_bAStar)
				AStarPathFind.SaveMap();
			if (g_bJPS)
				JPSPathFinding.SaveMap();
			break;
		case 4:
			if (g_bAStar)
				AStarPathFind.LoadMap("Snail.txt");
			if (g_bJPS)
				JPSPathFinding.LoadMap("Snail.txt");
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 6:
			if (g_bAStar)
				AStarPathFind.RandomMap();
			if (g_bJPS)
				JPSPathFinding.RandomMap();
			InvalidateRect(hWnd, NULL, false);
			break;
		case 7:
			if (g_bAStar)
				AStarPathFind.ResetPoint();
			if (g_bJPS)
				JPSPathFinding.ResetPoint();
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case 101:
			if (SendMessage(c1, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
			{
				SendMessage(c1, BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(c2, BM_SETCHECK, BST_UNCHECKED, 0);
				g_bAStar = true;
				g_bJPS = false;
				JPSPathFinding.ResetPoint();
				SendMessage(c3, BM_SETCHECK, BST_UNCHECKED, 0);
				g_bBresenham = false;
			}
			else
			{
				SendMessage(c1, BM_SETCHECK, BST_UNCHECKED, 0);
				g_bAStar = false;
				AStarPathFind.ResetPoint();
			}
			break;
		case 102:
			if (SendMessage(c2, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
			{
				SendMessage(c2, BM_SETCHECK, BST_CHECKED, 0);
				SendMessage(c1, BM_SETCHECK, BST_UNCHECKED, 0);
				g_bJPS = true;
				g_bAStar = false;
				AStarPathFind.ResetPoint();
			}
			else
			{
				SendMessage(c2, BM_SETCHECK, BST_UNCHECKED, 0);
				g_bJPS = false;
				JPSPathFinding.ResetPoint();
			}
			break;
		case 103:
			if (SendMessage(c3, BM_GETCHECK, 0, 0) == BST_UNCHECKED)
			{
				if (g_bAStar)
				{
					SendMessage(c3, BM_SETCHECK, BST_UNCHECKED, 0);
					g_bBresenham = false;
				}
				else
					SendMessage(c3, BM_SETCHECK, BST_CHECKED, 0);
				g_bBresenham = true;
			}
			else
			{
				SendMessage(c3, BM_SETCHECK, BST_UNCHECKED, 0);
				g_bBresenham = false;
			}
		}
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			JPSPathFinding.ResetPoint();
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDOWN:
		bDrag = TRUE;
	case WM_MOUSEMOVE:
		if (bDrag == TRUE)
		{
			if (!g_bAStar && !g_bJPS)
				break;
			positionY = HIWORD(lParam) / 10;
			positionX = LOWORD(lParam) / 10;
			if (positionY < 100 && positionX < 100)
			{
				if (prevPositionX != positionX || prevPositionY != positionY)
				{
					JPSPathFinding.PointBlockPoint(positionX, positionY);
					AStarPathFind.SetPointBlock(positionX, positionY);
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

		JPSPathFinding.PrintGrid(hdc);

		//// JPS Algoritm start
		if (g_bJPS)
		{
			JPSPathFinding.PrintBlock(hdc);
			JPSPathFinding.PrintStart(hdc);
			JPSPathFinding.PrintEnd(hdc);
			g_bAlgorithmStart = JPSPathFinding.GetEndIsClicked() && JPSPathFinding.GetStartIsClicked();
			if (g_bAlgorithmStart)
			{
				TimeList.PushBack(L"JPS");
				JPSPathFinding.SetHDC(hdc);
				JPSPathFinding.FindPath(g_iStartX, g_iStartY, g_iEndX, g_iEndY);
				if(g_bBresenham)
					JPSPathFinding.PrintRouteBresenham(hdc);

				JPSPathFinding.PrintRoute(hdc);
				oldFont = (HFONT)SelectObject(hdc, _hFont);
				time = TimeList.GetTime(L"JPS");

				printf("%.6f\n", time);
				wsprintf(stringtemp, TEXT("Exceed Time : %d second(will be fixed)"), time);
				TextOut(hdc, 1100, 630, stringtemp, _tcslen(stringtemp));

				SelectObject(hdc, oldFont);
			}
			else
			{
				JPSPathFinding.ClearVector();
			}
			JPSPathFinding.PrintNode(hdc);
			JPSPathFinding.PrintEnd(hdc);

		}

		if (g_bAStar)
		{
			AStarPathFind.PrintBlock(hdc);
			AStarPathFind.PrintStart(hdc);
			AStarPathFind.PrintEnd(hdc);
			g_bAlgorithmStart = AStarPathFind.GetEndIsClicked() && AStarPathFind.GetStartIsClicked();
			if (g_bAlgorithmStart)
			{
				TimeList.PushBack(L"AStar");
				AStarPathFind.SetDC(hdc);
				AStarPathFind.FindPath(g_iStartX, g_iStartY, g_iEndX, g_iEndY);
				AStarPathFind.PrintRoute(hdc);
				oldFont = (HFONT)SelectObject(hdc, _hFont);
				time = TimeList.GetTime(L"AStar");
				printf("%.6f\n", time);

				wsprintf(stringtemp, TEXT("Exceed Time : %d second(will be fixed)"), time);
				TextOut(hdc, 1100, 630, stringtemp, _tcslen(stringtemp));
				SelectObject(hdc, oldFont);
			}
			else
			{
				AStarPathFind.ClearVector();
			}
			AStarPathFind.PrintOpen(hdc);
			AStarPathFind.PrintEnd(hdc);
		}



		EndPaint(hWnd, &ps);
		break;

	case WM_RBUTTONDBLCLK:
		positionY = HIWORD(lParam) / 10;
		positionX = LOWORD(lParam) / 10;
		if (positionY < 100 && positionX < 100)
		{
			if (g_bAStar)
			{
				// 이미 클릭된 곳이 존재한 상태라면
				if (AStarPathFind.GetEndIsClicked())
				{
					if (g_iEndX == positionX && g_iEndY == positionY)
					{
						AStarPathFind.SetPointEnd(positionX, positionY, false);
						AStarPathFind.SetEndIsClicked(false);
					}
					else
					{
						AStarPathFind.SetPointEnd(g_iEndX, g_iEndY, false);
						AStarPathFind.SetPointEnd(positionX, positionY, true);
						AStarPathFind.SetEndIsClicked(true);
						g_iEndY = positionY;
						g_iEndX = positionX;
					}
				}
				else
				{
					AStarPathFind.SetPointEnd(positionX, positionY, true);
					AStarPathFind.SetEndIsClicked(true);
					g_iEndY = positionY;
					g_iEndX = positionX;
				}
			}
			if (g_bJPS)
			{
				// 이미 클릭된 곳이 존재한 상태라면
				if (JPSPathFinding.GetEndIsClicked())
				{
					if (g_iEndX == positionX && g_iEndY == positionY)
					{
						JPSPathFinding.SetPointEnd(positionX, positionY, false);
						JPSPathFinding.SetEndIsClicked(false);
					}
					else
					{
						JPSPathFinding.SetPointEnd(g_iEndX, g_iEndY, false);
						JPSPathFinding.SetPointEnd(positionX, positionY, true);
						JPSPathFinding.SetEndIsClicked(true);
						g_iEndY = positionY;
						g_iEndX = positionX;
					}
				}
				else
				{
					JPSPathFinding.SetPointEnd(positionX, positionY, true);
					JPSPathFinding.SetEndIsClicked(true);
					g_iEndY = positionY;
					g_iEndX = positionX;
				}
			}

		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDBLCLK:
		positionY = HIWORD(lParam) / 10;
		positionX = LOWORD(lParam) / 10;
		if (positionY < 100 && positionX < 100)
		{
			if (g_bAStar)
			{
				// 이미 클릭된 곳이 존재한 상태라면
				if (AStarPathFind.GetStartIsClicked())
				{
					if (g_iStartX == positionX && g_iStartY == positionY)
					{
						AStarPathFind.SetPointStart(positionX, positionY, false);
						AStarPathFind.SetPointBlock(positionX, positionY, false);
						AStarPathFind.SetStartIsClicked(false);
					}
					else
					{
						AStarPathFind.SetPointStart(g_iStartX, g_iStartY, false);
						AStarPathFind.SetPointStart(positionX, positionY, true);
						AStarPathFind.SetPointBlock(positionX, positionY, false);
						g_iStartX = positionX;
						g_iStartY = positionY;
						AStarPathFind.SetStartIsClicked(true);
					}
				}
				else
				{
					AStarPathFind.SetPointStart(g_iStartX, g_iStartY, true);
					AStarPathFind.SetPointBlock(positionX, positionY, false);
					AStarPathFind.SetStartIsClicked(true);
					g_iStartX = positionX;
					g_iStartY = positionY;
				}
			}
			if (g_bJPS)
			{
				// 이미 클릭된 곳이 존재한 상태라면
				if (JPSPathFinding.GetStartIsClicked())
				{
					if (g_iStartX == positionX && g_iStartY == positionY)
					{
						JPSPathFinding.SetPointStart(positionX, positionY, false);
						JPSPathFinding.PointBlockPoint(positionX, positionY, false);
						JPSPathFinding.SetStartIsClicked(false);
					}
					else
					{
						JPSPathFinding.SetPointStart(g_iStartX, g_iStartY, false);
						JPSPathFinding.SetPointStart(positionX, positionY, true);
						JPSPathFinding.PointBlockPoint(positionX, positionY, false);
						g_iStartX = positionX;
						g_iStartY = positionY;
						JPSPathFinding.SetStartIsClicked(true);
					}
				}
				else
				{
					JPSPathFinding.SetPointStart(positionX, positionY, true);
					JPSPathFinding.PointBlockPoint(positionX, positionY, false);
					JPSPathFinding.SetStartIsClicked(true);
					g_iStartX = positionX;
					g_iStartY = positionY;
				}
			}

		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		DeleteObject(_hFont);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}


