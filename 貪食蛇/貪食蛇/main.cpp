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
#define scissors_popout 5
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
	BODY food;//�]�������]���y�СA�ҥH�]�Q��BODY���c�h�x�s 
	BODY scissors;//�P�HBODY���c�B�s�ŤM�y��
	BODY body_last;//�������ڦ�m
	BODY cousor;//������Ъ��y��
	COORD coord;//��Ъ���m(COORD�O�bwindows.h�̩w�q�����c) 
	int dx;//�D�Vx�b���ʪ���V 
	int dy;//�D�Vy�b���ʪ���V
	int size;//�D������
	int speed;//���ʳt��
	int score;//�o��
	char temp_key = 0;//�e����V
	char key = 0;//������V
}SNAKE;



void init_food(SNAKE *snake)	//��l�ƭ����y�� 
{
	srand(time(NULL));//�]�m�üƺؤl 
	snake->food.x = (rand() % (WIDE - 1)) + 1;
	snake->food.y = (rand() % (HIGH - 1)) + 1;
}

void init_scissors(SNAKE *snake)	//��l�ưŤM�y�� 
{
	srand(time(NULL));//�]�m�üƺؤl 
	snake->scissors.x = (rand() % (WIDE - 1)) + 1;
	snake->scissors.y = (rand() % (HIGH - 1)) + 1;
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
	//��l�ưŤM�y��
	init_scissors(snake);
	//��l�ƳD�����ʤ�V
	snake->dx = 1;
	snake->dy = 0;
	//��l�Ƥ��� 
	snake->score = 0;
	//��l�Ƴt��
	snake->speed = 300;
	//��l�ƴ�Юy��
	snake->cousor.x = 0;
	snake->cousor.y = 0;

}

void set_cursor(SNAKE *snake)//�]�w��Юy��
{
	snake->coord.X = snake->cousor.x;//�`�NX��Y�O�j�g 
	snake->coord.Y = snake->cousor.y;//�`�N�n��אּ������x,y�y�� 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//�]�w��Ц�m(windows���ب��)

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
			printf("*");//@�|�Y���D�Y���@�Ӧr���A�b�����*�N�� 
		}

	}
	//��ܭ���
	snake->coord.X = snake->food.x;//�`�NX��Y�O�j�g 
	snake->coord.Y = snake->food.y;//�`�N�n��אּ������x,y�y�� 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//�]�w��Ц�m(windows���ب��) 
	printf("#");

	if (snake->size > 3 && snake->score%scissors_popout ==0) {
		//�Y�j�p�j��3�B���Ƭ��]�w�����ƥͦ��ŤM
		snake->coord.X = snake->scissors.x;//�`�NX��Y�O�j�g 
		snake->coord.Y = snake->scissors.y;//�`�N�n��אּ������x,y�y�� 
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//�]�w��Ц�m(windows���ب��)
		printf("X");
	}
	
	//���ʫ�N���ګe�@�Ӧ�m��ܦ��ť�
	snake->coord.X = snake->body_last.x;//�`�NX��Y�O�j�g 
	snake->coord.Y = snake->body_last.y;//�`�N�n��אּ������x,y�y�� 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//�]�w��Ц�m(windows���ب��) 
	printf(" ");
	//�����ܤ���
	snake->cousor.x = 64;
	snake->cousor.y = 10;
	set_cursor(snake);
	printf("������������������������������������������������������");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("����.�Шϥ�wasd�ާ@�g���D  ��");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("����.�ť���i�H�Ȱ��C��    ��");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("����.�Y��u#�v�[�@��       ��");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("����.�Y��uX�v��t���Y�u3����");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("����.���ƶV���t�׶V��      ��");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("������������������������������������������������������");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf(" �o��:%d",snake->score);
}
void move_snake(SNAKE *snake)
{	
	//�b���ڮy�Ч�s�e�O����y��
	snake->body_last.x = snake->list[snake->size - 1].x;
	snake->body_last.y = snake->list[snake->size - 1].y;
	//��s���F�D�Y�~���D���y�� (�e����N���Υt�]�Ȧs�ܼ�) 
	for (int i = snake->size - 1; i > 0; i--)
	{
		snake->list[i] = snake->list[i - 1];//��D����e�@�`���y�Ы��w����@�` (list[0]�O�D�Y) 
	}
	//��s�D�Y 
	snake->list[0].x += snake->dx;//dx�w�]=1�A�ҥH�@�}�l�|���k�� 
	snake->list[0].y += snake->dy;

}

char opposide_direction(char a) { //�˦V
	switch (a) {
	case 'a': return 'd'; break;
	case 'd': return 'a'; break;
	case 'w': return 's'; break;
	case 's': return 'w'; break;
	}
}


void control_snake(SNAKE *snake)
{
	while (_kbhit())//�P�_�O�_���U����A���U!=0 
	{
		snake->key = _getch();//��������
	}
	if (snake->temp_key != 0 && snake->key != opposide_direction(snake->temp_key)) {
		//�P�_���������B�����e����V�˦V
		switch (snake->key)
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
		case 32:
			while (_getch() != 32); //���U�ťռȰ�
			break;
		}
	}
	if (snake->key != opposide_direction(snake->temp_key))
		snake->temp_key = snake->key;		//�����e����V
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
		snake->list[0].y == snake->food.y)	//�Y��m�P��������m
	{
		init_food(snake);//�s�ͦ�����
		snake->size++;//�D���׼W�[
		snake->score++;//���ƼW�[
		if (snake->speed > 200)		//�Y�쭹����
			snake->speed -= 50;	//�̰Ϭq���ɳt��
		else if (snake->speed > 100)
			snake->speed -= 30;
		else if (snake->speed > 10)
			snake->speed -= 20;
	}
}
void snake_eat_scissors(SNAKE *snake)
{
	if (snake->size > 3 && snake->list[0].x == snake->scissors.x &&
		snake->list[0].y == snake->scissors.y) {	
		//�Y��m�P���ŤM��m�B�j�p�W�L3
		init_scissors(snake);	//�s�ͦ��ŤM
		snake->size -= 3;	//�D���װŵu3
		snake->speed += 200;	//�t�ש�C(����+0.2��)
		for (int i = snake->size; i < snake->size+4; i++)
		{
			snake->coord.X = snake->list[i].x;//�`�NX��Y�O�j�g 
			snake->coord.Y = snake->list[i].y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//�]�w��Ц�m(windows���ب��) 
			printf(" ");	//�N�Ű������]���ť�
		}
	}
		
		
}

void init_wall()	//�ͦ����
{
	for (int i = 0; i <= HIGH; i++)
	{
		for (int j = 0; j <= WIDE; j++)
			if (i == HIGH || i == 0 || j == WIDE || j == 0)
				printf("�i");
			else
				printf(" ");
		printf("\n");
	}
}



void start_game(SNAKE *snake)
{
	while (snake->list[0].x < WIDE && snake->list[0].x >= 1 &&
		snake->list[0].y < HIGH && snake->list[0].y >= 1)//�P�_�D�Y���y�ЬO���O�b�d�� 
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
		//�D�O�_�I��ŤM
		snake_eat_scissors(snake);
		//����g���D���t��
		Sleep(snake->speed);
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