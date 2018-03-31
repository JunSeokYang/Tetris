#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <windows.h>
#include <thread>

#define M 21
#define N 21

using namespace std;

void gotoxy(int x, int y);

class Tetris {
public:
	Tetris();
	void Play();
private:
	void RandBlock();
	void Rotation();
	void CheckPos();
	void CheckLine();
	void PrintScr();
	void TimeP();
	int **MapLoc;
	int **NowBlock;
	int x, y, flag, xcp;
	int Block[7][4][4] = {{{0,0,0,0},{1,1,1,1},{0,0,0,0},{0,0,0,0}},
						 {{0,0,0,0},{1,0,0,0},{1,1,1,0},{0,0,0,0}},
						 {{0,0,0,0},{0,0,0,1},{0,1,1,1},{0,0,0,0}},
						 {{0,0,0,0},{0,1,1,0},{0,1,1,0},{0,0,0,0}},
						 {{0,0,0,0},{0,0,1,0},{0,1,1,1},{0,0,0,0}},
						 {{0,0,0,0},{0,0,1,1},{0,1,1,0},{0,0,0,0}},
						 {{0,0,0,0},{1,1,0,0},{0,1,1,0},{0,0,0,0}}};
};

int main() {
	Tetris Game;
	Game.Play();
	return 0;
}

Tetris::Tetris() :
	x{ 0 }, y{ 11 }, MapLoc{ new int*[M] }, flag{ 1 }, NowBlock{ new int*[4] }{
	for (int i = 0; i < 4; i++) {
		NowBlock[i] = new int[4];
	}
}

void Tetris::RandBlock() {
	srand(time(NULL));

	int pivot;
	pivot = rand() % 7;

	for (int m = 0; m < 4; m++)
		for (int n = 0; n < 4; n++) {
			NowBlock[m][n] = Block[pivot][m][n];
		}
	x = 0;
	y = N / 2 - 2;
	xcp = M-1;
}

void Tetris::Rotation() {
	int ** NewBlock=new int*[4];

	for (int i = 0; i < 4; i++)
		NewBlock[i] = new int[4];

	for (int m = 0; m < 4; m++)
		for (int n = 0; n < 4; n++)
			NewBlock[n][3 - m] = NowBlock[m][n];

	for (int i = 0; i < 4; i++)
		delete(NowBlock[i]);
	delete(NowBlock);
	
	NowBlock = NewBlock;
}

void Tetris::Play() {
	char key;
	std::ios_base::sync_with_stdio(false);
	cin.tie(NULL);
	thread *thread1,*thread2;
	for (int i = 0; i < M; i++) {
		MapLoc[i] = new int[N];
	}

	for (int m = 0; m < M; m++)
		for (int n = 0; n < N; n++)
			MapLoc[m][n] = 0;
	thread1 = new thread([&] {PrintScr(); });
	thread2 = new thread([&] {TimeP(); });
	while (flag) {
		key = _getch();
		switch (key) {
		case -32:
			key = _getch();
			switch (key) {
			case 72:
				Rotation();
				break;
			case 75://    <- inserted
					y--;
				break;
			case 77://    -> inserted
					y++;
				break;
			case 80:
					x++;
				break;
			}
			CheckPos();
			break;
		}
	}
	thread1->join();
	thread2->join();
	cout << "Game Over" << endl;
	delete(thread1);
	delete(thread2);
}

void Tetris::CheckPos() {
	for (int m = 0; m < 4; m++)
		for (int n = 0; n < 4; n++)
			if (NowBlock[m][n]) {
				if (xcp + m >= M - 1) {
					xcp--;
					CheckPos();
				}
				if (MapLoc[x + m][y + n] == 1) {
					if (x == 0)
						flag = 0;
					x--;
					CheckPos();
				}
				if (MapLoc[xcp + m][y + n] == 1) {
					xcp--;
					CheckPos();
				}
				if (xcp + m >= M - 1) {
					xcp--;
					CheckPos();
				}
				if (x + m >= M-1) {
					x--;
					CheckPos();
				}
				if (y + n < 1) {
					y++;
					CheckPos();
				}
				if (y + n >= N-1) {
					y--;
					CheckPos();
				}
			}
}

void Tetris::PrintScr() {
	while (flag) {
		for (int m = 0; m < M; m++) {
			for (int n = 0; n < N; n++) {
				if (n == 0 || n == N - 1 || m == 0 || m == M - 1)
					cout << '@';
				else if (MapLoc[m][n])
					cout << '#';
				else
					cout << ' ';
			}
			cout << endl;
		} // 맵 출력


		for (int m = 0; m < 4; m++) {
			gotoxy(xcp + m, y);
			for (int n = 0; n < 4; n++)
				if (NowBlock[m][n])
					cout << '*';
				else if (xcp + m < M - 1 && y + n < N - 1)
					if ((xcp + m == 0 || xcp + m == M - 1 || y + n == 0 || y + n == N - 1))
						cout << '@';
					else if (MapLoc[xcp + m][y + n])
						cout << '#';
					else if (x + m == 1 && xcp == x)
						cout << '#';
					else
						cout << ' ';
		} // 예상 블럭 출력

		for (int m = 0; m < 4; m++) {
			gotoxy(x + m, y);
			for (int n = 0; n < 4; n++)
				if (NowBlock[m][n])
					cout << '#';
				else if (x + m < M - 1 && y + n < N - 1)
					if ((x + m == 0 || x + m == M - 1 || y + n == 0 || y + n == N - 1))
						cout << '@';
					else if (MapLoc[x + m][y + n])
						cout << '#';
					else
						cout << ' ';
		} // 블럭 출력

		Sleep(50);
		system("CLS");
	}

}

void Tetris::TimeP() {
	int buf=x, Checker = 0;
	RandBlock();
	while (flag) {
		Sleep(200);
		x++;
		xcp++;
		CheckPos();
		if (buf == x) {
			Checker++;
			if (Checker > 5) {
				for (int m = 0; m < 4; m++)
					for (int n = 0; n < 4; n++)
						if (((x + m)<M) && ((y + n)<N))
							if (MapLoc[x + m][y + n] == 0) {
								MapLoc[x + m][y + n] = NowBlock[m][n];
								CheckLine();
							}
				Checker = 0;
				x = 0;
				y = N / 2 - 2;
				RandBlock();
			}
		}
		buf = x;
	}
}

void Tetris::CheckLine() {
	int i;
	for (int m = M-1; m > 0; m--) {
		i = 0;
		for (int n = 1; n < N-1; n++)
			if (!MapLoc[m][n])
				i = 1;
		if (!i) {
			delete(MapLoc[m]);
			for (int p = m; p >0; p--)
				MapLoc[p] = MapLoc[p - 1];
			for (int p = 0; p < N; p++)
				MapLoc[1][p] = 0;
			m++;
		}
	}
}

void gotoxy(int x, int y)
{

	COORD Pos;

	Pos.X = y;
	Pos.Y = x;

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);

}