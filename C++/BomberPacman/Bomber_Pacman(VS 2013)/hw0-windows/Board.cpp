/*
 * Board.cpp
 *
 *  Created on: May 2, 2014
 *      Author: Sibt ul Hussain
 */
/*
* Board.cpp
*
*  Created on: May 2, 2014
*      Author: Sibt ul Hussain
*/

#include "Board.h"
#include <cstdio>
const int Board::BOARD_X = 19;
const int Board::BOARD_Y = 14;
//here's Bomberman's crazy board

// Note that all these enum constants from NILL onwards
// have been given numbers in increasing order
// e.g. NILL=0, and so on
// and these numbers are represented in the board array...
enum BoardParts {
	NILL, S_BRICK, G_BRICK, R_BRICK
};
// defining some utility functions...

static int board_array[Board::BOARD_Y][Board::BOARD_X] = { { 0 } };
#ifdef WITH_TEXTURES
const int nbricks = 3;

GLuint texture[nbricks];
GLuint tid[nbricks];
string tnames[] = { "solid.png", "brick.png", "brick-green.png" };
GLuint mtid[nbricks];
int cwidth = 60, cheight = 60; // 60x60 pixels bricks...

void RegisterTextures()
/*Function is used to load the textures from the
* files and display*/
{
	// allocate a texture name
	glGenTextures(nbricks, tid);

	vector<unsigned char> data;
	//ofstream ofile("image-data.bin", ios::binary | ios::out);
	// now load each bricks data...

	for (int i = 0; i < nbricks; ++i) {

		// Read current brick

		ReadImage(tnames[i], data);
		if (i == 0) {
			int length = data.size();
			//ofile.write((char*) &length, sizeof(int));
		}
		//ofile.write((char*) &data[0], sizeof(char) * data.size());

		cout << " Texture Id=" << tid[i] << endl;
		mtid[i] = tid[i];
		// select our current texture
		glBindTexture(GL_TEXTURE_2D, tid[i]);

		// select modulate to mix texture with color for shading
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		// when texture area is small, bilinear filter the closest MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR_MIPMAP_NEAREST);
		// when texture area is large, bilinear filter the first MIP map
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// if wrap is true, the texture wraps over at the edges (repeat)
		//       ... false, the texture ends at the edges (clamp)
		bool wrap = true;
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
			wrap ? GL_REPEAT : GL_CLAMP);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
			wrap ? GL_REPEAT : GL_CLAMP);

		// build our texture MIP maps
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, cwidth, cheight, GL_RGB,
			GL_UNSIGNED_BYTE, &data[0]);
	}
	//ofile.close();

}
void Drawbrick(const BoardParts &cname, float fx, float fy, float fwidth,
	float fheight)
	/*Draws a specfic brick at given position coordinate
	* sx = position of x-axis from left-bottom
	* sy = position of y-axis from left-bottom
	* cwidth= width of displayed brick in pixels
	* cheight= height of displayed bricki pixels.
	* */
{

	glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mtid[cname - 1]);
	//	glTranslatef(0, 0, 0);
	//	glRotatef(-M_PI / 2, 0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2d(0.0, 0.0);
	glVertex2d(fx, fy);
	glTexCoord2d(1.0, 0.0);
	glVertex2d(fx + fwidth, fy);
	glTexCoord2d(1.0, 1.0);
	glVertex2d(fx + fwidth, fy + fheight);
	glTexCoord2d(0.0, 1.0);
	glVertex2d(fx, fy + fheight);
	glEnd();

	glColor4f(1, 1, 1, 1);

	//	glBindTexture(GL_TEXTURE_2D, 0);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();

	//glutSwapBuffers();
}
#endif
// Destructor
Board::~Board(void) {
}
void Board::InitalizeBoard(int w, int h) {
	width = w;
	height = h;
	for (int i = 0; i < BOARD_Y - 1; ++i) {
		for (int j = 0; j < BOARD_X; ++j) {
			if (i == 0 || i == BOARD_Y - 2 || j == 0 || j == BOARD_X - 1)
				board_array[i][j] = S_BRICK;
			else if (i % 2 == 0 && j % 2 == 0)
				board_array[i][j] = S_BRICK;
			else
				board_array[i][j] =
				(GetRandInRange(0, 15)) < 11 ? NILL :
				(GetRandInRange(0, 10)) < 5 ? G_BRICK : R_BRICK;
			if (board_array[i][j] == G_BRICK || board_array[i][j] == R_BRICK)
				nBBricks++;
		}
	}
	if (board_array[3][3] != NILL)
	{
		nBBricks--;
	}
	if (board_array[3][BOARD_X-4] != NILL)
	{
		nBBricks--;
	}
	if (board_array[BOARD_Y - 5][3] != NILL)
	{
		nBBricks--;
	}
	if (board_array[BOARD_Y - 5][BOARD_X-4] != NILL)
	{
		nBBricks--;
	}
	/*if (board_array[6][7] != NILL)
	{
		nBBricks--;
	}*/
	board_array[BOARD_Y - 5][BOARD_X - 4] = NILL;
	board_array[BOARD_Y - 5][3] = NILL;
	board_array[3][BOARD_X - 4] = NILL;
	board_array[3][3] = NILL;
	//board_array[6][7] = NILL;
	cbBricks = nBBricks;

	int ** temparr = new int*[BOARD_Y];
	for (int i = 0; i < BOARD_Y; ++i)
	{
		temparr[i] = new int[BOARD_X];
		for (int j = 0; j < BOARD_X; ++j)
		{
			temparr[i][j] = board_array[i][j];
		}
	}
	int ii = BOARD_Y - 3, jj = 1;
	for (ii = BOARD_Y - 3; ii > 0 && board_array[ii][jj] != NILL; --ii)
	{
		for (jj = 1; jj < BOARD_X - 2 && board_array[ii][jj]!=NILL; ++jj) {}
	}
	bm.bAcordinates(jj, ii);
	Primitive::initBoard(BOARD_X, BOARD_Y, temparr);
	e[0] = new Blinky;
	e[1] = new Pinky;

	Primitive::BoardArray[3][3] = ENERGIZER;
	Primitive::BoardArray[3][BOARD_X - 4] = ENERGIZER;
	Primitive::BoardArray[BOARD_Y-5][BOARD_X - 4] = ENERGIZER;
	Primitive::BoardArray[BOARD_Y-5][3] = ENERGIZER;
	nitems = citems = 4;
	for (int i = 0; i < BOARD_Y - 2; ++i)
	{
		for (int j = 0; j < BOARD_X - 1; ++j)
		{
			if (Primitive::BoardArray[i][j] == Nill)
			{
				Primitive::BoardArray[i][j] = DOT;
				nitems++;
				citems++;
			}
		}
	}

	for (int i = 0; i < BOARD_Y; ++i)
		delete[] temparr[i];
	delete temparr;
}
//Constructor
Board::Board(int xsize, int ysize): dot(0,0), ene(0,0), g(0,0){ 
	xcellsize = xsize;
	ycellsize = ysize;
	pcolor = CHOCOLATE;
	bcolor = ORANGE_RED;
	gcolor = PINK;


	time = 180;
	t = 0;
	nitems = 0;
	citems = 0;
	nenemies = 2;
	nBBricks = 0;
	cbBricks = 0;
	GameOver = false;
	ispaused = false;
	//set up board
}

