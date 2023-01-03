#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctime>
#include <time.h> 
#include <conio.h>//判別按下的按鍵 
#include <windows.h>//設置游標位置 
#define WIDE 60
#define HIGH 20
#define scissors_popout 5
using namespace std;
/* run this program using the console pauser or add your own getch, system("pause") or input loop */

typedef struct _body//放蛇的座標 
{
	int x;
	int y;

}BODY;

typedef struct snake//放有關蛇的一切 
{
	BODY list[WIDE*HIGH];//儲存身體的每一節座標，身體最長=地圖大小 
	BODY food;//因為食物也有座標，所以也利用BODY結構去儲存 
	BODY scissors;//同以BODY結構處存剪刀座標
	BODY body_last;//紀錄尾巴位置
	BODY cousor;//紀錄游標的座標
	COORD coord;//游標的位置(COORD是在windows.h裡定義的結構) 
	int dx;//蛇向x軸移動的方向 
	int dy;//蛇向y軸移動的方向
	int size;//蛇的身長
	int speed;//移動速度
	int score;//得分
	char temp_key = 0;//前次方向
	char key = 0;//本次方向
}SNAKE;



void init_food(SNAKE *snake)	//初始化食物座標 
{
	srand(time(NULL));//設置亂數種子 
	snake->food.x = (rand() % (WIDE - 1)) + 1;
	snake->food.y = (rand() % (HIGH - 1)) + 1;
}

void init_scissors(SNAKE *snake)	//初始化剪刀座標 
{
	srand(time(NULL));//設置亂數種子 
	snake->scissors.x = (rand() % (WIDE - 1)) + 1;
	snake->scissors.y = (rand() % (HIGH - 1)) + 1;
}

void init_snake(SNAKE *snake)
{
	//初始化蛇頭的x座標和y座標(預設在畫面中間) 
	snake->list[0].x = WIDE / 2;
	snake->list[0].y = HIGH / 2;
	//初始化蛇尾座標 
	snake->list[1].x = (WIDE / 2) - 1;
	snake->list[1].y = HIGH / 2;
	//初始化蛇的身體長度為2 
	snake->size = 2;
	//初始化食物的座標 
	init_food(snake);
	//初始化剪刀座標
	init_scissors(snake);
	//初始化蛇的移動方向
	snake->dx = 1;
	snake->dy = 0;
	//初始化分數 
	snake->score = 0;
	//初始化速度
	snake->speed = 300;
	//初始化游標座標
	snake->cousor.x = 0;
	snake->cousor.y = 0;

}

void set_cursor(SNAKE *snake)//設定游標座標
{
	snake->coord.X = snake->cousor.x;//注意X跟Y是大寫 
	snake->coord.Y = snake->cousor.y;//注意要更改為食物的x,y座標 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//設定游標位置(windows內建函數)

}

void show_ui(SNAKE *snake)//顯示蛇與食物的函式 
{
	//顯示蛇(顯示蛇或食物時需要設置游標的位置，否則顯示的內容都會擠在程式第一排) 
	for (int i = 0; i < snake->size; i++)
	{
		snake->coord.X = snake->list[i].x;//注意X跟Y是大寫 
		snake->coord.Y = snake->list[i].y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//設定游標位置(windows內建函數) 
		if (i == 0)
		{
			printf("@");
		}
		else
		{
			printf("*");//@會吃掉蛇頭的一個字元，在此改用*代替 
		}

	}
	//顯示食物
	snake->coord.X = snake->food.x;//注意X跟Y是大寫 
	snake->coord.Y = snake->food.y;//注意要更改為食物的x,y座標 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//設定游標位置(windows內建函數) 
	printf("#");

	if (snake->size > 3 && snake->score%scissors_popout ==0) {
		//若大小大於3且分數為設定之倍數生成剪刀
		snake->coord.X = snake->scissors.x;//注意X跟Y是大寫 
		snake->coord.Y = snake->scissors.y;//注意要更改為食物的x,y座標 
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//設定游標位置(windows內建函數)
		printf("X");
	}
	
	//移動後將尾巴前一個位置顯示成空白
	snake->coord.X = snake->body_last.x;//注意X跟Y是大寫 
	snake->coord.Y = snake->body_last.y;//注意要更改為食物的x,y座標 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//設定游標位置(windows內建函數) 
	printf(" ");
	//實時顯示分數
	snake->cousor.x = 64;
	snake->cousor.y = 10;
	set_cursor(snake);
	printf("╔═════════════════════════╗");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("║Ⅰ.請使用wasd操作貪食蛇  ║");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("║Ⅱ.空白鍵可以暫停遊戲    ║");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("║Ⅲ.吃到「#」加一分       ║");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("║Ⅲ.吃到「X」減速並縮短3格║");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("║Ⅳ.分數越高速度越高      ║");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf("╚═════════════════════════╝");
	snake->cousor.y += 1;
	set_cursor(snake);
	printf(" 得分:%d",snake->score);
}
void move_snake(SNAKE *snake)
{	
	//在尾巴座標更新前記錄其座標
	snake->body_last.x = snake->list[snake->size - 1].x;
	snake->body_last.y = snake->list[snake->size - 1].y;
	//更新除了蛇頭外的蛇身座標 (前給後就不用另設暫存變數) 
	for (int i = snake->size - 1; i > 0; i--)
	{
		snake->list[i] = snake->list[i - 1];//把蛇身體前一節的座標指定給後一節 (list[0]是蛇頭) 
	}
	//更新蛇頭 
	snake->list[0].x += snake->dx;//dx預設=1，所以一開始會往右走 
	snake->list[0].y += snake->dy;

}

