#include "Classes.h"

int Primitive::xpixelsize = 60;
int Primitive::ypixelsize = 60;

int Primitive::xboardsize = 19;
int Primitive::yboardsize = 14;
int** Primitive::BoardArray = new int *[yboardsize];


int  Primitive::xpixels(int x)
{
	if (x > -1)
		xpixelsize = x;
	return xpixelsize;
}

int Primitive::ypixels(int y)
{
	if (y > -1)
		ypixelsize = y;
	return ypixelsize;
}

void Primitive::pCordinates(float x, float y)
{
	pixelCordinates.first = y;
	pixelCordinates.second = x;
}

void Primitive::bAcordinates(int x, int y)
{
	//Sets any primitive on the center of the given board array cordinates

	boardArrayCordinates.first = y;
	boardArrayCordinates.second = x;
	pixelCordinates.second = (boardArrayCordinates.second * xpixelsize) + (xpixelsize / 2);
	pixelCordinates.first = ((yboardsize-2-boardArrayCordinates.first) * ypixelsize) + (ypixelsize / 2);
}

void Primitive::initBoard(int x, int y, int **BA)
{
	if (BoardArray != NULL)
	{
		/*for (int i = 0; i < yboardsize; ++i)
		{
			if (BoardArray[i]==NULL)
				delete[] BoardArray[i];
		}*/
		delete[] BoardArray;
	}
	xboardsize = x;
	yboardsize = y;
	BoardArray = new int *[yboardsize];
	for (int i = 0; i < yboardsize; ++i)
	{
		BoardArray[i] = new int[xboardsize];
	}

	for (int i = 0; i < yboardsize; ++i)
	{
		for (int j = 0; j < xboardsize; ++j)
		{
			BoardArray[i][j] = BA[i][j];
		}
	}
}


void Primitive::setAtBoard(int x, int y, Primitives p)
{
	if (x >= 1 && y >= 1 && x < xboardsize - 1 && y < yboardsize - 1)
		BoardArray[y][x] = p;
}


//				CREATURE CLASS


Creature::Creature(int bx, int by, ColorNames c)
{
	color = c;
	bAcordinates(bx, by);
}

void Creature::Move(Direction dir)
{
	if (CanMove(dir, 4))
	{
		cDirection = dir;
		if (cDirection == UP)
			pixelCordinates.first += 5;
		else if (cDirection == DOWN)
			pixelCordinates.first -= 5;
		else if (cDirection == LEFT)
			pixelCordinates.second -= 5;
		else if (cDirection == RIGHT)
			pixelCordinates.second += 5;
		
		//cout << boardArrayCordinates.second << " " << boardArrayCordinates.first << endl;
		
		//SWAPPING
	/*	int ty = boardArrayCordinates.first;
		int tx = boardArrayCordinates.second;
		int temp1 = BoardArray[ty][tx];*/
		setboardxy();
		/*int temp2 = BoardArray[boardArrayCordinates.first][boardArrayCordinates.second];
		BoardArray[boardArrayCordinates.first][boardArrayCordinates.second] = temp1;
		BoardArray[ty][tx] = temp2;*/
	}
}

