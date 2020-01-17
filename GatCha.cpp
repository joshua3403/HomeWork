#include <iostream>
#include <cstdlib>
#include <time.h>
#include <conio.h>
#include <iomanip>

using namespace std;

struct st_ITEM
{
	char Name[30];
	int Rate; // �������� ���� ����
};

st_ITEM g_Gatcha[] = {
	{"Į", 20},
	{"����", 20},
	{"�Ź�", 20},
	{"����", 20},
	{"�ʰ��·����", 5},
	{"�ʰ��¹���", 5},
	{"�ʰ��·�����Ź�", 1}
};

struct st_ITEM2
{
	char Name[30];
	int Rate; // �Ϲ� ���� �̱�� ���� �뵵
	int WinTime; // �� �������� ���� �̱� ȸ��.
	// 0 �̸� �Ϲ� ������
	// 0 �� �ƴϸ� �� ȸ������ ����
};

st_ITEM2 g_Gatcha2[] = {
	{"Į", 20, 0},
	{"����", 20, 0},
	{"�Ź�", 20, 0},
	{"����", 20, 0},
	{"�ʰ��·����", 5, 0},
	{"�ʰ��¹���", 5, 0},
	{"�ʰ��·�����Ź�1", 1, 50},
	{"�ʰ��·�����Ź�2", 1, 51},
	{"�ʰ��·�����Ź�3", 1, 10}
};

int iCount = 0;

void PrintGatcha(int count,int index)
{
	cout << setfill('0') << setw(3) << count << ". " << g_Gatcha[index].Name << "��(��) ȹ���ϼ̽��ϴ�." << endl;
}

void PrintGatcha2(int count, int index)
{
	cout << setfill('0') << setw(3) << count << ". " << g_Gatcha2[index].Name << "��(��) ȹ���ϼ̽��ϴ�." << endl;
}

void GatCha1(int count)
{
	int percent_Sum = 0;

	for (int i = 0; i < 7; i++)
	{
		percent_Sum += g_Gatcha[i].Rate;
	}


	int item_Percent = rand() % percent_Sum + 1;

	int check_percent = 0;
	int index = 0;

	for (int i = 0; i < 7; ++i)
	{
		check_percent += g_Gatcha[i].Rate;
		if (item_Percent <= check_percent)
		{
			index = i;
			break;
		}
	}
	PrintGatcha(count, index);
}

void GatCha2()
{
	iCount++;

	int percent_Sum = 0;

	int index = 0;
	int check_percent = 0;


	for (int i = 0; i < 9 - 3; ++i)
	{
		percent_Sum += g_Gatcha2[i].Rate;
	}

	if (iCount == g_Gatcha2[6].WinTime)
	{
		index = 6;
	}
	else if (iCount == g_Gatcha2[7].WinTime)
	{
		index = 7;
	}
	else if (iCount == g_Gatcha2[8].WinTime)
	{
		index = 8;
	}
	else
	{
		int item_Percent = rand() % percent_Sum + 1;
		for (int i = 0; i < 7; ++i)
		{
			check_percent += g_Gatcha2[i].Rate;
			if (item_Percent <= check_percent)
			{
				index = i;
				break;
			}
		}
	}

	if (iCount > 51)
		iCount = 1;

	PrintGatcha2(iCount, index);
	
}

void GatCha3(int count)
{
	int percent_Sum = 0;

	for (int i = 0; i < 7; i++)
	{
		percent_Sum += g_Gatcha[i].Rate;
	}


	int item_Percent = rand() % percent_Sum + 1;

	int check_percent = 0;
	int index = 0;

	for (int i = 0; i < 7; ++i)
	{
		check_percent += g_Gatcha[i].Rate;
		if (item_Percent <= check_percent)
		{
			index = i;
			g_Gatcha[i].Rate--;
			break;
		}
	}
	PrintGatcha(count, index);
}

int main()
{
	int count = 1;

	srand((unsigned int)time(NULL));

	while (true)
	{
		_getch();
		GatCha3(count);
		count++;
	}
}