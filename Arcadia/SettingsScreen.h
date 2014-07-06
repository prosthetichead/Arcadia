#pragma once
#include "DBHandle.h"
#include "MenuNavigation.h"
#include "inputHandle.h"
#include "assetHandle.h"
#include <SFML/Graphics.hpp>

class SettingsScreen
{
private:

	dbHandle &db;
	assetHandle &ah;
	MenuNavigation menuNav;

	sf::RectangleShape menuRect;
	sf::RectangleShape menuButtonRect;
	dbHandle::gameListItem currentGameListItem;
	std::vector<dbHandle::findGameResultItem> gameDB_FindResults;

	sf::Text menuText;
	sf::Text menuShadowText;
	sf::Font menuFont;
	
	bool editGameVisible;
	std::vector<std::string> menuIDs;


public:
	SettingsScreen(dbHandle &db_ref, assetHandle &ah_ref);
	~SettingsScreen(void);

	void SettingsScreen::init(float posX, float posY);
	void SettingsScreen::setCurrentGameListItem(dbHandle::gameListItem gameListItem);
	bool SettingsScreen::update(inputHandle& ih);
	void SettingsScreen::draw(sf::RenderWindow& window);
};

