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


// char�� �迭�� �޾� ����Ʈ �ϴ� �Լ�
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

// �Է¹��� ��ǥ�� ���� '0'�� ��� ����Ǵ� ����Լ�
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

// �Է¹��� ��ǥ�� ���� ' '�� ��� ����Ǵ� ����Լ�
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
		// �Է¹��� x,y��ǥ ��
		int x = 0;
		int y = 0;
		// ���� �׸��� �����ǿ� �����ϰ� ���������� ���
		memcpy(Copy, Original, sizeof(Original));
		system("cls");
		print(Copy);
		printf("���ϴ� x��ǥ��(1 ~ 20)�� �Է��� �ּ��� x : ");
		scanf_s("%d", &x);
		if (x > SCREEN_WIDTH || x < 1)
		{
			printf("�߸��� ���� �Է��ϼ̽��ϴ�.");
			Sleep(1000);
			continue;
		}
		printf("���ϴ� y��ǥ��(1 ~ 10)�� �Է��� �ּ��� y : ");
		scanf_s("%d", &y);
		if (y > SCREEN_HEIGHT || y < 1)
		{
			printf("�߸��� ���� �Է��ϼ̽��ϴ�.");
			Sleep(1000);
			continue;
		}
		x--;
		y--;
		if (ReturnChar(x, y) == '0')
		{
			Solution1(x, y);
			printf("******�ϼ�******");
			Sleep(1000);
		}
		else if (ReturnChar(x, y) == ' ')
		{
			Solution2(x, y);
			printf("******�ϼ�******");
			Sleep(1000);
		}
		else
			continue;
	}


	return 0;
}