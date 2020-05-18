#include "stdafx.h"
#include "CRedBlackTree.h"

RBTNode* test;
RBTNode* node;
RBTNode* Nil;


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
void PrintTree(RBTNode*,int x,int y, int depth);

void PrintRoot(RBTNode*, int x, int y);

int main()
{
	Nil = RBT_CreateNode(0);
	Nil->Color = RBTNode::BLACK;

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
	wndclass.lpszClassName = L"RedRBTNode::BLACKTree";

	if (!RegisterClass(&wndclass))
		return 1;


	// 윈도우 생성
	g_hWnd = CreateWindow(L"RedRBTNode::BLACKTree", L"RedRBTNode::BLACKTree", WS_OVERLAPPEDWINDOW, 0, 0, g_iWindowWidth, g_iWindowHeight, NULL, NULL, NULL, NULL);

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
	int temp;
	switch (msg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_CONTROL:
			temp = rand() % 100 + 1;
			if (test != nullptr)
			{
				if (RBT_SearchNode(test, temp) == NULL)
					RBT_InsertNode(&test, RBT_CreateNode(temp));
			}else
				RBT_InsertNode(&test, RBT_CreateNode(temp));

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
				node = RBT_RemoveNode(&test, data);
				if (node != NULL)
					RBT_DestroyNode(node);
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
			oldFont = (HFONT)SelectObject(hdc, g_hFontTitle);
			TextOut(hdc, 100, 800, L"Input : ", 10);
			TextOut(hdc, 300, 800, (LPCWSTR)str, len);
			SelectObject(hdc, oldFont);
		}
		PrintTree(test,g_iWindowWidth / 2 , 0 , 0);
		
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

void PrintTree(RBTNode* Node,int x, int y, int depth)
{
	HDC hdc = GetDC(g_hWnd);
	HBRUSH oldBr;
	HFONT oldFont;
	SetBkMode(hdc, TRANSPARENT);
	CString str;

	int tempx = x;
	int tempy = y;

	if (Node == nullptr || Node == Nil)
		return;

	if (Node != nullptr && Node != Nil)
	{

		str.Format(_T("%d"), Node->Data);
		// 검정색이라면
		if (depth == 0)
			x = g_iWindowWidth / 2;
		else
		{
			// 내가 부모 노드의 왼 자식이라면
			if (Node == Node->Parent->Left)
			{
				x -= g_iWindowWidth / pow(2, depth + 1);
			}
			else
			{
				x += g_iWindowWidth / pow(2, depth + 1);
			}
		}
		y += 100;
		if (Node->Left != Nil && Node->Left != nullptr)
		{
			MoveToEx(hdc, tempx + 20, tempy, NULL);
			LineTo(hdc, x + 20, y);
		}
		if (Node->Right != Nil && Node->Right != nullptr)
		{
			MoveToEx(hdc, tempx + 20, tempy, NULL);
			LineTo(hdc, x + 20, y);
		}
		if (Node->Color == RBTNode::BLACK)
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

	if (Node->Left != nullptr && Node->Left != Nil)
		PrintTree(Node->Left, x,y,depth + 1);
	if (Node->Right != nullptr && Node->Right != Nil)
		PrintTree(Node->Right, x,y,depth + 1);

	ReleaseDC(g_hWnd, hdc);

}