void Board::Draw() {
	glColor3f(0, 0, 1);
	glPushMatrix();

#ifdef WITH_TEXTURES
	for (int i = BOARD_Y - 2, y = 0; i >= 0; --i, y += xcellsize) {
		for (int j = 0, x = 0; j < BOARD_X; j++, x += (ycellsize)) {
			//			cout << " " << board_array[i][j] << " " << flush;
			switch (board_array[i][j]) {
			case NILL:
				// Empty space
				break;
			case S_BRICK:
			case G_BRICK:
			case R_BRICK:
				float fwidth = (float)(xcellsize) / width * 2, fheight =
					(float)ycellsize / height * 2;
				float fx = (float)(x - 10) / width * 2 - 1, fy = (float)y
					/ height * 2 - 1;

				/*Drawbrick((BoardParts) board_array[i][j], fx, fy, fwidth,
				fheight);*/
				Drawbrick((BoardParts)board_array[i][j], (float)x / width - 1, (float)y / height - 1, fwidth,
					fheight);
				break;
			}
		}
	}
#else
	
	if ((citems <= 0 && cbBricks <= 0) || bm.getlives() <= 0 || time <= 0)
	{
		if (GameOver == false)
		{
			GameOver = true;
			t = time;
		}
		//if (abs(time - t)<=3 && abs(time-t>=0)) 
		{

		if (citems <= 0 && cbBricks <= 0)
		{
			//for (; time - i >= 0 && time - i <= 3;)
			{
				DrawString(300, 600, "CONGRATULATIONS!!!", colors[BLUE_VIOLET]);
				DrawString(330, 550, "YOU WON", colors[BLUE_VIOLET]);
				DrawString(280, 500, "YOUR SCORE: ", colors[LAWN_GREEN]);
				DrawString(500, 500, Num2Str(bm.getscore()), colors[LAWN_GREEN]);
				DrawString(600, 250, "CREDITS: SOHAIB BAQAI", colors[SKY_BLUE]);
			}
		}
		else if (bm.getlives() <= 0)
		{
			//for (; time - i >= 0 && time - i <= 3;)
			{
				DrawString(330, 550, "YOU LOSE", colors[BLUE_VIOLET]);
				//DrawString(280, 500, "YOUR SCORE: ", colors[BLUE_VIOLET]);
				//DrawString(370, 500, Num2Str(b->bm.getscore()), colors[BLUE_VIOLET]);
				DrawString(600, 250, "CREDITS: SOHAIB BAQAI", colors[SKY_BLUE]);
			}
		}
	}
		//else
		//	exit(1);
	}
	else
	{
		for (int i = BOARD_Y - 2, y = 0; i >= 0; --i, y += xcellsize) {
			for (int j = 0, x = 0; j < BOARD_X; j++, x += (ycellsize)) {
				//			cout <<      " " << board_array[i][j] << " " << flush;
				switch (board_array[i][j]) {
				case NILL:
					// Empty space
					if (Primitive::BoardArray[i][j] == DOT)
					{
						dot.bAcordinates(j, i);
						dot.draw();
					}
					else if (Primitive::BoardArray[i][j] == ENERGIZER)
					{
						ene.bAcordinates(j, i);
						ene.draw();
					}
					else if (Primitive::BoardArray[i][j] == GIFT)
					{
						g.bAcordinates(j, i);
						g.draw();
					}
					break;
				case S_BRICK:
					DrawRectangle(x , y, ycellsize, xcellsize,
						colors[SLATE_GRAY]);
					//DrawLine(x - 10, y, x - 10 + ycellsize, y, 4, colors[BLACK]);
					break;
				case G_BRICK:
					DrawRectangle(x, y, ycellsize, xcellsize,
						colors[LIGHT_GREEN]);
					break;
				case R_BRICK:
					DrawRectangle(x , y, ycellsize, xcellsize, colors[RED]);
					break;
				}
			}
		}

		for (int i = 0; i < nenemies; ++i)
			(e[i][0]).draw();
		bm.draw();
		if (bomb.blast(time))
		{
			BLAST(pair<int, int>(bomb.getBoardCordinates().first, bomb.getBoardCordinates().second));
		}
		if (bomb1.blast(time))
		{
			BLAST(pair<int, int>(bomb1.getBoardCordinates().first, bomb1.getBoardCordinates().second));
		}
		if (bomb2.blast(time))
		{
			BLAST(pair<int, int>(bomb2.getBoardCordinates().first, bomb2.getBoardCordinates().second));
		}
	}
	if (ispaused)
		DrawString(400, 400,"GAME PAUSED", colors[ORANGE]);
#endif
	glPopMatrix();
}

