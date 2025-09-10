#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <Windows.h>
#include <conio.h>
#include <time.h>
using namespace std;

char board[31][31] = { 0 };

enum color
{
	white =7,
	green =10,
	red =12,
	blue = 11
};

struct player {
	int y;
	int x;
};

struct X {
	int y;
	int x;
};

player p;

X x[4];

void createX() {
	srand(time(NULL));
	for (int i = 0; i < 4; i++) {
		x[i].y = rand() % 30;
		x[i].x = rand() % 30;
		board[x[i].y][x[i].x] = 'X';
	}
}

void printBoard() {
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			if (j == p.x && i == p.y) {
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), red);
				cout << "* ";
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), white);
			}
			else {
				switch (board[i][j])
				{
				case '0':
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), white);
					break;
				case 'X':
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), blue);
					break;
				default:
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), green);
					break;
				}
				cout << board[i][j] << ' ';
			}
		}
		cout << endl;
	}
	
}

void resetBoard() {
	memset(board, '0', sizeof(board));
	p.y = 0;
	p.x = 0;
	createX();
	printBoard();
}

int main() {
	char cmd=0;

	resetBoard();

	while (cmd!='q') {
		cmd=_getch();
		rewind(stdin);
		system("cls");
		board[p.y][p.x]++;
		switch (cmd)
		{
		case 'w':
			if(p.y>0&&board[p.y-1][p.x]!='X') p.y--;
			break;
		case 'a':
			if (p.x > 0 && board[p.y][p.x-1] != 'X')p.x--;
			break;
		case 's':
			if (p.y < 30 && board[p.y + 1][p.x] != 'X')p.y++;
			break;
		case 'd':
			if (p.x < 30 && board[p.y][p.x+1] != 'X')p.x++;
			break;
		case 13:
			resetBoard();
			break;
		case 'q':
			return 0;
		default:
			break;
		}
		printBoard();
	}
	return 0;
}