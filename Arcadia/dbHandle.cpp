#include "dbHandle.h"



using namespace std;

dbHandle::dbHandle(void)
{
	db_fileName = " ";

	movie_file_exts.push_back(".mp4");
	img_file_exts.push_back(".jpg");
	img_file_exts.push_back(".png");
}

void dbHandle::setFilePath(std::string path, std::string fileName)
{
	exe_path = path;
	db_fileName = path + "\\" + fileName;
}

std::string dbHandle::fileExists(std::string file, std::vector<std::string> file_exts)
{
    struct stat buf;
	std::string returnString;

	for (int i = 0; i < file_exts.size(); i++)
	{
		returnString = file + file_exts.at(i);
		if (stat(returnString.c_str(), &buf) == 0)
		{
			return returnString;
		}
	}
    return "NULL";
}



vector<dbHandle::gameListItem> dbHandle::getGamesListQuery(std::string whereStatment)
{
	std::cout << whereStatment << std::endl;
	vector<dbHandle::gameListItem> results;
	sqlite3pp::database db(db_fileName.c_str());
	std::string query = "select games.name, games.region, platforms.name, platforms.id, platforms.videos_path, games.file_name, platforms.images_path from games, platforms where games.platform_id = platforms.id and games.active = 1 " + whereStatment + " order by games.name"; 
	sqlite3pp::query qry(db, query.c_str());
	sqlite3pp::query::iterator intBegin = qry.begin();
	sqlite3pp::query::iterator intEnd = qry.end();
	results.reserve(std::distance(intBegin, intEnd));
	if (std::distance(intBegin, intEnd) == 0)
	{
		dbHandle::gameListItem item;
		item.platform = -1;
		item.fileName = "";
		item.region = "NULL";
		item.title = "No Games To Display";
		item.videoPath = "NULL";

		results.push_back(item);
	}

	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
			dbHandle::gameListItem newItem;
			newItem.title = (*i).get<const char*>(0);

			// Region
			if ((*i).get<const char*>(1) == NULL)
				newItem.region = "NULL";
			else
				newItem.region = (*i).get<const char*>(1);

			// Platform Name
			if ((*i).get<const char*>(2) == NULL)
				newItem.platform = "NULL";
			else
				newItem.platform = (*i).get<const char*>(2);

			// Platform ID
			newItem.platformID = (*i).get<const char*>(3);
			// File Name
			newItem.fileName = (*i).get<const char*>(5);



			// Video Path
			if ((*i).get<const char*>(4) == NULL)
				newItem.videoPath = "";
			else
				newItem.videoPath = (*i).get<const char*>(4);

			std::string image_path = (*i).get<const char*>(6);


			
			boost::replace_all(newItem.videoPath, "%PATH%", exe_path);
			newItem.videoPath = newItem.videoPath + "\\" + newItem.fileName;
			newItem.videoPath = fileExists(newItem.videoPath, movie_file_exts);
			
			boost::replace_all(image_path, "%PATH%", exe_path);
			newItem.fanArtPath = image_path + "\\fanart\\" + newItem.fileName;
			newItem.fanArtPath = fileExists(newItem.fanArtPath, img_file_exts);


			results.push_back(newItem);
	}
	db.disconnect();

	return results;
}

std::vector<dbHandle::filterListItem> dbHandle::getPlatformFilterList()
{
	vector<dbHandle::filterListItem> filterList;
	
	//Add the all items filter as first filter
	dbHandle::filterListItem newItemAll;
	newItemAll.filterIcon = "ALL";
	newItemAll.filterString = "NULL";
	newItemAll.title = "All Platforms";
	filterList.push_back(newItemAll);

	sqlite3pp::database db(db_fileName.c_str());
	std::string query = "select distinct platforms.id, platforms.name, platforms.icon_id from platforms, games where games.platform_id = platforms.id and games.active = 1";
	sqlite3pp::query qry(db, query.c_str());
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		dbHandle::filterListItem newItem;
		std::string platform_id = (*i).get<const char*>(0);
		std::string icon_id = "";
		if ((*i).get<const char*>(2) != NULL)
			 icon_id = (*i).get<const char*>(2);
		else
			icon_id = "ERROR";

		newItem.filterString = "platform_id = " + platform_id;
		newItem.title = (*i).get<const char*>(1);
		newItem.filterIcon = icon_id; 

		filterList.push_back(newItem);
	}
	db.disconnect();

	return filterList;	
}

std::vector<dbHandle::filterListItem> dbHandle::getFilterList()
{
	vector<dbHandle::filterListItem> filterList;
	
	//Add the all items filter as first filter
	dbHandle::filterListItem newItemAll;
	newItemAll.filterIcon = "ALL";
	newItemAll.filterString = "NULL";
	newItemAll.title = "No Filter";
	filterList.push_back(newItemAll);

	sqlite3pp::database db(db_fileName.c_str());
	std::string query = "select name, filter_string, icon_id  from filters";
	sqlite3pp::query qry(db, query.c_str());
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		dbHandle::filterListItem newItem;	
		newItem.title = (*i).get<const char*>(0);
		newItem.filterString = (*i).get<const char*>(1);
		newItem.filterIcon = (*i).get<const char*>(2);

		filterList.push_back(newItem);
		
	}
	db.disconnect();
	return filterList;	
}

std::string dbHandle::getLaunchCode(std::string platform_id, std::string file_name)
{
	sqlite3pp::database db(db_fileName.c_str());
	std::string query = "select load_string, game_load_string, roms_path, extension from games, platforms where platforms.id = :platform_id and file_name = :file_name";
	sqlite3pp::query qry(db, query.c_str());
	qry.bind(":platform_id", platform_id.c_str());
	qry.bind(":file_name", file_name.c_str());

	
	std::string load_string = "";
	std::string game_load_string = "";
	std::string roms_path = "";
	std::string extension = "";
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		if ((*i).get<const char*>(0) != NULL)
			load_string = (*i).get<const char*>(0);
		if ((*i).get<const char*>(1) != NULL)
			game_load_string = (*i).get<const char*>(1);
		if ((*i).get<const char*>(2) != NULL)
			roms_path = (*i).get<const char*>(2);		
		if ((*i).get<const char*>(3) != NULL)
		{
			extension = (*i).get<const char*>(3);
			extension = "." + extension;
		}
	}
	
	boost::replace_all(load_string, "%GAME_LOAD_STRING%", game_load_string);
	boost::replace_all(load_string, "%ROMS_PATH%", roms_path);
	boost::replace_all(load_string, "%FILE_NAME%", file_name + extension);
	boost::replace_all(load_string, "%PATH%", exe_path);
	
	db.disconnect();
	return load_string;
}



std::vector<dbHandle::assetItem> dbHandle::getIconPaths()
{
	std::vector<dbHandle::assetItem> list;

	sqlite3pp::database db(db_fileName.c_str());
	
	std::string query = "select id, file_path from assets";
	sqlite3pp::query qry(db, query.c_str()); 

	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		dbHandle::assetItem item;
		item.id = (*i).get<const char*>(0);
		item.path = (*i).get<const char*>(1);

		boost::replace_all(item.path, "%PATH%", exe_path);

		list.push_back(item);
	}	


	std::cout << "Completed icon Paths Load" << std::endl;
	return list;
	
}