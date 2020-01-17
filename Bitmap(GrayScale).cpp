#include <stdio.h>


int main()
{
	FILE* bitmap1;
	FILE* bitmap2;

	errno_t err1;
	errno_t err2;

	err1 = fopen_s(&bitmap1,"sample.bmp", "rb");
	if (err1 != 0)
	{
		printf("파일1번 열기에 실패했습니다.\n");
		return 0;
	}
	err2 = fopen_s(&bitmap2,"sample2.bmp", "rb");
	if (err2 != 0)
	{
		printf("파일1번 열기에 실패했습니다.\n");
		return 0;
	}


	return 0;
}