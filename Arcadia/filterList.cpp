#include "filterList.h"

bool newFilter = false;
float selectedScale = 2;

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

bool filterList::update(inputHandle::inputState inputStates)
{
	newFilter = false;
	if (inputStates.left_press || inputStates.left_hold)
	{
		selectedItemNum++;
		newFilter = true;
	}
	else if (inputStates.right_press || inputStates.right_hold)
	{
		selectedItemNum--;
		newFilter = true;
	}
	
	//Lock selectedItemNum To size of the vector
	if (selectedItemNum < 0)
		selectedItemNum = listOfItems.size() -1;
	else if (selectedItemNum >= listOfItems.size())
		selectedItemNum = 0;
	
	return newFilter;
}

std::string filterList::getFilterString()
{
	return listOfItems.at(selectedItemNum).filterString;
}

void filterList::draw(sf::RenderWindow& window)
{
	sf::Sprite selectedSprite;
	sf::Texture selectedTexture;
	
	dbHandle::filterListItem selectedItem = listOfItems.at(selectedItemNum);

	selectedTexture.loadFromFile(selectedItem.filterIcon);
	
	selectedSprite.setTexture(selectedTexture);
	if (newFilter)
		selectedScale = 6;
	if (selectedScale > 2) 
		selectedScale = selectedScale - 0.3; 
	else 
		selectedScale = 2;
	selectedSprite.setScale(selectedScale, selectedScale);
	selectedSprite.setOrigin(32/2, 32/2);

	selectedSprite.setPosition(rectangle.getPosition().x,rectangle.getPosition().y);



	window.draw(selectedSprite);
	
}