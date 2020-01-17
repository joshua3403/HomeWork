#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MY_ENCRYPT_PASSWORD_LEN 8
#define MAX_FILENAME_LEN 128
#define MAX_FILEPATH_LEN  128
#define KEY 5

//----------------------------------
// 패킹 파일 가장 앞 해더
//----------------------------------
typedef struct st_PACK_HEADER_
{
	unsigned int iType;	// 0x99886655 이 들어감.
	int iFileNum;

} stPACK_HEADER;

//----------------------------------
// 내부 파일 정보
//----------------------------------
typedef struct st_PACK_FILEINFO_
{
	int iFileSize;
	char szFileName[MAX_FILENAME_LEN];
	unsigned int iFilePointer;

} stPACK_FILEINFO;


// 파일 기본 경로
char file_Path[MAX_FILEPATH_LEN] = "C:\\Users\\user\\Desktop\\";


/**********************************************************
함수 : int AskAndGetInput(void)
기능 : 파일을 패킹 할 것인지 언패킹 할 것인지를 확인하는 함수
**********************************************************/
int AskAndGetInput(void)
{
	int iInput = 0;
	printf("1. 파일패킹\n2. 파일언패킹\n3. 종료\n원하는 번호를 입력하세요 : ");
	scanf_s("%d", &iInput);

	return iInput;
}

/**********************************************************
함수 : void AskFileNumber(void)
기능 : 파일을 패킹 할 것인지 언패킹 할 것인지를 확인하는 함수
**********************************************************/
void AskFileNumber(stPACK_HEADER* header)
{
	printf("원하는 파일의 개수를 입력해 주세요 : ");
	scanf_s("%d", &header->iFileNum);
	rewind(stdin);
}

/**********************************************************
함수 : void GetFileInfo(int FileNum)
기능 : Header구조체의 파일 개수를 인자로 받아
		그 수만큼 반복문을 돌면서 파일의 이름과 크기를 받는 함수
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

		printf("%d번째 파일 이름을 입력하세요 : ", i + 1);
		scanf_s("%s", Total_File_Info->szFileName, MAX_FILENAME_LEN);
		strcpy_s(file_Path_Temp, file_Path);
		strcat_s(file_Path_Temp, sizeof(temp.szFileName), Total_File_Info->szFileName);
		FILE* pFile;
		pFile = fopen(file_Path_Temp, "rb");
		if (pFile == NULL)
		{
			printf("잘못된 파일의 이름입니다.\n");
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
함수 : void PackingFiles(stPACK_HEADER*, stPACK_FILEINFO*)
기능 : 패킹 헤더의 정보와 패킹할 파일의 정보가 담긴 배열 포인터를 인자로 받아
		새로운 파일을 생성한 뒤 헤더와 파일 정보들 
		그리고 파일 데이터를 저장한다.
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
			printf("%d번째 파일을 읽어오는데 실패했습니다.\n패킹 실패\n", i + 1);
			return;
		}
		buffer = (char*)malloc(sizeof(char) * file_Infos->iFileSize);

		result = fread(buffer, 1, file_Infos->iFileSize, read_File);
		if (result != file_Infos->iFileSize)
		{
			printf("%d번째 파일을 읽어오는데 실패했습니다.\n패킹 실패\n", i + 1);
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
함수 : void GetPackedFile(char*)
기능 : 언패킹을 실행할 대상인 파일의 이름을 입력받고
		패킹 파일의 해더 정보를 읽어온다.
**********************************************************/
void GetPackedFile(char* fileName, int size)
{
	printf("언패킹을 원하는 파일의 이름을 입력하세요 : ");
	rewind(stdin);
	scanf_s("%s", fileName, size);
}

/**********************************************************
함수 : bool GetHeaderInfo(stPACK_HEADER*,char*)
기능 : 언패킹을 실행할 대상인 파일의 이름을 입력받고
		패킹 파일의 해더 정보를 읽어온다.
		패킹 파일의 헤더중 키의 값이 일치하면 true, 아니면 false를 반환
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
		printf("패킹된 파일 읽기를 실패했습니다.\n");
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
함수 : void GetFileInfo(stPACK_HEADER*,stPACK_FILEINFO* ,const char*)
기능 : 언패킹을 실행할 대상인 파일로 부터 파일 정보들을 추출하여
		FILEINFO 배열에 저장한다.
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
			printf("패킹된 파일의 %d번째 정보가 올바르지 않습니다.\n", i+1);
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
함수 : void UnpackFile(const stPACK_HEADER* ,stPACK_FILEINFO* ,const char*)
기능 : 언패킹을 실행할 대상인 파일의 이름과 해더 정보를 인자로 받아 파일에 관한 정보를
		인자로 받은 배열에 저장한 후	파일의 개수만큼 파일을 생성하고 저장한다. 
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
			printf("%d번째 파일을 언패킹하는데 실패했습니다.\n", i + 1);
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
	// 파일 정보들을 저장할 배열
	stPACK_FILEINFO* Total_File_Info;
	header.iType = 0x99886655;
	int iMenu = AskAndGetInput();
	// 언패킹을 할 배열의 이름
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
			printf("해당 파일을 언패킹하겠습니다.\n");
			Total_File_Info = (stPACK_FILEINFO*)malloc(sizeof(stPACK_FILEINFO) * header.iFileNum);
			GetFileInfo(&header, Total_File_Info, unPackFileName);
			UnpackFile(&header, Total_File_Info, unPackFileName);
			printf("해당 파일을 언패킹하는데 성공했습니다.\n, 총 %d개의 파일을 언패킹 했습니다.", header.iFileNum);
		}
		else
		{
			printf("지정한 파일과 패킹 포맷이 일치하지 않습니다.\n");
			exit(1);
		}
		break;
	case 3:
		exit(0);
	}

}