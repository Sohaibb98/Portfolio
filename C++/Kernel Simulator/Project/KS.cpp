#include "Classes.h"
Kernal k;
int fd[2];
pid_t ControllerPID;
void ReadCommand(int num)		//CONTROLLER PROCESS
{	//Writes to the Command queue and executes next command in queue
	char buff[100];
	read(fd[0], buff, 100);
	cout << "Text read from pipe: '" << buff << "'\n";
	k.PushCommand(string(buff));
	k.executeNext();
}
void InputCommand(int sig)		//IN CHARGE PROCESS
{	//Reads a command from terminal and writes it to pipe
	char arr[100];
	string s;
	cout << "Enter a command: ";
	getline(cin, s, '\n');
	if (s=="")
		getline(cin, s, '\n');
	write(fd[1], s.c_str(), 100);
	alarm(2);
	kill(ControllerPID, SIGUSR1);
}
int main()
{
	signal(SIGUSR1, ReadCommand);
	pipe(fd);
	ControllerPID = fork();
	if (ControllerPID==0)
	{	//CONTROLLER PROCESS
		close(fd[1]);
		while (1){}
	}
	else
	{	//IN CHARGE PROCESS
		signal(SIGALRM, InputCommand);
		alarm(2);
		close(fd[0]);
		wait(NULL);
	}
	return 0;
	return 0;
}

