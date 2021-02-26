#pragma once
#include <iostream>
using namespace std;
template <class t>
class BTree
{
	struct Node;
	short m;	//order of tree
	friend class Node;
	struct Node
	{
		short n;	//current keys in the node
		t*keys;
		Node**child;
		bool isLeaf;

		Node(t val, short m)
		{
			n = 1;
			keys = new t[m-1];
			keys[0] = val;
			child = new Node*[m];
			for (short i = 0; i < m; ++i)
				child[i] = NULL;
			isLeaf = true;
		}
		void addKey(t val, Node*temp)
		{
			short i = 0;
			for (;i<n && val>keys[i]; ++i)
			{}
			for (short j = i; j < n; ++j)
			{
				keys[n + i - j] = keys[n + i - j - 1];
				child[n + i + 1 - j] = child[n + i - j];
			}
			keys[i] = val;
			child[i + 1] = temp;
			n++;
		}
	}*root;
public:
	BTree(short n = 2)
	{
		m = n;
		root = NULL;
	}
	void split(Node*&p,Node*&current)
	{
		Node*newNode = new Node(current->keys[current->n / 2 + 1],m);
		newNode->isLeaf = current->isLeaf;
		for (short i = 1; i < m/2 - 1; ++i)
			newNode->keys[i] = current->keys[current->n / 2 + i + 1];
		for (short i = 0; i < m / 2 && !(current->isLeaf); ++i)
			newNode->child[i] = current->child[current->n / 2 + 1 + i];
		current->n = current->n / 2;
		newNode->n = m / 2 - 1;
		if (!p)
		{
			p = root = new Node(current->keys[current->n],m);
			p->n = 0;
			p->isLeaf = false;
			p->child[0] = current;
		}
		p->addKey(current->keys[current->n], newNode);
	}
	void insert(t val)
	{
		if (!root)
			root = new Node(val,m);
		else
		{
			Node*temp = root;
			Node*parent = NULL;
			short i = 0;
			while (1)
			{
				if (temp->n == m - 1)
				{
					split(parent, temp);
					temp = parent;
				}
				for (i = 0; i < temp->n && temp->keys[i]<val; ++i)
				{}
				if (i<temp->n&&temp->keys[i]==val)
				{
					cout << val << " is already in the BTree.\n";
					return;
				}
				parent = temp;
				if (!(temp->isLeaf))
					temp = temp->child[i];
				else
					break;
			}
			for (short j = i; j < temp->n; ++j)
				temp->keys[temp->n + i - j] = temp->keys[temp->n + i - j - 1];
			temp->keys[i] = val;
			++(temp->n);
		}
	}
	void remove(t val)
	{
		short temp = 0;
		Node*parent = root;
		bool found = false;
		while (!(parent->isLeaf) && !found)
		{
			for (temp = 0; temp<parent->n && temp->keys<val;++temp)
			{ }
			if (temp<parent->n && parent->keys[temp] == val)
				found = true;
			else
			{
				if (parent)
			}
		} //while (found);
	}
	Node*getRoot()const
	{
		return root;
	}
	void display(Node*n=NULL, short depth=0)const
	{
		for (short i = 0; i < n->n; ++i)
		{
			for (short j = 0; j < depth; ++j)
				cout << "  ";
			cout << n->keys[i] << endl;
		}
		for (short i = 0; i <= n->n&& !(n->isLeaf); ++i)
			display(n->child[i], depth+1);
	}
};
