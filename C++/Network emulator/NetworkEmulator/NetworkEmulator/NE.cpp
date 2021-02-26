#include "NE.h"

//				Message Class

ElectronicDevice::Message::Message(short id, short p, string s, string d, string msg)
{
	MsgID = id;
	Priority = p;
	Trace = Src = s;
	Dest = d;
	PayLoad = msg;
}

void ElectronicDevice::Message::display() const
{
	cout << "Source name: " << Src << endl;
	cout << "Destination Name: " << Dest << endl;
	cout << "Trace: " << Trace << endl;
	cout << "PayLoad: " << PayLoad << endl;
	cout << "Message ID: " << MsgID << endl;
	cout << "Priority: " << Priority << endl;
}

string ElectronicDevice::Message::Destination()const
{
	return Dest;
}

void ElectronicDevice::Message::addTrace(string n)
{
	Trace += "\n" + n;
}

bool ElectronicDevice::Message::operator < (const Message&m)
{
	return (Priority < m.Priority);
}

bool ElectronicDevice::Message::operator > (const Message&m)
{
	return (Priority > m.Priority);
}

bool ElectronicDevice::Message::operator == (const Message&m)
{
	return (Priority == m.Priority);
}

bool ElectronicDevice::Message::operator != (const Message&m)
{
	return !(Priority == m.Priority);
}

bool ElectronicDevice::Message::operator >= (const Message&m)
{
	return ((*this > m )|| (*this==m));
}

bool ElectronicDevice::Message::operator <= (const Message&m)
{
	return ((*this < m) || (*this == m));
}


//				ELECTRONICE DEVICE CLASS

ElectronicDevice::ElectronicDevice(string n)
{
	Name = n;
	OutQs = NULL;
	adjacent = NULL;
	nAdjacent = 0;
}

string ElectronicDevice::getName()const
{
	return Name;
}

void ElectronicDevice::initAdjacents(ElectronicDevice** devices, short**matrix, short nDevices)
{
	short i;
	for (i = 0; i<nDevices && devices[i] != this; ++i)
	{}
	for (short j = 0; j < nDevices; ++j)
	{
		if (i != j && matrix[i][j] != 0)
			++nAdjacent;
	}
	OutQs = new queue<Message>*[nAdjacent];
	adjacent = new ElectronicDevice*[nAdjacent];
	for (short j = 0, k = 0; j < nDevices&&k<nAdjacent; ++j)
	{
		if (i != j && matrix[i][j])
		{
			adjacent[k] = devices[j];
			k++;
			OutQs[k - 1] = new queue<Message>;
		}
	}
}

short ElectronicDevice::getMyIndex(ElectronicDevice**devices, short**matrix, short nDevices)
{
	short i;
	for (i = 0; i<nDevices && devices[i] != this; ++i)
	{}
	return i;
}

void ElectronicDevice::createEdge(ElectronicDevice*d, bool BothDone)
{
	ElectronicDevice**t1 = new ElectronicDevice*[nAdjacent + 1];
	queue<Message>**q1 = new queue<Message>*[nAdjacent + 1];
	for (short i = 0; i < nAdjacent; ++i)
	{
		t1[i] = adjacent[i];
		q1[i] = OutQs[i];
	}
	delete[] adjacent;
	delete[] OutQs;
	adjacent = t1;
	OutQs = q1;
	adjacent[nAdjacent] = d;
	OutQs[nAdjacent++] = new queue<Message>;
	if (!BothDone)
		d->createEdge(this, true);
}

void ElectronicDevice::deleteEdge(ElectronicDevice*d, bool BothDone)
{
	ElectronicDevice**t1 = new ElectronicDevice*[nAdjacent - 1];
	queue<Message>**q1 = new queue<Message>*[nAdjacent - 1];
	short i;
	for (i = 0; i < nAdjacent && adjacent[i]!=d; ++i)
	{
		t1[i] = adjacent[i];
		q1[i] = OutQs[i];
	}
	delete OutQs[i];
	for (; i < nAdjacent; ++i)
	{
		adjacent[i] = adjacent[i + 1];
		OutQs[i] = OutQs[i + 1];
	}
	--nAdjacent;
	delete[] adjacent;
	delete[] OutQs;
	adjacent = t1;
	OutQs = q1;
	if (!BothDone)
		d->deleteEdge(this, true);
}