void Board::GetInitPinkyPosition(int &x, int &y) {
	x = xcellsize * 3;
	y = (BOARD_Y - 3) * ycellsize;
}
void Board::GetInitTextPosition(int &x, int &y) {
	x = xcellsize;
	y = (BOARD_Y - 1) * ycellsize + ycellsize / 2;
}

void Board::addTime(int a)
{
	time -= a;
}

int Board::getTime()
{
	return time;
}

void Board::Move()
{
	if (GameOver==false && !ispaused)
	{
	int first = bm.getBoardCordinates().first;
	int second = bm.getBoardCordinates().second;

	for (int i = 0; i < nenemies; ++i)
	{
		if (Enemy::getcMode() == Chase)
		{
			first = bm.getBoardCordinates().first;
			second = bm.getBoardCordinates().second;
		}
		else if (Enemy::getcMode() == Scattered)
		{

			vector<pair<int, int>>corners;
			corners.push_back(pair<int, int>(0, 0));
			corners.push_back(pair<int, int>(0, BOARD_X - 1));
			corners.push_back(pair<int, int>(BOARD_Y - 2, BOARD_X - 1));
			corners.push_back(pair<int, int>(BOARD_Y - 2, 0));
			/*pair<int, int>t = e[i]->NearestCorner();
			int j;
			for (j = 0; corners[j]!=t && j<corners.size();++j)  {}
			if (corners[j] == t)
			{*/
			first = corners[i].first;
			second = corners[i].second;
			e[i]->move(pair<int, int>(first, second));
			/*	corners.erase(corners.begin() + j);
			}
			else
				n.push_back(i);*/
		}
		else if (Enemy::getcMode() == Frightened)
		{
			first = rand() % BOARD_Y - 1;
			second = rand() % BOARD_X;

		}
		e[i]->move(pair<int, int>(first, second));
	}
	if (bm.getcDirection()!= STATIONARY)
		bm.Move(bm.getcDirection());
	/*if (Enemy::getcMode() == Scattered && !n.empty())
	{
		for (int i = 0; i < n.size(); ++i)
		{
			first = corners[i].first;
			second = corners[i].second;
			e[n[i]]->move(pair<int, int>(first, second));
		}
	}*/
	}
}

