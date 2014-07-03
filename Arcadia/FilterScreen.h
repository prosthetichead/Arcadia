#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"
#include "assetHandle.h"
#include "MenuNavigation.h"
#include "inputHandle.h"
#include "FilterList.h"
#include "OnScreenKeyboard.h"


class FilterScreen
{
private:
	dbHandle &db;
	assetHandle &ah;

	OnScreenKeyboard osk;

	MenuNavigation menuNav;

	sf::RectangleShape menuRect;
	
	

	int players; 
	int maxPlayers;
	
	int co_op; //(-1, 0, 1)
	int favourite; //(-1, 0, 1)
	int year_operation; // -1 no filter, 0 Greater_Then, 1 Less_Then, 2 Equal_Too, 3 Not_Equal_Too
	int year;
	double userStars;
	double onlineStars;
	bool userSelected;

	filterList developerFilter;
	filterList publisherFilter;
	filterList regionFilter;
	filterList genreFilter;

	sf::Texture *screenTexture_p;
	sf::Texture	pointerTexture;

	sf::Font textFont;
	sf::Text searchStringText;
	sf::Text largeText;

	std::string filterString;
	std::string searchString;

	void FilterScreen::updateFilterString();

	bool showKeyboard;

	std::string filterDescription;

public:
	FilterScreen(dbHandle &db_ref, assetHandle &ah_ref);
	~FilterScreen(void);

	std::string FilterScreen::getFilterString();
	void FilterScreen::init(float posX, float posY, int width, float height);
	bool FilterScreen::update(inputHandle& ih);
	void FilterScreen::draw(sf::RenderWindow& window);
};

