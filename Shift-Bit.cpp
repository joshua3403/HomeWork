#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main()
{
	// �� ������
	unsigned short shData = 0;
	// ��Ʈ ������ ������
	unsigned short shTemp;

	int iInputBit;
	int iInputFlag;

	int iBitSize = 0;
	int iCount;

	// Data ��Ʈ �� ��� ����Ʈ *8
	iBitSize = sizeof(shTemp) * 8;

	while (true)
	{
		printf("��Ʈ ��ġ(1 ~ 16) : ");
		scanf("%d", &iInputBit);
		if (iInputBit < 1 || iInputBit > 16)
		{
			printf("��Ʈ ������ �ʰ��Ͽ����ϴ�.\n\n");
			continue;
		}
		printf("���ϴ� ��Ʈ ��(0 ~ 1) : ");
		scanf("%d", &iInputFlag);
		if (iInputFlag > 1 || iInputFlag < 0)
		{
			printf("���� ������ ������ϴ�.\n\n");
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
			printf("%d �� Bit : ", iBitSize - iCount);
			if (shTemp != 0)
				printf("ON");
			else
				printf("OFF");

			printf("\n");
		}

		printf("\n");

	}
}