#pragma once
#include "dbHandle.h"
#include "Windows.h"
#include "TCHAR.H"
#include "inputHandle.h"

class launcher
{
private:
	dbHandle db;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;


public:
	bool processRunning;
	launcher(void);
	~launcher(void);
	void launcher::init(dbHandle& db_obj);
//	bool update(inputHandle::inputState inputStates, dbHandle::gameListItem gameItem);
	bool launcher::launchGame(std::string commandString);
	bool launcher::terminate();
};

