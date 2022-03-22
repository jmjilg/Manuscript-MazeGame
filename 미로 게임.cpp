﻿
#include <iostream>
#include<conio.h>
#include<time.h>

using namespace std;

/*
0 : 벽
1 : 길
2 : 시작점
3 : 도착점
4 : 폭탄
5 : 파워아이템
6 : 벽밀기 아이템
7 : 투명아이템
8 : 웜홀

숙제 : 벽을 깨면 일정 확률로 아이템이 나온다.
아이템종류 : 폭탄 반경 1칸 증가, 벽 밀기 아이템, 투명아이템(벽통과)
3종류가 랜덤하게 등장한다. 등장하지 않을 수도 있다.

아이템을 먹게되면 해당 아이템 효과를 부여받게 된다.

벽을 깨면 랜덤하게 웜홀을 만든다. 웜홀에 들어가게 되면 다른 웜홀의 위치로 이동
한다. 웜홀은 맵 중간마다 몇개씩 만들어본다. 웜홀이 1개일 경우 다른곳으로
이동하지 않는다.
*/
struct _tagPoint
{
	int x;
	int y;
};

// typedef : 타입을 재정의하는 기능이다.
typedef _tagPoint	POINT;
typedef	_tagPoint* PPOINT;

typedef struct _tagPlayer
{
	_tagPoint	tPos;
	bool		bWallPush;
	bool		bPushOnOff;
	bool		bTransparency;
	int			iBombPower;
}PLAYER, *PPLAYER;

void SetMaze(char Maze[21][21], PPLAYER pPlayer, PPOINT pStartPos,
	PPOINT pEndPos)
{
	pStartPos->x = 0;
	pStartPos->y = 0;

	pEndPos->x = 19;
	pEndPos->y = 19;

	pPlayer->tPos = *pStartPos;

	strcpy_s(Maze[0],  "21100000000000000000");
	strcpy_s(Maze[1],  "00111111111100000000");
	strcpy_s(Maze[2],  "00100010000111111100");
	strcpy_s(Maze[3],  "01100010000000000100");
	strcpy_s(Maze[4],  "01000011110001111100");
	strcpy_s(Maze[5],  "01000000001111000000");
	strcpy_s(Maze[6],  "01100000001000000000");
	strcpy_s(Maze[7],  "00100000001111111000");
	strcpy_s(Maze[8],  "00001110000000001000");
	strcpy_s(Maze[9],  "01111011111111111000");
	strcpy_s(Maze[10], "01000000000000000000");
	strcpy_s(Maze[11], "01111100111111100000");
	strcpy_s(Maze[12], "00000111100000111110");
	strcpy_s(Maze[13], "01111100000000000010");
	strcpy_s(Maze[14], "01000000001111111110");
	strcpy_s(Maze[15], "01111110011000000000");
	strcpy_s(Maze[16], "00000010010000000000");
	strcpy_s(Maze[17], "01111110011111000000");
	strcpy_s(Maze[18], "01000000000001100000");
	strcpy_s(Maze[19], "11000000000000111113");
}

void Output(char Maze[21][21], PPLAYER pPlayer)
{
	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			if (pPlayer->tPos.x == j && pPlayer->tPos.y == i)
				cout << "☆";

			else if (Maze[i][j] == '4')
				cout << "♨";

			else if (Maze[i][j] == '0')
				cout << "■";

			else if (Maze[i][j] == '1')
				cout << "  ";

			else if (Maze[i][j] == '2')
				cout << "★";

			else if (Maze[i][j] == '3')
				cout << "◎";

			else if (Maze[i][j] == '5')
				cout << "㈜";

			else if (Maze[i][j] == '6')
				cout << "※";

			else if (Maze[i][j] == '7')
				cout << "▷";

		}

		cout << endl;
	}

	cout << "폭탄파워 : " << pPlayer->iBombPower << endl;
	cout << "벽통과 : ";
	if (pPlayer->bTransparency)
		cout << "ON\t";

	else
		cout << "OFF\t";

	cout << "벽밀기 : ";
	if (pPlayer->bWallPush)
	{
		cout << "가능 / ON" << endl;

		if (pPlayer->bPushOnOff)
			cout << "ON" << endl;

		else
			cout << "OFF" << endl;
	}
	else
		cout << "불가능 / OFF" << endl;
}

