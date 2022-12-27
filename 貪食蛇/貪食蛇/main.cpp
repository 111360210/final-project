#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <time.h> 
#include <conio.h>//�P�O���U������ 
#include <windows.h>//�]�m��Ц�m 
#define WIDE 60
#define HIGH 20
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

typedef struct _body//��D���y�� 
{
	int x;
	int y;

}BODY;

typedef struct snake//�����D���@�� 
{
	BODY list[WIDE*HIGH];//�x�s���骺�C�@�`�y�СA����̪�=�a�Ϥj�p 
	int size;//�D������
	BODY food;//�]�������]���y�СA�ҥH�]�Q��BODY���c�h�x�s 
	COORD coord;//��Ъ���m(COORD�O�bwindows.h�̩w�q�����c) 
	int dx;//�D�Vx�b���ʪ���V 
	int dy;//�D�Vy�b���ʪ���V
	int score;//�o��
	BODY body_last;//�������ڦ�m

}SNAKE;

void init_food(SNAKE *snake)
{
	srand(time(NULL));//�]�m�üƺؤl 
	//��l�ƭ����y�� 
	snake->food.x = rand() % WIDE;
	snake->food.y = rand() % HIGH;
}

void init_snake(SNAKE *snake)
{
	//��l�ƳD�Y��x�y�ЩMy�y��(�w�]�b�e������) 
	snake->list[0].x = WIDE / 2;
	snake->list[0].y = HIGH / 2;
	//��l�ƳD���y�� 
	snake->list[1].x = (WIDE / 2) - 1;
	snake->list[1].y = HIGH / 2;
	//��l�ƳD��������׬�2 
	snake->size = 2;
	//��l�ƭ������y�� 
	init_food(snake);
	//��l�ƳD�����ʤ�V
	snake->dx = 1;
	snake->dy = 0;
	//��l�Ƥ��� 
	snake->score = 0;

}


void show_ui(SNAKE *snake)//��ܳD�P�������禡 
{
	//��ܳD(��ܳD�έ����ɻݭn�]�m��Ъ���m�A�_�h��ܪ����e���|���b�{���Ĥ@��) 
	for (int i = 0; i < snake->size; i++)
	{
		snake->coord.X = snake->list[i].x;//�`�NX��Y�O�j�g 
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//�]�w��Ц�m(windows���ب��) 
		if (i == 0)
		{
			printf("@");
		}
		else
		{
			printf("*");//���|�Y���D�Y���@�Ӧr���A�b�����*�N�� 
		}

	}
	//��ܭ���
	snake->coord.X = snake->food.x;//�`�NX��Y�O�j�g 
	snake->coord.Y = snake->food.y;//�`�N�n��אּ������x,y�y�� 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//�]�w��Ц�m(windows���ب��) 
	printf("#");
	//���ʫ�N���ګe�@�Ӧ�m��ܦ��ť�
	snake->coord.X = snake->body_last.x;//�`�NX��Y�O�j�g 
	snake->coord.Y = snake->body_last.y;//�`�N�n��אּ������x,y�y�� 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//�]�w��Ц�m(windows���ب��) 
	printf(" ");

}
void move_snake(SNAKE *snake)
{	
	//�b���ڮy�Ч�s�e�O����y��
	snake->body_last.x = snake->list[snake->size - 1].x;
	snake->body_last.y = snake->list[snake->size - 1].y;

	//��s���F�D�Y�~���D���y�� (�e����N���Υt�]�Ȧs�ܼ�) 
	for (int i = snake->size - 1; i > 0; i--)
	{
		snake->list[i] = snake->list[i-1];//��D����e�@�`���y�Ы��w����@�` (list[0]�O�D�Y) 
	}
	//��s�D�Y 
	snake->list[0].x += snake->dx;//dx�w�]=1�A�ҥH�@�}�l�|���k�� 
	snake->list[0].y += snake->dy;

}

void control_snake(SNAKE *snake)
{
	char key = 0;
	while (_kbhit())//�P�_�O�_���U����A���U!=0 
	{
		key = _getch();//��������
	}
	switch (key)
	{
	case 'a':
		snake->dx = -1;//�D����V�V�� 
		snake->dy = 0;
		break;
	case 'w':
		snake->dx = 0;//�D����V�V�W
		snake->dy = -1;
		break;
	case 's':
		snake->dx = 0;//�D����V�V�U 
		snake->dy = 1;
		break;
	case 'd':
		snake->dx = 1;//�D����V�V�k 
		snake->dy = 0;
		break;
	}
}
void game_end(SNAKE *snake)
{
	//�o�̻ݦۦ�]�w�C�����ѮɰT���X�{����m�A�_�h��з|���b�W�@�Ӥ��G�X�{���a��
	snake->coord.X = 25;//�`�NX��Y�O�j�g 
	snake->coord.Y = 25;//�`�N�n��אּ������x,y�y�� 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//�]�w��Ц�m(windows���ب��)
	printf("�C������ �o����%d\n", snake->score);
	Sleep(5000);
	exit(0);//�h�X�{��
}

void snake_eat_body(SNAKE *snake)
{
	//�P�_�D�Y�M�D���y�ЬO�_�ۦP
	for (int i = 1; i < snake->size; i++)
	{
		if (snake->list[0].x == snake->list[i].x &&
			snake->list[0].y == snake->list[i].y)//��x,y�y�ЬҬۦP��
			game_end(snake);//�I�s�������
	}
}

void snake_eat_food(SNAKE *snake)
{
	if (snake->list[0].x == snake->food.x &&
		snake->list[0].y == snake->food.y )
	{
		//�쥻�������Q�D�Y�л\����ݭn�ͦ��s����
		init_food(snake);
		//�D�������׼W�[
		snake->size++;
		//�W�[����
		snake->score++;

	}
}

void init_wall()
{
	for (int i = 0; i <= HIGH; i++)
	{
		for (int j = 0; j <= WIDE; j++)
		{
			if (i == HIGH || j == WIDE)
			{
				printf("+");
			}
			else
			{
				printf(" ");
			}
		}
		printf("\n");
	}
}

void start_game(SNAKE *snake)
{
	while (snake->list[0].x < 60 && snake->list[0].x >= 0 &&
		snake->list[0].y < 20 && snake->list[0].y >= 0)//�P�_�D�Y���y�ЬO���O�b�d�� 
	{
		//����D����V 
		control_snake(snake);
		//��s�D���y��
		move_snake(snake);
		//�M�ſù�(�@�w�n��b��ܪ��e��)
		//system("cls");
		//������
		//init_wall();
		//�D���� 
		show_ui(snake);
		//�D�O�_�I��ۤv 
		snake_eat_body(snake);
		//�D�O�_�I�쭹�� 
		snake_eat_food(snake);
		Sleep(100);//����0.1��(�v�T�g���D���t��)
	}
	game_end(snake);
}


int main() 
{
	//���ô��
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);

	init_wall();//�L�X�C�����
	SNAKE *snake = (SNAKE *)malloc(sizeof(SNAKE));//�t�m�@��SNAKE�j�p���O����Ŷ��A�B�������ܼ�*snake���V�o�Ӧ�} 
	init_snake(snake);//�]���W�����檺���Y�A�u�n�ǤJ�@�Ӧ�}�Y�i����禡 
	show_ui(snake);//��ܤ��� 
	start_game(snake);//�}�l�C�� 
	system("pause");
	return 0;
}