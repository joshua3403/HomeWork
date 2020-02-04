#pragma once

#include "stdafx.h"

#define MAX_FILENAME_LENGTH 255
#define MAX_TRACE_ALLOC_COUNT 1024*10

typedef struct st_Alloc_Info
{
	void* pPtr;								// 할당된 메모리의 시작 주소값
	int iSize;								// 할당된 메모리의 크기
	bool bIsArray;							// 배열 형태로 할당 되었는지
	char szFile[MAX_FILENAME_LENGTH];		// 파일 이름 저장
	int iLine;								// main 함수에서 호출한 위치
} Alloc_Info;

namespace MemoryControl {
	class MemoryAllocator
	{
	private:
		int iTotal_Alloc_Size;	
		size_t alloc_count;
		size_t alloc_failed_count;
		size_t free_count;
		size_t free_failed_count;
		int current_alloc_index;
		int current_free_index;
		int current_count;

		FILE* file;

		Alloc_Info alloc_array[MAX_TRACE_ALLOC_COUNT];

	public:
		MemoryAllocator() : iTotal_Alloc_Size(0), alloc_count(0), alloc_failed_count(0), free_count(0), free_failed_count(0),
			current_alloc_index(0), current_free_index(0), current_count(0)
		{
			time_t now = time(NULL);
			struct tm date;
			setlocale(LC_ALL, "Korean");
			localtime_s(&date, &now);
			char date_string[128] = { '\0', };

			strftime(date_string, sizeof(date_string), "Alloc_%Y%m%d_%H%M%S.txt", &date);

			file = fopen(date_string, "wb");
		}

		~MemoryAllocator()
		{
			fclose(file);
		}

	public:
		void PrintAllocFail(void* ptr)
		{
			fprintf_s(file, "MallocFail [0x%08X]", ptr);
		}

		void PrintTraceCountShortage(void* ptr)
		{
			fprintf_s(file, "TraceCountShortage [0x%08X]", ptr);
		}

		void* Alloc(size_t size, const char* fileName, int line)
		{
			void* ptr = malloc(size);

			if (ptr == nullptr)
			{
				PrintAllocFail(ptr);
				++alloc_failed_count;
				return nullptr;
			}

			int i;
			for (int i = 0; i < MAX_TRACE_ALLOC_COUNT; ++i)
			{
				int index = (i + current_alloc_index) % MAX_TRACE_ALLOC_COUNT;
				if (nullptr == alloc_array[index].pPtr)
				{
					alloc_array[index].iSize = size;
					alloc_array[index].pPtr = ptr;
					alloc_array[index].bIsArray = false;
					alloc_array[index].iLine = line;
					strcpy_s(alloc_array[index].szFile,sizeof(alloc_array[index].szFile), fileName);

					current_alloc_index = (i + current_alloc_index) % MAX_TRACE_ALLOC_COUNT;
					iTotal_Alloc_Size += size;
					++alloc_count;
					++current_count;
					break;
				}
			}

			if (MAX_TRACE_ALLOC_COUNT == i)
			{
				PrintTraceCountShortage(ptr);
			}

			return ptr;
		}
	};
}