bool Creature::CanMove(Direction dir, int pix)
{
	int y = (int)pixelCordinates.first%ypixelsize;
	int x = (int)pixelCordinates.second%xpixelsize;
	//cout << x << "  " << y << endl;
	bool isInRange = (x>=(xpixelsize/2 - speed) && x <=(xpixelsize/2+speed)) || 
		(y >= (ypixelsize/2 - speed) && y <= (ypixelsize/2 + speed));
	if (isInRange)
	{
		if (dir == UP && (x >= (xpixelsize/2 - speed) && x <= (xpixelsize/2 + speed)))
		{
			int xx = boardArrayCordinates.second;
			int yy = yboardsize - 2 - (((int)pixelCordinates.first + speed + (ypixelsize / 2)) / ypixelsize);
			//cout << xx << "  " << yy << "    " << BoardArray[yy][xx] << endl;
			if (BoardArray[yy][xx] == Nill || BoardArray[yy][xx] > 3)
				return true;
			return false;
		}
		else if (dir == DOWN && (x >= (xpixelsize / 2 - speed) && x <= (xpixelsize / 2 + speed)))
		{
			int xx = boardArrayCordinates.second;
			int yy = yboardsize - 2 - (((int)pixelCordinates.first - speed - (ypixelsize / 2)) / ypixelsize);
			//cout << xx << "  " << yy << "    " << BoardArray[yy][xx] << endl;
			if (BoardArray[yy][xx] == Nill || BoardArray[yy][xx] > 3)
				return true;
			return false;
		}
		else if (dir == LEFT && (y >= (ypixelsize / 2 - speed) && y <= (ypixelsize / 2 + speed)))
		{
			int xx = ((int)pixelCordinates.second - speed - (xpixelsize / 2)) / xpixelsize;
			int yy = boardArrayCordinates.first;
			//cout << xx << "  " << yy << "    " << BoardArray[yy][xx] << endl;
			if (BoardArray[yy][xx] == Nill || BoardArray[yy][xx] > 3)
				return true;
			return false;
		}
		else if (dir == RIGHT && (y >= (ypixelsize / 2 - speed) && y <= (ypixelsize / 2 + speed)))
		{
			int xx = ((int)pixelCordinates.second + speed + (xpixelsize / 2)) / xpixelsize;
			int yy = boardArrayCordinates.first;
			//cout << xx << "  " << yy << "    " << BoardArray[yy][xx] << endl;
			if (BoardArray[yy][xx] == Nill || BoardArray[yy][xx] > 3)
				return true;
			return false;
		}
	}

	return false;
}

void Creature::setboardx()
{
	//Sets the Board X Cordinate based on the center of any creature
	boardArrayCordinates.second = pixelCordinates.second / xpixelsize;
}

void Creature::setboardy()
{
	boardArrayCordinates.first = yboardsize-1-(pixelCordinates.first / ypixelsize);
}

void Creature::setboardxy()
{
	setboardx();
	setboardy();
}

void Creature::pCordinates(float x, float y)
{
	pixelCordinates.first = y;
	pixelCordinates.second = x;
	setboardx();
	setboardy();
}

pair<float, float> & Creature::getpixels()
{
	return pixelCordinates;
}

pair<int, int> & Creature::getBoardCordinates()
{
	return boardArrayCordinates;
}


//				BOMBERMAN CLASS


BomberMan::BomberMan(int bx, int by, ColorNames c): Creature(bx, by, c)
{
	cDirection = STATIONARY;
	NextDirection = STATIONARY;
	score = 0;
	lives = 5;
	speed = 7;
	//color = VIOLET;
}

float xxx = 90;
void BomberMan::Move(Direction dir)
{
	if (dir!= cDirection)
		NextDirection = dir;
	if (CanMove(NextDirection))
		cDirection = NextDirection;
	Creature::Move(cDirection);
}

int BomberMan::getlives() const
{
	return lives;
}

void BomberMan::draw()
{
	DrawBomberman(pixelCordinates.second,pixelCordinates.first, xpixelsize/2-5, color, cDirection);
}

Direction Creature::getcDirection() const
{
	return cDirection;
}

void 
BomberMan::spawn()
{
	int ii = 1+rand()%(yboardsize-3), jj = 1+rand()%(xboardsize-2);
	for (ii = yboardsize/2; ii > 0 && BoardArray[ii][jj] != Nill && BoardArray[ii][jj]<=3; --ii)
	{
		for (jj = xboardsize/2; jj < xboardsize - 1 && BoardArray[ii][jj] != Nill 
			&& BoardArray[ii][jj] <= 3; ++jj) {}
	}
	bAcordinates(jj, ii);
	lives--;
	cDirection = STATIONARY;
	NextDirection = STATIONARY;
}

