#include "NE.h"
#include <ctime>
SplayTree<int> fun1()
{
	SplayTree<int>s1;
	s1.insert(2);
	s1.insert(4);
	s1.insert(3);
	return s1;
}

int main()
{
	/*Graph g;
	cout << "Enter the Routing table data structure.\n";
	cout << "1. B-Tree.\n2. Splay Tree.\n3. Linked List.\n";
	short choice;
	cin >>choice;
	while (choice < 1 || choice>3)
	{
		cout << "Invalid entry. Enter the option number again.\n";
		cin >> choice;
	}
	char ds='l';
	if (choice == 1)
		ds = 'b';
	else if (choice == 2)
		ds = 's';
	else
		ds = 'l';
	g.initGraph("network.csv", ds);
	g.mainScreen();*/
	BTree <char>bt(5);
	string allChars;
	char x, temp;
	short n = 30;
	srand(unsigned(time(0)));
	for (short i = 0; i < n; ++i)
	{
		x = rand() % 92 + 33;
		while (allChars.find(x)!=-1)
			x = rand() % 92 + 33;
		allChars += x;
	}
	cout << allChars << endl;
	for (short i = 0; i < n; ++i)
	{
		cout << "Enter a avalue: ";
		x = allChars[i];
		bt.insert(x);
		bt.display();
	}
	short tindex;
	for (short i = 0; i < n-1; ++i)
	{
		tindex = rand() % (n - i - 1);
		x = allChars[tindex];
		allChars.erase(tindex,1);
		cout << "Enter a value to remove: " << x << endl;
		bt.remove(x);
		bt.display();
	}
	bt.remove(allChars[0]);
	system("pause");
	return 0;
}