void ElectronicDevice::printPath(string d, string path)
{
	path += Name + '\n';
	if (path == Name + '\n' && adjacent && adjacent[0])
		adjacent[0]->printPath(d, path);
	else
		cout << path;
}

void ElectronicDevice::initRT(ElectronicDevice** devices, short**matrix, short nDevices)
{}
void ElectronicDevice::forwardMsg()
{}
void ElectronicDevice::recieveMsg(Message m)
{}
void ElectronicDevice::generateMsg(short p, string d, string pl)
{}
void ElectronicDevice::changeRT(ElectronicDevice* d,ElectronicDevice*nr, char op)
{}
void ElectronicDevice::reInitRT(ElectronicDevice**devices, short**matrix, short nDevices)
{}

//				Router Class

const char Router::DeviceType = 'R';
char Router::RtType='L';
SplayTree<short> Router::Rcount;


Router::Router()
{
	short temp = 1;
	while (Rcount.has(temp))
		temp += 1;
	Rcount.insert(temp);
	Name = DeviceType + Num2Str(temp);
	if (RtType == 'l' || RtType == 'L')
		RT = new List<RTNode>;
	else if (RtType == 's' || RtType == 'S')
		RT = new SplayTree<RTNode>;
	else
		RT = new BTree<RTNode>;
}

Router::Router(short rc)
{
	if (!Rcount.has(rc))
		Rcount.insert(rc);
	Name = DeviceType + Num2Str(rc);
	if (RtType == 'l' || RtType == 'L')
		RT = new List<RTNode>;
	else if (RtType == 's' || RtType == 'S')
		RT = new SplayTree<RTNode>;
	else
		RT = new BTree<RTNode>;
}

void Router::forwardMsg()
{
	while (!InQ.isEmpty())
	{
		Message m = InQ.pop();
		ElectronicDevice*temp = new ElectronicDevice(m.Destination());
		RTNode rtTemp = RT->search(RTNode(temp));
		delete temp;
		short adjacentIndex = rtTemp.index;
		OutQs[adjacentIndex]->push(m);
		adjacent[adjacentIndex]->recieveMsg(m);
	}
}

void Router::recieveMsg(Message m)
{
	m.addTrace(Name);
	InQ.push(m);
	forwardMsg();
	for (short i = 0; i < nAdjacent; ++i)
	{
		while (OutQs[i]->size())
		{
			Sleep(1000);
			adjacent[i]->InQ.push(OutQs[i]->pop());
		}
	}
}

void Router::printPath(string d, string path)
{
	ElectronicDevice*temp = new ElectronicDevice(d);
	RTNode rtTemp = RT->search(RTNode(temp));
	delete temp;
	short adjacentIndex = rtTemp.index;
	path += Name + '\n';
	adjacent[adjacentIndex]->printPath(d,path);
}

short Router::getShortestPath(ElectronicDevice**devices,short src,short dest, short**matrix, short nDevices)
{
	int*distance = new int[nDevices];
	short*p = new short[nDevices];
	bool*visited = new bool[nDevices];
	MinHeap<Pair>q;
	const int infinite = 1000;
	for (short i = 0; i < nDevices; ++i)
	{
		visited[i] = false;
		if (i == src)
			distance[i] = 0;
		else
			distance[i] = infinite;
		q.push(Pair(distance + i, i));
		p[i] = -1;
	}
	short i = 0;
	while (!(q.isEmpty()))
	{
		i = q.pop().index;
		visited[i] = true;
		if (i == dest)
			break;
		for (short j = 0, k = 0; k < devices[i]->nAdjacent; ++j)
		{
			if (matrix[i][j]) //if i and j r adjacent then do the following.\n
			{
				++k;
				if (!(visited[j]) && distance[i] + matrix[i][j] < distance[j])
				{
					distance[j] = distance[i] + matrix[i][j];
					p[j] = i;
				}
			}
		}
	}
	for (i=dest; p[i]!=src;i=p[i]){}
	short j;
	for (j = 0; j<nAdjacent && adjacent[j]!=devices[i];++j){}
	delete[] p;
	delete[] distance;
	delete[] visited;
	return j;
}