void BomberMan::addscore(int s)
{
	score += s;
}

int BomberMan::getscore()
{
	return score;
}

void BomberMan::addlives(int l)
{
	lives += l;
}

//				ENEMY CLASS


Mode Enemy::cMode = Chase;

int Enemy::ctime = 180;

Enemy::Enemy( int bx, int by, ColorNames c):Creature(bx, by, c) {
	t = pixelCordinates;
	speed = 1;
}

void Enemy::ChangeMode(Mode m, int t)
{
	cMode = m;
	ctime = t;
}

void Enemy::ChangeMode(int t)
{
	if (cMode == Frightened&&abs (t-ctime)>=10)
	{
		cMode = Chase;
		ctime = t;
	}
	if (cMode == Chase && abs (t - ctime) >= 20)
	{
		cMode = Scattered;
		ctime = t;
	}
	if (cMode == Scattered && abs (t - ctime) >= 7)
	{
		cMode = Chase;
		ctime = t;
	}
}

pair<int, int>Enemy::NearestCorner()
{
	int min = 100;
	pair<int, int>nearestC(0, 0);
	if (Distance(pair<int, int>(0, 0), 
		pair<int, int>(boardArrayCordinates.first,boardArrayCordinates.second)) < min)
	{
		min = Distance(pair<int, int>(0, 0),
			pair<int, int>(boardArrayCordinates.first, boardArrayCordinates.second));
	}

	if (Distance(pair<int, int>(0, xboardsize - 1),
		pair<int, int>(boardArrayCordinates.first, boardArrayCordinates.second)) < min)
	{
		min = Distance(pair<int, int>(0, xboardsize - 1),
			pair<int, int>(boardArrayCordinates.first, boardArrayCordinates.second));
		nearestC = pair<int, int>(0, xboardsize - 1);
	}

	if (Distance(pair<int, int>(yboardsize - 2, 0),
		pair<int, int>(boardArrayCordinates.first, boardArrayCordinates.second)) < min)
	{
		min = Distance(pair<int, int>(yboardsize - 2, 0),
			pair<int, int>(boardArrayCordinates.first, boardArrayCordinates.second));
		nearestC = pair<int, int>(yboardsize - 2, 0);
	}

	if (Distance(pair<int, int>(yboardsize - 2, xboardsize-1),
		pair<int, int>(boardArrayCordinates.first, boardArrayCordinates.second)) < min)
	{
		min = Distance(pair<int, int>(yboardsize - 2, xboardsize-1),
			pair<int, int>(boardArrayCordinates.first, boardArrayCordinates.second));
		nearestC = pair<int, int>(yboardsize - 2, xboardsize - 1);
	}
	return nearestC;
}



