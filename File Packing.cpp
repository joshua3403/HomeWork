#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MY_ENCRYPT_PASSWORD_LEN 8
#define MAX_FILENAME_LEN 128
#define MAX_FILEPATH_LEN  128
#define KEY 5

//----------------------------------
// ��ŷ ���� ���� �� �ش�
//----------------------------------
typedef struct st_PACK_HEADER_
{
	unsigned int iType;	// 0x99886655 �� ��.
	int iFileNum;

} stPACK_HEADER;

//----------------------------------
// ���� ���� ����
//----------------------------------
typedef struct st_PACK_FILEINFO_
{
	int iFileSize;
	char szFileName[MAX_FILENAME_LEN];
	unsigned int iFilePointer;

} stPACK_FILEINFO;


// ���� �⺻ ���
char file_Path[MAX_FILEPATH_LEN] = "C:\\Users\\user\\Desktop\\";


/**********************************************************
�Լ� : int AskAndGetInput(void)
��� : ������ ��ŷ �� ������ ����ŷ �� �������� Ȯ���ϴ� �Լ�
**********************************************************/
int AskAndGetInput(void)
{
	int iInput = 0;
	printf("1. ������ŷ\n2. ���Ͼ���ŷ\n3. ����\n���ϴ� ��ȣ�� �Է��ϼ��� : ");
	scanf_s("%d", &iInput);

	return iInput;
}

/**********************************************************
�Լ� : void AskFileNumber(void)
��� : ������ ��ŷ �� ������ ����ŷ �� �������� Ȯ���ϴ� �Լ�
**********************************************************/
void AskFileNumber(stPACK_HEADER* header)
{
	printf("���ϴ� ������ ������ �Է��� �ּ��� : ");
	scanf_s("%d", &header->iFileNum);
	rewind(stdin);
}

/**********************************************************
�Լ� : void GetFileInfo(int FileNum)
��� : Header����ü�� ���� ������ ���ڷ� �޾�
		�� ����ŭ �ݺ����� ���鼭 ������ �̸��� ũ�⸦ �޴� �Լ�
**********************************************************/
void GetFileInfo(int FileNum, stPACK_FILEINFO* Total_File_Info)
{
	unsigned int point = 0;
	point += sizeof(stPACK_HEADER) + sizeof(stPACK_FILEINFO) * FileNum;
	for (int i = 0; i < FileNum;)
	{
		stPACK_FILEINFO temp;
		char file_Path_Temp[MAX_FILEPATH_LEN * 2];
		long file_size_temp;

		printf("%d��° ���� �̸��� �Է��ϼ��� : ", i + 1);
		scanf_s("%s", Total_File_Info->szFileName, MAX_FILENAME_LEN);
		strcpy_s(file_Path_Temp, file_Path);
		strcat_s(file_Path_Temp, sizeof(temp.szFileName), Total_File_Info->szFileName);
		FILE* pFile;
		pFile = fopen(file_Path_Temp, "rb");
		if (pFile == NULL)
		{
			printf("�߸��� ������ �̸��Դϴ�.\n");
			continue;
		}		
		fseek(pFile, 0, SEEK_END);
		file_size_temp = ftell(pFile);

		Total_File_Info->iFileSize = file_size_temp;
		Total_File_Info->iFilePointer = point;
		Total_File_Info++;
		point += file_size_temp;
		fclose(pFile);
		++i;
	}

	Total_File_Info -= FileNum;
}