bool Board::enemycollides(pair<int, int>bmp, pair<int, int>ep)
{
	if (abs(ep.first - bmp.first) <= (ycellsize/3 *2) && abs(ep.first - bmp.first) >= 0)
	{
		if (abs(ep.second - bmp.second) <= (xcellsize/3 *2) && abs(ep.second - bmp.second) >= 0)
			return true;
	}

	if (abs(ep.second - bmp.second) <= (xcellsize/3 *2) && abs(ep.second - bmp.second) >= 0)
	{
		if (abs(ep.first - bmp.first) <= (ycellsize/3 *2) && abs(ep.first - bmp.first) >= 0)
			return true;
	}

	return false;
}

void Board::collision()
{
	if (!GameOver)
	{
		pair<int, int>bmp(bm.getpixels());
		for (int i = 0; i < nenemies; ++i)
		{
			pair<int, int>ep(e[i][0].getpixels());
			if (enemycollides(pair<int, int>(bmp.first, bmp.second), pair<int, int>(ep.first, ep.second)))
			{
				if (Enemy::getcMode() == Frightened)
				{
					e[i][0].spawn();
					bm.addscore(200);
				}
				else
					bm.spawn();
			}
		}
		if (Primitive::BoardArray[bm.getBoardCordinates().first][bm.getBoardCordinates().second] == DOT
			|| Primitive::BoardArray[bm.getBoardCordinates().first][bm.getBoardCordinates().second] == ENERGIZER
			|| Primitive::BoardArray[bm.getBoardCordinates().first][bm.getBoardCordinates().second] == GIFT)
		{
			if (Primitive::BoardArray[bm.getBoardCordinates().first][bm.getBoardCordinates().second] == DOT)
			{
				citems--;
				bm.addscore(10);
			}
			else if (Primitive::BoardArray[bm.getBoardCordinates().first][bm.getBoardCordinates().second] == ENERGIZER)
			{
				citems--;
				Enemy::ChangeMode(Frightened, time);
				bm.addscore(100);
			}
			else if (Primitive::BoardArray[bm.getBoardCordinates().first][bm.getBoardCordinates().second] == GIFT)
				bm.addlives();
			Primitive::BoardArray[bm.getBoardCordinates().first][bm.getBoardCordinates().second] = Nill;

		}
	}
}

