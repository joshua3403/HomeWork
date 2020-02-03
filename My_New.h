#pragma once

#include "stdafx.h"

#define MAX_FILENAME_LENGTH 255
#define MAX_TRACE_ALLOC_COUNT 1024*10

struct st_Alloc_Info
{
	void* pPtr;								// �Ҵ�� �޸��� ���� �ּҰ�
	int iSize;								// �Ҵ�� �޸��� ũ��
	bool bIsArray;							// �迭 ���·� �Ҵ� �Ǿ�����
	char* szFile[MAX_FILENAME_LENGTH];		// ���� �̸� ����
	int iLine;								// main �Լ����� ȣ���� ��ġ
} AllocInfo;

namespace MemoryControl {
	class MemoryAllocator
	{
	private:
		int iTotal_Alloc_Size;	
		size_t alloc_count;
		size_t alloc_failed_count;
	};
}