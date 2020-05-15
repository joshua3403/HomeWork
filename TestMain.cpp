#include "stdafx.h"
#include "CRBT.h"

RBTree test;
HWND g_hWnd;
int g_circleSize = 60;
int g_iWindowWidth = GetSystemMetrics(SM_CXSCREEN);
int g_iWindowHeight = GetSystemMetrics(SM_CYSCREEN);
HBRUSH g_hRedBrush = CreateSolidBrush(RGB(255, 0, 0));
HBRUSH g_hBlackBrush = CreateSolidBrush(RGB(0, 0, 0));
HFONT g_hFontTitle = CreateFont(40, 0, 0, 0, 300, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"궁서체");
bool isDelete = false;
char str[256];
int i = 1;

int len = 0;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void PrintTree(NodePtr, int x, int y);

void PrintRoot(NodePtr, int x, int y);

int main()
{
	srand((unsigned int)time(NULL));
	WNDCLASS wndclass;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
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
	static int StartX, StartY, EndX, EndY = 0;
	static BOOL bNowDraw = FALSE;
	HBRUSH oldBr;
	HFONT oldFont;
	HDC hdc;

	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_CONTROL:
			test.insert(rand() % 100);
			break;
		case VK_SPACE:
			isDelete = true;
			InvalidateRect(hWnd, NULL, TRUE);
			break;
		case VK_RETURN:
			if (isDelete)
			{
				isDelete = false;
				int data = atoi(str);
				test.deleteNode(data);
				InvalidateRect(hWnd, NULL, TRUE);

				str[0] = '\0';
			}
			break;
		}
		InvalidateRect(hWnd, NULL, TRUE);
		break;
	case WM_CHAR:
		if (isDelete)
		{
			len = strlen(str);
			str[len] = (TCHAR)wParam;
			str[len + 1] = 0;
			InvalidateRect(hWnd, NULL, FALSE);
		}


		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		SetTextColor(hdc, RGB(0, 0, 0));
		if (isDelete)
		{
			TextOut(hdc, 100, 1200, L"Input : ", 10);
			TextOut(hdc, 150, 1200, (LPCWSTR)str, len);
		}
		PrintRoot(test.getRoot(), g_iWindowWidth / 2,50);
		PrintTree(test.getRoot()->left, g_iWindowWidth / 2 -500, 120);
		PrintTree(test.getRoot()->right, g_iWindowWidth / 2 + 500, 120);

		EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		DeleteObject(g_hRedBrush);
		DeleteObject(g_hBlackBrush);
		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

void PrintRoot(NodePtr Node, int x, int y)
{
	HDC hdc = GetDC(g_hWnd);
	HBRUSH oldBr;
	HFONT oldFont;
	SetBkMode(hdc, TRANSPARENT);
	CString str;

	if (Node == RBTree::TNULL)
		return;

	if (Node != RBTree::TNULL)
	{
		str.Format(_T("%d"), Node->data);
		// 검정색이라면
		if (Node->color == 0)
		{
			oldBr = (HBRUSH)SelectObject(hdc, g_hBlackBrush);
			Ellipse(hdc, x, y, x + g_circleSize, y + g_circleSize);
			SelectObject(hdc, oldBr);

			SetTextColor(hdc, RGB(255, 255, 255));
			oldFont = (HFONT)SelectObject(hdc, g_hFontTitle);
			TextOut(hdc, x + 12, y + 10, str, str.GetLength());
			SelectObject(hdc, oldFont);
		}
		else
		{
			oldBr = (HBRUSH)SelectObject(hdc, g_hRedBrush);
			Ellipse(hdc, x, y, x + g_circleSize, y + g_circleSize);
			SelectObject(hdc, oldBr);

			SetTextColor(hdc, RGB(0, 0, 0));
			oldFont = (HFONT)SelectObject(hdc, g_hFontTitle);
			TextOut(hdc, x + 12, y + 10, str, str.GetLength());
			SelectObject(hdc, oldFont);

		}
	}

	ReleaseDC(g_hWnd, hdc);
}

void PrintTree(NodePtr Node, int x, int y)
{
	HDC hdc = GetDC(g_hWnd);
	HBRUSH oldBr;
	HFONT oldFont;
	SetBkMode(hdc, TRANSPARENT);
	CString str;

	if (Node == RBTree::TNULL)
		return;

	if (Node != RBTree::TNULL && Node != nullptr)
	{
		str.Format(_T("%d"), Node->data);
		// 검정색이라면
		if (Node->color == 0)
		{
			oldBr = (HBRUSH)SelectObject(hdc, g_hBlackBrush);
			Ellipse(hdc, x, y, x + g_circleSize, y + g_circleSize);
			SelectObject(hdc, oldBr);

			SetTextColor(hdc, RGB(255, 255, 255));
			oldFont = (HFONT)SelectObject(hdc, g_hFontTitle);
			TextOut(hdc, x + 12, y + 10, str, str.GetLength());
			SelectObject(hdc, oldFont);
		}
		else
		{
			oldBr = (HBRUSH)SelectObject(hdc, g_hRedBrush);
			Ellipse(hdc, x, y, x + g_circleSize, y + g_circleSize);
			SelectObject(hdc, oldBr);

			SetTextColor(hdc, RGB(0, 0, 0));
			oldFont = (HFONT)SelectObject(hdc, g_hFontTitle);
			TextOut(hdc, x + 12, y + 10, str, str.GetLength());
			SelectObject(hdc, oldFont);

		}

		PrintTree(Node->left, x - 150, y + 70);
		PrintTree(Node->right, x + + 150, y + 70);
	}

	ReleaseDC(g_hWnd, hdc);

}