#include "DBHandle.h"



using namespace std;

sqlite3pp::database db(0);

dbHandle::dbHandle(void)
{
	db_fileName = " ";

	movie_file_exts.push_back(".mp4");
	img_file_exts.push_back(".jpg");
	img_file_exts.push_back(".png");
}
dbHandle::~dbHandle(void)
{
	db.disconnect();
}

void dbHandle::setFilePath(std::string path, std::string fileName)
{	
	exe_path = path;
	//db_fileName = path + "\\" + fileName;
	db_fileName = "C:\\Users\\Ashley\\Documents\\Python Projects\\arcadia_admin\\arcadia.db";

	db.connect(db_fileName.c_str());
	//db.enableREGEX();
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



vector<dbHandle::gameListItem> dbHandle::getGamesListQuery(std::string whereStatment1, std::string whereStatment2  )
{
	vector<dbHandle::gameListItem> results;

	std::string query = "select distinct "
						"games.name, games.file_name, platforms.id, platforms.name, games.id "
						"from games "
						"join platforms on games.platform_id = platforms.id "
						"left join game_genres on games.id = game_genres.game_id "
						"left join genres on game_genres.genre_id = genres.id "
						"left join game_developers on games.id = game_developers.game_id "
						"left join companies developers on game_developers.developer_id = developers .id "
						"left join game_publishers on games.id = game_publishers.game_id "
						"left join companies publishers on game_publishers.publisher_id = publishers.id "
						"where games.active = 1 and (" + whereStatment1 + ") and (" + whereStatment2 + ") order by games.name ";
	sqlite3pp::query qry(db, query.c_str());
	sqlite3pp::query::iterator intBegin = qry.begin();
	sqlite3pp::query::iterator intEnd = qry.end();
	results.reserve(std::distance(intBegin, intEnd));
	if (std::distance(intBegin, intEnd) == 0)
	{
		dbHandle::gameListItem item;
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

	return results;
}




// Get detailed info strut based on a game list item
dbHandle::gameInfoItem dbHandle::getGameInfo( dbHandle::gameListItem listItem )
{
	gameInfoItem infoItem;
	if ((listItem.platformID == "NULL") && (listItem.fileName == "NULL"))
	{
		return infoItem;
	}

	std::string query = "select "
						" platforms.images_path "
						" ,platforms.videos_path "
						" ,games.description "
						" ,games.players "
						" ,games.stars "
						" ,0 "
						" ,platforms.icon "
						" ,games.seconds_played "
						" ,strftime('%d/%m/%Y', games.last_played) "
						" ,release_year "
						" ,games.id "
						" from games, platforms"
						" where games.platform_id = platforms.id and games.platform_id = :platform_id and games.file_name = :file_name"; 
	sqlite3pp::query qry(db, query.c_str());
	qry.bind(":platform_id", listItem.platformID.c_str());
	qry.bind(":file_name", listItem.fileName.c_str());

	std::string game_id = " ";

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
			infoItem.description = "";
		else
			infoItem.description = (*i).get<const char*>(2);
		
		//players
		infoItem.players = (*i).get<int>(3);

		//User Stars
		infoItem.user_stars =  (*i).get<double>(4);

		//online Stars 
		infoItem.online_stars = (*i).get<double>(5);
		
		//platform Icon ID
		if ((*i).get<const char*>(6) == NULL)
			infoItem.platformIconID = "None";
		else
			infoItem.platformIconID = (*i).get<const char*>(6);

		//played time
		double seconds_played = (*i).get<int>(7);

		int hours = (int)seconds_played / 3600;
		int remainder = (int) seconds_played - hours * 3600;
		int mins = remainder / 60;
		remainder = remainder - mins * 60;
		int secs = remainder;
		if (hours > 0)
			infoItem.playTime = std::to_string(hours) + " Hours " + std::to_string(mins) + " Minites ";
		else if (mins > 0)
			infoItem.playTime = std::to_string(mins) + " Minites " + std::to_string(secs) + " Seconds";
		else if (secs > 0)
			infoItem.playTime = std::to_string(secs) + " Seconds";
		else
			infoItem.playTime = "Not Yet Played";
		
		//last played
		if ((*i).get<const char*>(8) == NULL)
			infoItem.lastPlayed = "Not Yet Played";
		else
			infoItem.lastPlayed = (*i).get<const char*>(8);

		//release year
		if ((*i).get<const char*>(9) == NULL)
			infoItem.release_year = "None";
		else
			infoItem.release_year = (*i).get<const char*>(9);

		game_id = (*i).get<const char*>(10);
	}
	
	//Get Genres
	query = "SELECT genres.name FROM games, game_genres, genres where games.id = game_genres.game_id and genres.id = game_genres.genre_id and games.id = :game_id";
	sqlite3pp::query genreqry(db, query.c_str());
	genreqry.bind(":game_id", game_id.c_str());
	for (sqlite3pp::query::iterator i = genreqry.begin(); i != genreqry.end(); ++i) {
		infoItem.genres.push_back((*i).get<const char*>(0));
	}



	return infoItem;
}

std::vector<dbHandle::filterListItem> dbHandle::getPlatformFilterList()
{
	vector<dbHandle::filterListItem> filterList;
	
	//Add the all items filter as first filter
	dbHandle::filterListItem newItemAll;
	newItemAll.filterIcon = "FILTER_ALL";
	newItemAll.filterString = " 1=1 ";
	newItemAll.title = "All Platforms";
	filterList.push_back(newItemAll);


	std::string query = "select distinct platforms.id, platforms.name, platforms.icon from platforms, games where games.platform_id = platforms.id and games.active = 1";
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

		newItem.filterString = " platform_id = " + platform_id;
		newItem.title = (*i).get<const char*>(1);
		newItem.filterIcon = icon_id; 


		filterList.push_back(newItem);
	}

	return filterList;	
}

