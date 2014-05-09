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
	std::cout << path << std::endl;
	initialize();

	while (window.isOpen())
   {	
	   std::cout << "HERE" << std::endl;
		inputStates = ih.update(); //Get Input States
		
		if((inputStates.exit_hold) && (launch.processRunning))
			launch.terminate();
		else if((inputStates.exit_hold) && (!launch.processRunning))
			window.close();

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
	
	int gameListWidth = desktopMode.width * .35;

	gameList.init(db, assets, 0, 70,  gameListWidth, desktopMode.height - 140);
	gameInfo.init(db, 0, 0, desktopMode.width, desktopMode.height);
			
	platformFilters.init(db, assets, 1, 35,  gameListWidth, db.getPlatformFilterList(), "platform Filter List");
	userFilters.init(db, assets, 70, desktopMode.height - 35,  gameListWidth, db.getFilterList(), "User Filter List");
		std::cout << "here" << std::endl;

	gameList.updateFilter(platformFilters.getFilterString());

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> 165aee21e6cd873ef12890b0ce9dddac0d9e6a3b
	window.create(desktopMode, "Arcadia", sf::Style::Default);


	
<<<<<<< HEAD

=======
<<<<<<< HEAD
	window.create(desktopMode, "Arcadia", sf::Style::Fullscreen);

=======
>>>>>>> a71cd55a2ad7459dc9ad1c79fdcff9e7aa418ed3
	window.create(desktopMode, "Arcadia", sf::Style::None);
>>>>>>> parent of 8d1aec3... Added clearlogo and improved layout
<<<<<<< HEAD
=======

	
	window.create(desktopMode, "Arcadia", sf::Style::None);
>>>>>>> parent of 9557af4... errors
=======
>>>>>>> 165aee21e6cd873ef12890b0ce9dddac0d9e6a3b
	window.setVerticalSyncEnabled(true);

	//movie.resizeToFrame(500,100,640,480,true);
	//movie.play();
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
		std::cout << platformFilters.getFilterString() + userFilters.getFilterString() << std::endl;
		gameList.updateFilter(platformFilters.getFilterString() + userFilters.getFilterString());	


	gameList.update(inputStates);
	gameInfo.update(gameList.getCurrentItem());
	launch.update(inputStates, gameList.getCurrentItem());

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




