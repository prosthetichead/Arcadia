#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include "GameInfo.h"
#include "gameList.h"
#include "dbHandle.h"
#include "inputHandle.h"
#include "FilterList.h"
#include "launcher.h"
#include "assetHandle.h"
#include "FilterScreen.h"
#include "SettingsScreen.h"
#include "SkinHandle.h"



void activateDebugConsole();
void initialize();
void update();
void draw();


bool pause = false;
bool displayFilterScreen = false;
bool displaySettingsScreen = false;

sf::RenderWindow window;
sf::Event event;

dbHandle db;
assetHandle ah; //Dont use untill init
inputHandle ih;  //Dont use untill init
SkinHandle sh; //Dont use untill init

GameList gameList(db, ah);
GameInfo gameInfo(&db, &ah, &sh);
FilterList platformFilters(db, ah);
FilterScreen filterScreen(db, ah);
SettingsScreen settingsScreen(db, ah);
launcher launch(&db);

std::string path;
sf::RectangleShape hideScreen;

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
		
		if(ih.inputPress(inputHandle::inputs::exit) && (launch.processRunning))
		{
			launch.terminate();
		}
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
				sf::VideoMode desktopMode = sf::VideoMode(sh.resolution.x, sh.resolution.y);
				window.create(desktopMode, "Arcadia", sf::Style::Fullscreen);
				window.setVerticalSyncEnabled(true);
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
			sf::sleep(sf::milliseconds(170)); // slow down updating and drawing as do not have focus
		}
	}
    return 0;
}

//Called at the start of the game
//runs just once
void initialize()
{	
	
	
	db.setFilePath(path, "database.db");
	ah.init(db);
	ih.init(db);
	sh.init(db);

	sf::VideoMode desktopMode = sf::VideoMode(sh.resolution.x,sh.resolution.y);

	int gameListWidth = desktopMode.width * .35;

	hideScreen.setSize(sf::Vector2f(desktopMode.width, desktopMode.height));
	hideScreen.setPosition(0,0);
	hideScreen.setFillColor(sf::Color::Color(0,0,0,180));

	gameList.init(sh);//0, 70,  gameListWidth, desktopMode.height - 140);
	gameInfo.init();
			
	platformFilters.init(1, 35,  gameListWidth, db.getPlatformFilterList(), "platform Filter List");

	filterScreen.init(desktopMode.width/2 - 800/2, desktopMode.height/2 - 600/2, 800,600);
	settingsScreen.init(desktopMode.width/2,  desktopMode.height/2);
	
	gameList.updateFilter(platformFilters.getFilterString());

	window.create(desktopMode, "Arcadia", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
}

void update()
{
	if (displayFilterScreen)
	{
		if (filterScreen.update(ih))  // only returns true once done. Maybe change to a enum? so we can tell if it was accept or cancel pressed?
		{
			displayFilterScreen = false;
			gameList.updateFilter(platformFilters.getFilterString() + filterScreen.getFilterString());
			gameInfo.update(gameList.getCurrentItem());
		}
	}
	else if (displaySettingsScreen)
	{
		if(settingsScreen.update(ih))
		{
			displaySettingsScreen = false;
		}

	}
	else /// no screen to show run a normal update
	{
		if (ih.inputPress(inputHandle::inputs::filter_menu))
			displayFilterScreen = true;
		else if (ih.inputPress(inputHandle::inputs::settings))
			displaySettingsScreen = true;

		if (ih.inputPress(inputHandle::inputs::start_game))
		{
			launch.launchGame(gameList.getCurrentItem().platformID, gameList.getCurrentItem().fileName);
			std::cout << db.getLaunchCode(gameList.getCurrentItem().platformID, gameList.getCurrentItem().fileName) << std::endl;
		}
		if (ih.inputPress(inputHandle::inputs::platform_filter_left))
		{
			platformFilters.update(-1);
			gameList.updateFilter(platformFilters.getFilterString() + filterScreen.getFilterString());		
		}
		if (ih.inputPress(inputHandle::inputs::platform_filter_right))
		{
			platformFilters.update(+1);
			gameList.updateFilter(platformFilters.getFilterString() + filterScreen.getFilterString());		
		}		
	

		gameList.update(ih);
		gameInfo.update(gameList.getCurrentItem());
	}

}

void draw()
{

	window.clear(sf::Color::Black);

	gameInfo.draw(window);
	gameList.draw(window);
	platformFilters.draw(window);
	
	if(displayFilterScreen)
	{
		window.draw(hideScreen);
		filterScreen.draw(window);
	}
	else if(displaySettingsScreen)
	{
		window.draw(hideScreen);
		settingsScreen.draw(window);
	}
	window.display();
}




