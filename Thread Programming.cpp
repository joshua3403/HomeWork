#include "stdafx.h"
#include <process.h>

int g_Data = 0;
int g_Connect = 0;
bool g_Shutdown = false;
DWORD dwTick = 0;
HANDLE threadHandle[5];
DWORD threadID[5];
// 스레드 함수
unsigned int WINAPI IncreaseConnect(LPVOID lpParam);
unsigned int WINAPI DecreaseConnect(LPVOID lpParam);
unsigned int WINAPI IncreaseData(LPVOID lpParam);

void PlusData(void);
void PrintData(void);
// 동기화 객체
//CRITICAL_SECTION hCriticalSection;
SRWLOCK hSrwLock;
int main()
{
	// 스레드 생성 전에 동기화 객체를 초기화 하자
	//InitializeCriticalSection(&hCriticalSection);

	InitializeSRWLock(&hSrwLock);
	timeBeginPeriod(1);
	DWORD starttime = timeGetTime();
	threadHandle[0] = (HANDLE)_beginthreadex(NULL, 0, IncreaseConnect, NULL, 0, (unsigned*)&threadID[0]);
	threadHandle[1] = (HANDLE)_beginthreadex(NULL, 0, DecreaseConnect, NULL, 0, (unsigned*)&threadID[1]);
	threadHandle[2] = (HANDLE)_beginthreadex(NULL, 0, IncreaseData, NULL, 0, (unsigned*)&threadID[2]);
	threadHandle[3] = (HANDLE)_beginthreadex(NULL, 0, IncreaseData, NULL, 0, (unsigned*)&threadID[3]);
	threadHandle[4] = (HANDLE)_beginthreadex(NULL, 0, IncreaseData, NULL, 0, (unsigned*)&threadID[4]);
	DWORD flag = timeGetTime();
	while (true)
	{
		if (dwTick + 1000 < timeGetTime())
		{
			wprintf(L"g_Connect : %d\n", g_Connect);
			dwTick = timeGetTime();
		}

		if (timeGetTime() - flag >= 20000)
		{
			g_Shutdown = true;
			break;
		}
	}

	WaitForMultipleObjects(5, threadHandle, TRUE, INFINITE);

	//DeleteCriticalSection(&hCriticalSection);

	for (int i = 0; i < 5; i++)
	{
		CloseHandle(threadHandle[i]);
	}
	return 0;
}

unsigned int __stdcall IncreaseConnect(LPVOID lpParam)
{
	srand(threadID[0]);
	int time = rand() % 901 + 100;
	while (!g_Shutdown)
	{
		Sleep(time);
		InterlockedIncrement((LONG*)&g_Connect);
		//wprintf(L"increase %d\n", g_Connect);
	}
	return 0;
}

unsigned int __stdcall DecreaseConnect(LPVOID lpParam)
{
	srand(threadID[1]);
	int time = rand() % 901 + 100;
	while (!g_Shutdown)
	{
		Sleep(time);
		InterlockedDecrement((LONG*)&g_Connect);
		//wprintf(L"decrease %d\n", g_Connect);
	}

	return 0;
}

unsigned int __stdcall IncreaseData(LPVOID lpParam)
{
	while (!g_Shutdown)
	{
		Sleep(10);
		PlusData();
		PrintData();
	}

	return 0;
}

void PlusData(void)
{
	//EnterCriticalSection(&hCriticalSection);
	AcquireSRWLockExclusive(&hSrwLock);
	g_Data++;
	ReleaseSRWLockExclusive(&hSrwLock);

	//LeaveCriticalSection(&hCriticalSection);

	// 이 방법으로 하면 동기화 객체를 사용 안해도 된다
	//InterlockedIncrement((LONG*)&g_Connect);
}

void PrintData(void)
{	
	AcquireSRWLockShared(&hSrwLock);
	if (g_Data % 1000 == 0)
		wprintf(L"g_Data : %d\n", g_Data);
	ReleaseSRWLockShared(&hSrwLock);
}
