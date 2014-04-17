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
	dbHandle db;
	bool newFilter;
	float selectedScale;
	std::string filterListName;
	assetHandle assets;

public:
	filterList(void);
	~filterList(void);
	void filterList::update(int move);
	void filterList::draw(sf::RenderWindow& window);
	void filterList::init(dbHandle& db_obj, assetHandle& asset_obj, float posX, float posY, int width, std::vector<dbHandle::filterListItem> listItems, std::string name);
	void filterList::addNewFilterItem(dbHandle::filterListItem);
	std::string filterList::getFilterString();
};

