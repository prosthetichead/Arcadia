#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"
#include "inputHandle.h"
#include "assetHandle.h"


class filterList
{
private:
	sf::RectangleShape rectangle;
	int selectedItemNum;
	std::vector<dbHandle::filterListItem> listOfItems;
	
	bool newFilter;
	std::string filterListName;

	assetHandle &ah;
	dbHandle &db;

	int selectedSpriteSize;
	int normalSpriteSize;
	int normalPading;
	int selectedPadding;
	float selectedSpriteCurrentScaleX;
	float selectedSpriteCurrentScaleY;

public:
	filterList(dbHandle& db_ref, assetHandle& ah_ref);
	~filterList(void);
	void filterList::update(int move);
	void filterList::draw(sf::RenderWindow& window);
	void filterList::init(float posX, float posY, int width, std::vector<dbHandle::filterListItem> listItems, std::string name);
	void filterList::addNewFilterItem(dbHandle::filterListItem);
	std::string filterList::getFilterString();
	void filterList::setSelectedSize(int newSize);
};

