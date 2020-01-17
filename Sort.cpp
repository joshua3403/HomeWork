#include <iostream>
#include <Windows.h>

using namespace std;

void PrintArray(int* array,int size,int target_index1, int target_index2)
{
	for (int i = 0; i < size; i++)
	{
		if (i == target_index1)
		{
			cout << "[" << array[i] << "]\t";
		}
		else if (i == target_index2)
		{
			cout << "[" << array[i] << "]\t";
		}
		else
		{
			cout << array[i] << "\t";
		}

	}
	cout << endl << endl;
	Sleep(1000);

}


// 간단한 구현에 비해 시간 복잡도가 n^2로 비효율 적인 정렬
void BubbleSort(int* array, int size)
{
	for (int i = 0; i < size; ++i)
	{
		for (int j = 0; j < size - (i+1); ++j)
		{
			if (array[j] > array[j + 1])
			{
				PrintArray(array, size, j, j + 1);

				int temp = array[j + 1];
				array[j + 1] = array[j];
				array[j] = temp;
			}
		}
	}
}

// 버블정렬에 비해 빠르지만 시간 복잡도는 여전히 n^2
void SelectionSort(int* array, int size)
{
	int min_index = 0;
	int temp = 0;
	for (int i = 0; i < size - 1; ++i)
	{
		min_index = i;

		for (int j = i + 1; j < size; ++j)
		{
			if (array[min_index] > array[j])
				min_index = j;
		}

		PrintArray(array, size, i, min_index);

		temp = array[i];
		array[i] = array[min_index];
		array[min_index] = temp;
	}
}

// 제일 효율적이라고 알려진 분할 합병 방식의 정렬, 최고 : nlog2n 최악 : n^2
void QuickSort(int* array, int left, int right)
{
	if (left >= right)
		return;
	int pivot = array[(left + right) / 2];
	int left_index = left;
	int right_index = right;

	while (left_index <= right_index)
	{
		while (array[left_index] < pivot) left_index++;
		while (array[right_index] > pivot) right_index--;
		if (left_index <= right_index)
		{
			PrintArray(array, 10, left_index, right_index);
			int temp = array[right_index];
			array[right_index] = array[left_index];
			array[left_index] = temp;

			left_index++;
			right_index--;
		}
	}
	QuickSort(array, left, right_index);
	QuickSort(array, left_index, right);

	PrintArray(array, 10, left_index, right_index);

}

int main()
{
	int temp[10] = { 45,467,2,5,7,9,325,5,3,1 };

	//BubbleSort(temp, 10);

	//SelectionSort(temp, 10);

	QuickSort(temp, 0, 9);

}