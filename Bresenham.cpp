#include "stdafx.h"

HWND g_hWnd;
int g_iWindowWidth = GetSystemMetrics(SM_CXSCREEN);
int g_iWindowHeight = GetSystemMetrics(SM_CYSCREEN);
HBRUSH MyBrushRed = CreateSolidBrush(RGB(255, 0, 0));
HPEN MyPenRed = CreatePen(PS_SOLID, 3, RGB(255, 0, 0));

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void PrintGrid(HDC hdc);


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

		EndPaint(hWnd, &ps);
		break;



	case WM_DESTROY:


		DeleteObject(MyBrushRed);
		DeleteObject(MyPenRed);

		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
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