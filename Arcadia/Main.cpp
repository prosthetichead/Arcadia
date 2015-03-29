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

GameList gameList(&db, &ah, &sh);
GameInfo gameInfo(&db, &ah, &sh);
FilterList platformFilters(db, ah);
FilterList filters(db, ah);
SettingsScreen settingsScreen(&db, &ah, &ih);
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

void setupWindow()
{
	sf::VideoMode desktopMode = sf::VideoMode(sh.resolution.x,sh.resolution.y);
	window.create(desktopMode, "Arcadia", sh.windowStyle);
	window.setVerticalSyncEnabled(true);
	window.setJoystickThreshold(5);
}


int main()
{
	activateDebugConsole();  //turn on debug console
	path = getExePath();
	initialize();

	while (window.isOpen())
   {	

		ih.update(); //update inputs
		
		if(ih.inputPress(inputHandle::inputs::exit_game) && (launch.processRunning))
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
				//setupWindow();
				pause = false;
				gameInfo.pauseMovie();
			}

			ih.checkEventForInput(event);
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
	db.setFilePath(path, "arcadia.db");
	ih.init(&db, &window);
	sh.init(db);
	ah.init(db, sh.skin_id);
	printf("handelers Init complete");	

	hideScreen.setSize(sf::Vector2f(sh.resolution.x, sh.resolution.y));
	hideScreen.setPosition(0,0);
	hideScreen.setFillColor(sf::Color::Color(0,0,0,190));

	gameInfo.init();
			
	platformFilters.init(1, 35,  400, db.getPlatformFilterList(), "Platform Filters");
	filters.init(1, 92,  400, db.getCustomFilterList(), "Filters");

	settingsScreen.init(sh.resolution.x / 2, sh.resolution.y / 2);

	gameList.updateFilter(platformFilters.getFilterString(), filters.getFilterString());

	setupWindow();
}

void update()
{
	if (displaySettingsScreen)
	{
		if(settingsScreen.update(window))
		{
			displaySettingsScreen = false;
		}
		
	}
	else /// no screen to show run a normal update
	{
		if (ih.inputPress(inputHandle::inputs::settings))
			displaySettingsScreen = true;

		if (ih.inputPress(inputHandle::inputs::start_game))
		{
			launch.launchGame(gameList.getCurrentItem().platformID, gameList.getCurrentItem().fileName);
			std::cout << db.getLaunchCode(gameList.getCurrentItem().platformID, gameList.getCurrentItem().fileName) << std::endl;
		}
		if (ih.inputPress(inputHandle::inputs::platform_filter_left))
		{
			platformFilters.update(-1);
			gameList.updateFilter(platformFilters.getFilterString(), filters.getFilterString());//+ filterScreen.getFilterString());		
		}
		if (ih.inputPress(inputHandle::inputs::platform_filter_right))
		{
			platformFilters.update(+1);
			gameList.updateFilter(platformFilters.getFilterString(), filters.getFilterString());//+ filterScreen.getFilterString());	
		}
		if (ih.inputPress(inputHandle::inputs::filter_left))
		{
			filters.update(-1);
			gameList.updateFilter( platformFilters.getFilterString(), filters.getFilterString() );//+ filterScreen.getFilterString());	
		}
		if (ih.inputPress(inputHandle::inputs::filter_right))
		{
			filters.update(+1);
			gameList.updateFilter(platformFilters.getFilterString(), filters.getFilterString());//+ filterScreen.getFilterString());	
		}

		gameList.update(ih);
		gameInfo.update(gameList.getCurrentItem());
	}

}

void draw()
{
	window.clear(sf::Color::Black);


	for (SkinHandle::Skin_Element se : sh.skin_elements) {
	
		switch (se.type)
		{
		case SkinHandle::element_type::static_text:
			ah.drawText(se.text, se, window);
			break;
		case SkinHandle::element_type::static_rectangle:
			window.draw(se.getRectangle());			
			break;
		case SkinHandle::element_type::static_image:
			window.draw( se.getSprite(ah.getStaticImageAsset(se.asset_file) ));
			break;
		case SkinHandle::element_type::game_list:
			gameList.draw(window);
			break;
		default:
			gameInfo.draw(window, se);
			break;
		}

	}
	
	
	platformFilters.draw(window);
	filters.draw(window);

	if(displaySettingsScreen)
	{
		window.draw(hideScreen);
		settingsScreen.draw(window);
	}




	window.display();	
}