char opposide_direction(char a) { //倒向
	switch (a) {
	case 'a': return 'd'; break;
	case 'd': return 'a'; break;
	case 'w': return 's'; break;
	case 's': return 'w'; break;
	}
}


void control_snake(SNAKE *snake)
{
	while (_kbhit())//判斷是否按下按鍵，按下!=0 
	{
		snake->key = _getch();//獲取按鍵值
	}
	if (snake->temp_key != 0 && snake->key != opposide_direction(snake->temp_key)) {
		//判斷不為首次且不為前次方向倒向
		switch (snake->key)
		{
		case 'a':
			snake->dx = -1;//蛇的方向向左 
			snake->dy = 0;
			break;
		case 'w':
			snake->dx = 0;//蛇的方向向上
			snake->dy = -1;
			break;
		case 's':
			snake->dx = 0;//蛇的方向向下 
			snake->dy = 1;
			break;
		case 'd':
			snake->dx = 1;//蛇的方向向右 
			snake->dy = 0;
			break;
		case 32:
			while (_getch() != 32); //按下空白暫停
			break;
		}
	}
	if (snake->key != opposide_direction(snake->temp_key))
		snake->temp_key = snake->key;		//紀錄前次方向
}

void game_end(SNAKE *snake)
{
	//這裡需自行設定遊戲失敗時訊息出現的位置，否則游標會停在上一個水果出現的地方
	snake->coord.X = 25;//注意X跟Y是大寫 
	snake->coord.Y = 25;//注意要更改為食物的x,y座標 
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//設定游標位置(windows內建函數)
	printf("遊戲結束 得分為%d\n", snake->score);
	Sleep(5000);
	exit(0);//退出程式
}

void snake_eat_body(SNAKE *snake)
{
	//判斷蛇頭和蛇身座標是否相同
	for (int i = 1; i < snake->size; i++)
	{
		if (snake->list[0].x == snake->list[i].x &&
			snake->list[0].y == snake->list[i].y)//當x,y座標皆相同時
			game_end(snake);//呼叫結束函數
	}
}

void snake_eat_food(SNAKE *snake)
{
	if (snake->list[0].x == snake->food.x &&
		snake->list[0].y == snake->food.y)	//頭位置同為食物位置
	{
		init_food(snake);//新生成食物
		snake->size++;//蛇長度增加
		snake->score++;//分數增加
		if (snake->speed > 200)		//吃到食物時
			snake->speed -= 50;	//依區段提升速度
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
		//頭位置同為剪刀位置且大小超過3
		init_scissors(snake);	//新生成剪刀
		snake->size -= 3;	//蛇長度剪短3
		snake->speed += 200;	//速度放慢(延遲+0.2秒)
		for (int i = snake->size; i < snake->size+4; i++)
		{
			snake->coord.X = snake->list[i].x;//注意X跟Y是大寫 
			snake->coord.Y = snake->list[i].y;
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), snake->coord);//設定游標位置(windows內建函數) 
			printf(" ");	//將剪除部分設為空白
		}
	}
		
		
}

void init_wall()	//生成邊界
{
	for (int i = 0; i <= HIGH; i++)
	{
		for (int j = 0; j <= WIDE; j++)
			if (i == HIGH || i == 0 || j == WIDE || j == 0)
				printf("█");
			else
				printf(" ");
		printf("\n");
	}
}



void start_game(SNAKE *snake)
{
	while (snake->list[0].x < WIDE && snake->list[0].x >= 1 &&
		snake->list[0].y < HIGH && snake->list[0].y >= 1)//判斷蛇頭的座標是不是在範圍內 
	{
		//控制蛇的方向 
		control_snake(snake);
		//更新蛇的座標
		move_snake(snake);
		//清空螢幕(一定要放在顯示的前面)
		//system("cls");
		//顯示邊界
		//init_wall();
		//蛇移動 
		show_ui(snake);
		//蛇是否碰到自己 
		snake_eat_body(snake);
		//蛇是否碰到食物 
		snake_eat_food(snake);
		//蛇是否碰到剪刀
		snake_eat_scissors(snake);
		//控制貪食蛇的速度
		Sleep(snake->speed);
	}
	game_end(snake);
}


int main()
{
	//隱藏游標
	CONSOLE_CURSOR_INFO cci;
	cci.dwSize = sizeof(cci);
	cci.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cci);
	init_wall();//印出遊戲邊界
	SNAKE *snake = (SNAKE *)malloc(sizeof(SNAKE));//配置一個SNAKE大小的記憶體空間，且讓指標變數*snake指向這個位址 
	init_snake(snake);//因為上面那行的關係，只要傳入一個位址即可執行函式 
	show_ui(snake);//顯示介面 
	start_game(snake);//開始遊戲 
	system("pause");
	return 0;
}