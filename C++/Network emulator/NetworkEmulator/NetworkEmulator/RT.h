#pragma once
#include <iostream>
#include <assert.h>
using namespace std;
template<class t>
class RoutingTable
{
public:
	virtual void insert(t v){}
	virtual void remove(t v){}
	virtual void display()const{}
	virtual t search(t v) { return v; }
	virtual void makeEmpty() {}
};

template <class t>
class BTree : public RoutingTable<t>
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
			keys = new t[m - 1];
			keys[0] = val;
			child = new Node*[m];
			for (short i = 0; i < m; ++i)
				child[i] = NULL;
			isLeaf = true;
		}
		void addKey(t val, Node*temp)
		{
			short i = 0;
			for (; i<n && val>keys[i]; ++i)
			{
			}
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
	~BTree()
	{
		makeEmpty();
	}
	void makeEmpty()
	{
		while (root)
		{
			remove(root->keys[0]);
			display();
			if (!root->n)
			{
				delete root;
				root = NULL;
			}
		}
	}
	void borrowFromRight(Node*parent, short childIndex)
	{
		Node*current = parent->child[childIndex];
		Node*right = parent->child[childIndex + 1];
		current->keys[current->n] = parent->keys[childIndex];
		if (!(current->isLeaf))
		{
			current->child[current->n+1] = right->child[0];
			for (short i = 0; i < right->n; ++i)
				right->child[i] = right->child[i + 1];
		}
		parent->keys[childIndex] = right->keys[0];
		for (short i = 0; i < right->n-1; ++i)
			right->keys[i] = right->keys[i + 1];
		++(current->n);
		--(right->n);
	}
	void borrowFromLeft(Node*parent, short childIndex)
	{
		Node*current = parent->child[childIndex];
		Node*left = parent->child[childIndex-1];
		for (short i = current->n; i > 0; --i)
			current->keys[i] = current->keys[i - 1];
		current->keys[0] = parent->keys[childIndex-1];
		parent->keys[childIndex-1] = left->keys[left->n - 1];
		if (!(current->isLeaf))
		{
			for (short i = current->n+1; i > 0; --i)
				current->child[i] = current->child[i - 1];
			current->child[0] = left->child[left->n];
		}
		++(current->n);
		--(left->n);
	}
	void mergeWithLeft(Node*&parent, short childIndex)
	{
		Node*current = parent->child[childIndex];
		Node*left = parent->child[childIndex-1];
		left->keys[left->n] = parent->keys[childIndex - 1];
		for (short i = 0; i < current->n; ++i)
			left->keys[left->n + i + 1] = current->keys[i];
		if (!(current->isLeaf))
		{
			for (short i = 0; i <= current->n; ++i)
				left->child[left->n + i + 1] = current->child[i];
		}
		left->n += 1 + current->n;
		delete	current;
		--(parent->n);
		if (!(parent->n))
		{
			delete parent;
			parent = root = left;
		}
		else
		{
			for (short i = childIndex; i <= parent->n; ++i)
			{
				parent->keys[i-1] = parent->keys[i];
				parent->child[i] = parent->child[i + 1];
			}
		}
	}
	void mergeWithRight(Node*&parent, short childIndex)
	{
		mergeWithLeft(parent, childIndex + 1);
	}
	void split(Node*&p, Node*&current)
	{
		Node*newNode = new Node(current->keys[current->n / 2 + 1], m);
		newNode->isLeaf = current->isLeaf;
		for (short i = 1; i < m / 2 - 1; ++i)
			newNode->keys[i] = current->keys[current->n / 2 + i + 1];
		for (short i = 0; i < m / 2 && !(current->isLeaf); ++i)
			newNode->child[i] = current->child[current->n / 2 + 1 + i];
		current->n = current->n / 2;
		newNode->n = m / 2 - 1;
		if (!p)
		{
			p = root = new Node(current->keys[current->n], m);
			p->n = 0;
			p->isLeaf = false;
			p->child[0] = current;
		}
		p->addKey(current->keys[current->n], newNode);
	}
	t search(t v)
	{
		Node*temp = root;
		if (temp)
		{
			short index = 0;
			search(temp, v,index);
			return temp->keys[index];
		}
		return v;
	}
	void search(Node*&n, t v,short index)
	{
		for (short i = 0; i < n->n; ++i)
		{
			if (n->keys[i] == v)
			{
				index = i;
				return;
			}
		}
		for (short i = 0; i <= n->n && !(n->isLeaf); ++i)
			search(n->child[i], v,index);
	}
	void insert(t val)
	{
		if (!root)
			root = new Node(val, m);
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
				if (i<temp->n&&temp->keys[i] == val)
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
		static Node*parent=NULL;
		if (!parent)
			parent = root;
		bool found = false;
		do //(!(parent->isLeaf) && !found)
		{
			for (temp = 0; temp<parent->n && parent->keys[temp]<val; ++temp)
			{}
			found = (temp < parent->n && parent->keys[temp] == val);
			//else
			{
				if (parent->isLeaf)
					break;
				if (parent->child[temp]->n <= m / 2) //adjusting the keys in the first pass.\n
				{
					if (temp<parent->n && parent->child[temp + 1]->n > m / 2)
						borrowFromRight(parent, temp);
					else if (temp > 0 && parent->child[temp - 1]->n > m / 2)
						borrowFromLeft(parent, temp);
					else if (temp < parent->n)
						mergeWithRight(parent, temp);
					else
						mergeWithLeft(parent, temp);
					found = false;
				}
				else if (!found)
					parent = parent->child[temp];
			}
		} while (!found);
		if (found)
		{
			Node*left = parent->child[temp];
			if (!(parent->isLeaf))
			{
				t swap = parent->keys[temp];
				parent->keys[temp] = left->keys[left->n-1];
				left->keys[left->n-1] = swap;
				parent = left;
				remove(val);
			}
			else
			{
				for (short i = temp; i <parent->n-1; ++i)
					parent->keys[i] = parent->keys[i+1];
				--(parent->n);
				parent = NULL;
			}
		}
		else
		{
			cout << val << " not found in the BTree.\n";
			parent = NULL;
		}
	}
	Node*getRoot()const
	{
		return root;
	}
	void display() const
	{
		if (root)
			display(root);
	}
	void display(Node*n, short depth = 0)const
	{
		for (short i = 0; i < n->n; ++i)
		{
			for (short j = 0; j < depth; ++j)
				cout << "  ";
			cout << n->keys[i] << endl;
		}
		for (short i = 0; i <= n->n && !(n->isLeaf); ++i)
			display(n->child[i], depth + 1);
	}
};