bool AddItem(char cItemType, PPLAYER pPlayer)
{
	if (cItemType == '5')
	{
		if(pPlayer->iBombPower<5)
			++pPlayer->iBombPower;

		return true;
	}

	else if (cItemType == '6')
	{
		pPlayer->bWallPush = true;
		pPlayer->bPushOnOff = true;

		return true;
	}

	else if (cItemType == '7')
	{
		pPlayer->bTransparency = true;
		return true;
	}

	return false;
}

void MoveUp(char Maze[21][21], PPLAYER pPlayer)
{
	if (pPlayer->tPos.y - 1 >= 0)
	{
		// 벽인지 체크한다.
		if (Maze[pPlayer->tPos.y - 1][pPlayer->tPos.x] != '0' &&
			Maze[pPlayer->tPos.y - 1][pPlayer->tPos.x] != '4')
		{
			--pPlayer->tPos.y;
		}

		// 벽 밀기가 가능하고 바로 윗칸이 벽일 경우
		else if (pPlayer->bWallPush && Maze[pPlayer->tPos.y - 1][pPlayer->tPos.x] == '0')
		{
			// 벽 밀기가 ON 상태일 경우
			if (pPlayer->bPushOnOff)
			{
				// 위의 위칸이 0보다 크거나 같을 경우는 인덱스가 있다는 의미.
				if (pPlayer->tPos.y - 2 >= 0)
				{
					// 위의 위칸이 길이어야 밀기가 가능하다. 그러므로 길인지 체크한다.
					if (Maze[pPlayer->tPos.y - 2][pPlayer->tPos.x] == '0')
					{
						if (pPlayer->bTransparency)
							--pPlayer->tPos.y;
					}

					// 길일 경우 벽을 밀어낸다.
					else if (Maze[pPlayer->tPos.y - 2][pPlayer->tPos.x] == '1')
					{
						// 위의 위칸을 벽으로 하고
						Maze[pPlayer->tPos.y - 2][pPlayer->tPos.x] = '0';
						// 위칸은 벽이었는데 길로 만들어준다.
						Maze[pPlayer->tPos.y - 1][pPlayer->tPos.x] = '1';
						// 플레이어를 이동시킨다.
						--pPlayer->tPos.y;
					}
				}

				else if (pPlayer->bTransparency)
					--pPlayer->tPos.y;
			}

			// 벽 밀기 OFF 상태일 경우
			else if (pPlayer->bTransparency)
				--pPlayer->tPos.y;
		}

		else if (pPlayer->bTransparency)
			--pPlayer->tPos.y;

		if(AddItem(Maze[pPlayer->tPos.y][pPlayer->tPos.x], pPlayer))
			Maze[pPlayer->tPos.y][pPlayer->tPos.x] = '1';

	}
}

void MoveDown(char Maze[21][21], PPLAYER pPlayer)
{
	if (pPlayer->tPos.y + 1 < 20)
	{
		// 벽인지 체크한다.
		if (Maze[pPlayer->tPos.y + 1][pPlayer->tPos.x] != '0' &&
			Maze[pPlayer->tPos.y + 1][pPlayer->tPos.x] != '4')
		{
			++pPlayer->tPos.y;
		}

		// 벽 밀기가 가능하고 바로 윗칸이 벽일 경우
		else if (pPlayer->bWallPush && Maze[pPlayer->tPos.y + 1][pPlayer->tPos.x] == '0')
		{
			// 벽 밀기가 ON 상태일 경우
			if (pPlayer->bPushOnOff)
			{
				// 위의 위칸이 0보다 크거나 같을 경우는 인덱스가 있다는 의미.
				if (pPlayer->tPos.y + 2 < 20)
				{
					// 위의 위칸이 길이어야 밀기가 가능하다. 그러므로 길인지 체크한다.
					if (Maze[pPlayer->tPos.y + 2][pPlayer->tPos.x] == '0')
					{
						if (pPlayer->bTransparency)
							++pPlayer->tPos.y;
					}

					// 길일 경우 벽을 밀어낸다.
					else if (Maze[pPlayer->tPos.y + 2][pPlayer->tPos.x] == '1')
					{
						// 위의 위칸을 벽으로 하고
						Maze[pPlayer->tPos.y + 2][pPlayer->tPos.x] = '0';
						// 위칸은 벽이었는데 길로 만들어준다.
						Maze[pPlayer->tPos.y + 1][pPlayer->tPos.x] = '1';
						// 플레이어를 이동시킨다.
						++pPlayer->tPos.y;
					}
				}

				else if (pPlayer->bTransparency)
					++pPlayer->tPos.y;
			}

			// 벽 밀기 OFF 상태일 경우
			else if (pPlayer->bTransparency)
				++pPlayer->tPos.y;
		}

		else if (pPlayer->bTransparency)
			++pPlayer->tPos.y;

		if (AddItem(Maze[pPlayer->tPos.y][pPlayer->tPos.x], pPlayer))
			Maze[pPlayer->tPos.y][pPlayer->tPos.x] = '1';
	}
}

