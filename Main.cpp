/*
	Main.cpp
	프로그램 설명 : 키보드 123입력을 받아 맞는 오브젝트를 생성하고 화면에 출력하여
					자동으로 소멸되는 형태의 프로그램
					main에서는 모든 오브젝트의 베이스가되는 클래스의 포인터 배열로 모든 오브젝트를 관리한다.
*/

#include "stdafx.h"

#include "BaseObject.h"
#include "OneStar.h"
#include "TwoStar.h"
#include "ThreeStar.h"

#define MAX_OBJECT_COUNT 20

using namespace std;

// 모든 오브젝트를 베이스 포인터 하나로 관리할 배열
BaseObject* ObjectArr[MAX_OBJECT_COUNT];

// 키보드 1~3의 입력을 받아 각각의 오브젝트를 생성하고 배열에 저장
void KeyProcess();

// 배열 전체를 순회하면서 배열에 저장되어있는 오브젝트의 Action 함수를 실행
void Action();

// 배열 전체를 순회하면서 배열에 저장되어있는 오브젝트의 Draw 함수를 실행
void Draw();

int main()
{
	while (true)
	{
		KeyProcess();

		Action();

		system("cls");
		Draw();

		Sleep(50);
	}

	return 0;
}

// 키보드 1~3의 입력을 받아 각각의 오브젝트를 생성하고 배열에 저장
void KeyProcess()
{
	if (_kbhit())
	{
		char cInput = _getch();

		int iIndex;

		for (iIndex = 0; iIndex < MAX_OBJECT_COUNT; ++iIndex)
		{
			if (ObjectArr[iIndex] == nullptr)
			{
				break;
			}
		}

		switch (cInput)
		{
		case '1':
			ObjectArr[iIndex] = new OneStar(0);
			break;
		case '2':
			ObjectArr[iIndex] = new TwoStar(0);
			break;
		case '3':
			ObjectArr[iIndex] = new ThreeStar(0);
			break;
		}
	}
}

// 배열 전체를 순회하면서 배열에 저장되어있는 오브젝트의 Action 함수를 실행
void Action()
{
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i)
	{
		if (ObjectArr[i] != nullptr)
		{
			if (ObjectArr[i]->Action() == true)
			{
				delete ObjectArr[i];
				ObjectArr[i] = nullptr;
			}
			else
			{
				ObjectArr[i]->Action();
			}
		}
	}
}

// 배열 전체를 순회하면서 배열에 저장되어있는 오브젝트의 Draw 함수를 실행
void Draw()
{
	for (int i = 0; i < MAX_OBJECT_COUNT; ++i)
	{
		if (ObjectArr[i] == nullptr)
		{
			cout << endl;
		}
		else
		{
			ObjectArr[i]->Draw();
			cout << endl;
		}
	}
}

