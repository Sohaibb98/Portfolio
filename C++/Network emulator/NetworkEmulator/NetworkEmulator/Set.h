#pragma once

template <typename t>
class Set
{
	struct Node
	{
		Node*left, *right;
		t data;
		int height;
		Node(t d)
		{
			data = d;
			left = right = NULL;
			height = 0;
		}
	}*root;
	Node* copy(const Node*n)
	{
		Node*temp = new Node(n->data);
		if (n->left)
			temp->left = copy(n->left);
		if (n->right)
			temp->right = copy(n->right);
		return temp;
	}
public:
	Set()
	{
		root = NULL;
	}
	Set(const Set&s):root(NULL)
	{
		if (s.root)
			root = copy(s.root);
	}
};
