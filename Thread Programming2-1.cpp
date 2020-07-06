#include "stdafx.h"


std::list<int> g_List;

HANDLE threadHandle[6];
HANDLE Handle[5];
DWORD threadID[6];
BOOL g_bShutDown = false;
FILE* pFile;

// 작업 함수
void Insert(int num);
void Delete();
void PrintInsert(int ID, int num);
void Print();
void Save();

// 동기화 객체
CRITICAL_SECTION gCriticalSectionInsert;
CRITICAL_SECTION gCriticalSectionDelete;

// 완료루틴
VOID CALLBACK InsertList(LPVOID, DWORD, DWORD);
VOID CALLBACK DeleteList(LPVOID, DWORD, DWORD);
VOID CALLBACK PrintList(LPVOID, DWORD, DWORD);

int main()
{
	srand((unsigned int)(time(NULL)));

	LARGE_INTEGER secondTimer;

	LARGE_INTEGER milTimer;

	secondTimer.QuadPart = -1000000;
	milTimer.QuadPart = -3330000;
	//InitializeCriticalSection(&gCriticalSectionInsert);
	//InitializeCriticalSection(&gCriticalSectionDelete);
	Handle[0] = CreateWaitableTimer(NULL, FALSE, _T("FirstTimer"));
	Handle[1] = CreateWaitableTimer(NULL, FALSE, _T("SecondTimer"));
	Handle[2] = CreateWaitableTimer(NULL, FALSE, _T("ThirdTimer"));
	Handle[3] = CreateWaitableTimer(NULL, FALSE, _T("MilTimer"));
	Handle[4] = CreateWaitableTimer(NULL, FALSE, _T("PrintTimer"));

	SetWaitableTimer(Handle[0], &secondTimer, 1000, InsertList, NULL, FALSE);
	SetWaitableTimer(Handle[1], &secondTimer, 1000, InsertList, NULL, FALSE);
	SetWaitableTimer(Handle[2], &secondTimer, 1000, InsertList, NULL, FALSE);
	SetWaitableTimer(Handle[3], &milTimer, 450, DeleteList, NULL, FALSE);
	SetWaitableTimer(Handle[4], &secondTimer, 1000, PrintList, NULL, FALSE);

	while (true)
	{
		if (GetAsyncKeyState(0x51) & 0x0001)
		{
			//wprintf(L"Q clicked\n");
			Save();
		}
		SleepEx(INFINITE, TRUE);

	}

	for (int i = 0; i < 5; i++)
	{
		CloseHandle(Handle[i]);
	}

	//DeleteCriticalSection(&gCriticalSectionInsert);
	//DeleteCriticalSection(&gCriticalSectionDelete);
	return 0;
}

void Insert(int num)
{
	//EnterCriticalSection(&gCriticalSectionInsert);
	//wprintf(L"Insert\n");
	g_List.push_back(num);
	//LeaveCriticalSection(&gCriticalSectionInsert);
}

void Delete()
{
	//EnterCriticalSection(&gCriticalSectionInsert);
	if (g_List.empty() == false)
	{
		//wprintf(L"Delete\n");
		g_List.pop_back();
	}
	//LeaveCriticalSection(&gCriticalSectionInsert);
}

void PrintInsert(int ID, int num)
{
	wprintf(L"ThreadID : %d, Inserted : %d\n", ID, num);
}

void Print()
{
	//EnterCriticalSection(&gCriticalSectionInsert);
	if (g_List.empty() == false)
	{
		for (std::list<int>::iterator itor = g_List.begin(); itor != g_List.end(); itor++)
		{
			wprintf(L"%d ", *itor);
		}
		wprintf(L"\n");
	}
	//LeaveCriticalSection(&gCriticalSectionInsert);
}

void Save()
{
	pFile = fopen("ListSave.txt", "w+");
	if (pFile == nullptr)
	{
		wprintf(L" file open failed\n");
		return;
	}
	for (std::list<int>::iterator itor = g_List.begin(); itor != g_List.end(); itor++)
	{
		fprintf(pFile, "%d ", *itor);
	}
	fclose(pFile);
}

VOID InsertList(LPVOID lpArg, DWORD, DWORD)
{
	int num = rand() % 1000;
	Insert(num);
}

VOID DeleteList(LPVOID, DWORD, DWORD)
{
	Delete();
}

VOID PrintList(LPVOID, DWORD, DWORD)
{
	Print();
}
