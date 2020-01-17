#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main()
{
	unsigned int uiData = 0;		// �� ������
	unsigned int uiMask = 0;		// ����Ʈ �ɰ� �� ����� ����ũ ������
	unsigned char byByteData = 0;	// ����Ʈ ���� �ɰ� ���� ����

	int iInputPos;
	int iInputData;
	unsigned char byInputData;
	int iCount;

	while (true)
	{
		printf("��ġ(1 ~ 4) : ");
		scanf("%d", &iInputPos);
		printf("��(0 ~ 255) : ");
		scanf("%d", &iInputData);

		// ���� ���� üũ
		if (iInputPos <= 0 || iInputPos >= 5)
		{
			printf("��ġ ������ �ʰ��Ͽ����ϴ�. \n\n");
			continue;
		}

		// �� ���� üũ
		if (iInputData < 0 || iInputData > 255)
		{
			printf("������ ������ �ʰ��Ͽ����ϴ�. \n\n");
			continue;
		}
		byInputData = iInputData;

		// �Է���ġ�� �ش��ϴ� ��Ʈ ����ũ ����

		uiMask = 0xff << (iInputPos - 1) * 8;

		// ���ο� ���� �ֱ����� �Է� �� ��ġ�� ���� ���� �����.
		uiData = uiData & ~uiMask;

		// �Է¹��� �����͸� �ش� ��ġ�� �ִ´�.
		uiData = uiData | (byInputData << (iInputPos - 1) * 8);

		// ���
		for (iCount = 0; iCount < 4; ++iCount)
		{
			uiMask = 0xff << iCount * 8;

			byByteData = (uiData & uiMask) >> iCount * 8;
			printf("%d ��° ����Ʈ �� : %d", iCount + 1, byByteData);
			printf("\n");
		}

		printf("\n");
		printf("��ü 4����Ʈ �� : 0x%08x\n\n", uiData);
	}

}