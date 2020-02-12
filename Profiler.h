#pragma once

#include "stdafx.h"

namespace Joshua {
	class Node
	{
	private:
		TCHAR			szName[64];			// 프로파일 샘플 이름.

		LARGE_INTEGER	lStartTime;			// 프로파일 샘플 실행 시간.

		__int64			iTotalTime;			// 전체 사용시간 카운터 Time.	(출력시 호출회수로 나누어 평균 구함)
		__int64			iMin[2];			// 최소 사용시간 카운터 Time.	(초단위로 계산하여 저장 / [0] 가장최소 [1] 다음 최소 [2])
		__int64			iMax[2];			// 최대 사용시간 카운터 Time.	(초단위로 계산하여 저장 / [0] 가장최대 [1] 다음 최대 [2])

		__int64			iCall;				// 누적 호출 횟수.


		Node* Next;
		friend class List;

	public:
		Node(const TCHAR* workName)
		{
			StringCchCopyW(szName, sizeof(szName), workName);
			iTotalTime = 0;
			iCall = 0;

			Next = nullptr;
			for (int i = 0; i < 2; ++i)
			{
				iMin[i] = LLONG_MAX;
				iMax[i] = 0;
			}


		}

		void GetProcessTime(double time)
		{
			// 마이크로세컨드 단위
			time *= 1000000.0;

				if (time < iMin[0])
				{
					__int64 temp = iMin[0];
					iMin[0] =  time;
					iMin[1] = temp;
				}
				else if (time > iMin[0] && time < iMin[1])
				{
					iMin[1] = time;
				}


				if (time > iMax[1])
				{
					__int64 temp = iMax[1];
					iMax[1] = time;
					iMax[0] = temp;
				}
				else if (time < iMax[1] && time > iMax[0])
				{
					iMax[0] = time;
				}

				iTotalTime += time;
			iCall++;
		}
	};

	class List
	{
	private:
		Node* head;
		Node* tail;
		int iListSize;

		FILE* file;

	public:
		List()
		{
			head = tail = nullptr;
			iListSize = 0;
		}

		bool IsEmpty()
		{
			if (iListSize == 0)
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		bool PushBack(const TCHAR* workName)
		{
			Node* newNode = new Node(workName);
			LARGE_INTEGER startTime;


			if (newNode == nullptr)
				return false;

			if (IsEmpty())
			{
				head = newNode;
			}
			else
			{
				tail->Next = newNode;
			}

			tail = newNode;
			iListSize++;

			QueryPerformanceCounter(&startTime);

			newNode->lStartTime = startTime;
			return true;
		}

		bool PutNewData(const TCHAR* workName)
		{
			LARGE_INTEGER End;
			QueryPerformanceCounter(&End);
			LARGE_INTEGER Freq;
			QueryPerformanceFrequency(&Freq);
			Node* nowNode = head;

			while (nowNode != nullptr)
			{
				if (_tcscmp(nowNode->szName, workName) == 0)
					break;
				else
				{
					nowNode = nowNode->Next;
				}
			}

			if (nowNode == nullptr)
			{
				throw 1;
			}

			// 초단위
			double processTime = (double)(End.QuadPart - nowNode->lStartTime.QuadPart) / (double)(Freq.QuadPart);
			
				
			nowNode->GetProcessTime(processTime);

			return true;
		}

		void ProfileDataOutText()
		{
			time_t now = time(NULL);
			struct tm date;
			setlocale(LC_ALL, "Korean");
			localtime_s(&date, &now);
			char fileName[32];

			strftime(fileName, sizeof(fileName), "Profiler_%Y%m%d_%H%M%S.txt", &date);
			file = fopen(fileName, "ab");
			fprintf_s(file, "-----------------------------------------------------------------------------------------------------\n\n");
			fprintf_s(file, "                Name |                Average |                Min |                Max |              Call |\n");
			fprintf_s(file, "-----------------------------------------------------------------------------------------------------\n\n");

			Node* nowNode = head;
			while (nowNode != nullptr)
			{
				if (nowNode->iCall > 4)
				{
					fwprintf(file, L"%-10s | %.4f ㎲ | %.4f ㎲ | %.4f ㎲ | %d \n", nowNode->szName, ((double)nowNode->iTotalTime / (double)(nowNode->iCall - 4.0)) , (double)(nowNode->iMin[0] ), (double)(nowNode->iMax[1] ), nowNode->iCall);
				}
				else
				{
					fwprintf(file, L"%-10s | %.4f ㎲ | %.4f㎲ | %.4f㎲ | %d \n", nowNode->szName, ((double)nowNode->iTotalTime / (double)(nowNode->iCall)), (double)(nowNode->iMin[0] ), (double)(nowNode->iMax[1] ), nowNode->iCall);
				}


				nowNode = nowNode->Next;
			}

			fprintf_s(file, "-----------------------------------------------------------------------------------------------------\n\n");
			fclose(file);
		}

		~List()
		{
			ProfileDataOutText();
			Node* nowNode = head;

			while (nowNode != nullptr)
			{
				head = head->Next;
				delete nowNode;
				nowNode = head;
			}
		}

	};

	static List ProfilerList;

}

