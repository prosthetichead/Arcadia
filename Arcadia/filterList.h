#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"

class filterList
{
private:
	sf::RectangleShape rectangle;
	int selectedItemNum;
	std::vector<dbHandle::filterListItem> listOfItems;
	
public:
	filterList(void);
	~filterList(void);
	void filterList::init(dbHandle& db_obj, float posX, float posY, int width, float height);
	void filterList::addNewFilterItem(dbHandle::filterListItem);
};

