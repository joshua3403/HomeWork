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
bool g_bAlgorithmStart = false;




LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void PrintGrid(HDC hdc);
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
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			AStarPathFind.ResetPoint();
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
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


		AStarPathFind.PrintBlock(hdc);
		// A Star Algoritm start
		g_bAlgorithmStart = g_bEndClicked && g_bStartClicked;
		if(g_bAlgorithmStart)
		{
			AStarPathFind.SetDC(hdc);
			AStarPathFind.FindPath(g_iStartX, g_iStartY, g_iEndX, g_iEndY);

		}
		else
		{
			AStarPathFind.ClearVector();
		}
		g_bAlgorithmStart = !g_bAlgorithmStart;

		EndPaint(hWnd, &ps);
		break;

	case WM_RBUTTONDBLCLK:
		positionY = HIWORD(lParam) / 10;
		positionX = LOWORD(lParam) / 10;
		if (positionY < 100 && positionX < 100)
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
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_LBUTTONDBLCLK:
		positionY = HIWORD(lParam) / 10;
		positionX = LOWORD(lParam) / 10;
		if (positionY < 100 && positionX < 100)
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
		InvalidateRect(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:

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


