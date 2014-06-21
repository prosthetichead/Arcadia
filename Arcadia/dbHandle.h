#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "sqlite3pp.h"
#include <boost/algorithm/string/replace.hpp>
#include <sys/stat.h>
#include <SFML/Window.hpp>
#include <math.h>

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
		std::string fileName;
		std::string platformID;
		std::string platformName;
	};

	struct gameInfoItem {
		std::string videoPath;
		std::string fanArtPath;
		std::string screenPath;
		std::string clearLogoPath;

		std::string description;
		
		std::string regionName;
		std::string regionIconID;

		std::string genreName;
		std::string genreIconID;

		std::string developer;
		std::string developerIconID;
		std::string publisher;
		std::string publisherIconID;

		std::string release_year;
		std::string rating;
		int players;
		double user_stars;
		double online_stars;
		bool co_op;
		

		gameInfoItem()
		{
			videoPath = "NULL";
			fanArtPath = "NULL";
			screenPath = "NULL";
			clearLogoPath = "NULL";
			description = " ";

			players = 0;
			user_stars = 0;
			online_stars = 0;
			co_op = false;
		}

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

	struct inputItem
	{
		std::string inputName;

		std::string inputType;  // joystick, keyboard, mouse
		sf::Keyboard::Key key;
		int buttonNumber;
		bool press;
		bool hold;
		bool repeat;
		int counter;
		int repeat_time;
	};

	dbHandle(void);
	~dbHandle(void);
	void setFilePath(std::string path, std::string fileName);
	std::vector<dbHandle::gameListItem> dbHandle::getGamesListQuery(std::string whereStatment);
	gameInfoItem dbHandle::getGameInfo( gameListItem );
	std::vector<dbHandle::filterListItem> dbHandle::getPlatformFilterList();
	std::vector<dbHandle::filterListItem> dbHandle::getCustomFilterList(std::string name, std::string sql, std::string games_column_name);
	std::string dbHandle::getLaunchCode(std::string platform_id, std::string file_name);
	std::vector<dbHandle::assetItem> dbHandle::getIconPaths();
	inputItem dbHandle::getInputItem(int input);
	int dbHandle::getMaxPlayers();

};

