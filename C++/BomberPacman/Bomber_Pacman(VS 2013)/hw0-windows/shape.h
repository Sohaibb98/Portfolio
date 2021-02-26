#ifndef SHAPE_H_
#define SHAPE_H_
class shape
{
public:
	virtual short getlives()
	{
		return 0;
	}
	virtual long getscore()
	{
		return 0;
	}
	virtual void die()
	{

	}
	virtual void move()
	{

	}
	virtual void check()
	{

	}
	virtual void draw()
	{

	}
	virtual void getposition(int & x, int & y)
	{

	}
	virtual void newset()
	{

	}
	virtual void operator--()
	{

	}
	virtual void load()
	{

	}
	virtual void save()
	{

	}
	virtual void decrease()
	{

	}
	virtual void increase(int s1)
	{

	}
	virtual short getx()
	{
		return 0;
	}
	virtual short gety()
	{
		return 0;
	}
	virtual ~shape()
	{

	}
};
#endif /* SHAPE_H_ */
