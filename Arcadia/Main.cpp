#include <SFML/Graphics.hpp>
#include <sfeMovie\Movie.hpp>
#include <iostream>
#include <windows.h>
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

sfe::Movie movie;
sf::RenderWindow window;
sf::Event event;
GameList gameList;
dbHandle db;
inputHandle ih;
launcher launch;
filterList platformFilters;
filterList userFilters;

assetHandle assets;



std::string path;
inputHandle::inputState inputStates;


//turns on the debug console
void activateDebugConsole()
{
	AllocConsole();
	freopen("CONIN$", "r",stdin);
	freopen("CONOUT$", "w",stdout);
	freopen("CONOUT$", "w",stderr);
	printf( "--DEBUG CONSOLE-- \n" );
}
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
	std::cout << path << std::endl;
	initialize();

	while (window.isOpen())
   {	

		inputStates = ih.update(); //Get Input States
		
		if((inputStates.exit_hold) && (launch.processRunning))
			launch.terminate();

		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
			if (event.type == sf::Event::LostFocus)
				pause = true;
			if (event.type == sf::Event::GainedFocus)
				pause = false;
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

	movie.openFromFile("c:\\Adventure Island (USA).mp4");

	db.setFilePath(path, "database.db");
	launch.init(db);
	assets.init(db);
	gameList.init(db, assets, 1, 70, 500,900);
	platformFilters.init(db, 1, 40, 500, db.getPlatformFilterList(), "platform Filter List");
	userFilters.init(db, 70, 1000, 500, db.getFilterList(), "User Filter List");
	gameList.updateFilter(platformFilters.getFilterString());

	window.create(sf::VideoMode(1400, 1050), "Arcadia");
	//window.setVerticalSyncEnabled(true);

	movie.resizeToFrame(500,100,640,480,true);
	movie.play();
}

void update()
{
	

	bool newFilter = false;
	if (inputStates.platform_filter_left_press)
	{
		platformFilters.update(-1);
		newFilter = true;		
	}
	if (inputStates.platform_filter_right_press)
	{
		platformFilters.update(+1);
		newFilter = true;		
	}
	if (inputStates.user_filter_left_press)
	{
		userFilters.update(-1);
		newFilter = true;	
	}
	if (inputStates.user_filter_right_press)
	{
		userFilters.update(+1);
		newFilter = true;		
	}
	if(newFilter)
		gameList.updateFilter(platformFilters.getFilterString() + userFilters.getFilterString());	


	gameList.update(inputStates);
	launch.update(inputStates, gameList.getCurrentItem());

	movie.update();
}

void draw()
{

	window.clear(sf::Color::Black);

	gameList.draw(window);
	platformFilters.draw(window);
	userFilters.draw(window);

	window.draw(movie);
	

	window.display();
}