void Board::BLAST(pair<int,int>t)
{
	
	if (board_array[t.first][t.second] != S_BRICK)
	{
		for (int i = 0; i < nenemies; ++i)
		{
			if (t == e[i]->getBoardCordinates())
			{
				bm.addscore(200);
				e[i]->spawn();
			}
		}
		if (t == bm.getBoardCordinates())
			bm.spawn();
	}

	pair<int, int>t1(t.first + 1, t.second);
	if (board_array[t1.first][t1.second] != S_BRICK)
	{
		for (int i = 0; i < nenemies; ++i)
		{
			if (t1 == e[i]->getBoardCordinates())
			{
				bm.addscore(200);
				e[i]->spawn();
			}
		}
		if (t1 == bm.getBoardCordinates())
			bm.spawn();
		else if (board_array[t1.first][t1.second] == G_BRICK)
		{
			greenBlast(pair<int, int>(t1.first, t1.second));
		}
		else if (board_array[t1.first][t1.second] == R_BRICK)
		{
			bm.addscore(100);
			board_array[t1.first][t1.second] = NILL;
			Primitive::BoardArray[t1.first][t1.second] = Nill;
			cbBricks--;
			int cb = nBBricks*0.7;
			if (cbBricks == cb)
			{
				nenemies++;
				e[2] = new Inky;
			}
			cb = nBBricks*0.3;
			if (cbBricks == cb)
			{
				nenemies++;
				e[3] = new Clyde;
			}
			redBLAST(pair<int,int>(t1.first, t1.second));
		}
	}
	t1.first = t.first - 1;
	if (board_array[t1.first][t1.second] != S_BRICK)
	{
		for (int i = 0; i < nenemies; ++i)
		{
			if (t1 == e[i]->getBoardCordinates())
			{
				bm.addscore(200);
				e[i]->spawn();
			}
		}
		if (t1 == bm.getBoardCordinates())
			bm.spawn();
		else if (board_array[t1.first][t1.second] == G_BRICK)
		{
			greenBlast(pair<int, int>(t1.first, t1.second));
		}
		else if (board_array[t1.first][t1.second] == R_BRICK)
		{
			bm.addscore(50);
			board_array[t1.first][t1.second] = NILL;
			Primitive::BoardArray[t1.first][t1.second] = Nill;
			cbBricks--;
			int cb = nBBricks*0.7;
			if (cbBricks == cb)
			{
				nenemies++;
				e[2] = new Inky;
			}
			cb = nBBricks*0.3;
			if (cbBricks == cb)
			{
				nenemies++;
				e[3] = new Clyde;
			}
			redBLAST(pair<int, int>(t1.first, t1.second));
		}
	}

	t1.first = t.first;
	t1.second = t.second + 1;
	if (board_array[t1.first][t1.second] != S_BRICK)
	{
		for (int i = 0; i < nenemies; ++i)
		{
			if (t1 == e[i]->getBoardCordinates())
			{
				bm.addscore(200);
				e[i]->spawn();
			}
		}
		if (t1 == bm.getBoardCordinates())
			bm.spawn();
		else if (board_array[t1.first][t1.second] == G_BRICK)
		{
			greenBlast(pair<int, int>(t1.first, t1.second));
		}
		else if (board_array[t1.first][t1.second] == R_BRICK)
		{
			bm.addscore(50);
			board_array[t1.first][t1.second] = NILL;
			Primitive::BoardArray[t1.first][t1.second] = Nill;
			cbBricks--;
			int cb = nBBricks*0.7;
			if (cbBricks == cb)
			{
				nenemies++;
				e[2] = new Inky;
			}
			cb = nBBricks*0.3;
			if (cbBricks == cb)
			{
				nenemies++;
				e[3] = new Clyde;
			}
			redBLAST(pair<int, int>(t1.first, t1.second));
		}
	}

	t1.second = t.second - 1;
	if (board_array[t1.first][t1.second] != S_BRICK)
	{
		for (int i = 0; i < nenemies; ++i)
		{
			if (t1 == e[i]->getBoardCordinates())
			{
				bm.addscore(200);
				e[i]->spawn();
			}
		}
		if (t1 == bm.getBoardCordinates())
			bm.spawn();
		else if (board_array[t1.first][t1.second] == G_BRICK)
		{
			greenBlast(pair<int, int>(t1.first, t1.second));
		}
		else if (board_array[t1.first][t1.second] == R_BRICK)
		{
			bm.addscore(50);
			board_array[t1.first][t1.second] = NILL;
			Primitive::BoardArray[t1.first][t1.second] = Nill;
			cbBricks--;
			int cb = nBBricks*0.7;
			if (cbBricks == cb)
			{
				nenemies++;
				e[2] = new Inky;
			}
			cb = nBBricks*0.3;
			if (cbBricks == cb)
			{
				nenemies++;
				e[3] = new Clyde;
			}
			redBLAST(pair<int, int>(t1.first, t1.second));
		}
	}
}

