#include <iostream>
#include <windows.h>
#include <time.h>
#include <conio.h>

using namespace std;

class Board
{
private:
	int height, width;
	int grade, score, speed;
public:
	char **fence;
	void setHeight(int h);
	void setWidth(int w);
	void setGrade(int g);
	void setScore(int s);
	void setSpeed(int s);

	int getHeight();
	int getWidth();
	int getScore();
	int getSpeed();
	int getGrade();
	Board(int height, int width);
	void show();
};
void Board::setGrade(int g) {
	grade = g;
}
void Board::setScore(int s) {
	score = s;
}
void Board::setSpeed(int s) {
	speed = s;
}
void Board::setHeight(int h) {
	height = h;
}
void Board::setWidth(int w) {
	width = w;
}
int Board::getGrade() {
	return grade;
}
int Board::getScore() {
	return score;
}
int Board::getSpeed() {
	return speed;
}
int Board::getHeight() {
	return this->height;
}
int Board::getWidth() {
	return width;
}
Board::Board(int h=22, int w=22)
{
	setHeight(h);
	setWidth(w);
	setScore(0);
	setGrade(1);
	setSpeed(500);
	fence = new char*[height];
	for (int i = 0; i < height; ++i)
		fence[i] = new char[width]();
	for (int i = 0; i <= height - 1; i++)
		fence[0][i] = fence[height - 1][i] = '#';
	for (int i = 1; i <= height - 2; i++)
		fence[i][0] = fence[i][width - 1] = '#';
	for (int i = 1; i <= height - 2; i++)
		for (int j = 1; j <= width - 2; j++)
			fence[i][j] = ' ';
}

void Board::show()
{
	system("cls");
	cout << endl;
	for (int i = 0; i < height; i++)
	{
		cout << "\t";
		for (int j = 0; j < width; j++)
			cout << fence[i][j] << ' ';
		if (i == 0)
			cout << "\tgrade:" << grade;
		if (i == 2)
			cout << "\tscore:" << score;
		if (i == 4)
			cout << "\tautomatic forward";
		if (i == 5)
			cout << "\ttime interval:" << speed << " ms";
		cout << endl;
	}
}
//create a food for snake to eat on the board
class Food
{
private:
	int x, y;

public:
	Food(Board);
	void setX(int);
	void setY(int);
	int getX();
	int getY();
	void reproduce(Board);
};
void Food::setX(int _x) {
	x = _x;
}
void Food::setY(int _y) {
	y = _y;
}
int Food::getX() {
	return x;
}
int Food::getY() {
	return y;
}
Food::Food(Board board)
{
	srand((unsigned)time(NULL));
	do
	{
		x = rand() % board.getWidth() - 2 + 1;
		y = rand() % board.getHeight() - 2 + 1;
	} while (board.fence[x][y] != ' ');
	board.fence[x][y] = '$';
}
void Food::reproduce(Board board) {
	srand((unsigned)time(NULL));
	do
	{
		x = rand() % board.getWidth() - 2 + 1;
		y = rand() % board.getHeight() - 2 + 1;
	} while (board.fence[x][y] != ' ');
	board.fence[x][y] = '$';
}
enum Direction { Up = 72, Left = 75, Right = 77, Down = 80 };
class Snake
{
public:
	int body[2][100];
	int head, tail,length, x, y;
	char direction;
	Snake(Board);
	void move(Board, Food);
};
Snake::Snake(Board board)
{
	direction = Right;
	length = 4;
	board.fence[1][length] = '@';
	for (int i = 1; i < length; i++)
		board.fence[1][i] = '*';
	for (int i = 0; i < length; i++)
	{
		body[0][i] = 1;
		body[1][i] = i + 1;
	}
}
void Snake::move(Board board, Food food)
{
	head = 3, tail = 0;
	while (1)
	{
		bool timeover = true;
		long start = clock();
		while ((timeover = ((clock() - start) < board.getSpeed())) && !_kbhit())
			;
		if (timeover)
		{
			_getch();
			direction=_getch();
		}
		switch (direction)
		{
		case Up:
			x = body[0][head] - 1;
			y = body[1][head];
			break; //up
		case Down:
			x = body[0][head] + 1;
			y = body[1][head];
			break; //down
		case Left:
			x = body[0][head];
			y = body[1][head] - 1;
			break; //left
		case Right:
			x = body[0][head];
			y = body[1][head] + 1;
			break; //right
		}
		//hit the board
		if (x == 0 || x == board.getWidth()-1 || y == 0 || y == board.getHeight()-1)
		{
			cout << "\tGame over!" << endl;
			break;
		}
		//eat itself
		if (board.fence[x][y] != ' ' && !(x == food.getX() && y == food.getY()))
		{
			cout << "\tGame over!" << endl;
			break;
		}
		//eat food
		if (x == food.getX() && y == food.getY())
		{
			length++;
			board.setScore(board.getScore() + 100);
			if (length >= 8)
			{
				length -= 8;
				board.setGrade(board.getGrade()+1);
				if (board.getSpeed() >= 200)
					board.setSpeed(550 - board.getGrade() * 50);
			}
			board.fence[x][y] = '@';
			board.fence[body[0][head]][body[1][head]] = '*';
			head = (head + 1) % 100;
			body[0][head] = x;
			body[1][head] = y;
			food.reproduce(board);
			board.show();
		}
		else
		{
			board.fence[body[0][tail]][body[1][tail]] = ' ';
			tail = (tail + 1) % 100;
			board.fence[body[0][head]][body[1][head]] = '*';
			head = (head + 1) % 100;
			body[0][head] = x;
			body[1][head] = y;
			board.fence[body[0][head]][body[1][head]] = '@';
			board.show();
		}
	}
}
void countdown(int second) {
	
	for (int i = second; i > 0; i--)
	{
		cout << "\n\n\t\tCountdown:" << i << endl;
		long start = clock();
		while (clock() - start < CLK_TCK)
			;
		system("cls");
		//cout << "\n\n\t\tThe game is about to begin!" << endl;
	}
}
int main()
{
	countdown(1);
	Board board = Board(22, 22);
	Snake snake = Snake(board);
	Food food = Food(board);
	board.show();
	snake.move(board,food);
	return 0;
}