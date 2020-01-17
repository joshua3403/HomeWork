#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MY_ENCRYPT_PASSWORD_LEN 8
#define MAX_FILENAME_LEN 16
#define MAX_FILEPATH_LEN  64
#define KEY 5

using namespace std;

char password[MY_ENCRYPT_PASSWORD_LEN] = "Joshua3";

/**********************************************************
함수 : bool CheckEnDecryption(char*)
기능 : 파일의 데이터를 저장한 char*를 인자로 받아와서 앞의 
		MY_ENCRYPT_PASSWORD_LEN길이만큼을 비교하여
		암호화를 해야하는지 복호화를 해야하는지 판단.
		true를 반환하면 복호화 된 상태, false를 반환하면 암호화 된 상태
**********************************************************/
bool CheckEnDecryption(char* context)
{
	char temp[MY_ENCRYPT_PASSWORD_LEN];
	temp[0] = '\n';
	memcpy_s(temp, sizeof(temp), context, MY_ENCRYPT_PASSWORD_LEN);
	if (memcmp(temp, password, MY_ENCRYPT_PASSWORD_LEN-1) == 0)
		return true;
	else
		return false;
}

/**********************************************************
함수 : void EnDecryption(char*)
기능 : 파일의 데이터를 저장한 char*를 인자로 받아와서 
		KEY값과 XOR연산을 하여 복호화, 암호화를 실행
**********************************************************/
void EnDecryption(char* context, long size)
{
	for (int i = 0; i < size; ++i)
	{
		context[i] = context[i] ^ KEY;
	}
}

int main()
{
	char file_Path[MAX_FILEPATH_LEN] = "C:\\Users\\user\\Desktop\\";
	char file_Name[MAX_FILENAME_LEN];	// 파일 이름을 저장할 변수
	file_Name[0] = '\0';
	char* file_Context;	// 파일 데이터를 저장할 변수
	long lFile_Size;	// 파일 크기를 저장할 변수
	size_t result;	// 파일에서 읽어온 데이터의 크기
	bool flag;	// true면 복호화된 상태, false면 암호화된 상태

	printf("파일의 암호화/복호화를 하는 프로그램입니다.\n원하는 프로그램의 이름을 입력하세요 : ");

	scanf_s("%s", file_Name, sizeof(file_Name));

	strcat_s(file_Path, file_Name);

	FILE *pFile = fopen(file_Path, "rb");



	if (pFile == NULL)
	{
		printf("입력하신 파일이 존재하지 않습니다.\n");
		exit(1);
	}
	else
	{
		fseek(pFile, 0, SEEK_END);
		lFile_Size = ftell(pFile);
		rewind(pFile);

		file_Context = (char*)malloc(sizeof(char) * lFile_Size);
		if (file_Context == NULL)
		{
			printf("메모리 에러가 발생했습니다.");
			exit(2);
		}
		result = fread(file_Context, 1, lFile_Size, pFile);
		if (result != lFile_Size)
		{
			printf("%d \t %d", lFile_Size, result);
			printf("파일 읽기에 실패했습니다.\n");
			exit(3);
		}
		else
		{
			flag = CheckEnDecryption(file_Context);
			if (flag)
			{
				printf("암호화 시작\n");
			}
			else
			{
				printf("복호화 시작\n");
			}
			fclose(pFile);
			pFile = fopen(file_Path, "wb");
			EnDecryption(file_Context, lFile_Size);
			fwrite(file_Context, sizeof(char), lFile_Size, pFile);
			free(file_Context);
			fclose(pFile);
			if (flag)
			{
				printf("암호화 완료\n");
			}
			else
			{
				printf("복호화 완료\n");
			}
		}
	}

}