#include "launcher.h"




launcher::launcher(dbHandle& db_ref): db(db_ref)
{
	processRunning = false; 
}


launcher::~launcher(void)
{
	//TerminateProcess(pi.hProcess, 1);
}

bool launcher::terminate()
{
	try
	{
		//PostThreadMessage(pi.dwThreadId,WM_QUIT, 0, 0);
		TerminateProcess(pi.hProcess, 1);
		processRunning = false;
		return true;
	}
	catch (...)
	{
		std::cout << "WTF Just Happened?" << std::endl;
	}
}

//bool launcher::update()
//{
//	if(inputStates.start_game_press)
//	{
//		launchGame(db.getLaunchCode(gameItem.platformID, gameItem.fileName));
//	}
//	return true;
//}

bool launcher::launchGame(std::string commandString)
{
	if (processRunning)
	{
		TerminateProcess(pi.hProcess, 1);
		processRunning = false;
	}

	std::wstring test;
	LPSTR szCmdline = const_cast<char *>(commandString.c_str());
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
			std::cout << "Unable to execute." << std::endl;
			return false;
		}

	//dwExitCode = WaitForSingleObject(pi.hProcess, (1000));
	processRunning = true;
	
	//TerminateProcess(pi.hProcess, dwExitCode);
	
	return true;
}

