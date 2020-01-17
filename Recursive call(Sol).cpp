#define _CRT_SECURE_NO_WARNING
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#define SCREEN_WIDTH 20
#define SCREEN_HEIGHT 10


void DrawBuffer(char(*Copy)[SCREEN_WIDTH]);
void PaintChar( int ix, int iy, char chData );

int funcCallCount = 0;

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

char Buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

int main()
{
	int iCountHeight, iCountWidth;
	int* p;

	memcpy(Buffer, Original, sizeof(Original));

	DrawBuffer(Original);
	Sleep(3000);
	PaintChar(10, 3, '.');
	printf("Count : %d\n", funcCallCount);

	Sleep(3000);

	return 0;
}

// char형 배열을 받아 프린트 하는 함수
// char형 배열을 받아 프린트 하는 함수
void DrawBuffer(char(*Copy)[SCREEN_WIDTH])
{
	system("cls");
	for (int i = 0; i < SCREEN_HEIGHT; ++i)
	{
		for (int j = 0; j < SCREEN_WIDTH; ++j)
		{
			printf("%c", Copy[i][j]);
		}
		printf("\n");
	}
}

void PaintChar(int iX, int iY, char chData)
{
	Sleep(30);
	funcCallCount++;

	if (iX < 0) return;
	if (iX >= SCREEN_WIDTH) return;
	if (iY < 0) return;
	if (iY >= SCREEN_HEIGHT) return;
	if (Buffer[iY][iX] == chData || Buffer[iY][iX] == ' ') return;

	Buffer[iY][iX] = chData;

	DrawBuffer(Buffer);
	PaintChar(iX - 1, iY, chData);
	PaintChar(iX + 1, iY, chData);
	PaintChar(iX, iY - 1, chData);
	PaintChar(iX, iY + 1, chData);

}