void Enemy::move(pair<int, int> bfsTarget)
{

	if (pixelCordinates == t) {
		int first = bfsTarget.first;
		int second = bfsTarget.second;
		cDirection = BFS(pair<int,int>(first, second));
		
		switch (cDirection)
		{
		case UP:
			t.first += ypixelsize;
			break;
		case RIGHT:
			t.second += xpixelsize;
			break;
		case LEFT:
			t.second -= xpixelsize;
			break;
		case DOWN:
			t.first -= ypixelsize;
			break;
		default:
			break;
		}
		Move(cDirection);
	}
	else {
		if (!CanMove(cDirection))
			Move(Direction(rand()%4));
		else
			Move(cDirection);
	}
}
Direction Enemy::BFS(pair <int,int>Target )
{
	
	//Creates a 2d array named temparr that indicates whether the block can be visited or not
	// 0= Empty 1= a block   2=Up 3=Right 4=Left 5= Down 6= Target and most importantly 7 = ENEMY himself
	//int temparr[14][17];
	int **temparr = new int*[yboardsize];
	for (int i = 0; i < yboardsize; ++i)
	{
		temparr[i] = new int[xboardsize];
		for (int j = 0; j < xboardsize; ++j)
		{
			temparr[i][j] = BoardArray[i][j];
			if (temparr[i][j] >= 1 && temparr[i][j] <= 4)
				temparr[i][j] = 1;
			else if (temparr[i][j] >4)
				temparr[i][j] = 0;
		}
	}
	//temparr[boardArrayCordinates.first][boardArrayCordinates.second] = 7;
	//temparr[Target.first][Target.second] = 6;

	deque<pair<int, int>> tempBACordinates;
	pair<int, int>t(boardArrayCordinates.first, boardArrayCordinates.second);
	int nDirections = 0;
	if (temparr[t.first + 1][t.second] == Nill /*&& cDirection != UP*/)
	{
		//pair<int, int> t1(t.first + 1, t.second);
		tempBACordinates.push_front(pair<int, int>(t.first + 1, t.second));
		temparr[t.first + 1][t.second] = DOWN + 2;
		nDirections++;
	}
	if (temparr[t.first - 1][t.second] == Nill/* && cDirection != DOWN*/)
	{
		//pair<int, int> t1(t.first - 1, t.second);
		tempBACordinates.push_front(pair<int, int>(t.first - 1, t.second));
		temparr[t.first - 1][t.second] = UP + 2;
		nDirections++;
	}
	if (temparr[t.first][t.second + 1] == Nill /*&& cDirection != LEFT*/)
	{
		//pair<int, int> t1(t.first, t.second+1);
		tempBACordinates.push_front(pair<int, int>(t.first, t.second + 1));
		temparr[t.first][t.second + 1] = RIGHT + 2;
		nDirections++;
	}
	if (temparr[t.first][t.second - 1] == Nill /*&& cDirection != RIGHT*/)
	{
		//pair<int, int> t1(t.first, t.second-1);
		tempBACordinates.push_front(pair<int, int>(t.first, t.second - 1));
		temparr[t.first][t.second - 1] = LEFT + 2;
		nDirections++;
	}

	/*if there is only one possible direction to go,
	then returns it without any further unnecassary processing*/
//	if (nDirections == 1)
//		return Direction(temparr[tempBACordinates[0].first][tempBACordinates[0].second]);

	/*if there is no possible direction for the ghost to go,
	then it means that the ghost came across a dead end and should go in the reverse direction*/
//	if (nDirections == 0)
//		return Direction(3 - cDirection);
	cout << nDirections << endl;
	int min = 100;
	Direction dMin = cDirection;
	int ty = -1, tx = -1;
	int min1 = 0;
	int i = 0;

	while (!tempBACordinates.empty())
	{
		i++;
		//if (i > 17*14)
		//	break;
		ty = (tempBACordinates.front()).first;
		tx = (tempBACordinates.front()).second;
		min1 = Distance(pair<int, int>(Target.first, Target.second), pair<int, int>(ty, tx));
		if (min1< min)
		{
			min = min1;
			dMin = Direction(temparr[ty][tx] - 2);
		}
		cout << tx << " " << ty << " " << min << " " << i << " " << endl;
		if (temparr[ty + 1][tx] == Nill)
		{
			temparr[ty + 1][tx] = temparr[ty][tx];
			tempBACordinates.push_back(pair<int, int>(ty + 1, tx));

		}

		if (temparr[ty - 1][tx] == Nill)
		{
			temparr[ty - 1][tx] = temparr[ty][tx];
			tempBACordinates.push_back(pair<int, int>(ty - 1, tx));
		}

		if (temparr[ty][tx + 1] == Nill)
		{
			temparr[ty][tx + 1] = temparr[ty][tx];
			tempBACordinates.push_back(pair<int, int>(ty, tx + 1));
		}

		if (temparr[ty][tx - 1] == Nill)
		{
			temparr[ty][tx - 1] = temparr[ty][tx];
			tempBACordinates.push_back(pair<int, int>(ty, tx - 1));
		}
		tempBACordinates.pop_front();
	}

	for (int i = 0; i < yboardsize; ++i)
		delete[] temparr[i];
	delete[] temparr;
	
	cout << nDirections << " Direction: " << dMin << " Target: " << Target.second << " " << Target.first << endl;
	return dMin;
}

