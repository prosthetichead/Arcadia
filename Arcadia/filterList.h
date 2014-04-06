#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"
#include "inputHandle.h"


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
	bool filterList::update(inputHandle::inputState inputStates);
	void filterList::draw(sf::RenderWindow& window);
	void filterList::init(dbHandle& db_obj, float posX, float posY, int width, std::vector<dbHandle::filterListItem> listItems);
	void filterList::addNewFilterItem(dbHandle::filterListItem);
	std::string filterList::getFilterString();
};

