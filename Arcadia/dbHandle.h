#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "sqlite3pp.h"



class dbHandle
{
private:
	std::string db_fileName;

public:
	struct listItem {
		std::string title;
		std::string gameID;
		std::string region;
		std::string platform;
		std::string platformID;
	};

	dbHandle(void);
	dbHandle(std::string fileName);
	std::vector<listItem> getFullGamesList();
	void setFilePath(std::string fileName);
	std::vector<dbHandle::listItem> dbHandle::getFullGamesList2();
	std::vector<dbHandle::listItem> dbHandle::getGamesListQuery(std::string whereStatment);
};