template <class t>
class SplayTree : public RoutingTable<t>
{
	struct Node
	{
		t val;
		Node*left;
		Node*right;
		Node(t v)
		{
			val = v;
			left = right = NULL;
		}
	}*root;

	void RightRotation(Node*&n)
	{
		Node*temp = n->left;
		n->left = temp->right;
		temp->right = n;
		n = temp;
	}
	void LeftRotation(Node*&n)
	{
		Node*temp = n->right;
		n->right = temp->left;
		temp->left = n;
		n = temp;
	}
	void splayTD(const t & v, Node*&n)
	{
		Node temp(v);
		Node*leftMax, *rightMin;
		leftMax = rightMin = &temp;
		while (n && v != n->val)
		{
			if (n->val < v)
			{
				if (!(n->right))
					break;
				leftMax->right = n;
				leftMax = n;
				n = n->right;
			}
			else if (v < n->val)
			{
				if (!(n->left))
					break;
				rightMin->left = n;
				rightMin = n;
				n = n->left;
			}
		}
		leftMax->right = n->left;
		rightMin->left = n->right;
		n->left = temp.right;
		n->right = temp.left;
	}
	Node* copy(const Node*n)
	{
		Node*temp = new Node(n->val);
		if (n->left)
			temp->left = copy(n->left);
		if (n->right)
			temp->right = copy(n->right);
		return temp;
	}
public:
	SplayTree()
	{
		root = NULL;
	}
	SplayTree(const SplayTree&s) :root(NULL)
	{
		if (s.root)
			root = copy(s.root);
	}
	~SplayTree()
	{
		makeEmpty();
	}
	void makeEmpty()
	{
		while (root)
			remove(root->val);
	}
	void insert(t v)
	{
		if (root == NULL)
			root = new Node(v);
		else
		{
			Node*temp = new Node(v);
			splayTD(v, root);
			if (root->val < v)
			{
				temp->left = root;
				temp->right = root->right;
				root->right = NULL;
				root = temp;
			}
			else if (v < root->val)
			{
				temp->right = root;
				temp->left = root->left;
				root->left = NULL;
				root = temp;
			}
			else
				cout << v << " is already in the splay tree.\n";
		}
	}
	void remove(t v)
	{
		if (!root)
		{
			cout << v << " not found in the tree." << " The Tree is empty.\n";
			return;
		}
		splayTD(v, root);
		if (root->val != v)
		{
			cout << v << " not found in the tree.\n";
			return;
		}
		Node*temp = root->left;
		if (temp)
		{
			splayTD(v, temp);
			temp->right = root->right;
		}
		else
			temp = root->right;
		delete root;
		root = temp;
	}
	Node*getRoot()
	{
		return root;
	}
	t search(t v)
	{
		find(v);
		return root->val;
	}
	bool find(t v)	//check whether a certain value is in the tree or not by splaying.
	{
		splayTD(v, root);
		return (root->val == v);
	}
	bool has(t v)	//check whether a certain value is in the tree or not. 
	{
		Node*temp = root;
		while (temp)
		{
			if (v < temp->val)
				temp = temp->left;
			else if (temp->val < v)
				temp = temp->right;
			else
				return true;
		}
		return false;
	}
	void display() const
	{
		display(root);
	}
	void display(Node*n = NULL, int space = 0) const
	{
		if (n)
		{
			for (int i = 0; i < space; ++i)
				cout << "  ";
			cout << n->val << endl;
			if (n->left)
				cout << "L:";
			display(n->left, space + 1);
			if (n->right)
				cout << "R:";
			display(n->right, space + 1);
		}
	}
};

