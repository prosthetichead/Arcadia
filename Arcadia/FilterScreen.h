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
	
	std::string developer_id;
	std::string publisher_id;
	std::string genre_id;
	int year;
	std::string year_operation; // Greater_Then, Less_Then, Equal_Too, Not_Equal_Too

	filterList developerFilter;
	filterList publisherFilter;
	filterList regionFilter;
	filterList genreFilter;

	sf::Texture *screenTexture_p;
	sf::Texture	pointerTexture;

	sf::Font textFont;
	sf::Text searchStringText;

	std::string filterString;
	std::string searchString;

	void FilterScreen::updateFilterString();

	bool showKeyboard;

public:
	FilterScreen(dbHandle &db_ref, assetHandle &ah_ref);
	~FilterScreen(void);

	std::string FilterScreen::getFilterString();
	void FilterScreen::init(float posX, float posY, int width, float height);
	bool FilterScreen::update(inputHandle& ih);
	void FilterScreen::draw(sf::RenderWindow& window);
};

