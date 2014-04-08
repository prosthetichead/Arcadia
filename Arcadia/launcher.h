#pragma once
#include "dbHandle.h"
#include "Windows.h"
#include "TCHAR.H"

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
	bool launcher::launchGame();
	bool launcher::terminate();
};

