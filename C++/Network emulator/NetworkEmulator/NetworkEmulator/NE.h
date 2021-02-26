#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <windows.h>
#include <fstream>
#include "PQ.h"
#include "queue.h"
#include "RT.h"
using namespace std;

class ElectronicDevice
{
protected:
	struct Message
	{
		void display() const;
		void addTrace(string);
		string Destination()const;
		bool operator < (const Message&m);
		bool operator > (const Message&m);
		bool operator == (const Message&m);
		bool operator != (const Message&m);
		bool operator <= (const Message&m);
		bool operator >= (const Message&m);
	private:
		Message(short = 0, short = 0, string = "", string = "", string = "");
		friend class Machine; friend class MinHeap<Message>; friend class queue<Message>;

		short MsgID;
		short Priority;
		string Src;
		string Dest;
		string PayLoad;
		string Trace;
	};
	string Name;
	MinHeap<Message> InQ;
	queue<Message>**OutQs;
	ElectronicDevice**adjacent;
	short nAdjacent;
	friend class Graph; friend class Router;
public:
	ElectronicDevice(string = "");
	virtual void initRT(ElectronicDevice**, short**, short);
	virtual void reInitRT(ElectronicDevice**, short**, short);
	void initAdjacents(ElectronicDevice**, short**, short);
	short getMyIndex(ElectronicDevice**, short**, short);
	string getName()const;
	virtual void forwardMsg();//picks messages from the incoming queue 
	//and places in the corresponding outgoing queues (only for routers)
	virtual void recieveMsg(Message m); //puts the messages from the outgoing queue to the 
	//corresponding incoming queue (for routers)
	//saves the message in a file and discards it (for machines)
	virtual void generateMsg(short, string, string); //(only for machines)
	virtual void changeRT(ElectronicDevice*,ElectronicDevice*, char);
	void createEdge(ElectronicDevice*, bool = false);
	void deleteEdge(ElectronicDevice*, bool = false);
	virtual void printPath(string, string);
};

struct RTNode
{
	ElectronicDevice*Destination;
	short index;	//index of the adjacent device in the adjacent array

	RTNode(ElectronicDevice* = NULL, short=0);
	bool operator <(const RTNode &)const;
	bool operator ==(const RTNode &)const;
	bool operator >(const RTNode &)const;
	bool operator!=(const RTNode&)const;
	bool operator >=(const RTNode &)const;
	bool operator <=(const RTNode &)const;
};

ostream& operator <<(ostream&out, const RTNode&);


class Router : public ElectronicDevice
{
	static const char DeviceType;
	RoutingTable<RTNode>*RT;
public:
	Router();
	Router(short);
	static SplayTree<short> Rcount;
	void recieveMsg(Message m);
	void forwardMsg();
	void initRT(ElectronicDevice**, short**, short) override;
	virtual void reInitRT(ElectronicDevice**, short**, short);
	short getShortestPath(ElectronicDevice**,short,short, short**, short);
	static char RtType;
	void changeRT(ElectronicDevice*,ElectronicDevice*, char)override;
	void printPath(string, string)override;
};

class Machine : public ElectronicDevice
{
	static const char DeviceType;
	short MsgCount;
	Message *tMsg;
public:
	Machine();
	Machine(short);
	static SplayTree<short> Mcount;
	void recieveMsg(Message m)override;
	void generateMsg(short, string, string)override;
};

class Graph
{
	ElectronicDevice**Devices;
	short**Edges;
	short nDevices;

	short devicesCount(string);
	short findDevice(string);
public:
	Graph();
	void initGraph(string, char);
	void changeRT(string,char);
	void changeRT();
	void changeEdge(string);
	void changeEdge();
	void mainScreen();
	void displayOptions();
	void sendMsg();
	void sendMsg(string);
	void printPath();
};

struct Pair
{
	int*data;
	short index;
	Pair(int* = NULL, short=0);
	bool operator ==(const Pair&)const;
	bool operator !=(const Pair&)const;
	bool operator >(const Pair&)const;
	bool operator <(const Pair&)const;
	bool operator <=(const Pair&)const;
	bool operator >= (const Pair&)const;
};

ostream & operator<< (ostream & out, const Pair&p);


string Num2Str(int);
int stringToInt(string);