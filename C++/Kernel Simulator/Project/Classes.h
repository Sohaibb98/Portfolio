#ifndef CLASSES_H_
#define CLASSES_H_
#include <stdio.h>
#include <signal.h>
#include <iostream>
#include <string>
#include <unistd.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <queue>
#include <sys/wait.h>
#include <sys/types.h>
using namespace std;
enum State { RUNNING, READY, BLOCKED, ENDED };
struct Instruction
{
	char ins;
	string operand;

	void convertToInst(string);
};
struct Process
{
	Instruction*IS;	//INSTRUCTION SET
	State s;
	short PC, PID, IR;
	int val;
	int ST;		//START TIME
	int TE;		//TIME EXECUTED SO FAR

	Process();
	void init(int, short, string);
	void execute();
	void display();
	~Process();
};

struct CPU
{
	int Time;
	short nProcess;
	Process**PA;	//PROGRAM ARRAY
	Process*CurRunning;
	short CPTS;	//Current Process Time Slice
	
	CPU();
	State INC();
	bool CRT(string);
	~CPU();
};
struct Kernal
{
	CPU c;
	queue<Process*>Ready;
	queue<Process*>Blocked;
	queue<Process*>Running;
	queue<string> Commands;
	
	void PushCommand(string);
	void executeNext();
	void INC();
	void UNB();
	void CRT(string);
	void END();
	void PRT();
};

void BroadCaster(queue<Process*>&, queue<Process*>&, queue<Process*>&, int);
int stringToInt(string str);

#endif //!CLASSES_H_