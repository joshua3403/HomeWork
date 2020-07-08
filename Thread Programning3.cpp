#include "stdafx.h"
#include "CRingBuffer.h"

#define dfTYPE_ADD_STR 0
#define dfTYPE_DEL_STR 1
#define dfTYPE_PRINT_LIST 2
#define dfTYPE_QUIT 3

#define sPRACTICE_STRING L"PROCADEMY"

// msgQ에서 다 뽑아내는 구조로 만들자

struct st_MSG_HEAD
{
	short shType;
	short shStrLen;
};

//-----------------------------------------------
// 컨텐츠 부, 문자열 리스트
//-----------------------------------------------
std::list<std::wstring> g_List;

//-----------------------------------------------
// 스레드 메시지 큐 (사이즈 넉넉하게 크게 4~5만 바이트)
//-----------------------------------------------
CRingBuffer g_msgQ;

//-----------------------------------------------
// 메인 루프용 변수
//-----------------------------------------------
DWORD dwTick = 0;
DWORD iCount = 0;
DWORD iFrame = 0;
DWORD next_game_tick = timeGetTime();
const int SKIP_TICKS = 1000 / 50;
DWORD _AccumulTime = 0;
int _Balance = 0;

BOOL g_bQuit = false;

SRWLOCK hListLock;

HANDLE threadHandle[3];
DWORD threadID[3];

HANDLE EventHandle[2];

int g_iInsertCount = 0;
int g_iDeleteCount = 0;
int g_iPrintCount = 0;


int main();

//-----------------------------------------------
// 메인 루프용 함수
//-----------------------------------------------
// 메세지 생성 함수
void CreateMessage();
void CreateQuitMessage();

//-----------------------------------------------
// 스레드 루프용 함수
//-----------------------------------------------
unsigned int WINAPI ThreadProc(LPVOID lpParam);

// 메시지 프로시져
void Insert(int len);
void Delete();
void PrintList();


int main()
{
	timeBeginPeriod(1);

	srand((unsigned int)time(NULL));

	InitializeSRWLock(&hListLock);

	EventHandle[0] = CreateEvent(NULL, FALSE, FALSE, NULL);

	threadHandle[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, (unsigned*)&threadID[0]);
	threadHandle[1] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, (unsigned*)&threadID[1]);
	threadHandle[2] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, (unsigned*)&threadID[2]);
	//threadHandle[3] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, (unsigned*)&threadID[3]);
	//threadHandle[4] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, (unsigned*)&threadID[4]);
	//threadHandle[5] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, (unsigned*)&threadID[5]);
	//threadHandle[6] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, (unsigned*)&threadID[6]);
	//threadHandle[7] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, (unsigned*)&threadID[7]);


	while (true)
	{
		DWORD dwCurTick = timeGetTime();

		if (GetAsyncKeyState(VK_SPACE) & 0x8000)
		{
			CreateQuitMessage();
			break;
		}
		if (dwTick + 1000 < timeGetTime())
		{
			wprintf(L"Thread Programming FPS : %d, g_List FreeSize : %d, UsingSize : %d, InsertCount : %d, DeleteCount : %d, PrintCount : %d\n", iFrame, g_msgQ.GetFreeSize(), g_msgQ.GetUsingSize(), g_iInsertCount, g_iDeleteCount, g_iPrintCount);
			iCount = 0;
			dwTick = timeGetTime();
			iFrame = 0;
			g_iInsertCount = 0;
			g_iDeleteCount = 0;
			g_iPrintCount = 0;
		}

		// 이전 프레임의 시간에서 현재 시간을 빼자.
		_AccumulTime = (dwCurTick - next_game_tick);
		_Balance = SKIP_TICKS - _AccumulTime;
		//wprintf(L"_Balance : %d\n", _Balance);

		if (_AccumulTime < 20)
		{
			continue;
		}
		else
		{
			next_game_tick = dwCurTick + _Balance;
			iFrame++;
			_Balance = 0;
		}

		CreateMessage();
	}

	WaitForMultipleObjects(3, threadHandle, TRUE, INFINITE);

	wprintf(L"All thread closed, g_msgQ usingsize : %d\n", g_msgQ.GetUsingSize());
	for (int i = 0; i < 3; i++)
	{
		CloseHandle(threadHandle[i]);
	}

	for (int i = 0; i < 1; i++)
	{
		CloseHandle(EventHandle[i]);
	}

	return 0;
}

