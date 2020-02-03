/*
	Main.cpp
	���α׷� ���� : Ű���� 123�Է��� �޾� �´� ������Ʈ�� �����ϰ� ȭ�鿡 ����Ͽ�
					�ڵ����� �Ҹ�Ǵ� ������ ���α׷�
					main������ ��� ������Ʈ�� ���̽����Ǵ� Ŭ������ ������ �迭�� ��� ������Ʈ�� �����Ѵ�.
*/

#include "stdafx.h"

#include "BaseObject.h"
#include "OneStar.h"
#include "TwoStar.h"
#include "ThreeStar.h"

#define MAX_OBJECT_COUNT 20

using namespace std;

// ��� ������Ʈ�� ���̽� ������ �ϳ��� ������ �迭
BaseObject* ObjectArr[MAX_OBJECT_COUNT];

// Ű���� 1~3�� �Է��� �޾� ������ ������Ʈ�� �����ϰ� �迭�� ����
void KeyProcess();

// �迭 ��ü�� ��ȸ�ϸ鼭 �迭�� ����Ǿ��ִ� ������Ʈ�� Action �Լ��� ����
void Action();

// �迭 ��ü�� ��ȸ�ϸ鼭 �迭�� ����Ǿ��ִ� ������Ʈ�� Draw �Լ��� ����
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

// Ű���� 1~3�� �Է��� �޾� ������ ������Ʈ�� �����ϰ� �迭�� ����
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

// �迭 ��ü�� ��ȸ�ϸ鼭 �迭�� ����Ǿ��ִ� ������Ʈ�� Action �Լ��� ����
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

// �迭 ��ü�� ��ȸ�ϸ鼭 �迭�� ����Ǿ��ִ� ������Ʈ�� Draw �Լ��� ����
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