void Router::initRT(ElectronicDevice** devices, short**matrix, short nDevices)
{
	short myIndex = getMyIndex(devices, matrix, nDevices);
	for (short i = 0; i < nDevices; ++i)
	{
		if (devices[i]->getName()[0] == 'M' || devices[i]->getName()[0] == 'm')
			RT->insert(RTNode(devices[i], getShortestPath(devices, myIndex, i, matrix, nDevices)));
	}
	cout << Name << " Routing Table.\n";
	RT->display();
}

void Router::reInitRT(ElectronicDevice**devices, short**matrix, short nDevices)
{
	RT->makeEmpty();
	initRT(devices, matrix, nDevices);
}

void Router::changeRT(ElectronicDevice* d, ElectronicDevice*nr, char op)
{
	RT->remove(RTNode(d));
	if (op == 'a' || op == 'A')
	{
		short i;
			for (i = 0; i<nAdjacent && adjacent[i] != d; ++i) {}
			if (i == nAdjacent)
				createEdge(nr);
		RT->insert(RTNode(d, i));
	}
}


//						ROUTING TABLE Node CLASS


RTNode::RTNode(ElectronicDevice* ed, short i)
{
	Destination = ed;
	index = i;
}

bool RTNode::operator <(const RTNode &r)const
{
	return (Destination->getName() < r.Destination->getName());
}

bool RTNode::operator == (const RTNode &r)const
{
	return (Destination->getName() == r.Destination->getName());
}

bool RTNode::operator >(const RTNode &r)const
{
	return (Destination->getName() > r.Destination->getName());
}

bool RTNode::operator != (const RTNode &r)const
{
	return !(Destination->getName() == r.Destination->getName());
}

bool RTNode::operator <= (const RTNode &r)const
{
	return ((*this == r) || (*this < r));
}

bool RTNode::operator >= (const RTNode &r)const
{
	return ((*this == r) || (*this > r));
}

ostream& operator <<(ostream&out,const RTNode&RT)
{
	out << RT.Destination->getName() << "  " << RT.index << endl;
	return out;
}


//				Machine Class

const char Machine::DeviceType = 'M';
SplayTree<short> Machine::Mcount;

Machine::Machine()
{
	short temp = 1;
	while (Mcount.has(temp))
		temp += 1;
	Mcount.insert(temp);
	Name = DeviceType + Num2Str(temp);
	MsgCount = 0;
	tMsg = NULL;
}

Machine::Machine(short mc) 
{
	if (!Mcount.has(mc))
		Mcount.insert(mc);
	Name = DeviceType + Num2Str(mc);
	MsgCount = 0;
	tMsg = NULL;
}

void Machine::generateMsg(short p, string d, string pl)
{
	tMsg = new Message(++MsgCount, p, Name, d, pl);
	OutQs[0]->push(*tMsg);
	adjacent[0]->recieveMsg(OutQs[0]->pop());
}

void Machine::recieveMsg(Message m)
{
	InQ.push(m);
	if (!InQ.isEmpty())
		m = InQ.pop();
	m.addTrace(Name);
	string fname = Name + " Recieved Message.txt";
	fstream ofile(fname, ios::out);
	string text = "Message Text: " + m.PayLoad + '\n' + "Message id: " + Num2Str(m.MsgID) + '\n';
	text += "Path from " + m.Src + " to " + m.Dest+'\n'+m.Trace;
	ofile << text;
}


//					GRAPH CLASS


Graph::Graph()
{
	Devices = NULL;
	Edges = NULL;
	nDevices = 0;
}

short Graph::devicesCount(string s)
{
	string t;
	short count=0;
	for (int i = s.find(',') + 1; i<s.length(); i += t.length() + 1, ++count)
	{
		int j = s.find(',', i) - 1;
		t = s.substr(i, j + 1 - i);
	}
	return count;
}

void Graph::initGraph(string fn, char rt)
{
	Router::RtType = rt;
	fstream ifile(fn, ios::in);
	string s, t;
	getline(ifile, s, '\n');
	nDevices=devicesCount(s);
	Edges = new short*[nDevices];
	Devices = new ElectronicDevice*[nDevices];
	for (int i = 0; i < nDevices && ifile.good(); ++i)
	{
		Edges[i] = new short[nDevices];
		getline(ifile, t, ',');
		if (t[0] == 'M' || t[0] == 'm')
			Devices[i] = new Machine(stringToInt(t.substr(1, t.length() - 1)));
		else if (t[0] == 'R' || t[0] == 'r')
			Devices[i] = new Router(stringToInt(t.substr(1, t.length() - 1)));
		getline(ifile, t, '\n');
		for (short k = 0, j = 0; j < nDevices; ++j)
		{
			s = "";
			for (;t[k] != ',' && k<t.length(); ++k)
				s += t[k];
			++k;
			if (s == "?")
				s = "0";
			Edges[i][j] = stringToInt(s);
		}
	}
	ifile.close();
	for (short i = 0; i < nDevices; ++i)
		Devices[i]->initAdjacents(Devices, Edges, nDevices);
	for (short i = 0; i < nDevices; ++i)
	{
		if (Devices[i]->getName()[0]=='r' || Devices[i]->getName()[0] == 'R')
			Devices[i]->initRT(Devices, Edges, nDevices);
	}
}

