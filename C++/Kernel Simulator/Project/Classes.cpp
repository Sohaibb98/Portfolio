#include "Classes.h"

void Instruction::convertToInst(string inst)
{
	short pos = inst.find(' ', 0);
	string t = inst.substr(0, pos);
	if (t == "S" || t == "s" || t == "A" || t == "a" || t == "D" || t == "d" || t == "R" || t == "r")
	{
		ins = t[0];
		if (pos + 1 < inst.size())
			operand = inst.substr(pos + 1, inst.find('\n', pos + 1) - pos - 1);
		else
			ins = 'X';
	}
	else if (inst[0]=='B' || inst[0]=='b' || inst[0]=='E' || inst[0]=='e')
		ins = t[0];
	else
		ins = 'X';
	if (ins > 90)	//Converting to Upper Case letters
		ins -= 32;
}

Process::Process()
{
	IS = new Instruction[0];
}
void Process::init(int start, short pid, string fn)
{
	delete[] IS;
	ST = start;
	PID = pid;
	val = PC = TE = 0;
	IR = 0;
	fstream file(fn.c_str(), ios::in);
	string line;
	for (; file.good(); ++IR)
		getline(file, line, '\n');
	file.close();
	file.open(fn.c_str(), ios::in);
	IS = new Instruction[IR];
	for (short i = 0; i < IR; ++i)
	{
		getline(file, line, '\n');
		IS[i].convertToInst(line);
	}
	file.close();
	s = READY;
	cout << "INITIALIZED with IR: " << IR << endl;
}

void Process::display()
{
	cout << PID << '\t' << getppid() << '\t' << val << '\t' << ST << '\t' << TE << endl;
}

void Process::execute()
{
	if (PC < IR)
	{
		s = RUNNING;
		char curIns = IS[PC].ins;
		stringstream ss(IS[PC].operand);
		int tval;
		switch (curIns)
		{
		case 'S': ss >> val;
			break;
		case 'A': ss >> tval;
			val += tval;
			break;
		case 'D': ss >> tval;
			val -= tval;
			break;
		case 'R': tval = TE;
			init(ST, PID, IS[PC].operand);
			TE = tval;
			PC = -1;
			break;
		case 'E':s = ENDED;
			break;
		case 'B':s = BLOCKED;
			break;
		default: break;
		}
		++PC;
		++TE;
	}
	if (PC==IR)
		s = ENDED;
}
Process::~Process()
{
	delete[] IS;
}



CPU::CPU()
{
	Time = 0;
	CPTS = 0;
	nProcess = 0;
	CurRunning = NULL;
	PA = new Process*[0];
}

State CPU::INC()
{
	Time += 1;
	if (CurRunning)
	{
		CPTS += 1;
		CurRunning->execute();
		if (CPTS == 3 || CurRunning->s == ENDED || CurRunning->s == BLOCKED)
		{
			CPTS = 0;
			if (CurRunning->s == RUNNING)
				CurRunning->s = READY;
		}
		return CurRunning->s;
	}
	return ENDED;
}

bool CPU::CRT(string fn)
{
	fstream file(fn.c_str(), ios::in);
	if (file.good())
	{
		file.close();
		Process**temp = new Process*[nProcess + 1];
		for (short i = 0; i < nProcess; ++i)
			temp[i] = PA[i];
		delete[] PA;
		PA = temp;
		PA[nProcess++] = new Process();
		PA[nProcess - 1]->init(Time, nProcess, fn);
		if (!CurRunning)
			CurRunning = PA[nProcess - 1];
		return true;
	}
	cout << "File is Empty or Invalid file name: " << fn << ". Process Not Created." << endl;
	return false;
}

CPU::~CPU()
{
	delete[] PA;
}



void Kernal::PushCommand(string c)
{
	Commands.push(c);
}

void Kernal::executeNext()
{
	if (!Commands.empty())
	{
		string com = Commands.front();
		Commands.pop();
		if (com.substr(0, com.find(' ', 0)) == "CRT")
		{
			CRT(com.substr(com.find(' ', 0) + 1, com.size() - com.find(' ', 0) - 1));
		}
		else if (com == "INC")
			INC();
		else if (com == "UNB")
			UNB();
		else if (com == "PRT")
			PRT();
		else if (com == "END")
			END();
		else
			cout << "Invalid command '" << com << "'. Command Skipped.\n";
	}
	else
		cout << "No more commands in the Commands Queue.\n";
}
void Kernal::INC()
{
	State s = c.INC();
	if (!Running.empty())
	{
		if (s != RUNNING)
		{
			if (s == BLOCKED)
				Blocked.push(c.CurRunning);
			else if (s == READY)
			{
				cout << "Pushing to ready.\n";
				Ready.push(c.CurRunning);
			}
			Running.pop();
			c.CurRunning = NULL;
			if (!Ready.empty())
			{
				cout << "Pushing to Running from Ready.\n";
				Running.push(Ready.front());
				c.CurRunning = Running.front();
				Ready.pop();
			}
			else
				cout << "All processes are now Blocked or terminated. CPU is FREE.\n";
		}
	}
	else
		cout << "No Process is running right now.\n";
}
void Kernal::UNB()
{
	if (!Blocked.empty())
	{
		Ready.push(Blocked.front());
		Blocked.pop();
		if (Running.empty())
		{
			Running.push(Ready.front());
			c.CurRunning = Running.front();
			Ready.pop();
		}
	}
	else
		cout << "No Process is blocked currently.\n";
}
void Kernal::CRT(string fn)
{
	if (c.CRT(fn))
	{
		if (Running.empty())
		{
			Running.push(c.CurRunning);
			c.CurRunning->s = RUNNING;
		}
		else
		{
			Ready.push(c.PA[c.nProcess - 1]);
			c.PA[c.nProcess - 1]->s = READY;
		}
	}
}
void Kernal::END()
{
	PRT();
	exit(0);
}
void Kernal::PRT()
{
	if (fork() == 0)		//BroadCaster Process
		BroadCaster(Running, Ready, Blocked, c.Time);
	else
		wait(NULL);		//Wait for Broad Caster Process to end
}



void BroadCaster(queue<Process*>&run, queue<Process*>&ready, queue<Process*>&b, int t)
{
	cout << endl;
	cout << "***************************************" << endl;
	cout << "The current system state is follows: " << endl;
	cout << "***************************************" << endl;
	cout << "CURRENT TIME: " << t << endl << endl << endl;

	cout << "RUNNING PROCESS: \n";
	if (!run.empty())
		run.front()->display();
	else
		cout << "No Running Process Currently.\n";
	cout << endl << "READY QUEUE PROCESSES: \n";
	for (short i = 0; i < ready.size(); ++i)
	{
		ready.front()->display();
		ready.push(ready.front());
		ready.pop();
	}

	cout << endl << "BLOCKED QUEUE PROCESSES: \n";
	for (short i = 0; i < b.size(); ++i)
	{
		b.front()->display();
		b.push(b.front());
		b.pop();
	}
	cout << endl << endl;
	exit(0);
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