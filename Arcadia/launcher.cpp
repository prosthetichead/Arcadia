#include "launcher.h"




launcher::launcher(void)
{
	processRunning = false; 
}


launcher::~launcher(void)
{
	//TerminateProcess(pi.hProcess, 1);
}

void launcher::init(dbHandle& db_obj)
{
	db = db_obj;
}
bool launcher::terminate()
{
	TerminateProcess(pi.hProcess, 1);
	processRunning = false;
	return true;
}
bool launcher::launchGame()
{
	if (processRunning)
	{
		TerminateProcess(pi.hProcess, 1);
		processRunning = false;
	}

	LPTSTR szCmdline = _tcsdup(TEXT("C:\\Users\\Ashley\\Documents\\Visual Studio 2012\\Projects\\Arcadia\\Release\\platforms\\NES\\emu\\fceux.exe \"C:\\Users\\Ashley\\Documents\\Visual Studio 2012\\Projects\\Arcadia\\Arcadia\\platforms\\NES\\roms\\River City Ransom (USA).nes\" "));
	DWORD dwExitCode = 0;

	
	ZeroMemory( &si, sizeof(si) );
	si.cb = sizeof(si);
	ZeroMemory( &pi, sizeof(pi) );
	if (!
	CreateProcess
			(
			NULL,
			szCmdline
			,NULL,NULL,FALSE,
			CREATE_NEW_CONSOLE,
			NULL,NULL,
			&si,
			&pi
			)
			)
		{
			std::cout << "Unable to execute.";
			return false;
		}

	//dwExitCode = WaitForSingleObject(pi.hProcess, (1000));
	processRunning = true;
	
	//TerminateProcess(pi.hProcess, dwExitCode);
	
	return true;
}

