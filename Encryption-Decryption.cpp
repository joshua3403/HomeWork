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
�Լ� : bool CheckEnDecryption(char*)
��� : ������ �����͸� ������ char*�� ���ڷ� �޾ƿͼ� ���� 
		MY_ENCRYPT_PASSWORD_LEN���̸�ŭ�� ���Ͽ�
		��ȣȭ�� �ؾ��ϴ��� ��ȣȭ�� �ؾ��ϴ��� �Ǵ�.
		true�� ��ȯ�ϸ� ��ȣȭ �� ����, false�� ��ȯ�ϸ� ��ȣȭ �� ����
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
�Լ� : void EnDecryption(char*)
��� : ������ �����͸� ������ char*�� ���ڷ� �޾ƿͼ� 
		KEY���� XOR������ �Ͽ� ��ȣȭ, ��ȣȭ�� ����
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
	char file_Name[MAX_FILENAME_LEN];	// ���� �̸��� ������ ����
	file_Name[0] = '\0';
	char* file_Context;	// ���� �����͸� ������ ����
	long lFile_Size;	// ���� ũ�⸦ ������ ����
	size_t result;	// ���Ͽ��� �о�� �������� ũ��
	bool flag;	// true�� ��ȣȭ�� ����, false�� ��ȣȭ�� ����

	printf("������ ��ȣȭ/��ȣȭ�� �ϴ� ���α׷��Դϴ�.\n���ϴ� ���α׷��� �̸��� �Է��ϼ��� : ");

	scanf_s("%s", file_Name, sizeof(file_Name));

	strcat_s(file_Path, file_Name);

	FILE *pFile = fopen(file_Path, "rb");



	if (pFile == NULL)
	{
		printf("�Է��Ͻ� ������ �������� �ʽ��ϴ�.\n");
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
			printf("�޸� ������ �߻��߽��ϴ�.");
			exit(2);
		}
		result = fread(file_Context, 1, lFile_Size, pFile);
		if (result != lFile_Size)
		{
			printf("%d \t %d", lFile_Size, result);
			printf("���� �б⿡ �����߽��ϴ�.\n");
			exit(3);
		}
		else
		{
			flag = CheckEnDecryption(file_Context);
			if (flag)
			{
				printf("��ȣȭ ����\n");
			}
			else
			{
				printf("��ȣȭ ����\n");
			}
			fclose(pFile);
			pFile = fopen(file_Path, "wb");
			EnDecryption(file_Context, lFile_Size);
			fwrite(file_Context, sizeof(char), lFile_Size, pFile);
			free(file_Context);
			fclose(pFile);
			if (flag)
			{
				printf("��ȣȭ �Ϸ�\n");
			}
			else
			{
				printf("��ȣȭ �Ϸ�\n");
			}
		}
	}

}