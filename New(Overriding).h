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

	// 메모리 할당 정보 하나
	struct stALLOC_INFO
	{
		void* pAlloc;					// 할당받은 포인터
		int iSize;						// 할당 사이즈
		char szFile[FILE_NAME_LEN];		// 할당 파일 위치
		int iLine;						// 파일 라인
		bool bArray;					// 배열 생성 여부
		bool bFree;						// 해제 여부
	};

private:
	// 메모리 할당 정보들 저장 배열
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

	// new 연산자 오버로딩 함수에서는 본 클래스를 마음껏 접근 할 수 있다.
	friend void* operator new(size_t size, char* File, int Line);
	friend void* operator new[](size_t size, char* File, int Line);
	friend void operator delete(void* p);
	friend void operator delete[](void* p);
};