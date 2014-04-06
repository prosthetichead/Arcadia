#include "dbHandle.h"

using namespace std;

dbHandle::dbHandle(void)
{
	db_fileName = " ";
}
dbHandle::dbHandle(std::string fileName)
{
	db_fileName = fileName;
}
void dbHandle::setFilePath(std::string fileName)
{
	db_fileName = fileName;
}

vector<dbHandle::gameListItem> dbHandle::getFullGamesList()
{
	vector<dbHandle::gameListItem> results;
	results.reserve(400000);
	sqlite3pp::database db("database.db");
	sqlite3pp::query qry(db, "select games.name, games.game_id, games.region, platforms.alias from games, platforms where games.platform_id = platforms.id");
	
	sqlite3pp::query::iterator intBegin = qry.begin();
	sqlite3pp::query::iterator intEnd = qry.end();

	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
			dbHandle::gameListItem newItem;
			newItem.title = (*i).get<const char*>(0);
			newItem.gameID = (*i).get<const char*>(1);
			if ((*i).get<const char*>(2) == NULL)
				newItem.region = "NULL";
			else
				newItem.region = (*i).get<const char*>(2);
			if ((*i).get<const char*>(3) == NULL)
				newItem.platform = "NULL";
			else
				newItem.platform = (*i).get<const char*>(3);
			
			results.push_back(newItem);
	}
	return results;
}

vector<dbHandle::gameListItem> dbHandle::getGamesListQuery(std::string whereStatment)
{
	std::cout << "select games.name, games.game_id, games.region, platforms.alias from games, platforms where games.platform_id = platforms.id " + whereStatment;
	vector<dbHandle::gameListItem> results;
	sqlite3pp::database db("database.db");
	std::string query = "select games.name, games.game_id, games.region, platforms.alias from games, platforms where games.platform_id = platforms.id " + whereStatment; 
	sqlite3pp::query qry(db, query.c_str());
	sqlite3pp::query::iterator intBegin = qry.begin();
	sqlite3pp::query::iterator intEnd = qry.end();
	results.reserve(std::distance(intBegin, intEnd));
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
			dbHandle::gameListItem newItem;
			newItem.title = (*i).get<const char*>(0);
			newItem.gameID = (*i).get<const char*>(1);
			if ((*i).get<const char*>(2) == NULL)
				newItem.region = "NULL";
			else
				newItem.region = (*i).get<const char*>(2);
			if ((*i).get<const char*>(3) == NULL)
				newItem.platform = "NULL";
			else
				newItem.platform = (*i).get<const char*>(3);
			
			results.push_back(newItem);
	}
	return results;
}

std::vector<dbHandle::filterListItem> dbHandle::getPlatformFilterList()
{
	vector<dbHandle::filterListItem> filterList;
	sqlite3pp::database db("database.db");
	std::string query = "select distinct platforms.id, platforms.name, platforms.alias from platforms, games where games.platform_id = platforms.id and games.active = 1";
	sqlite3pp::query qry(db, query.c_str());
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{

		dbHandle::filterListItem newItem;
		std::string platform_ID = (*i).get<const char*>(0);
		std::string platform_alias = (*i).get<const char*>(2);
		newItem.filterString = "platform_id = " + platform_ID;
		newItem.title = (*i).get<const char*>(1);
		newItem.filterIcon = ".\\assets\\icons\\PLATFORM_" + platform_alias + ".png"; 

		filterList.push_back(newItem);
		
	}
	return filterList;
	
}