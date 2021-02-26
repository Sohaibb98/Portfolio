#pragma once
#include <assert.h>


template<class t>
class queue
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
	queue()
	{
		top = NULL;
		tsize = 0;
	}
	queue(const queue<t> & q)
	{
		Node*n = q.top;
		for (; n; n=n->next)
			push(n->val);
	}
	void push(t val)
	{
		Node*temp = top;
		if (temp)
		{
			while (temp->next)
				temp = temp->next;
			temp->next = new Node(val);
		}
		else
			top = new Node(val);
		++tsize;
	}
	t front()
	{
		assert(top);
		return top->val;
	}
	t pop()
	{
		assert(top);
		Node*temp = top;
		t tval = top->val;
		top = top->next;
		--tsize;
		delete temp;
		return tval;
	}

	int size()
	{
		return tsize;
	}

	~queue()
	{
		while (top)
			pop();
	}
};
