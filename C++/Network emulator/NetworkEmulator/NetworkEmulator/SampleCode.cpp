#include <iostream>
#include <Windows.h>
#include <process.h>
#include <string.h>
using namespace std;

HANDLE h_mainThread, h_timer = NULL;
void newThread(void *);
void timer(void *);

void resume(HANDLE h, string str)
{
	const char *c = str.c_str();
	DWORD dwStatus = ResumeThread(h);
	DWORD dwErr = GetLastError();
	printf(c,
		dwStatus,
		dwErr);
}
void newThread(void *)
{
	//****************************************************
	//Functionality
	//****************************************************
	int y = 0;
	while (y != 5)
	{
		cout << "\nEnter in new thread: ";
		cin >> y;
		cin.ignore();
	}
	//----------------------------------------------------

	//****************************************************
	//Resume main thread
	//****************************************************
	resume(h_mainThread, "\nResuming Main thread");
	//----------------------------------------------------

	//****************************************************
	//Resume timer
	//****************************************************
	resume(h_timer, "\nResuming Timer");
	//----------------------------------------------------
}
void timer(void *)
{
	//Getting handler for timer thread
	h_timer = OpenThread(THREAD_ALL_ACCESS,
		FALSE,
		GetCurrentThreadId());

	while (1)
	{
		//Keep main thread alive for 1 sec.
		Sleep(    // minutes to sleep
			3 *   // seconds to a minute
			1000); // milliseconds to a second;

		printf("\nSuspending main thread.\n");
		//****************************************************
		//Suspend main thread
		//****************************************************
		SuspendThread(h_mainThread);
		//----------------------------------------------------

		_beginthread(newThread, 0, NULL); // start another thread

		printf("\nSuspending timer.\n");
		//****************************************************
		//Suspend timer
		//****************************************************
		SuspendThread(h_timer);
		//----------------------------------------------------
	}
}

//int main()
//{
//	//Getting handler for main thread
//	h_mainThread = OpenThread(THREAD_ALL_ACCESS,
//		FALSE,
//		GetCurrentThreadId());
//
//	_beginthread(timer, 0, NULL); // start timer thread
//
//								  //****************************************************
//								  //Functionality
//								  //****************************************************
//	int x = 0;
//	while (x != 5)
//	{
//		cout << "\nResiding in Main...";
//		//cout << "Enter in Main:";
//		//cin >> x;
//		//cin.ignore();
//		Sleep(1000);
//	}
//	//----------------------------------------------------
//	return 0;
//}