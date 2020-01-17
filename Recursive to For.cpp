#include <stdio.h>
#include <Windows.h>
#define SCREEN_WIDTH 20
#define SCREEN_HEIGHT 10
#define STACK_SIZE SCREEN_HEIGHT * SCREEN_WIDTH

char Original[SCREEN_HEIGHT][SCREEN_WIDTH] = {
{ ' ', ' ', ' ', ' ', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ' },
{ ' ', ' ', '0', '0', '0', ' ', ' ', ' ', ' ', '0', '0', '0', ' ', ' ', ' ', ' ', ' ', '0', '0', ' ' },
{ ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', ' ' },
{ ' ', '0', '0', '0', ' ', '0', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ' },
{ ' ', '0', '0', '0', ' ', ' ', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ' },
{ ' ', '0', '0', '0', ' ', ' ', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ' },
{ ' ', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ' },
{ ' ', '0', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ', ' ', ' ' },
{ ' ', ' ', '0', '0', '0', ' ', ' ', '0', '0', '0', '0', '0', '0', '0', '0', ' ', ' ', ' ', ' ', ' ' },
{ ' ', ' ', ' ', ' ', ' ', ' ', ' ', '0', '0', '0', '0', '0', '0', ' ', ' ', ' ', ' ', ' ', ' ', ' ' }
};

char Copy[SCREEN_HEIGHT][SCREEN_WIDTH] = { 0 };

int pushCount = 0;
int popCount = 0;
int maxTop = -1;

struct Node
{
	int x;
	int y;
};

struct Stack
{
	int top;
	struct Node arrNode[STACK_SIZE];
};

void StackIni(Stack* stack)
{
	stack->top = -1;
}

bool StackIsEmpty(Stack* stack)
{
	if (stack->top < 0)
		return true;
	else
		return false;
}

bool StackIsFull(Stack* stack)
{
	if (stack->top >= STACK_SIZE)
		return true;
	else
		return false;
}

void StackPush(Stack* stack, Node* node)
{
	if (StackIsFull(stack))
		return;
	else
	{
		Node temp;
		temp.x = node->x;
		temp.y = node->y;
		stack->top++;
		stack->arrNode[stack->top] = temp;
		if (stack->top > maxTop)
			maxTop = stack->top;
		pushCount++;
		//printf("%d", stack->top);
	}
}

void StackPop(Stack* stack, Node* node)
{
	if (StackIsEmpty(stack))
		return;
	else
	{
		node->x = stack->arrNode[stack->top].x;
		node->y = stack->arrNode[stack->top].y;
		stack->top--;
		popCount--;
	}
}

// char형 배열을 받아 프린트 하는 함수
void print(char(*Copy)[SCREEN_WIDTH])
{
	for (int i = 0; i < SCREEN_HEIGHT; ++i)
	{
		for (int j = 0; j < SCREEN_WIDTH; ++j)
		{
			printf("%c", Copy[i][j]);
		}
		printf("\n");
	}
}

void Solution(Stack* stack, char cmp)
{
	while (!(StackIsEmpty(stack)))
	{
		Node copyTemp;
		Node temp;
		StackPop(stack, &copyTemp);
		if (Copy[copyTemp.y][copyTemp.x] == cmp)
		{
			system("cls");
			Copy[copyTemp.y][copyTemp.x] = '.';
			print(Copy);
			Sleep(30);
			if (copyTemp.x - 1 >= 0)
			{
				temp.x = copyTemp.x - 1;
				temp.y = copyTemp.y;
				StackPush(stack,&temp);
			}
			if (copyTemp.x + 1 < SCREEN_WIDTH)
			{
				temp.x = copyTemp.x + 1;
				temp.y = copyTemp.y;
				StackPush(stack, &temp);
			}
			if (copyTemp.y - 1 >= 0)
			{
				temp.x = copyTemp.x;
				temp.y = copyTemp.y - 1;
				StackPush(stack, &temp);
			}
			if (copyTemp.y + 1 < SCREEN_HEIGHT)
			{
				temp.x = copyTemp.x;
				temp.y = copyTemp.y + 1;
				StackPush(stack, &temp);
			}
		}
	}
	printf("*******완성*******\n");
	Sleep(2000);
}



int main()
{
	Stack temp_stack;
	StackIni(&temp_stack);
	while (true)
	{
		system("cls");
		fflush(stdin);
		int x = 0;
		int y = 0;

		memcpy(Copy, Original, sizeof(Original));

		print(Copy);

		printf("원하는 x값의 좌표를 입력하세요(1 ~ 20) : ");
		scanf_s("%d", &x);
		if (x < 1 || x > SCREEN_WIDTH)
		{
			printf("값의 범위를 벗어났습니다.\n");
			Sleep(1000);
			continue;
		}
		printf("원하는 y값의 좌표를 입력하세요(0 ~ 10) : ");
		scanf_s("%d", &y);

		if (y < 1 || y > SCREEN_HEIGHT)
		{
			printf("값의 범위를 벗어났습니다.\n");
			Sleep(1000);
			continue;
		}

		Node temp_Node;
		temp_Node.x = --x;
		temp_Node.y = --y;

		StackPush(&temp_stack, &temp_Node);
		Solution(&temp_stack, Copy[temp_Node.y][temp_Node.x]);

		printf("push : %d\t pop : %d\t maxTop : %d\n", pushCount, popCount, maxTop);

		char answer;
		printf("계속 하시겠습니까? Y / N : ");
		scanf_s(" %c", &answer);
		if (answer == 'Y' || answer == 'y')
			continue;
		else
			break;
	}

	return 0;
}