std::vector<dbHandle::filterListItem> dbHandle::getCustomFilterList()
{
	vector<dbHandle::filterListItem> filterList;

	//Add the all items filter as first filter
	dbHandle::filterListItem newItemAll;
	newItemAll.filterIcon = "FILTER_ALL";
	newItemAll.filterString = " 1=1 ";
	newItemAll.title = "No Filter";
	filterList.push_back(newItemAll);

	std::string query = "select name, icon, filter_string from filters";
	sqlite3pp::query qry(db, query.c_str());
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		dbHandle::filterListItem newItem;
		std::string filterString = (*i).get<const char*>(2);
		std::string icon_id = "";
		if ((*i).get<const char*>(1) != NULL)
			 icon_id = (*i).get<const char*>(1);
		else
			icon_id = "ERROR";		

		newItem.filterString = filterString;
		newItem.title = (*i).get<const char*>(0);
		newItem.filterIcon = icon_id; 
		filterList.push_back(newItem);
	}
	return filterList;			
}

std::string dbHandle::getLaunchCode(std::string platform_id, std::string file_name)
{
	//db.connect(db_fileName.c_str());
	std::string query = "select load_string, game_load_string, roms_path, extension, emu_path from games, platforms where platforms.id = :platform_id and file_name = :file_name";
	sqlite3pp::query qry(db, query.c_str());
	qry.bind(":platform_id", platform_id.c_str());
	qry.bind(":file_name", file_name.c_str());

	std::string load_string = "";
	std::string game_load_string = "";
	std::string roms_path = "";
	std::string emu_path = "";
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
			if (!extension.empty())
				extension = "." + extension;
		}
		if ((*i).get<const char*>(4) != NULL)
			emu_path = (*i).get<const char*>(4);
	}
	
	boost::replace_all(load_string, "%EMU_PATH%", emu_path);
	boost::replace_all(load_string, "%GAME_LOAD_STRING%", game_load_string);
	boost::replace_all(load_string, "%ROMS_PATH%", roms_path);
	boost::replace_all(load_string, "%FILE_NAME%", file_name + extension);
	boost::replace_all(load_string, "%ARCADIA_PATH%", exe_path);
	//db.disconnect;

	return load_string;
}



std::vector<dbHandle::assetItem> dbHandle::getIconPaths()
{
	std::vector<dbHandle::assetItem> list;

	//sqlite3pp::database db(db_fileName.c_str());
	
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

	//db.disconnect();
	return list;
}