void MoveRight(char Maze[21][21], PPLAYER pPlayer)
{
	if (pPlayer->tPos.x + 1 < 20)
	{
		// 벽인지 체크한다.
		if (Maze[pPlayer->tPos.y][pPlayer->tPos.x+1] != '0' &&
			Maze[pPlayer->tPos.y][pPlayer->tPos.x+1] != '4')
		{
			++pPlayer->tPos.x;
		}

		// 벽 밀기가 가능하고 바로 윗칸이 벽일 경우
		else if (pPlayer->bWallPush && Maze[pPlayer->tPos.y][pPlayer->tPos.x + 1] == '0')
		{
			// 벽 밀기가 ON 상태일 경우
			if (pPlayer->bPushOnOff)
			{
				// 위의 위칸이 0보다 크거나 같을 경우는 인덱스가 있다는 의미.
				if (pPlayer->tPos.x + 2 < 20)
				{
					// 위의 위칸이 길이어야 밀기가 가능하다. 그러므로 길인지 체크한다.
					if (Maze[pPlayer->tPos.y][pPlayer->tPos.x + 2] == '0')
					{
						if (pPlayer->bTransparency)
							++pPlayer->tPos.x;
					}

					// 길일 경우 벽을 밀어낸다.
					else if (Maze[pPlayer->tPos.y][pPlayer->tPos.x + 2] == '1')
					{
						// 위의 위칸을 벽으로 하고
						Maze[pPlayer->tPos.y][pPlayer->tPos.x + 2] = '0';
						// 위칸은 벽이었는데 길로 만들어준다.
						Maze[pPlayer->tPos.y][pPlayer->tPos.x + 1] = '1';
						// 플레이어를 이동시킨다.
						++pPlayer->tPos.x;
					}
				}

				else if (pPlayer->bTransparency)
					++pPlayer->tPos.x;
			}

			// 벽 밀기 OFF 상태일 경우
			else if (pPlayer->bTransparency)
				++pPlayer->tPos.x;
		}

		else if (pPlayer->bTransparency)
			++pPlayer->tPos.x;

		if (AddItem(Maze[pPlayer->tPos.y][pPlayer->tPos.x], pPlayer))
			Maze[pPlayer->tPos.y][pPlayer->tPos.x] = '1';
	}
}

void MoveLeft(char Maze[21][21], PPLAYER pPlayer)
{
	if (pPlayer->tPos.x - 1 >= 0)
	{
		// 벽인지 체크한다.
		if (Maze[pPlayer->tPos.y][pPlayer->tPos.x - 1] != '0' &&
			Maze[pPlayer->tPos.y][pPlayer->tPos.x - 1] != '4')
		{
			--pPlayer->tPos.x;
		}

		// 벽 밀기가 가능하고 바로 윗칸이 벽일 경우
		else if (pPlayer->bWallPush && Maze[pPlayer->tPos.y][pPlayer->tPos.x - 1] == '0')
		{
			// 벽 밀기가 ON 상태일 경우
			if (pPlayer->bPushOnOff)
			{
				// 위의 위칸이 0보다 크거나 같을 경우는 인덱스가 있다는 의미.
				if (pPlayer->tPos.x - 2 >= 0)
				{
					// 위의 위칸이 길이어야 밀기가 가능하다. 그러므로 길인지 체크한다.
					if (Maze[pPlayer->tPos.y][pPlayer->tPos.x - 2] == '0')
					{
						if (pPlayer->bTransparency)
							--pPlayer->tPos.x;
					}

					// 길일 경우 벽을 밀어낸다.
					else if (Maze[pPlayer->tPos.y][pPlayer->tPos.x - 2] == '1')
					{
						// 위의 위칸을 벽으로 하고
						Maze[pPlayer->tPos.y][pPlayer->tPos.x - 2] = '0';
						// 위칸은 벽이었는데 길로 만들어준다.
						Maze[pPlayer->tPos.y][pPlayer->tPos.x - 1] = '1';
						// 플레이어를 이동시킨다.
						--pPlayer->tPos.x;
					}
				}

				else if (pPlayer->bTransparency)
					--pPlayer->tPos.x;
			}

			// 벽 밀기 OFF 상태일 경우
			else if (pPlayer->bTransparency)
				--pPlayer->tPos.x;
		}

		else if (pPlayer->bTransparency)
			--pPlayer->tPos.x;

		if (AddItem(Maze[pPlayer->tPos.y][pPlayer->tPos.x], pPlayer))
			Maze[pPlayer->tPos.y][pPlayer->tPos.x] = '1';
	}
}

