#include <iostream>
#include <time.h>
#include <Windows.h>

#define COMPLETE_TIME 10000

using namespace std;

typedef struct Queue
{
	int* buff;
	int front;
	int rear;
	int q_Size;
	int count;
};

void CreateQueue(Queue *q, int size)
{
	q->buff = (int *)malloc(sizeof(int) * size);
	q->q_Size = size;
	q->front = 0;
	q->rear = 0;
	q->count = 0;
}

bool IsFull(Queue* q)
{
	if (q->front == (q->rear+1)%q->q_Size)
		return true;
	else
		return false;
}

bool IsEmpty(Queue* q)
{
	if (q->count == 0)
		return true;
	else
		return false;
}

bool EnQueue(Queue* q, int value)
{
	if (IsFull(q))
	{
		cout << "생산 명령이 최대치 입니다." << endl;
		return false;
	}
	else
	{
		q->rear = (q->rear + 1) % q->q_Size;
		q->buff[q->rear] = value;
		q->count++;
		cout << "생성이 입력되었습니다." << endl;
		return true;
	}
}

bool DeQueue(Queue *q, int* val)
{
	if (IsEmpty(q))
		return false;
	else
	{
		//*val = q->buff[q->front + 1];
		q->front = (q->front + 1) % q->q_Size;
		q->count--;
		cout << "생성이 완료되었습니다" << endl;
		return true;
	}
}

void PrintInstuction(Queue* q)
{
	cout << "[ 1. 유닛 생산\t 2. 종료 ]" << endl;
	cout << "------------------------------------" << endl;
	cout << "###########################################" << endl;
	cout << "#  ";
	if (!IsEmpty(q))
	{
		int f = q->front;
		while (f != q->rear)
		{
			f = (f + 1) % q->q_Size;
			if ((clock() - q->buff[f]) * 100 / COMPLETE_TIME >= 100)
			{
				cout << "100%\t";
			}
			else
				cout << (clock() - q->buff[f]) * 100 / COMPLETE_TIME << "%" << "\t";
		}
	}

	cout <<"  #";
	cout << endl << "###########################################" << endl;
}

int main()
{
	Queue* q = new Queue;
	CreateQueue(q, 6);

	while (true)
	{
		system("cls");
		time_t now_Time = clock();
		PrintInstuction(q);
		if (GetAsyncKeyState(0x31) & 0x0001 )
		{
			EnQueue(q, clock());
		}
		if (!IsEmpty(q))
		{
			if ((now_Time - q->buff[(q->front + 1) % q->q_Size]) >= COMPLETE_TIME)
			{
				int *temp = 0;
				DeQueue(q, temp);
				Sleep(50);
				continue;
			}
		}

		if (GetAsyncKeyState(0x32) & 0x8000)
		{
			free(q->buff);
			exit(0);
		}
		//system("cls");
	}
}