short Graph::findDevice(string dName)
{
	for (short i = 0; i<nDevices; ++i)
	{
		if (Devices[i]->Name == dName)
			return i + 1;
	}
	return 0;
}

void Graph::changeRT(string fn, char op)
{
	fstream ifile(fn, ios::in);
	string rout, nextRouter(""), dest;
	short rindex, dindex, nrindex(0);
	while (ifile.good())
	{
		getline(ifile, rout, ':');
		rindex = findDevice(rout);
		if (!rindex)
		{
			cout << "Router " << rout << " not found.\n";
			return;
		}
		--rindex;
		getline(ifile, dest, ':');
		dindex = findDevice(dest);
		if (!dindex)
		{
			cout << "Destination address "<<dest<<" not found.\n";
			return;
		}
		--dindex;
		if (op == 'd' || op == 'D')
		{
			getline(ifile, nextRouter, ':');
			nrindex = findDevice(nextRouter);
			if (!nrindex)
			{
				cout << "Next router address not found.\n";
				return;
			}
		}
		--nrindex;
		if (dest[0] != 'm' || dest[0] != 'M' || (nextRouter[0] != rout[0]))
		{
			cout << "invalid input. Input format should be:\nRouter:Destination Address:Next Router.\n";
		}
		Devices[rindex]->changeRT(Devices[dindex],Devices[nrindex], op);
	}
	ifile.close();
}

void Graph::changeRT()
{
	cout << "Enter the contents of the change RT operation manually or enter through a file?\n";
	cout << "1. Enter through a file.\n";
	cout << "2. Enter manually.\n";
	cout << "0. cancel operation.\n";
	short choice;
	cin >> choice;
	while (choice < 0 || choice > 2)
	{
		cout << "invalid entry. enter again from the above mentioned options.";
		cin >> choice;
	}
	if (!choice)
		return;
	cout << "Enter the option number: \n";
	cout << "1. Add RT.\n";
	cout << "2. Delete RT.\n";
	cout << "0. cancel operation.\n";
	short opchoice;
	cin >> opchoice;
	while (opchoice < 0 || opchoice > 2)
	{
		cout << "invalid entry. enter again from the above mentioned options.";
		cin >> opchoice;
	}
	if (!opchoice)
		return;
	fstream file;
	string fn = "changeRT.csv";
	if (choice == 2)
	{
		string rout, dest, nextRouter;
		cout << "Enter router name: ";
		getline(cin, rout, '\n');
		if (rout=="")
			getline(cin, rout, '\n');
		cout << "Enter Destination address: ";
		getline(cin, dest, '\n');
		if (dest == "")
			getline(cin, dest, '\n');
		cout << "Enter next router.\n";
		getline(cin, nextRouter, '\n');
		if (nextRouter == "")
			getline(cin, nextRouter, '\n');
		file.open(fn, ios::out);
		file << rout << ':' << dest << ':' << nextRouter << '\n';
		file.close();
	}
	else
	{
		cout << "Enter the file name: ";
		cin >> fn;
		file.open(fn, ios::in);
		while (!file.good())
		{
			cout << "invalid file name or file is empty.\n Enter file name again. 0 to cancel: ";
			cin >> fn;
			if (fn == "0")
				return;
			file.close();
			file.open(fn, ios::in);
		}
		file.close();
	}
	char op;
	if (opchoice = 1)
		op = 'A';
	else
		op = 'D';
	changeRT(fn, op);
}