void MovePlayer(char Maze[21][21], PPLAYER pPlayer, char cInput)
{
	switch (cInput)
	{
	case 'w':
	case 'W':
		MoveUp(Maze, pPlayer);
		break;
	case 's':
	case 'S':
		MoveDown(Maze, pPlayer);
		break;
	case 'a':
	case 'A':
		MoveLeft(Maze, pPlayer);
		break;
	case 'd':
	case 'D':
		MoveRight(Maze, pPlayer);
		break;
	}
}

// 포인터 변수를 const로 생성하면 가리키는 대상의 값을 변경할 수 없다.
void CreateBomb(char Maze[21][21], const PPLAYER pPlayer,
	PPOINT pBombArr, int* pBombCount)
{
	if (*pBombCount == 5)
		return;

	else if (Maze[pPlayer->tPos.y][pPlayer->tPos.x] == '0')
		return;

	for (int i = 0; i < *pBombCount; ++i)
	{
		if (pPlayer->tPos.x == pBombArr[i].x &&
			pPlayer->tPos.y == pBombArr[i].y)
			return;
	}

	pBombArr[*pBombCount] = pPlayer->tPos;
	++(*pBombCount);

	Maze[pPlayer->tPos.y][pPlayer->tPos.x] = '4';
}

void Fire(char Maze[21][21], PPLAYER pPlayer, PPOINT pBombArr,
	int* pBombCount)
{
	for (int i = 0; i < *pBombCount; ++i)
	{
		Maze[pBombArr[i].y][pBombArr[i].x] = '1';

		// 플레이어가 폭탄에 맞았을때 시작점으로 보낸다.
		if (pPlayer->tPos.x == pBombArr[i].x && pPlayer->tPos.y == pBombArr[i].y)
		{
			pPlayer->tPos.x = 0;
			pPlayer->tPos.y = 0;
		}

		for (int j = 1; j <= pPlayer->iBombPower; ++j)
		{
			if (pBombArr[i].y - j >= 0)
			{
				if (Maze[pBombArr[i].y - j][pBombArr[i].x] == '0')
				{
					// 아이템 드랍 확률을 구한다.
					if (rand() % 100 < 20)
					{
						int iPerCent = rand() % 100;
						if(iPerCent<40)
							Maze[pBombArr[i].y - j][pBombArr[i].x] = '5';

						else if (iPerCent < 70)
							Maze[pBombArr[i].y - j][pBombArr[i].x] = '6';

						else 
							Maze[pBombArr[i].y - j][pBombArr[i].x] = '7';
					}

					else
						Maze[pBombArr[i].y - j][pBombArr[i].x] = '1';
				}

				// 플레이어가 폭탄에 맞았을때 시작점으로 보낸다.
				if (pPlayer->tPos.x == pBombArr[i].x && pPlayer->tPos.y == pBombArr[i].y - j)
				{
					pPlayer->tPos.x = 0;
					pPlayer->tPos.y = 0;
				}
			}

			if (pBombArr[i].y + j < 20)
			{
				if (Maze[pBombArr[i].y + j][pBombArr[i].x] == '0')
				{
					// 아이템 드랍 확률을 구한다.
					if (rand() % 100 < 20)
					{
						int iPerCent = rand() % 100;
						if (iPerCent < 40)
							Maze[pBombArr[i].y + j][pBombArr[i].x] = '5';

						else if (iPerCent < 70)
							Maze[pBombArr[i].y + j][pBombArr[i].x] = '6';

						else
							Maze[pBombArr[i].y + j][pBombArr[i].x] = '7';
					}

					else
						Maze[pBombArr[i].y + j][pBombArr[i].x] = '1';
				}
				// 플레이어가 폭탄에 맞았을때 시작점으로 보낸다.
				if (pPlayer->tPos.x == pBombArr[i].x && pPlayer->tPos.y == pBombArr[i].y + j)
				{
					pPlayer->tPos.x = 0;
					pPlayer->tPos.y = 0;
				}
			}

			if (pBombArr[i].x - j >= 0)
			{
				if (Maze[pBombArr[i].y][pBombArr[i].x - j] == '0')
				{
					// 아이템 드랍 확률을 구한다.
					if (rand() % 100 < 20)
					{
						int iPerCent = rand() % 100;
						if (iPerCent < 40)
							Maze[pBombArr[i].y][pBombArr[i].x - j] = '5';

						else if (iPerCent < 70)
							Maze[pBombArr[i].y][pBombArr[i].x - j] = '6';

						else
							Maze[pBombArr[i].y][pBombArr[i].x - j] = '7';
					}

					else
						Maze[pBombArr[i].y][pBombArr[i].x - j] = '1';
				}
				// 플레이어가 폭탄에 맞았을때 시작점으로 보낸다.
				if (pPlayer->tPos.x == pBombArr[i].x - 1 && pPlayer->tPos.y == pBombArr[i].y)
				{
					pPlayer->tPos.x = 0;
					pPlayer->tPos.y = 0;
				}
			}

			if (pBombArr[i].x + j < 20)
			{
				if (Maze[pBombArr[i].y][pBombArr[i].x + j] == '0')
				{
					// 아이템 드랍 확률을 구한다.
					if (rand() % 100 < 20)
					{
						int iPerCent = rand() % 100;
						if (iPerCent < 40)
							Maze[pBombArr[i].y][pBombArr[i].x + j] = '5';

						else if (iPerCent < 70)
							Maze[pBombArr[i].y][pBombArr[i].x + j] = '6';

						else
							Maze[pBombArr[i].y][pBombArr[i].x + j] = '7';
					}

					else
						Maze[pBombArr[i].y][pBombArr[i].x + j] = '1';
				}
				// 플레이어가 폭탄에 맞았을때 시작점으로 보낸다.
				if (pPlayer->tPos.x == pBombArr[i].x + j && pPlayer->tPos.y == pBombArr[i].y)
				{
					pPlayer->tPos.x = 0;
					pPlayer->tPos.y = 0;
				}
			}
		}
	}

	*pBombCount = 0;
}

