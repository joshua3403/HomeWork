#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main()
{
	// 본 데이터
	unsigned short shData = 0;
	// 비트 반전용 데이터
	unsigned short shTemp;

	int iInputBit;
	int iInputFlag;

	int iBitSize = 0;
	int iCount;

	// Data 비트 수 계산 바이트 *8
	iBitSize = sizeof(shTemp) * 8;

	while (true)
	{
		printf("비트 위치(1 ~ 16) : ");
		scanf("%d", &iInputBit);
		if (iInputBit < 1 || iInputBit > 16)
		{
			printf("비트 범위를 초과하였습니다.\n\n");
			continue;
		}
		printf("원하는 비트 값(0 ~ 1) : ");
		scanf("%d", &iInputFlag);
		if (iInputFlag > 1 || iInputFlag < 0)
		{
			printf("값의 범위를 벗어났습니다.\n\n");
			continue;
		}

		if (iInputFlag == 0)
		{
			shData = shData & (~(1 << iInputBit - 1));
		}
		else
			shData = shData | (1 << iInputBit - 1);

		for (iCount = 0; iCount < iBitSize; ++iCount)
		{
			shTemp = shData & (1 << iBitSize - iCount - 1);
			printf("%d 번 Bit : ", iBitSize - iCount);
			if (shTemp != 0)
				printf("ON");
			else
				printf("OFF");

			printf("\n");
		}

		printf("\n");

	}
}