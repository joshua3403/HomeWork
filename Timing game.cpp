#include <Windows.h>
#include <iostream>
#include <time.h>
#include <iomanip>
#include <conio.h>
#include <timeapi.h>
#include <stdio.h>

#pragma comment (lib, "Winmm.lib")
using namespace std;

int g_Timing[9] = { 5, 10, 14, 17, 20, 25, 29, 31, 33 };
string score_String[7] = { "Fail", "Bad", "Not Bad", "Good", "Great", "Perfect", "" };

// �Է¹��� �ð��� �迭�� ���ʴ�� ����
void GetInputTime(double* array, int *index)
{
	array[*index] = clock() / 1000 + (clock() % 1000) * 0.001;
}

// �Է¹��� �ð��� ���ؽð��� ���ؼ� ä��
void CheckPoint(int *index, double* inputArray, int* result)
{
	double now_time_chane = clock() * 0.001;
	double check_time = CLOCKS_PER_SEC / 6 * 0.001;
	if (g_Timing[*index] - inputArray[*index] > 1)
	{
		result[*index] = 0;
	}
	else
	{
		// ����Ʈ
		if(abs(g_Timing[*index] - inputArray[*index]) <= check_time)
			result[*index] = 5;
		// �׷���
		else if(abs(g_Timing[*index] - inputArray[*index])> check_time && abs(g_Timing[*index] - inputArray[*index]) <= check_time *2)
			result[*index] = 4;
		// ��
		else if(abs(g_Timing[*index] - inputArray[*index]) > check_time * 2 && abs(g_Timing[*index] - inputArray[*index]) <= check_time * 3)
			result[*index] = 3;
		// �� ���
		else if(abs(g_Timing[*index] - inputArray[*index]) > check_time * 3 && abs(g_Timing[*index] - inputArray[*index]) <= check_time * 4)
			result[*index] = 2;
		// ���
		else if (abs(g_Timing[*index] - inputArray[*index]) > check_time * 4 && abs(g_Timing[*index] - inputArray[*index]) <= check_time * 5)
			result[*index] = 1;
		// ����
		else
			result[*index] = 0;
	}
}

// �Է¿� ���� �ǽð� ���� ���
void PrintScore(int *result)
{
	for (int i = 0; i < 9; ++i)
	{
		cout << i + 1 << ". " << setfill('0') << setw(2) << g_Timing[i] << " : ";
		cout << score_String[result[i]] << endl;
	}
}

// ��� ���
void PrintResult(int* result)
{
	int array[6] = { 0 };
	int score = 0;
	double accuracy = 0;
	for (int i = 0; i < 9; ++i)
	{
		array[result[i]]++;
		score += result[i];
	}

	accuracy = (double)score * 100 / 45;

	cout << endl << "Perfect�� Ƚ�� : " << array[5] << ", " << "Greate�� Ƚ�� : " << array[4] << ", " << "Good�� Ƚ�� : " << array[3] << ", " << "Not Bad�� Ƚ�� : " << array[2] << ", " << "Bad�� Ƚ�� : " << array[1] << ", " << "Fail�� Ƚ�� : " << array[0] << endl;
	
	cout << endl << "���� : " << score << "\t" << "��Ȯ�� : " << accuracy << "%" << endl;
}

int main()
{
	// Ű���带 ���� �ð��� ������ �迭
	double i_Timing[9] = { 0 };
	// i_Timing�� g_Timing�� ���Ͽ� ��� Ȯ�� 6(null), 5(Perfect)~0(Fail)
	int result_index[9] = { 6, 6, 6, 6, 6, 6, 6, 6, 6 };

	// ���� �迭�� �ε���
	int index = 0;

	timeBeginPeriod(1);

	while (true)
	{
		system("cls");
		if (index >= 9)
			break;
		cout << setfill('0') << setw(3) << clock() / 1000;
		cout << "." << clock() % 1000 << endl;

		// ���� �ð��� ���� �ð��� �� �ð� 
		double check_time = clock() * 0.001 - g_Timing[index];


		// �ش� ���� �ð����� 1�� �Ѿ�� Fail�� ������ �Ѿ
		if (check_time > 1)
		{
			result_index[index] = 0;
			index++;
			continue;
		}
		else
		{
			// Ű���尡 ������ �Է� �ð��� ������ ���
			if (_kbhit())
			{	 				
				string temp = "";
				cin >> temp;
				cout << temp << endl;
				if (temp.size() <= 1)
					_getch();
				else if (temp.size() == 2)
				{
					_getch();
					_getch();
				}
				GetInputTime(i_Timing, &index);
				CheckPoint(&index, i_Timing, result_index);
				index++;
			}
		}

		PrintScore(result_index);
	}

	system("cls");

	PrintScore(result_index);
	PrintResult(result_index);

	timeEndPeriod(15);
}