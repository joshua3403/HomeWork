#include <iostream>
#include <cstdlib>
#include <time.h>
#include <conio.h>
#include <iomanip>

using namespace std;

struct st_ITEM
{
	char Name[30];
	int Rate; // 아이템이 뽑힐 비율
};

st_ITEM g_Gatcha[] = {
	{"칼", 20},
	{"방패", 20},
	{"신발", 20},
	{"물약", 20},
	{"초강력레어무기", 5},
	{"초강력방패", 5},
	{"초강력레어레어레어신발", 1}
};

struct st_ITEM2
{
	char Name[30];
	int Rate; // 일반 랜덤 뽑기와 같은 용도
	int WinTime; // 이 아이템이 나올 뽑기 회차.
	// 0 이면 일반 아이템
	// 0 이 아니면 그 회차에만 나옴
};

st_ITEM2 g_Gatcha2[] = {
	{"칼", 20, 0},
	{"방패", 20, 0},
	{"신발", 20, 0},
	{"물약", 20, 0},
	{"초강력레어무기", 5, 0},
	{"초강력방패", 5, 0},
	{"초강력레어레어레어신발1", 1, 50},
	{"초강력레어레어레어신발2", 1, 51},
	{"초강력레어레어레어신발3", 1, 10}
};

int iCount = 0;

void PrintGatcha(int count,int index)
{
	cout << setfill('0') << setw(3) << count << ". " << g_Gatcha[index].Name << "을(를) 획득하셨습니다." << endl;
}

void PrintGatcha2(int count, int index)
{
	cout << setfill('0') << setw(3) << count << ". " << g_Gatcha2[index].Name << "을(를) 획득하셨습니다." << endl;
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