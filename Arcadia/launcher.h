#pragma once
#include "dbHandle.h"
#include "Windows.h"
#include "TCHAR.H"
#include "inputHandle.h"
#include <time.h>

class launcher
{
private:
	dbHandle* db;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	double startTime;
	double endTime;
	int totalSeconds;
	std::string currentPlatformID;
	std::string currentFileName;

public:
	bool processRunning;
	launcher(dbHandle* db_ref);
	~launcher(void);
//	bool update(inputHandle::inputState inputStates, dbHandle::gameListItem gameItem);
	bool launcher::launchGame(std::string platform_id, std::string file_name);
	bool launcher::terminate();
};

