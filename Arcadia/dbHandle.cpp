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

vector<dbHandle::listItem> dbHandle::getFullGamesList()
{
	vector<dbHandle::listItem> results;
	results.reserve(400000);
	sqlite3pp::database db("database.db");
	sqlite3pp::query qry(db, "select games.name, games.game_id, games.region, platforms.alias from games, platforms where games.platform_id = platforms.id");
	
	sqlite3pp::query::iterator intBegin = qry.begin();
	sqlite3pp::query::iterator intEnd = qry.end();

	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
			dbHandle::listItem newItem;
			newItem.title = (*i).get<const char*>(0);
			newItem.gameID = (*i).get<const char*>(1);
			if ((*i).get<const char*>(2) == NULL)
				newItem.region = 'NULL';
			else
				newItem.region = (*i).get<const char*>(2);
			if ((*i).get<const char*>(3) == NULL)
				newItem.platform = 'NULL';
			else
				newItem.platform = (*i).get<const char*>(3);
			
			results.push_back(newItem);
	}
	return results;
}

vector<dbHandle::listItem> dbHandle::getGamesListQuery(std::string whereStatment)
{
	vector<dbHandle::listItem> results;
	sqlite3pp::database db("database.db");
	std::string query = "select games.name, games.game_id, games.region, platforms.alias from games, platforms where games.platform_id = platforms.id  and " + whereStatment; 
	sqlite3pp::query qry(db, query.c_str());
	sqlite3pp::query::iterator intBegin = qry.begin();
	sqlite3pp::query::iterator intEnd = qry.end();
	results.reserve(std::distance(intBegin, intEnd));
	for (sqlite3pp::query::iterator i = qry.begin(); i != qry.end(); ++i)
	{
			dbHandle::listItem newItem;
			newItem.title = (*i).get<const char*>(0);
			newItem.gameID = (*i).get<const char*>(1);
			if ((*i).get<const char*>(2) == NULL)
				newItem.region = 'NULL';
			else
				newItem.region = (*i).get<const char*>(2);
			if ((*i).get<const char*>(3) == NULL)
				newItem.platform = 'NULL';
			else
				newItem.platform = (*i).get<const char*>(3);
			
			results.push_back(newItem);
	}
	return results;

}




std::vector<dbHandle::listItem> dbHandle::getFullGamesList2()
{
	sqlite3 *db;

	sqlite3_stmt    *res;
	const char      *errMSG;
	const char      *tail;
	vector<dbHandle::listItem> results;

	int error = sqlite3_open("database.db", &db);
	if (error)
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
	error = sqlite3_prepare_v2(db,
        "select games.name, games.game_id, games.region, platforms.alias from games, platforms where games.platform_id = platforms.id;",
        1000, &res, &tail);

	if (error != SQLITE_OK)
	{
		puts("We did not get any data!");
	}


	while (sqlite3_step(res) == SQLITE_ROW)
    {
		dbHandle::listItem newItem;
		newItem.title = (const char*)sqlite3_column_text(res, 1);	
		newItem.gameID = (const char*)sqlite3_column_text(res, 1);
		if (sqlite3_column_text(res, 2) != NULL)
			newItem.region = (const char*)sqlite3_column_text(res, 2);
		else
			newItem.platform = 'NONE';
		if (sqlite3_column_text(res, 3) != NULL)
			newItem.platform = (const char*)sqlite3_column_text(res, 3);
		else
			newItem.platform = 'NONE';

		results.push_back(newItem);
    }
	sqlite3_finalize(res);
	sqlite3_close (db); 

	return results;
}