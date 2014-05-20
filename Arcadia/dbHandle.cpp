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


/******  
*   File Exists
*	Checks using the file path and extentions list if a file with the given extention exists.
*	and returns filename if it does
*   else retuns NULL.
*/
std::string dbHandle::fileExists(std::string file, std::vector<std::string> file_exts)
{
    struct stat buf;
	std::string returnString;
	//boost::replace_all(file, "%PATH%", exe_path);
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
	vector<dbHandle::gameListItem> results;
	sqlite3pp::database db(db_fileName.c_str());
			
	std::string query = "select games.name, games.file_name, platforms.id, platforms.name from games, platforms, genres where games.genre_id = genres.id and games.platform_id = platforms.id and games.active = 1 " + whereStatment + " order by games.name"; 

	sqlite3pp::query qry(db, query.c_str());
	sqlite3pp::query::iterator intBegin = qry.begin();
	sqlite3pp::query::iterator intEnd = qry.end();
	results.reserve(std::distance(intBegin, intEnd));
	if (std::distance(intBegin, intEnd) == 0)
	{
		dbHandle::gameListItem item;
		item.platformID= "NULL";
		item.fileName = "NULL";
		item.title = "No Games To Display";

		results.push_back(item);
	}

	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
			// Tile / Name
			dbHandle::gameListItem newItem;
			newItem.title = (*i).get<const char*>(0);

			// File Name
			newItem.fileName = (*i).get<const char*>(1);

			// Platform ID
			newItem.platformID = (*i).get<const char*>(2);
			
			// platform Name
			newItem.platformName = (*i).get<const char*>(3);

			results.push_back(newItem);
	}
	db.disconnect();

	return results;
}

// Get detailed info strut based on a game list item
dbHandle::gameInfoItem dbHandle::getGameInfo( dbHandle::gameListItem listItem )
{
	
	gameInfoItem infoItem;
	sqlite3pp::database db(db_fileName.c_str());
	std::string query = "select "
						" platforms.images_path "
						" ,platforms.videos_path "
						" ,games.description "
						" ,regions.name "
						" ,regions.icon_id "
						" ,genres.genre_name "
						" ,genres.icon_id "
						" ,case when developer_id = 0 then games.developer else d_comp.name end " // Take the name from games table if its an unknow developer
						" ,d_comp.icon_id "
						" ,case when publisher_id = 0 then games.publisher else p_comp.name end " // Take the name from games table if its an unknow publisher
						" ,p_comp.icon_id "
						" from games, platforms, genres, regions, companies p_comp, companies d_comp "
						" where p_comp.id = games.publisher_id and d_comp.id = games.developer_id and regions.id = games.region_id and games.genre_id = genres.id and games.platform_id = platforms.id and games.platform_id = :platform_id and games.file_name = :file_name"; 
	sqlite3pp::query qry(db, query.c_str());
	qry.bind(":platform_id", listItem.platformID.c_str());
	qry.bind(":file_name", listItem.fileName.c_str());

	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		// LOAD IMAGES
		std::string images_path;
		if ((*i).get<const char*>(0) == NULL)
			images_path = "";
		else
		{
			images_path = (*i).get<const char*>(0);
			boost::replace_all(images_path, "%PATH%", exe_path);
		}
		infoItem.fanArtPath = fileExists(images_path + "\\fanart\\" + listItem.fileName , img_file_exts);  //Fan Art
		infoItem.screenPath = fileExists(images_path + "\\screen\\" + listItem.fileName , img_file_exts);  //Screen Shot
		infoItem.clearLogoPath = fileExists(images_path + "\\clearlogo\\" + listItem.fileName , img_file_exts);  //Clear logo

		// LOAD VIDEO
		std::string video_path;
		if ((*i).get<const char*>(1) == NULL)
			video_path = "NULL";
		else
		{
			video_path = (*i).get<const char*>(1);	
			boost::replace_all(video_path, "%PATH%", exe_path);
			infoItem.videoPath = fileExists(video_path + "\\" + listItem.fileName, movie_file_exts);
		}

		// DESCRIPTION
		if ((*i).get<const char*>(2) == NULL)
			infoItem.description = " ";
		else
			infoItem.description = (*i).get<const char*>(2);

		// REGION
		infoItem.regionName = (*i).get<const char*>(3);
		infoItem.regionIconID = (*i).get<const char*>(4);

		// GENRE
		infoItem.genreName = (*i).get<const char*>(5);
		infoItem.genreIconID = (*i).get<const char*>(6);

		// Developer - Publisher
		if ((*i).get<const char*>(7) == NULL)
			infoItem.developer = "";
		else
			infoItem.developer = (*i).get<const char*>(7);
		infoItem.developerIconID = (*i).get<const char*>(8);
		cout<<infoItem.developerIconID << endl;
		if ((*i).get<const char*>(9) == NULL)
			infoItem.publisher = "";
		else
			infoItem.publisher = (*i).get<const char*>(9);
		infoItem.publisherIconID = (*i).get<const char*>(10);

		

	}
	db.disconnect();

	return infoItem;
}

std::vector<dbHandle::filterListItem> dbHandle::getPlatformFilterList()
{
	vector<dbHandle::filterListItem> filterList;
	
	//Add the all items filter as first filter
	dbHandle::filterListItem newItemAll;
	newItemAll.filterIcon = "ALL";
	newItemAll.filterString = " ";
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

		newItem.filterString = "and platform_id = " + platform_id;
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
	newItemAll.filterString = " ";
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
		newItem.filterString = " and " + newItem.filterString; 
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
	
	std::string query = "select id, file_path from image_assets where type = 'icon'";
	sqlite3pp::query qry(db, query.c_str());

	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		dbHandle::assetItem item;
		item.id = (*i).get<const char*>(0);
		item.path = (*i).get<const char*>(1);

		boost::replace_all(item.path, "%PATH%", exe_path);

		list.push_back(item);
	}	

	db.disconnect();
	return list;
}

dbHandle::inputItem dbHandle::getInputItem(int input)
{
	dbHandle::inputItem item;
	sqlite3pp::database db(db_fileName.c_str());

	std::string query = "select id, name, input_type, keyboard_key_id, buttonNumber from inputs where id = :inputID";
	sqlite3pp::query qry(db, query.c_str());
	qry.bind(":inputID", input);

	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		item.inputName = (*i).get<const char*>(1);
		item.inputType = (*i).get<const char*>(2);

		int key_id = (*i).get<int>(3); 
		item.key = (sf::Keyboard::Key)key_id;
	}

	return item;

	db.disconnect();

}