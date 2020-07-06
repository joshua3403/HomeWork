#include "stdafx.h"

std::list<int> g_List;

HANDLE threadHandle[6];
HANDLE EventHandle[3];

DWORD threadID[6];
BOOL g_bShutDown = false;

// thread 함수
unsigned int WINAPI InsertList(LPVOID lpParam);
unsigned int WINAPI DeleteList(LPVOID lpParam);
unsigned int WINAPI SaveList(LPVOID lpParam);
unsigned int WINAPI PrintList(LPVOID lpParam);

// 작업 함수
void Insert(int num);
void Delete();
void PrintInsert(int ID ,int num);
void PrintList();

// 동기화 객체
CRITICAL_SECTION gCriticalSectionInsert;
CRITICAL_SECTION gCriticalSectionDelete;

FILE* pFile;

int main()
{
	timeBeginPeriod(1);

	InitializeCriticalSection(&gCriticalSectionInsert);
	InitializeCriticalSection(&gCriticalSectionDelete);

	// 자동 리셋 이벤트
	EventHandle[0] = CreateEvent(NULL, FALSE, FALSE, NULL);
	EventHandle[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	EventHandle[2] = CreateEvent(NULL, FALSE, FALSE, NULL);



	threadHandle[0] = (HANDLE)_beginthreadex(NULL, 0, InsertList, NULL, 0, (unsigned*)&threadID[0]);
	threadHandle[1] = (HANDLE)_beginthreadex(NULL, 0, InsertList, NULL, 0, (unsigned*)&threadID[1]);
	threadHandle[2] = (HANDLE)_beginthreadex(NULL, 0, InsertList, NULL, 0, (unsigned*)&threadID[2]);
	threadHandle[3] = (HANDLE)_beginthreadex(NULL, 0, DeleteList, NULL, 0, (unsigned*)&threadID[3]);
	threadHandle[4] = (HANDLE)_beginthreadex(NULL, 0, SaveList, NULL, 0, (unsigned*)&threadID[4]);
	threadHandle[5] = (HANDLE)_beginthreadex(NULL, 0, PrintList, NULL, 0, (unsigned*)&threadID[5]);

	int input = 0;
	while (true)
	{
		input = _getch();
		if (input == 's')
		{
			SetEvent(EventHandle[0]);
			continue;
		}
		else if (input == 'q')
		{
			SetEvent(EventHandle[1]);
			g_bShutDown = true;
			break;
		}

	}

	WaitForMultipleObjects(6, threadHandle, TRUE, INFINITE);

	wprintf(L"All thread closed\n");
	for (int i = 0; i < 6; i++)
	{
		CloseHandle(threadHandle[i]);
	}
	for (int i = 0; i < 3; i++)
	{
		CloseHandle(EventHandle[i]);
	}
	DeleteCriticalSection(&gCriticalSectionInsert);
	DeleteCriticalSection(&gCriticalSectionDelete);


	return 0;
}

unsigned int __stdcall InsertList(LPVOID lpParam)
{
	DWORD threadID = GetCurrentThreadId();
	srand(threadID);
	int num = 0;
	DWORD result;
	while (!g_bShutDown)
	{
		result = WaitForSingleObject(EventHandle[2], 1000);

		if (result == WAIT_TIMEOUT)
		{
			num = rand() % 1000;
			Insert(num);
		}
	}

	return 0;
}

unsigned int __stdcall DeleteList(LPVOID lpParam)
{
	DWORD result;

	while (!g_bShutDown)
	{
		result = WaitForSingleObject(EventHandle[2], 333);
		if (result == WAIT_TIMEOUT)
		{
			Delete();
		}
	}

	return 0;
}

unsigned int __stdcall SaveList(LPVOID lpParam)
{
	DWORD result;

	while (!g_bShutDown)
	{
		result = WaitForMultipleObjects(2,EventHandle, FALSE, INFINITE);
		if (result == WAIT_OBJECT_0)
		{
			EnterCriticalSection(&gCriticalSectionInsert);
			pFile = fopen("ListSave.txt", "w+");
			if (pFile == nullptr)
			{
				wprintf(L" file open failed\n");
				return 0;
			}
			for (std::list<int>::iterator itor = g_List.begin(); itor != g_List.end(); itor++)
			{
				fprintf(pFile, "%d ", *itor);
			}
			fclose(pFile);

			LeaveCriticalSection(&gCriticalSectionInsert);
		}
		else if (result == WAIT_OBJECT_0 + 1)
			break;
	}

	return 0;
}

unsigned int __stdcall PrintList(LPVOID lpParam)
{
	DWORD result;
	while (!g_bShutDown)
	{
		result = WaitForSingleObject(EventHandle[2], 1000);

		if (result == WAIT_TIMEOUT)
		{
			PrintList();
		}
	}

	return 0;
}

void Insert(int num)
{
	EnterCriticalSection(&gCriticalSectionInsert);
	g_List.push_back(num);
	LeaveCriticalSection(&gCriticalSectionInsert);
}

void Delete()
{
	EnterCriticalSection(&gCriticalSectionInsert);
	if(g_List.empty() == false)
		g_List.pop_back();
	LeaveCriticalSection(&gCriticalSectionInsert);
}

void PrintInsert(int ID, int num)
{
	wprintf(L"ThreadID : %d, Inserted : %d\n", ID, num);
}

void PrintList()
{
	EnterCriticalSection(&gCriticalSectionInsert);
	if (g_List.empty() == false)
	{
		for (std::list<int>::iterator itor = g_List.begin(); itor != g_List.end(); itor++)
		{
			wprintf(L"%d ", *itor);
		}
		wprintf(L"\n");
	}
	LeaveCriticalSection(&gCriticalSectionInsert);
}
