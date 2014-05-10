#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include "GameInfo.h"
#include "gameList.h"
#include "dbHandle.h"
#include "inputHandle.h"
#include "filterList.h"
#include "launcher.h"
#include "assetHandle.h"



void activateDebugConsole();
void initialize();
void update();
void draw();


bool pause = false;

sf::RenderWindow window;
sf::Event event;
GameInfo gameInfo;
GameList gameList;
dbHandle db;
inputHandle ih;
launcher launch;
filterList platformFilters;
filterList userFilters;

assetHandle assets;



std::string path;

//turns on the debug console
void activateDebugConsole()
{
	AllocConsole();
	freopen("CONIN$", "r",stdin);
	freopen("CONOUT$", "w",stdout);
	freopen("CONOUT$", "w",stderr);
	printf( "--DEBUG CONSOLE-- \n" );
}

// Get the EXE path
std::string getExePath()
  {
	char appPath[MAX_PATH];//always use MAX_PATH for filepaths
	GetModuleFileNameA(NULL,appPath,sizeof(appPath));
	std::string strAppDirectory = appPath;
	strAppDirectory = strAppDirectory.substr(0, strAppDirectory.rfind("\\"));
	return strAppDirectory;
  }

int main()
{
	activateDebugConsole();  //turn on debug console
	path = getExePath();
	initialize();

	while (window.isOpen())
   {	

		ih.update(); //update inputs
		
		if(ih.inputHold(inputHandle::inputs::exit) && (launch.processRunning))
			launch.terminate();
		
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::LostFocus)
			{
				pause = true;
				gameInfo.pauseMovie();
			}
			if (event.type == sf::Event::GainedFocus)
			{
				pause = false;
				gameInfo.pauseMovie();
			}
		}
		if (!pause)
		{
			update();
			draw();
		}
		else
		{
			sf::sleep(sf::milliseconds(170));
		}
	}
    return 0;
}

//Called at the start of the game
//runs just once
void initialize()
{	

	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

	db.setFilePath(path, "database.db");
	launch.init(db);
	assets.init(db);
	ih.init(db);
	
	int gameListWidth = desktopMode.width * .35;

	gameList.init(db, assets, 0, 70,  gameListWidth, desktopMode.height - 140);
	gameInfo.init(db, gameListWidth, 0, desktopMode.width - gameListWidth, desktopMode.height);
			
	platformFilters.init(db, assets, 1, 35,  gameListWidth, db.getPlatformFilterList(), "platform Filter List");
	userFilters.init(db, assets, 70, desktopMode.height - 35,  gameListWidth, db.getFilterList(), "User Filter List");

	gameList.updateFilter(platformFilters.getFilterString());


	
	window.create(desktopMode, "Arcadia", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
}

void update()
{
	

	bool newFilter = false;
	if (ih.inputPress(inputHandle::inputs::filter_1_left))
	{
		platformFilters.update(-1);
		newFilter = true;		
	}
	if (ih.inputPress(inputHandle::inputs::filter_1_right))
	{
		platformFilters.update(+1);
		newFilter = true;		
	}
	if (ih.inputPress(inputHandle::inputs::filter_2_left))
	{
		userFilters.update(-1);
		newFilter = true;	
	}
	if (ih.inputPress(inputHandle::inputs::filter_2_right))
	{
		userFilters.update(+1);
		newFilter = true;		
	}
	if(newFilter)
		gameList.updateFilter(platformFilters.getFilterString() + userFilters.getFilterString());	


	gameList.update(ih);
	gameInfo.update(gameList.getCurrentItem());
	//launch.update(inputStates, gameList.getCurrentItem());

	//movie.update();
}

void draw()
{

	window.clear(sf::Color::Black);

	gameInfo.draw(window);
	gameList.draw(window);
	platformFilters.draw(window);
	userFilters.draw(window);


	//window.draw(movie);
	

	window.display();
}




