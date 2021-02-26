/*
* Board.h
*
*  Created on: May 2, 2014
*      Author: Sibt ul Hussain
*/
#ifndef _BOARD_H_
#define _BOARD_H_

//#include <GL/glut.h>
#include <iostream>
#include "Classes.h"
#include "util.h"
using namespace std;
class Board {
private:

	int xcellsize, ycellsize;
	int width, height;
	ColorNames pcolor, bcolor, gcolor;
	int time;
	int nBBricks;
	int cbBricks;
	int nenemies;
	Energizer ene;
	Dot dot;
	Gift g;
	int nitems;
	int citems;
	bool GameOver;
public:
	Enemy * e[4];
	BomberMan bm;
	Bomb bomb;
	Bomb bomb1;
	Bomb bomb2;
	void BLAST(pair<int, int>);
	void redBLAST(pair<int, int>);
	void greenBlast(pair<int, int>);
	void Move();
	void collision();
	bool enemycollides(pair<int, int>, pair<int, int>);
	int getcbricks()const;
	int getcitems()const;
	bool getGameStatus();
	bool ispaused;

	static const int BOARD_X;
	static const int BOARD_Y;
	int t;
	Board(int xsize = 8, int ysize = 8);
	~Board(void);
	void InitalizeBoard(int, int);
	//draw the board
	void Draw();

	static int GetBoardX() {
		return BOARD_X;
	}
	static int GetBoardY() {
		return BOARD_Y;
	}
	int GetMidX() {
		return BOARD_X * xcellsize / 2.0;
	}
	int GetMidY() {
		return BOARD_Y * ycellsize / 2.0;
	}
	int GetCellSize() {
		return xcellsize;
	}
	void GetInitBombermanPosition(int &x, int &y) {
		x = xcellsize + xcellsize / 2;
		y = ycellsize + ycellsize / 2;
	}

	void GetInitTextPosition(int &x, int &y);
	void GetInitPinkyPosition(int &x, int &y);
	//	int GetMidx

	void addTime(int = 1);
	int getTime();
};

#ifdef WITH_TEXTURES
void RegisterTextures();
#endif

#endif
