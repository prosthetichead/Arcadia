#include "launcher.h"




launcher::launcher(dbHandle* db_ref)
{
	db = db_ref;
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
		endTime = time(NULL);
		totalSeconds = endTime - startTime;
		
		std::cout << totalSeconds << std::endl;
		db->updateGamePlaytime(currentPlatformID, currentFileName, totalSeconds);
		return true;
	}
	catch (...)
	{
		std::cout << "WTF Just Happened?" << std::endl;
	}
}

bool launcher::launchGame(std::string platform_id, std::string file_name)
{
	std::string commandString = db->getLaunchCode(platform_id, file_name);
	std::cout << commandString << std::endl;
	if (processRunning)
	{
		terminate();
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

	processRunning = true;
	startTime = time(NULL);
	currentPlatformID = platform_id;
	currentFileName = file_name;

	return true;
}

