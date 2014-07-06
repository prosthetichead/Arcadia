#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"
#include "inputHandle.h"
#include "assetHandle.h"


class FilterList
{
private:
	sf::RectangleShape rectangle;
	int selectedItemNum;
	std::vector<dbHandle::filterListItem> listOfItems;
	
	bool newFilter;
	std::string FilterListName;

	assetHandle &ah;
	dbHandle &db;

	int selectedSpriteSize;
	int normalSpriteSize;
	int normalPading;
	int selectedPadding;
	float selectedSpriteCurrentScaleX;
	float selectedSpriteCurrentScaleY;

public:
	FilterList(dbHandle& db_ref, assetHandle& ah_ref);
	~FilterList(void);
	void FilterList::update(int move);
	void FilterList::draw(sf::RenderWindow& window);
	void FilterList::init(float posX, float posY, int width, std::vector<dbHandle::filterListItem> listItems, std::string name);
	void FilterList::addNewFilterItem(dbHandle::filterListItem);
	std::string FilterList::getFilterString();
	std::string FilterList::getSelectedTitle();
	void FilterList::setSelectedSize(int newSize);
};

