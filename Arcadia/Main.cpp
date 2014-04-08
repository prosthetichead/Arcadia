#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include "gameList.h"
#include "dbHandle.h"
#include "inputHandle.h"
#include "filterList.h"
#include "launcher.h"

void activateDebugConsole();
void initialize();
void update();
void draw();


bool pause = false;

sf::RenderWindow window;
sf::Event event;
GameList gameList;
dbHandle db;
inputHandle ih;
launcher launch;
filterList platformFilter;
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
		
		if((inputStates.exit_press) && (launch.processRunning))
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
	db.setFilePath(path, "database.db");
	gameList.init(db, 1, 80, 500, 800);
	platformFilter.init(db, 1, 50, 500, db.getPlatformFilterList());
	gameList.updateFilter(platformFilter.getFilterString());

	launch.init(db);

	window.create(sf::VideoMode(1400, 1050), "Arcadia");
	window.setFramerateLimit(30); //window.setVerticalSyncEnabled(true);

	
}

void update()
{
	
	if (platformFilter.update(inputStates))
	{
		gameList.updateFilter(platformFilter.getFilterString());
		launch.launchGame();
		
	}
	gameList.update(inputStates);
}

void draw()
{

	window.clear(sf::Color::Black);
	
	gameList.draw(window);
	platformFilter.draw(window);
	window.display();
}




