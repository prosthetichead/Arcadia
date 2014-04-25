#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "sqlite3pp.h"
#include <boost/algorithm/string/replace.hpp>
#include <sys/stat.h>



class dbHandle
{
private:
	std::string db_fileName;
	std::vector<std::string> movie_file_exts;
	std::vector<std::string> img_file_exts;

	std::string dbHandle::fileExists(std::string file, std::vector<std::string> file_exts);

public:
	std::string exe_path;

	struct gameListItem {
		std::string title;
		std::string platformID;
		std::string fileName;
		std::string region;
		std::string platform;
		std::string videoPath;
		std::string fanArtPath;
	};
	struct filterListItem {
		std::string title;
		std::string filterIcon;
		std::string filterString;
	};
	struct assetItem {
		std::string id;
		std::string path;
	};

	

	dbHandle(void);
	void setFilePath(std::string path, std::string fileName);
	std::vector<dbHandle::gameListItem> dbHandle::getGamesListQuery(std::string whereStatment);
	std::vector<dbHandle::filterListItem> dbHandle::getPlatformFilterList();
	std::vector<dbHandle::filterListItem> dbHandle::getFilterList();
	std::string dbHandle::getLaunchCode(std::string platform_id, std::string game_id);
	std::vector<dbHandle::assetItem> dbHandle::getIconPaths();

};

