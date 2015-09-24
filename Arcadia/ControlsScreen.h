#pragma once
#include "MenuNavigation.h"
#include "AssetHandle.h"
#include "InputHandle.h"
#include "DBHandle.h"
#include <SFML\Graphics.hpp>


class ControlsScreen
{
private:
	MenuNavigation menu;
	sf::RectangleShape rectBackground;
	assetHandle *ah;
	dbHandle *db;
	inputHandle *ih;

	MenuNavigation menuNav;

	sf::Text text;
	int selectedItem;
	bool selected;

public:
	ControlsScreen();
	~ControlsScreen(void);
	void ControlsScreen::init(dbHandle* db_ref, assetHandle* ah_ref, inputHandle* ih_ref);
	bool ControlsScreen::update();
	void ControlsScreen::draw(sf::RenderWindow &window);


};

