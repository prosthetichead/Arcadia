#include "filterList.h"


filterList::filterList(void)
{

}


filterList::~filterList(void)
{
}

void filterList::init(dbHandle& db_obj, float posX, float posY, int width, float height, std::vector<dbHandle::filterListItem> listItems)
{
	//setup Database handeler
	db = db_obj;
	
	listOfItems = listItems; 

	
	selectedItemNum = 0;
	
	//setup rectangle 
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setPosition(posX, posY);
	rectangle.setFillColor(sf::Color::Color(0,0,0,40));
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(1);
}

void filterList::update()
{



}

void filterList::draw(sf::RenderWindow& window)
{

}