template<class t>
class List : public RoutingTable<t>
{
	struct Node
	{
		Node*next;
		t val;

		Node(t v)
		{
			val = v;
			next = NULL;
		}
	} *top;
	int tsize;
public:
	List()
	{
		top = NULL;
		tsize = 0;
	}
	List(const queue<t> & q)
	{
		Node*n = q.top;
		for (; n; n = n->next)
			push(n->val);
	}
	t search(t v)
	{
		Node*temp = top;
		if (!temp)
		{
			cout << "The list is empty.\n";
			return v;
		}
		while (temp->next && temp->next->val != v && temp->val != v)
			temp = temp->next;
		if (temp->val != v && !(temp->next))
		{
			cout << v << " not found in the list.\n";
			return v;
		}
		if (temp->val == v)
			return temp->val;
		else
			return temp->next->val;
	}
	void insert(t v)
	{
		Node*temp = top;
		if (temp)
		{
			while (temp->next && temp->val != v)
				temp = temp->next;
			if (temp->val == v)
			{
				cout << v << " is already in the list.\n";
				return;
			}
			temp->next = new Node(v);
		}
		else
			top = new Node(v);
		++tsize;
	}
	t front()
	{
		assert(top);
		return top->val;
	}
	void remove(t v)
	{
		Node*temp = top;
		if (!temp)
		{
			cout << "The list is empty.\n";
			return;
		}
		while (temp->next && temp->next->val != v && temp->val !=v)
			temp = temp->next;
		if (temp->val != v && !(temp->next))
		{
			cout << v << " not found in the list.\n";
			return;
		}
		if (temp->val == v)
		{
			top = top->next;
			delete temp;
		}
		else
		{
			Node*temp1 = temp->next;
			temp->next = temp1->next;
			delete temp1;
		}
		--tsize;
	}

	int size()
	{
		return tsize;
	}

	void display() const
	{
		Node*temp = top;
		while (temp)
		{
			cout << temp->val <<"  ";
			temp = temp->next;
		}
		cout << endl;
	}
	void makeEmpty()
	{
		while (top)
			remove(top->val);
	}
	~List()
	{
		makeEmpty();
	}
};