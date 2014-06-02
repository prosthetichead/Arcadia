#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"
#include "assetHandle.h"
#include "MenuNavigation.h"
#include "inputHandle.h"
#include "FilterList.h"


class FilterScreen
{
private:
	dbHandle &db;
	assetHandle &ah;

	MenuNavigation menuNav;

	sf::RectangleShape menuRect;
	
	std::string developer_id;
	std::string publisher_id;
	std::string genre_id;
	int year;
	std::string year_operation; // Greater_Then, Less_Then, Equal_Too, Not_Equal_Too

	filterList developerFilter;

	sf::Texture screenTexture;
	sf::Texture	pointerTexture;

public:
	FilterScreen(dbHandle &db_ref, assetHandle &ah_ref);
	~FilterScreen(void);

	void FilterScreen::init(float posX, float posY, int width, float height);
	void FilterScreen::update(inputHandle& ih);
	void FilterScreen::draw(sf::RenderWindow& window);
};

