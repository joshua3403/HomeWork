#pragma once

#include "stdafx.h"

namespace Joshua {
	class Node
	{
	private:
		TCHAR			szName[64];			// �������� ���� �̸�.

		LARGE_INTEGER	lStartTime;			// �������� ���� ���� �ð�.

		__int64			iTotalTime;			// ��ü ���ð� ī���� Time.	(��½� ȣ��ȸ���� ������ ��� ����)
		__int64			iMin[2];			// �ּ� ���ð� ī���� Time.	(�ʴ����� ����Ͽ� ���� / [0] �����ּ� [1] ���� �ּ� [2])
		__int64			iMax[2];			// �ִ� ���ð� ī���� Time.	(�ʴ����� ����Ͽ� ���� / [0] �����ִ� [1] ���� �ִ� [2])

		__int64			iCall;				// ���� ȣ�� Ƚ��.


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
			// ����ũ�μ����� ����
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

			// �ʴ���
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
					fwprintf(file, L"%-10s | %.4f �� | %.4f �� | %.4f �� | %d \n", nowNode->szName, ((double)nowNode->iTotalTime / (double)(nowNode->iCall - 4.0)) , (double)(nowNode->iMin[0] ), (double)(nowNode->iMax[1] ), nowNode->iCall);
				}
				else
				{
					fwprintf(file, L"%-10s | %.4f �� | %.4f�� | %.4f�� | %d \n", nowNode->szName, ((double)nowNode->iTotalTime / (double)(nowNode->iCall)), (double)(nowNode->iMin[0] ), (double)(nowNode->iMax[1] ), nowNode->iCall);
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

