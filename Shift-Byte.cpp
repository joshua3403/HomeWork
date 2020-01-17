#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

void main()
{
	unsigned int uiData = 0;		// 본 데이터
	unsigned int uiMask = 0;		// 바이트 쪼갤 때 사용할 마스크 데이터
	unsigned char byByteData = 0;	// 바이트 단위 쪼개 넣을 공간

	int iInputPos;
	int iInputData;
	unsigned char byInputData;
	int iCount;

	while (true)
	{
		printf("위치(1 ~ 4) : ");
		scanf("%d", &iInputPos);
		printf("값(0 ~ 255) : ");
		scanf("%d", &iInputData);

		// 오류 범위 체크
		if (iInputPos <= 0 || iInputPos >= 5)
		{
			printf("위치 범위를 초가하였습니다. \n\n");
			continue;
		}

		// 갑 오류 체크
		if (iInputData < 0 || iInputData > 255)
		{
			printf("데이터 범위를 초과하였습니다. \n\n");
			continue;
		}
		byInputData = iInputData;

		// 입력위치에 해당하는 비트 마스크 생성

		uiMask = 0xff << (iInputPos - 1) * 8;

		// 새로운 값을 넣기위해 입력 할 위치의 값을 먼저 지운다.
		uiData = uiData & ~uiMask;

		// 입력받은 데이터를 해당 위치에 넣는다.
		uiData = uiData | (byInputData << (iInputPos - 1) * 8);

		// 출력
		for (iCount = 0; iCount < 4; ++iCount)
		{
			uiMask = 0xff << iCount * 8;

			byByteData = (uiData & uiMask) >> iCount * 8;
			printf("%d 번째 바이트 값 : %d", iCount + 1, byByteData);
			printf("\n");
		}

		printf("\n");
		printf("전체 4바이트 값 : 0x%08x\n\n", uiData);
	}

}