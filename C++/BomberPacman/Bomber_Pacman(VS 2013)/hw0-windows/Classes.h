#ifndef CLASSES_H_
#define CLASSES_H_

#include <deque>
#include <map>
#include "util.h"
#include "Draw.h"
//#include <iostream>
//#include <utility>

using namespace std;


enum Mode { Chase, Scattered, Frightened };
enum Movement { Chasing, Scattering, Random };
enum Primitives { Nill, SBRICK, GBRICK, RBRICK, BOMB, BOMBERMAN, ENEMY, DOT, ENERGIZER, GIFT };

class Primitive
{
protected:
	static int xpixelsize;
	static int ypixelsize;
	pair<float, float> pixelCordinates;
	pair<int, int> boardArrayCordinates;
	
	static int xboardsize;
	static int yboardsize;
public:
	static int **BoardArray;
	static int xpixels(int = -1);
	static int ypixels(int = -1);
	void pCordinates(float, float);
	void bAcordinates(int, int);
	void static initBoard(int, int, int**);
	void static setAtBoard(int, int, Primitives);
	//void static drawboard();
};


class Creature :public Primitive
{
protected:
	Direction cDirection;
	int speed;
	ColorNames color;
public:
	Creature(int, int, ColorNames);
	void setboardx();
	void setboardy();
	void setboardxy();
	void pCordinates(float, float);
	//template <typename T>
	//virtual void draw(T func) = 0;
	bool CanMove(Direction, int = 4); 
	pair<float,float> &getpixels();
	pair<int, int> &getBoardCordinates();
	virtual void Move(Direction = STATIONARY);
	Direction getcDirection() const;
	//virtual void spawn();
	//virtual void respawn();
};

class BomberMan : public Creature
{
protected:
	int score;
	int lives;
	Direction NextDirection;
public:
	BomberMan(int=1, int=yboardsize-3, ColorNames = YELLOW);
	
	/*template<typename T>
	void draw(T func)
	{

		func(pixelCordinates.second, pixelCordinates.first, xpixelsize / 2 - 2, color, cDirection);
	}*/
	//void respawn();
	void addscore(int);
	int getlives()const;
	int getscore();
	void spawn();
	void draw();
	void addlives(int = 1);
	void Move(Direction = STATIONARY) override;
};


class Enemy : public Creature
{
protected:
	static Mode cMode;
	static int ctime;
	pair<float, float> t;
	virtual Direction BFS(pair<int, int>);
public:
	Enemy(int, int, ColorNames);
	void static ChangeMode(Mode, int );
	void static ChangeMode(int);
	Mode static getcMode();
	virtual void spawn();
	void move(pair<int,int>);
	void draw();
	pair<int, int> NearestCorner();

	/*template<typename T>
	void draw(T func)
	{
		func(pixelCordinates.second - xpixelsize, pixelCordinates.first - ypixelsize
			, color, xpixelsie, ypixelsize);

	}*/
};

class Pinky: public Enemy
{
public:
	Pinky(int=xboardsize-2, int=1, ColorNames = DEEP_PINK);
	//void Move(Direction = STATIONARY) override;
	void spawn();
	Direction BFS(pair<int,int>) override;
};

class Blinky : public Enemy
{
public:
	Blinky(int = 1, int = 1 , ColorNames = GREEN);
	void spawn();
};

class Inky : public Enemy
{
public:
	Inky(int=xboardsize-2, int= yboardsize-3, ColorNames = DARK_BLUE);
	void spawn();
};

class Clyde : public Enemy
{
public:
	Clyde(int = 1, int=yboardsize-3, ColorNames = SILVER);
	void spawn();
};


class Item: public Primitive
{
protected:
	ColorNames color;
public:
	Item(int, int, ColorNames);
};


class Dot: public Item
{
protected:
	static ColorNames color;
	static float radius;
public:
	Dot(int, int);
	void draw();
};

class Bomb : public Item
{
protected:
	static ColorNames color;
	static float radius;
	int ctime;
	bool bombstatus;
public:
	Bomb(int=1, int=1);
	void draw();
	bool place(pair<int, int>, int);
	bool blast(int);
	pair<int, int> &getBoardCordinates();
	void drawflame();
};

class Energizer : public Item
{
	static ColorNames color;
	static float radius;
public:
	Energizer(int, int);
	void draw();
};

class Gift: public Item
{
	static ColorNames color;
	//static float radius;
public:
	Gift(int, int);
	void draw();
};
#endif // !CLASSES_H_