/**********************************************************
�Լ� : void PackingFiles(stPACK_HEADER*, stPACK_FILEINFO*)
��� : ��ŷ ����� ������ ��ŷ�� ������ ������ ��� �迭 �����͸� ���ڷ� �޾�
		���ο� ������ ������ �� ����� ���� ������ 
		�׸��� ���� �����͸� �����Ѵ�.
**********************************************************/
void PackingFiles(stPACK_HEADER* header,stPACK_FILEINFO* file_Infos)
{
	FILE* new_File;
	char new_file_name[MAX_FILEPATH_LEN * 2];
	strcpy(new_file_name, file_Path);
	strcat(new_file_name, "PackedFile.txt");
	new_File = fopen(new_file_name, "wb");
	fwrite(header, 1, sizeof(stPACK_HEADER), new_File);

	for (int i = 0; i < header->iFileNum; )
	{
		fwrite(file_Infos, 1, sizeof(stPACK_FILEINFO), new_File);
		file_Infos++;
		++i;
	}
	file_Infos -= header->iFileNum;

	for (int i = 0; i < header->iFileNum; ++i)
	{
		FILE* read_File;
		char read_file_name[MAX_FILEPATH_LEN * 2];
		char* buffer;
		long result;

		strcpy(read_file_name, file_Path);
		strcat(read_file_name, file_Infos->szFileName);

		read_File = fopen(read_file_name, "rb");
		if (read_File == NULL)
		{
			printf("%d��° ������ �о���µ� �����߽��ϴ�.\n��ŷ ����\n", i + 1);
			return;
		}
		buffer = (char*)malloc(sizeof(char) * file_Infos->iFileSize);

		result = fread(buffer, 1, file_Infos->iFileSize, read_File);
		if (result != file_Infos->iFileSize)
		{
			printf("%d��° ������ �о���µ� �����߽��ϴ�.\n��ŷ ����\n", i + 1);
			return;
		}
		fwrite(buffer, 1, file_Infos->iFileSize, new_File);
		free(buffer);
		fclose(read_File);
		file_Infos++;
	}
	file_Infos -= header->iFileNum;
	fclose(new_File);
}

/**********************************************************
�Լ� : void GetPackedFile(char*)
��� : ����ŷ�� ������ ����� ������ �̸��� �Է¹ް�
		��ŷ ������ �ش� ������ �о�´�.
**********************************************************/
void GetPackedFile(char* fileName, int size)
{
	printf("����ŷ�� ���ϴ� ������ �̸��� �Է��ϼ��� : ");
	rewind(stdin);
	scanf_s("%s", fileName, size);
}