void Enemy::draw()
{
	if (cMode == Frightened)
		DrawEnemy(pixelCordinates.second - xpixelsize / 2, pixelCordinates.first - ypixelsize/2,
			BLUE, 0.8*ypixelsize, 0.8*ypixelsize);

	else
		DrawEnemy(pixelCordinates.second - xpixelsize/2, pixelCordinates.first - ypixelsize/2,
			color, 0.8*ypixelsize, 0.8*ypixelsize);

}

Mode Enemy::getcMode()
{
	return cMode;
}

void Enemy::spawn(){}


//					PINKY CLASS


Pinky::Pinky(int bx, int by, ColorNames c) : Enemy(bx,by,c)
{
	cDirection = DOWN;
	spawn();
	//speed = 5;
}

void Pinky::spawn()
{
	int ii = 1, jj = xboardsize-2;
	for (ii = 1; ii < yboardsize-3 && BoardArray[ii][jj] <= 3 && BoardArray[ii][jj] != Nill; ++ii)
	{
		for (jj = xboardsize-2; jj >0 && BoardArray[ii][jj] <= 3 && BoardArray[ii][jj] != Nill; --jj) {}
	}
	bAcordinates(jj, ii);
	t = pixelCordinates;
}

Direction Pinky::BFS(pair<int,int> Target)
{
	//srand((unsigned)time(0));
	if (cMode == Scattered)
		return Enemy::BFS(pair<int, int>(Target.first, Target.second));
	return Enemy::BFS(pair<int, int>(rand()%yboardsize-1, rand()%xboardsize));
}


//					BLINKY CLASS


Blinky::Blinky(int bx, int by, ColorNames c): Enemy(bx, by, c)
{
	cDirection = DOWN;
	spawn();
	//speed = 1;
}

void Blinky::spawn()
{
	int ii = 1, jj = 1;
	for (ii = 1; ii < yboardsize-3 && BoardArray[ii][jj] <= 3 && BoardArray[ii][jj] != Nill; ++ii)
	{
		for (jj = 1; jj <xboardsize-2 && BoardArray[ii][jj] <=3 && BoardArray[ii][jj] != Nill; ++jj) {}
	}
	bAcordinates(jj, ii);
	t = pixelCordinates;
}


//					INKY CLASS


Inky::Inky(int bx, int by, ColorNames c) : Enemy(bx,by,c)
{
	cDirection = LEFT;
	spawn();
	speed = 2;
}

void Inky::spawn()
{
	int ii = yboardsize-3, jj = xboardsize-2;
	for (ii = yboardsize-3; ii > 0 && BoardArray[ii][jj] <= 3 && BoardArray[ii][jj] != Nill; --ii)
	{
		for (jj = xboardsize-2; jj >0 && BoardArray[ii][jj] <= 3 && BoardArray[ii][jj] != Nill; --jj) {}
	}
	bAcordinates(jj, ii);
	t = pixelCordinates;
}


//					CLYDE CLASS


Clyde::Clyde(int bx, int by, ColorNames c):Enemy(bx,by,c)
{
	cDirection = UP;
	spawn();
	speed = 3;
}

void Clyde::spawn()
{
	int ii = yboardsize-3, jj = 1;
	for (ii = yboardsize - 3; ii > 0 && BoardArray[ii][jj] <= 3 && BoardArray[ii][jj] != Nill; --ii)
	{
		for (jj = 1; jj < xboardsize-2 && BoardArray[ii][jj] <= 3 && BoardArray[ii][jj] != Nill; ++jj) {}
	}
	bAcordinates(jj, ii);
	t = pixelCordinates;
}


//				ITEMS CLASS


Item::Item(int bx, int by, ColorNames c)
{
	color = c;
	bAcordinates(bx, by);
}



//				DOTS CLASS


