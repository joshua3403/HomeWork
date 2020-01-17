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

// 입력받은 시간을 배열에 차례대로 저장
void GetInputTime(double* array, int *index)
{
	array[*index] = clock() / 1000 + (clock() % 1000) * 0.001;
}

// 입력받은 시간과 기준시간을 비교해서 채점
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
		// 퍼펙트
		if(abs(g_Timing[*index] - inputArray[*index]) <= check_time)
			result[*index] = 5;
		// 그레잇
		else if(abs(g_Timing[*index] - inputArray[*index])> check_time && abs(g_Timing[*index] - inputArray[*index]) <= check_time *2)
			result[*index] = 4;
		// 굿
		else if(abs(g_Timing[*index] - inputArray[*index]) > check_time * 2 && abs(g_Timing[*index] - inputArray[*index]) <= check_time * 3)
			result[*index] = 3;
		// 낫 배드
		else if(abs(g_Timing[*index] - inputArray[*index]) > check_time * 3 && abs(g_Timing[*index] - inputArray[*index]) <= check_time * 4)
			result[*index] = 2;
		// 배드
		else if (abs(g_Timing[*index] - inputArray[*index]) > check_time * 4 && abs(g_Timing[*index] - inputArray[*index]) <= check_time * 5)
			result[*index] = 1;
		// 페일
		else
			result[*index] = 0;
	}
}

// 입력에 따른 실시간 점수 출력
void PrintScore(int *result)
{
	for (int i = 0; i < 9; ++i)
	{
		cout << i + 1 << ". " << setfill('0') << setw(2) << g_Timing[i] << " : ";
		cout << score_String[result[i]] << endl;
	}
}

// 결과 출력
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

	cout << endl << "Perfect의 횟수 : " << array[5] << ", " << "Greate의 횟수 : " << array[4] << ", " << "Good의 횟수 : " << array[3] << ", " << "Not Bad의 횟수 : " << array[2] << ", " << "Bad의 횟수 : " << array[1] << ", " << "Fail의 횟수 : " << array[0] << endl;
	
	cout << endl << "총점 : " << score << "\t" << "정확도 : " << accuracy << "%" << endl;
}

int main()
{
	// 키보드를 누른 시간을 저장할 배열
	double i_Timing[9] = { 0 };
	// i_Timing과 g_Timing을 비교하여 결과 확인 6(null), 5(Perfect)~0(Fail)
	int result_index[9] = { 6, 6, 6, 6, 6, 6, 6, 6, 6 };

	// 비교할 배열의 인덱스
	int index = 0;

	timeBeginPeriod(1);

	while (true)
	{
		system("cls");
		if (index >= 9)
			break;
		cout << setfill('0') << setw(3) << clock() / 1000;
		cout << "." << clock() % 1000 << endl;

		// 현재 시간과 기준 시간을 뺀 시간 
		double check_time = clock() * 0.001 - g_Timing[index];


		// 해당 기준 시간에서 1초 넘어가면 Fail로 지정후 넘어감
		if (check_time > 1)
		{
			result_index[index] = 0;
			index++;
			continue;
		}
		else
		{
			// 키보드가 눌리면 입력 시간과 점수를 계산
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