int main()
{
	srand((unsigned int)time(0));

	// 20 x 20 미로를 만들어준다.
	char	strMaze[21][21] = {};

	PLAYER	tPlayer = {};
	POINT	tStartPos;
	POINT	tEndPos;

	tPlayer.iBombPower = 1;

	int iBombCount = 0;

	POINT	tBombPos[5];

	// 미로를 설정한다.
	SetMaze(strMaze, &tPlayer, &tStartPos, &tEndPos);
	
	while (true)
	{
		system("cls");
		// 미로를 출력한다.
		Output(strMaze, &tPlayer);

		if (tPlayer.tPos.x == tEndPos.x && tPlayer.tPos.y == tEndPos.y)
		{
			cout << "도착했습니다." << endl;
			break;
		}

		cout << "t : 폭탄설치 u : 폭탄 터뜨리기 i : 벽밀기 ON/OFF" << endl;
		cout << "w : 위 s : 아래 a : 왼쪽 d : 오른쪽 q : 종료 : ";
		char	cInput = _getch();

		if (cInput == 'q' || cInput == 'Q')
			break;

		else if (cInput == 't' || cInput == 'T')
			CreateBomb(strMaze, &tPlayer, tBombPos, &iBombCount);

		else if (cInput == 'u' || cInput == 'U')
			Fire(strMaze, &tPlayer, tBombPos, &iBombCount);

		else if (cInput == 'i' || cInput == 'I')
		{
			if (tPlayer.bWallPush)
				tPlayer.bPushOnOff = !tPlayer.bPushOnOff;
		}

		else
			MovePlayer(strMaze, &tPlayer, cInput);
	}

	return 0;
}