float Dot::radius = xpixelsize / 12;
ColorNames Dot::color = BLUE;

Dot::Dot(int bx, int by) : Item(bx, by, color)
{}

void Dot::draw()
{
	DrawCircle(pixelCordinates.second, pixelCordinates.first, radius, colors[color]);
	/*for (int i = 0; i < 6; i+=2)
	{
		DrawCircle(pixelCordinates.second, pixelCordinates.first, radius-(i*0.5), colors[color+(i*i)]);
	}*/
}


//					BOMB CLASS


ColorNames Bomb::color = WHITE;
float Bomb::radius = xpixelsize/4;

Bomb::Bomb(int bx, int by): Item (bx, by, color)
{
	ctime = 0;
	bombstatus = false;
}

void Bomb::draw()
{
	DrawCircle(pixelCordinates.second, pixelCordinates.first, radius, colors[color]);
	DrawLine(pixelCordinates.second, pixelCordinates.first + radius,
		pixelCordinates.second, pixelCordinates.first + radius + 15, 3, colors[SILVER]);
}

bool Bomb::place(pair<int, int>pl, int ct)
{
	if (bombstatus == false)
	{
		bAcordinates(pl.second, pl.first);
		ctime = ct;
		bombstatus = true;
		draw();
		BoardArray[pl.first][pl.second] = BOMB;
		return true;
	}

	return false;
}

bool Bomb::blast(int t)
{
	if (bombstatus == true)
	{
		draw();
		if (abs(t - ctime) >= 0 && abs(t - ctime) < 3)
			return false;
		drawflame();
		ctime = t;
		bombstatus = false;
		BoardArray[boardArrayCordinates.first][boardArrayCordinates.second] = Nill;
		return true;
	}
	return false;
}

pair<int, int> &Bomb::getBoardCordinates()
{
	return boardArrayCordinates;
}

void Bomb::drawflame()
{
	DrawRoundRect(pixelCordinates.second - (xpixelsize/2), pixelCordinates.first - (ypixelsize * 3 / 2),
		xpixelsize, ypixelsize * 3, colors[RED], xpixelsize / 2);
	DrawRoundRect(pixelCordinates.second - (xpixelsize*3 / 2), pixelCordinates.first - (ypixelsize/2),
		xpixelsize*3, ypixelsize, colors[RED], xpixelsize / 2);
}


//						ENERGIZER CLASS


ColorNames Energizer::color= SALMON;
float Energizer::radius=xpixelsize/5;

Energizer::Energizer(int bx, int by) : Item(bx, by, color)
{}

void Energizer::draw()
{
	DrawCircle(pixelCordinates.second, pixelCordinates.first, radius, colors[color]);
	for (int i = 0; i < 6; i+=2)
	{
		DrawCircle(pixelCordinates.second, pixelCordinates.first, radius - (i*0.5), colors[color + (i*i)]);
	}

}


//						GIFT CLASS


ColorNames Gift::color = PLUM;

Gift::Gift(int bx, int by): Item(bx, by, color)
{}

void Gift::draw()
{
	DrawRectangle(pixelCordinates.second - (xpixelsize / 12), pixelCordinates.first - (ypixelsize / 4),
		xpixelsize / 6, ypixelsize / 2, colors[RED]);
	DrawRectangle(pixelCordinates.second - (xpixelsize / 4), pixelCordinates.first - (ypixelsize / 12),
		xpixelsize / 2, ypixelsize / 6, colors[RED]);
	
	DrawRectangle(pixelCordinates.second - (xpixelsize / 12)+2, pixelCordinates.first - (ypixelsize / 4)+2,
		xpixelsize / 6 -2, ypixelsize / 2-2, colors[color]);
	DrawRectangle(pixelCordinates.second - (xpixelsize / 4) + 2, pixelCordinates.first - (ypixelsize / 12) + 2,
		xpixelsize / 2 - 2, ypixelsize / 6 - 2, colors[color]);
}