void CreateMessage()
{
	short type = rand() % 1000;
	if (0 <= type && type <= 500)
	{
		type = dfTYPE_ADD_STR;
		g_iInsertCount++;
	}
	else if (type > 500 && type <= 800)
	{
		type = dfTYPE_DEL_STR;
		g_iDeleteCount++;
	}
	else if (type > 800 && type < 1000)
	{
		type = dfTYPE_PRINT_LIST;
		g_iPrintCount++;
	}

	short len = rand() % 8 + 1;

	st_MSG_HEAD newMessage;
	newMessage.shType = type;
	newMessage.shStrLen = len;

	g_msgQ.SrwExcluciveLock();
	if(g_msgQ.GetFreeSize() >= sizeof(newMessage))
		g_msgQ.Enqueue((char*)&newMessage, sizeof(newMessage));

	//wprintf(L"g_msgQ usingsize : %d\n", g_msgQ.GetUsingSize());

	g_msgQ.SrwExcluciveRelease();
	SetEvent(EventHandle[0]);
}

void CreateQuitMessage()
{
	st_MSG_HEAD Message;
	Message.shType = dfTYPE_QUIT;
	g_msgQ.SrwExcluciveLock();
	for(int i = 0 ; i < 3; i++)
		g_msgQ.Enqueue((char*)&Message, sizeof(Message));
	g_msgQ.SrwExcluciveRelease();
	SetEvent(EventHandle[0]);
}

unsigned int __stdcall ThreadProc(LPVOID lpParam)
{
	wprintf(L"Thread Started, ID : %d\n", GetCurrentThreadId());

	DWORD result = 0;
	BOOL Flag = FALSE;
	BOOL Flag2= FALSE;
	st_MSG_HEAD message;

	while (!Flag)
	{
		result = WaitForSingleObject(EventHandle[0], INFINITE);
		if (result == WAIT_OBJECT_0)
		{
			//wprintf(L"Thread Started, ID : %d\n", GetCurrentThreadId());

			while (!Flag2)
			{

				if (g_msgQ.GetUsingSize() < sizeof(st_MSG_HEAD))
					break;
				g_msgQ.SrwExcluciveLock();

				g_msgQ.Dequeue((char*)&message, sizeof(st_MSG_HEAD));

				g_msgQ.SrwExcluciveRelease();


				switch (message.shType)
				{
				case dfTYPE_ADD_STR:
					Insert(message.shStrLen);
					break;
				case dfTYPE_DEL_STR:
					Delete();
					break;
				case dfTYPE_PRINT_LIST:
					PrintList();
					break;

				case dfTYPE_QUIT:
					Flag = TRUE;
					Flag2 = TRUE;
					SetEvent(EventHandle[0]);
					break;
				default:
					break;
				}
			}
			//wprintf(L"Thread Stopped, ID : %d\n", GetCurrentThreadId());
		}	

	}
	return 0;
}


void Insert(int len)
{
	std::wstring newString;
	newString.assign(sPRACTICE_STRING, 0, len);
	AcquireSRWLockExclusive(&hListLock);
	g_List.push_back(newString);
	ReleaseSRWLockExclusive(&hListLock);
}

void Delete()
{	
	AcquireSRWLockExclusive(&hListLock);
	if(g_List.empty() == false)
		g_List.pop_back();
	ReleaseSRWLockExclusive(&hListLock);
}

void PrintList()
{
	AcquireSRWLockShared(&hListLock);
	wprintf(L"List : ");
	for (std::list<std::wstring>::iterator itor = g_List.begin(); itor != g_List.end(); itor++)
	{
		std::wcout << L"[" <<  (*itor) << L"] ";
	}
	//wprintf(L"g_msgQ usingSize : %d", g_msgQ.GetUsingSize());
	wprintf(L"\n");
	ReleaseSRWLockShared(&hListLock);
}