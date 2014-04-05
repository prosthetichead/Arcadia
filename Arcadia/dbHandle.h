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
	struct gameListItem {
		std::string title;
		std::string gameID;
		std::string region;
		std::string platform;
		std::string platformID;
	};
	struct filterListItem {
		std::string title;
		std::string filterIcon;
		std::string filterString;
	};
	

	dbHandle(void);
	dbHandle(std::string fileName);
	std::vector<gameListItem> getFullGamesList();
	void setFilePath(std::string fileName);
	std::vector<dbHandle::gameListItem> dbHandle::getFullGamesList2();
	std::vector<dbHandle::gameListItem> dbHandle::getGamesListQuery(std::string whereStatment);
	std::vector<dbHandle::filterListItem> dbHandle::getPlatformFilterList();
};

