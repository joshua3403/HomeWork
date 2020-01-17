#include <stdio.h>
#include <Windows.h>
#define SCREEN_WIDTH 20
#define SCREEN_HEIGHT 10
#define SLEPPSPEED 10

char Original[SCREEN_HEIGHT][SCREEN_WIDTH] = {
{ ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
{ ' ', ' ', '0', '0', '0', ' ', ' ', ' ', ' ', '0', '0', '0', ' ', ' ', ' ', ' ', ' ', '0', '0', ' ' },
{ ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', ' ' },
{ ' ', '0', '0', '0', ' ', '0', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ' },
{ ' ', '0', '0', '0', ' ', ' ', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ' },
{ ' ', '0', '0', '0', ' ', ' ', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ' },
{ ' ', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ' },
{ ' ', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ', ' ', ' ' },
{ ' ', ' ', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0', '0', '0', '0', '0', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
};

char Copy[SCREEN_HEIGHT][SCREEN_WIDTH] = { 0 };


// char형 배열을 받아 프린트 하는 함수
void print(char(*Copy)[SCREEN_WIDTH])
{
	for (int i = 0; i < SCREEN_HEIGHT; ++i)
	{
		for (int j = 0; j < SCREEN_WIDTH; ++j)
		{
			printf("%c", Copy[i][j]);
		}
		printf("\n");
	}
}

char ReturnChar(int x, int y)
{
	return Original[y][x];
}

// 입력받은 좌표의 값이 '0'일 경우 실행되는 재귀함수
void Solution1(int x, int y)
{
	system("cls");
	print(Copy);	
	Sleep(SLEPPSPEED);

	if (Copy[x][y] == '0')
	{
		Copy[x][y] = '.';
		Solution1(x - 1, y);
		Solution1(x + 1, y);
		Solution1(x, y - 1);
		Solution1(x, y + 1);
	}

	return;
}

// 입력받은 좌표의 값이 ' '일 경우 실행되는 재귀함수
void Solution2(int x, int y)
{
	system("cls");
	print(Copy);
	Sleep(SLEPPSPEED);
  
	if (Copy[x][y] == ' ')
	{
		Copy[x][y] = '.';
		Solution2(x - 1, y);
		Solution2(x + 1, y);
		Solution2(x, y - 1);
		Solution2(x, y + 1);
	}

	return;
}



int main()
{
	

	while (true)
	{
		// 입력받을 x,y좌표 값
		int x = 0;
		int y = 0;
		// 원본 그림을 복사판에 복사하고 복사판으로 출력
		memcpy(Copy, Original, sizeof(Original));
		system("cls");
		print(Copy);
		printf("원하는 x좌표값(1 ~ 20)을 입력해 주세요 x : ");
		scanf_s("%d", &x);
		if (x > SCREEN_WIDTH || x < 1)
		{
			printf("잘못된 수를 입력하셨습니다.");
			Sleep(1000);
			continue;
		}
		printf("원하는 y좌표값(1 ~ 10)을 입력해 주세요 y : ");
		scanf_s("%d", &y);
		if (y > SCREEN_HEIGHT || y < 1)
		{
			printf("잘못된 수를 입력하셨습니다.");
			Sleep(1000);
			continue;
		}
		x--;
		y--;
		if (ReturnChar(x, y) == '0')
		{
			Solution1(x, y);
			printf("******완성******");
			Sleep(1000);
		}
		else if (ReturnChar(x, y) == ' ')
		{
			Solution2(x, y);
			printf("******완성******");
			Sleep(1000);
		}
		else
			continue;
	}


	return 0;
}