void Graph::changeEdge()
{
	cout << "change edges through a file or enter manually?\n";
	cout << "1. Through a file.\n";
	cout << "2. Enter manually.\n";
	cout << "0. cancel operation.\n";
	short choice;
	cin >> choice;
	while (choice < 0 || choice > 2)
	{
		cout << "invalid entry. choose from the above options.\n";
		cin >> choice;
	}
	if (!choice)
		return;
	fstream file;
	string fn, src, dest;
	short edge;
	if (choice == 1)
	{
		cout << "Enter file name.\n";
		cin >> fn;
		file.open(fn, ios::in);
		while (!file.good())
		{
			cout << "file does not exit or is empty. enter again. 0 to cancel operation.\n";
			cin >> fn;
			if (fn == "0")
				return;
			file.close();
			file.open(fn, ios::in);
		}
		file.close();
	}
	else
	{
		cout << "Enter device 1 name: ";
		cin >> src;
		cout << "Enter device 2 name: ";
		cin >> dest;
		cout << "Enter the edge weight: ";
		cin >> edge;
		fn = "changeEdge.csv";
		file.open(fn, ios::out);
		file << ',' << src << ',' << dest << '\n' << src << ',' << '0' << ',' << edge << '\n';
		file << dest << ',' << edge << ',' << '0' << '\n';
		file.close();
	}
	changeEdge(fn);
}

void Graph::changeEdge(string fn)
{
	fstream file(fn, ios::in);
	string line, t,s;
	short tindex, uindex;
	getline(file, line, '\n');
	short count = devicesCount(line);
	string*tArr = new string[count];
	short**tEdges = new short*[count];
	for (short i = 0; i < count; ++i)
		tEdges[i] = new short[count];
	for (int i = line.find(',') + 1, j, k=0; i<line.length(); i += t.length() + 1, ++k)
	{
		j = line.find(',', i) - 1;
		t = line.substr(i, j + 1 - i);
		tArr[k] = t;
		if (!findDevice(t))
		{
			cout << t << " device not found.\n";
			delete[] tArr;
			for (short i = 0; i < count; ++i)
				delete[] tEdges[i];
			delete[] tEdges;
			return;
		}
	}
	for (short i = 0; i<count && file.good();++i)
	{
		getline(file, line, ',');
		tindex = findDevice(line);
		--tindex;
		getline(file, line, '\n');
		for (short k = 0, j = 0; j < count; ++j)
		{
			s = "";
			for (; line[k] != ',' && k<line.length(); ++k)
				s += line[k];
			++k;
			if (s == "?")
				s = "0";
			tEdges[i][j] = stringToInt(s);
			uindex = findDevice(tArr[j]);
			--uindex;
			if (!Edges[tindex][uindex] && tEdges[i][j])
				Devices[tindex]->createEdge(Devices[uindex]);
			else if (Edges[tindex][uindex] && !tEdges[i][j])
				Devices[tindex]->deleteEdge(Devices[uindex]);
			Edges[tindex][uindex] = tEdges[i][j];
		}
	}
	file.close();
	for (short i = 0; i < nDevices; ++i)
	{
		if (Devices[i]->getName()[0] == 'r' || Devices[i]->getName()[0] == 'R')
			Devices[i]->reInitRT(Devices, Edges, nDevices);
	}
	delete[] tArr;
	for (short i = 0; i < count; ++i)
		delete[]tEdges[i];
	delete[] tEdges;
}

void Graph::mainScreen()
{
	displayOptions();
	short choice;
	cin >> choice;
	while (choice)
	{
		switch (choice)
		{
		case 1: changeEdge();
			break;
		case 2: changeRT();
			break;
		case 3: sendMsg();
			break;
		case 4: printPath();
			break;
		case 0: break;
		default: cout << "Invalid option.\n";
		}
		displayOptions();
		cin >> choice;
	}
}

void Graph::displayOptions()
{
	cout << "Choose from the following options.Enter:\n";
	cout << "1. change edge.\n";
	cout << "2. Change routing table.\n";
	cout << "3. Send a Message.\n";
	cout << "4. Print path.\n";
	cout << "0. exit.\n";
}

