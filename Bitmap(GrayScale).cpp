#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <Windows.h>

int main()
{
	FILE* bitmap1;
	FILE* bitmap2;

	BITMAPFILEHEADER fileHeader;
	BITMAPINFOHEADER infoHeader;

	errno_t err1;
	errno_t err2;

	char* buffer1 = nullptr;

	int iImageSize = 0;
	int iWidth = 0;
	int iHeight = 0;

	err1 = fopen_s(&bitmap1, "sample.bmp", "rb");
	if (err1 != 0)
	{
		printf("파일1번 열기에 실패했습니다.\n");
		return 0;
	}

	if (fread(&fileHeader, sizeof(BITMAPFILEHEADER), 1, bitmap1) != 1)
	{
		fclose(bitmap1);
		return 0;
	}

	if (fileHeader.bfType != 'MB')
	{
		fclose(bitmap1);

		return 0;
	}

	if (fread(&infoHeader, sizeof(BITMAPINFOHEADER), 1, bitmap1) != 1)
	{
		fclose(bitmap1);

		return 0;
	}

	if (infoHeader.biBitCount != 32)
	{
		fclose(bitmap1);
		return 0;
	}


	iImageSize = infoHeader.biSizeImage;
	iWidth = infoHeader.biWidth;
	iHeight = infoHeader.biHeight;

	fseek(bitmap1, fileHeader.bfOffBits, SEEK_SET);

	buffer1 = (char*)malloc(sizeof(char) * iImageSize);

	if (fread(buffer1, 1, iImageSize, bitmap1) != iImageSize)
	{
		fclose(bitmap1);
		return 0;
	}
	printf("%s", buffer1);

	fclose(bitmap1);

	for (int i = 0; i < 4 * iWidth * iHeight; i += 4)
	{
		// RGBTRIPLE 구조체로 파랑, 초록, 빨강값을 가져옴
		unsigned char blue = *(buffer1 + i + 1);
		unsigned char green = *(buffer1 + i + 2);
		unsigned char red = *(buffer1 + i + 3);


		// 파랑, 초록, 빨강값의 평균을 구하면 흑백 이미지를 얻을 수 있음
		unsigned char gray = blue / 2 + green / 2 + red / 2;

		*(buffer1 + i + 1) = gray;
		*(buffer1 + i + 2) = gray;
		*(buffer1 + i + 3) = gray;

	}

	printf("%d %d", sizeof(BITMAPFILEHEADER), sizeof(BITMAPINFOHEADER));

	bitmap2 = fopen("GrayScale.bmp", "wb");

	if (bitmap2 == NULL)
	{
		printf("파일2번 저장에 실패했습니다.\n");
		return 0;
	}

	fwrite(&fileHeader, sizeof(fileHeader), 1, bitmap2);
	fwrite(&infoHeader, sizeof(infoHeader), 1, bitmap2);
	fwrite(buffer1, 1, iImageSize, bitmap2);

	free(buffer1);
	fclose(bitmap2);

	return 0;
}