void Board::redBLAST(pair<int, int>t)
{
	pair<int, int> t1(t.first + 1, t.second);
	if (board_array[t1.first][t1.second] != S_BRICK)
	{
		if (board_array[t1.first][t1.second] == G_BRICK)
		{
			greenBlast(pair<int, int>(t1.first, t1.second));
		}
		else if (board_array[t1.first][t1.second] == R_BRICK)
		{
			bm.addscore(50);
			board_array[t1.first][t1.second] = NILL;
			Primitive::BoardArray[t1.first][t1.second] = Nill;
			cbBricks--;
			int cb = nBBricks*0.7;
			if (cbBricks == cb)
			{
				nenemies++;
				e[2] = new Inky;
			}
			cb = nBBricks*0.3;
			if (cbBricks == cb)
			{
				nenemies++;
				e[3] = new Clyde;
			}
			redBLAST(pair<int, int>(t1.first, t1.second));
		}
	}
	t1.first = t.first - 1;
	if (board_array[t1.first][t1.second] != Nill)
	{
		if (board_array[t1.first][t1.second] == G_BRICK)
		{
			greenBlast(pair<int, int>(t1.first, t1.second));
		}
		else if (board_array[t1.first][t1.second] == R_BRICK)
		{
			bm.addscore(50);
			board_array[t1.first][t1.second] = NILL;
			Primitive::BoardArray[t1.first][t1.second] = Nill;
			cbBricks--;
			int cb = nBBricks*0.7;
			if (cbBricks == cb)
			{
				nenemies++;
				e[2] = new Inky;
			}
			cb = nBBricks*0.3;
			if (cbBricks == cb)
			{
				nenemies++;
				e[3] = new Clyde;
			}
			redBLAST(pair<int, int>(t1.first, t1.second));
		}
	}

	t1.first = t.first;
	t1.second += 1;
	if (board_array[t1.first][t1.second] != Nill)
	{
		if (board_array[t1.first][t1.second] == G_BRICK)
		{
			greenBlast(pair<int, int>(t1.first, t1.second));
		}
		else if (board_array[t1.first][t1.second] == R_BRICK)
		{
			bm.addscore(50);
			board_array[t1.first][t1.second] = NILL;
			Primitive::BoardArray[t1.first][t1.second] = Nill;
			cbBricks--;
			int cb = nBBricks*0.7;
			if (cbBricks == cb)
			{
				nenemies++;
				e[2] = new Inky;
			}
			cb = nBBricks*0.3;
			if (cbBricks == cb)
			{
				nenemies++;
				e[3] = new Clyde;
			}
			redBLAST(pair<int, int>(t1.first, t1.second));
		}
	}

	t1.second = t.second-1;
	if (board_array[t1.first][t1.second] != Nill)
	{
		if (board_array[t1.first][t1.second] == G_BRICK)
		{
			greenBlast(pair<int, int>(t1.first, t1.second));
		}
		else if (board_array[t1.first][t1.second] == R_BRICK)
		{
			bm.addscore(50);
			board_array[t1.first][t1.second] = NILL;
			Primitive::BoardArray[t1.first][t1.second] = Nill;
			cbBricks--;
			int cb = nBBricks*0.7;
			if (cbBricks == cb)
			{
				nenemies++;
				e[2] = new Inky;
			}
			cb = nBBricks*0.3;
			if (cbBricks == cb)
			{
				nenemies++;
				e[3] = new Clyde;
			}
			redBLAST(pair<int, int>(t1.first, t1.second));
		}
	}
}

int Board::getcbricks()const
{
	return cbBricks;
}
int Board::getcitems()const
{
	return citems;

}

bool Board::getGameStatus()
{
	return GameOver;
}

void Board::greenBlast(pair<int, int>t)
{
	bm.addscore(50);
	board_array[t.first][t.second] = NILL;
	Primitive::BoardArray[t.first][t.second] = Nill;
	int bonus = rand() % 8;
	if (bonus == 0)
	{
		Primitive::BoardArray[t.first][t.second] = ENERGIZER;
		citems++;
	}
	else if (bonus == 1)
		Primitive::BoardArray[t.first][t.second] = GIFT;
	cbBricks--;
	int cb = nBBricks*0.7;
	if (cbBricks == cb)
	{
		nenemies++;
		e[2] = new Inky;
	}
	cb = nBBricks*0.3;
	if (cbBricks == cb)
	{
		nenemies++;
		e[3] = new Clyde;
	}
}