dbHandle::inputItem dbHandle::getInputItem(int input, sf::Keyboard::Key default_key)
{
	dbHandle::inputItem item;
	item.inputID = input;
	item.key = default_key;

	std::string query = "select id, name, input_type, keyboard_key_id from inputs where id = :inputID";
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
}
void dbHandle::updateInputItem(inputItem &item)
{
	//if(item.inputType == "Keyboard")
	//{
		sqlite3pp::command cmd(db, "UPDATE inputs set input_type = :input_type, keyboard_key_id = :key_id where id = :id");
		cmd.bind(":input_type", item.inputType.c_str());
		cmd.bind(":key_id", item.key);
		cmd.bind(":id", item.inputID);
		cmd.execute();
	//}

}

int dbHandle::getMaxPlayers()
{
	int maxPlayers;
	//db.connect(db_fileName.c_str());

	std::string query = "select max(players) from games";
	sqlite3pp::query qry(db, query.c_str());

	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
		maxPlayers = (*i).get<int>(0);
	}

	return maxPlayers;
}

void dbHandle::updateGamePlaytime(std::string platform_id, std::string file_name, int secondsPlayed )
{
	sqlite3pp::command cmd(db, "UPDATE games set seconds_played = seconds_played  + :secondsPlayed , last_played = date('now') where platform_id = :platform_id and file_name = :file_name");
	cmd.bind(":secondsPlayed", secondsPlayed);
	cmd.bind(":platform_id", platform_id.c_str());
	cmd.bind(":file_name", file_name.c_str());
	cmd.execute();
}

//static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
//{
//    ((std::string*)userp)->append((char*)contents, size * nmemb);
//    return size * nmemb;
//}

//std::string dbHandle::getHTMLdata(std::string URL)
//{
//	CURL *curl;
//    CURLcode res;
//	std::string readBuffer;
//
//    curl = curl_easy_init();
//    if (curl) {
 //       curl_easy_setopt(curl, CURLOPT_URL, URL);
//		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
//		 curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
 //       res = curl_easy_perform(curl);
//
 //       /* always cleanup */
  //      curl_easy_cleanup(curl);
  //  }
//	return readBuffer;
//}

std::vector<dbHandle::findGameResultItem> dbHandle::findGame_onGameDb( dbHandle::gameListItem list_item )
{
	
	std::string searchString = regex_replace(list_item.title,  regex("\([^)]+\)"), "");
	searchString = regex_replace(searchString, regex("[^a-zA-Z0-9 ]"), "");

	std::string platformName = list_item.platformName;
	std::string url = "/api/GetGamesList.php?name=" + searchString + "&platform=" + platformName;
	url = regex_replace(url, regex(" "), "%20");

	sf::Http http("http://thegamesdb.net/");
	sf::Http::Request request;
	request.setMethod(sf::Http::Request::Get);
	request.setUri(url);
	sf::Http::Response response = http.sendRequest(request);
	std::string xmlData = response.getBody();
	std::cout << xmlData << std::endl;

	tinyxml2::XMLDocument doc;
	doc.Parse(xmlData.c_str());
	tinyxml2::XMLNode *rootnode = doc.RootElement();

	std::vector<dbHandle::findGameResultItem> result;
	dbHandle::findGameResultItem item;
	
	for(tinyxml2::XMLElement* elem = rootnode->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string elemName = elem->Value();
		if (elemName == "Game")
		{
			item.gameDB_ID = "0";
			item.gameName = "NULL";
			for(tinyxml2::XMLElement* gameElem = elem->FirstChildElement(); gameElem != NULL; gameElem = gameElem->NextSiblingElement())
			{
				std::string gameElemName = gameElem->Value();
				if (gameElemName == "id")
				{
					item.gameDB_ID = gameElem->GetText();
				}
				if (gameElemName == "GameTitle")
				{
					std::string gameTitle = regex_replace(gameElem->GetText(), regex("[^a-zA-Z0-9 ]"), "");  //strip special chars from title
					item.gameName = gameTitle;
				}
			}
			result.push_back(item);
			std::cout << item.gameDB_ID << " - " << item.gameName << std::endl;
		}
	}
	
	return result;
}