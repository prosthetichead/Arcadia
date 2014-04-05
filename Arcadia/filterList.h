#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"

class filterList
{
private:
	sf::RectangleShape rectangle;
	int selectedItemNum;
	std::vector<dbHandle::filterListItem> listOfItems;
	dbHandle db;
	
public:
	filterList(void);
	~filterList(void);
	void filterList::update();
	void filterList::draw(sf::RenderWindow& window);
	void filterList::init(dbHandle& db_obj, float posX, float posY, int width, float height, std::vector<dbHandle::filterListItem> listItems);
	void filterList::addNewFilterItem(dbHandle::filterListItem);
};

