#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
using namespace std;

template <class t>
class MinHeap
{

	t*Array;
	int nElements;
	int size;
	int Min(int index)
	{
		if (index * 2 > nElements)
			return nElements + 1;
		if (index * 2 == nElements)
			return index * 2;
		return index * 2 + (Array[index * 2] > Array[index * 2 + 1]);
	}
	void percolateDown(int index)
	{
		for (int m = Min(index); m <= nElements; m = Min(index))
		{
			if (Array[index] <= Array[m])
				break;
			swapElements(Array[m], Array[index]);
			index = m;
		}
	}
	void swapElements(t& v1, t& v2)
	{
		t temp = v1;
		v1 = v2;
		v2 = temp;
	}
	void percolateUp(int index)
	{
		if (index / 2 == 0)
			return;
		if (Array[index] < Array[index / 2])
		{
			swapElements(Array[index], Array[index / 2]);
			percolateUp(index / 2);
		}
	}
public:
	MinHeap()
	{
		size = 1;
		nElements = 0;
		Array = new t[size];
	}
	MinHeap(const MinHeap&m)
	{
		size = m.size;
		nElements = m.nElements;
		Array = new t[size];
		for (int i = 0; i < size; ++i)
			Array[i] = m.Array[i];
	}
	void push(t v)
	{
		if (nElements == size - 1)
		{
			t*temp = new t[2 * size];
			for (int i = 0; i < size; ++i)
				temp[i] = Array[i];
			delete[] Array;
			Array = temp;
			size *= 2;
		}
		Array[++nElements] = v;
		percolateUp(nElements);
		//cout << Array[1] << endl;
	}
	t pop()
	{
		if (nElements)		//To check if there are any elements in the heap
		{
			swapElements(Array[1], Array[nElements--]);
			percolateDown(1);
			return Array[nElements + 1];
		}
		exit(-1);
	}
	bool isEmpty()
	{
		return !(nElements);
	}
	void display()
	{
		for (int i = 1; i <= nElements; ++i)
			cout << Array[i] << ' ';
		cout << endl;
	}
	~MinHeap()
	{
		delete[] Array;
	}
};