void Graph::sendMsg()
{
	string src, dest, pl, fn="";
	short priority, choice;
	cout << "Send msg by reading details through file or enter manally\n";
	cout << "1. Enter manually.\n2, Enter through file.\n0. Cancel operation.\n";
	cin>>choice;
	while (choice < 0 || choice > 2)
	{
		cout << "invalic opteion. Enter again from the above choice.\n";
		cin >> choice;
	}
	if (!choice)
		return;
	if (choice == 1)
	{
		cout << "Enter source of message: ";
		getline(cin, src, '\n');
		if (src == "")
			getline(cin, src, '\n');
		short sindex = findDevice(src);
		while (!sindex)
		{
			cout << src << " invalid source address.Enter source again. 0 to cancel operation: ";
			cin >> src;
			if (src == "0")
				return;
			sindex = findDevice(src);
		}
		--sindex;
		cout << "Enter destination of message: ";
		getline(cin, dest, '\n');
		if (dest == "")
			getline(cin, dest, '\n');
		while (!findDevice(dest))
		{
			cout << "invalid destination addres. Enter again. 0 to cancel operation: ";
			cin >> dest;
			if (dest == "0")
				return;
		}
		cout << "Enter priority of message from 1 to 10. 0 to cancel operation: ";
		cin >> priority;
		while (priority < 0 || priority > 10)
		{
			cout << "Invalid priority entered. Enter priority again, 0 to cancel: ";
			cin >> priority;
		}
		if (!priority)
			return;
		cout << "Enter the text of message: ";
		getline(cin, pl, '\n');
		if (pl == "")
			getline(cin, pl, '\n');
		fn = "sendMsg.txt";
		fstream file(fn, ios::out);
		file << '0' << ':' << priority << ':' << src << ':' << dest << ':' << pl << '\n';
	}
	else
	{
		cout << "Enter file Name.\n";
		getline(cin, fn, '\n');
		if (fn=="")
			getline(cin, fn, '\n');
		fstream file(fn, ios::in);
		if (!file.good())
		{
			cout << "File eihter epty or incorrect file name entered.\n";
			return;
		}
		file.close();
	}
	sendMsg(fn);
}

void Graph::sendMsg(string fn)
{
	short pr, sindex, dindex;
	string src, dest, pl, priority;
	fstream file(fn, ios::in);
	while (file.good())
	{
		getline(file, priority, ':');
		getline(file, priority, ':');
		pr = stringToInt(priority);
		getline(file, src, '\n');
		getline(file, dest, '\n');
		getline(file, pl, '\n');
		sindex = findDevice(src);
		dindex = findDevice(dest);
		if (!sindex || !dindex)
		{
			cout << "Invalid Device name. ";
			if (!sindex)
				cout << src << ' ';
			else
				cout << dest << ' ';
			cout << "not found.\n";
			return;
		}
		Devices[sindex - 1]->generateMsg(pr, dest, pl);
	}
	file.close();
}

void Graph::printPath()
{
	cout << "Enter source: ";
	string src, d;
	cin >> src;
	short sindex=findDevice(src), dindex;
	while (!sindex)
	{
		cout << "Incorrect source address. Enter source again: ";
		cin >> src;
		sindex = findDevice(src);
	}
	--sindex;
	cout << "Enter destination: ";
	cin >> d;
	dindex = findDevice(d);
	while (!dindex)
	{
		cout << "Incorrect destination address. Enter destination again: ";
		cin >> d;
		dindex = findDevice(d);
	}
	--dindex;
	Devices[sindex]->printPath(d, string(""));
}



string Num2Str(int t) {
	stringstream s;
	s << t;
	return s.str();
}

int stringToInt(string str)
{
	int num = 0;
	for (int i = 0; i < str.length(); ++i)
	{
		if (str[i] < 48 || str[i]>57)
		{
			cout << "Invalid string for converting it to integer.\n";
			system("pause");
			exit(-1);
		}
		num = num * 10 + (str[i] - 48);
	}
	return num;
}

Pair::Pair(int* d, short i): data(d), index(i) {}

bool Pair::operator ==(const Pair& p)const 
{
	return ((*data) == *(p.data));
}
bool Pair::operator !=(const Pair&p)const
{
	return !((*data) == *(p.data));
}
bool Pair::operator >(const Pair&p)const
{
	return ((*data) > *(p.data));
}
bool Pair::operator <(const Pair&p)const
{
	return ((*data) < *(p.data));
}

bool Pair::operator <=(const Pair&p)const
{
	return ((*this < p) || (*this == p));
}
bool Pair::operator >=(const Pair&p)const
{
	return ((*this > p) || (*this == p));
}

ostream & operator<< (ostream & out, const Pair&p)
{
	out << *(p.data) << " " << p.index << endl;
	return out;
}