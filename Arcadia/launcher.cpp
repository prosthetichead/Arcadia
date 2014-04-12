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

bool launcher::update(inputHandle::inputState inputStates, dbHandle::gameListItem gameItem)
{
	if(inputStates.btn_1_press)
	{
		launchGame(db.getLaunchCode(gameItem.platformID, gameItem.gameID));
	}
	return true;
}

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