/**********************************************************
�Լ� : bool GetHeaderInfo(stPACK_HEADER*,char*)
��� : ����ŷ�� ������ ����� ������ �̸��� �Է¹ް�
		��ŷ ������ �ش� ������ �о�´�.
		��ŷ ������ ����� Ű�� ���� ��ġ�ϸ� true, �ƴϸ� false�� ��ȯ
**********************************************************/
bool GetHeaderInfo(stPACK_HEADER* header,char* fileName)
{
	FILE* pFile;
	char packed_File_Name[MAX_FILENAME_LEN * 2];
	stPACK_HEADER* buffer;
	size_t result;
	strcpy(packed_File_Name, file_Path);
	strcat(packed_File_Name, fileName);
	buffer = (stPACK_HEADER*)malloc(sizeof(stPACK_HEADER));
	pFile = fopen(packed_File_Name, "rb");
	result = fread(buffer, 1, sizeof(stPACK_HEADER), pFile);
	if (result != sizeof(stPACK_HEADER))
	{
		printf("��ŷ�� ���� �б⸦ �����߽��ϴ�.\n");
		exit(1);
	}
	unsigned int temp = buffer->iType;

	bool flag;

	if (temp == header->iType)
	{
		header->iFileNum = buffer->iFileNum;
		flag = true;
	}
	else
		flag = false;
	
	free(buffer);
	fclose(pFile);

	return flag;
}
/**********************************************************
�Լ� : void GetFileInfo(stPACK_HEADER*,stPACK_FILEINFO* ,const char*)
��� : ����ŷ�� ������ ����� ���Ϸ� ���� ���� �������� �����Ͽ�
		FILEINFO �迭�� �����Ѵ�.
**********************************************************/
void GetFileInfo(stPACK_HEADER* header,stPACK_FILEINFO* fileInfo, const char* fileName)
{
	FILE* pFile;
	char unpackedFile[MAX_FILEPATH_LEN * 2];
	char* buffer;
	strcpy(unpackedFile, file_Path);
	strcat(unpackedFile, fileName);
	pFile = fopen(unpackedFile, "rb");
	fseek(pFile, sizeof(stPACK_HEADER), SEEK_SET);
	for (int i = 0; i < header->iFileNum;)
	{
		//char* buffer;
		size_t result;
		//buffer = (char*)malloc(sizeof(stPACK_FILEINFO) * header->iFileNum);
		result = fread(fileInfo, 1, sizeof(stPACK_FILEINFO), pFile);
		if (result != sizeof(stPACK_FILEINFO))
		{
			printf("��ŷ�� ������ %d��° ������ �ùٸ��� �ʽ��ϴ�.\n", i+1);
			i++;
			continue;
		}
		//fileInfo->iFilePointer = ((stPACK_FILEINFO*)buffer)->iFilePointer;
		//fileInfo->iFileSize = ((stPACK_FILEINFO*)buffer)->iFileSize;
		//strcpy(fileInfo->szFileName, ((stPACK_FILEINFO*)buffer)->szFileName);

		fileInfo++;
		i++;
		//free(buffer);
	}
	fileInfo -= header->iFileNum;
	fclose(pFile);
}
/**********************************************************
�Լ� : void UnpackFile(const stPACK_HEADER* ,stPACK_FILEINFO* ,const char*)
��� : ����ŷ�� ������ ����� ������ �̸��� �ش� ������ ���ڷ� �޾� ���Ͽ� ���� ������
		���ڷ� ���� �迭�� ������ ��	������ ������ŭ ������ �����ϰ� �����Ѵ�. 
**********************************************************/
void UnpackFile(const stPACK_HEADER* header, stPACK_FILEINFO* fileInfo,char* fileName)
{
	FILE* pFile;
	char unpackedFile[MAX_FILEPATH_LEN * 2];
	strcpy(unpackedFile, file_Path);
	strcat(unpackedFile, fileName);
	pFile = fopen(unpackedFile, "rb");
	
	for (int i = 0; i < header->iFileNum;)
	{
		char saveFile[MAX_FILEPATH_LEN * 2];
		char* buffer;
		size_t result;
		strcpy(saveFile, file_Path);
		strcat(saveFile, fileInfo->szFileName);
		FILE* new_File = fopen(saveFile, "wb");
		fseek(pFile, fileInfo->iFilePointer, SEEK_SET);
		buffer = (char*)malloc(sizeof(char) * fileInfo->iFileSize);
		result = fread(buffer, 1, fileInfo->iFileSize, pFile);
		if (result != fileInfo->iFileSize)
		{
			printf("%d��° ������ ����ŷ�ϴµ� �����߽��ϴ�.\n", i + 1);
			i++;
			continue;
		}
		fwrite(buffer, 1, fileInfo->iFileSize, new_File);
		free(buffer);
		fclose(new_File);
		++i;
		fileInfo++;
	}
	fclose(pFile);
}

int main()
{

	stPACK_HEADER header;
	// ���� �������� ������ �迭
	stPACK_FILEINFO* Total_File_Info;
	header.iType = 0x99886655;
	int iMenu = AskAndGetInput();
	// ����ŷ�� �� �迭�� �̸�
	char unPackFileName[MAX_FILENAME_LEN];

	switch (iMenu)
	{
	case 1:
		AskFileNumber(&header);
		Total_File_Info = (stPACK_FILEINFO*)malloc(sizeof(stPACK_FILEINFO) * header.iFileNum);
		GetFileInfo(header.iFileNum, Total_File_Info);
		PackingFiles(&header, Total_File_Info);
		break;
	case 2:
		GetPackedFile(unPackFileName, MAX_FILENAME_LEN);
		if (GetHeaderInfo(&header, unPackFileName))
		{
			printf("�ش� ������ ����ŷ�ϰڽ��ϴ�.\n");
			Total_File_Info = (stPACK_FILEINFO*)malloc(sizeof(stPACK_FILEINFO) * header.iFileNum);
			GetFileInfo(&header, Total_File_Info, unPackFileName);
			UnpackFile(&header, Total_File_Info, unPackFileName);
			printf("�ش� ������ ����ŷ�ϴµ� �����߽��ϴ�.\n, �� %d���� ������ ����ŷ �߽��ϴ�.", header.iFileNum);
		}
		else
		{
			printf("������ ���ϰ� ��ŷ ������ ��ġ���� �ʽ��ϴ�.\n");
			exit(1);
		}
		break;
	case 3:
		exit(0);
	}

}