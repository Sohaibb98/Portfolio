#pragma once
#include <iostream>
using namespace std;
template <class t>
class SplayTree
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
		temp ->right = n;
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
	SplayTree(const SplayTree&s):root(NULL)
	{
		if (s.root)
			root = copy(s.root);
	}
	~SplayTree()
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
	void display(Node*n=NULL, int space=0)
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
