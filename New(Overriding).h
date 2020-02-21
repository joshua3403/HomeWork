#pragma once

#include "stdafx.h"

class CMemoryHistory
{
public:
	enum
	{
		FILE_NAME_LEN = 128,
		ALLOC_INFO_MAX = 1000
	};

	// �޸� �Ҵ� ���� �ϳ�
	struct stALLOC_INFO
	{
		void* pAlloc;					// �Ҵ���� ������
		int iSize;						// �Ҵ� ������
		char szFile[FILE_NAME_LEN];		// �Ҵ� ���� ��ġ
		int iLine;						// ���� ����
		bool bArray;					// �迭 ���� ����
		bool bFree;						// ���� ����
	};

private:
	// �޸� �Ҵ� ������ ���� �迭
	stALLOC_INFO _allocInfos[ALLOC_INFO_MAX];
	char _logFileName[64];

public:
	CMemoryHistory(const char* szLogFile = "AllocInfo")
	{
		char szTime[17] = "";
		time_t timer;
		struct tm TM;

		memset(_allocInfos, 0, sizeof(stALLOC_INFO) * ALLOC_INFO_MAX);
		memset(_logFileName, 0, 64);

		time(&timer);

		localtime_s(&TM, &timer);

		sprintf_s(szTime, 17, "%04d%02d%02d_%02d%02d%02d", TM.tm_year + 1900, TM.tm_mon + 1, TM.tm_mday, TM.tm_hour, TM.tm_min, TM.tm_sec);

		strcat_s(_logFileName, 64, szLogFile);
	}	
	~CMemoryHistory()
	{
		SaveLogFile();
	}

public:
	bool SaveLogFile(void);
	bool NewAlloc(void* pPtr, char* szFile, int iLine, int iSize, bool bArray = false);
	bool Delete(void* pPtr, bool bArray = false);

	// new ������ �����ε� �Լ������� �� Ŭ������ ������ ���� �� �� �ִ�.
	friend void* operator new(size_t size, char* File, int Line);
	friend void* operator new[](size_t size, char* File, int Line);
	friend void operator delete(void* p);
	friend void operator delete[](void* p);
};