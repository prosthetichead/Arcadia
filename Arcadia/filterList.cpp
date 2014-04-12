#include "filterList.h"

bool newFilter = true;
float selectedScale = 2;

filterList::filterList(void)
{
	selectedItemNum = 1;
}


filterList::~filterList(void)
{
}

void filterList::init(dbHandle& db_obj, float posX, float posY, int width, std::vector<dbHandle::filterListItem> listItems)
{
	//setup Database handeler
	db = db_obj;
	
	listOfItems = listItems; 
		
	//setup rectangle 
	rectangle.setSize(sf::Vector2f(width, 0));
	rectangle.setPosition(posX, posY);
	rectangle.setFillColor(sf::Color::Color(0,0,0,40));
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(1);
}

bool filterList::update(inputHandle::inputState inputStates)
{
	newFilter = false;
	if (inputStates.left_press)
	{
		selectedItemNum--;
		newFilter = true;
	}
	else if (inputStates.right_press)
	{
		selectedItemNum++;
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
	//window.draw(rectangle);

	sf::Sprite selectedSprite;
	sf::Texture selectedTexture;
	
	dbHandle::filterListItem selectedItem = listOfItems.at(selectedItemNum);

	selectedTexture.loadFromFile(selectedItem.filterIcon);
	selectedSprite.setTexture(selectedTexture);
	float selectedPosX = rectangle.getSize().x/2;
	float selectedPosY = rectangle.getPosition().y;

	if (newFilter)
		selectedScale = 1;
	if (selectedScale < 2) 
		selectedScale = selectedScale + 0.3; 
	else 
		selectedScale = 2;
	selectedSprite.setOrigin(32/2, 32/2);
	selectedSprite.setScale(selectedScale, selectedScale);
	selectedSprite.setPosition(selectedPosX, selectedPosY);	

	int numNormalItems = (rectangle.getSize().x - 64) / 32;
	float normalLeftPosX = selectedPosX - 32 - 16;
	float normalRightPosX = selectedPosX + 32 + 16;

	if (numNormalItems > listOfItems.size())
		numNormalItems = listOfItems.size();
	for(int i=0; i < numNormalItems/2; ++i)
	{
		normalLeftPosX = normalLeftPosX - 32*i;
		normalRightPosX = normalRightPosX + 32*i;
		int itemNumLeft = selectedItemNum - i - 1;
		int itemNumRight = selectedItemNum + i + 1;
		if (itemNumLeft < 0)
			itemNumLeft = listOfItems.size() + itemNumLeft;
		if (itemNumRight > listOfItems.size() - 1)
			itemNumRight = 0 + (itemNumRight - listOfItems.size());
		dbHandle::filterListItem itemLeft = listOfItems.at(itemNumLeft);
		dbHandle::filterListItem itemRight = listOfItems.at(itemNumRight);
		
		sf::Sprite normalSpriteLeft;
		sf::Texture normalTextureLeft;
		
		sf::Sprite normalSpriteRight;
		sf::Texture normalTextureRight;
		
		normalTextureLeft.loadFromFile(itemLeft.filterIcon);
		normalSpriteLeft.setTexture(normalTextureLeft);

		normalTextureRight.loadFromFile(itemRight.filterIcon);
		normalSpriteRight.setTexture(normalTextureRight);
		
		normalSpriteLeft.setOrigin(32/2, 32/2);
		normalSpriteRight.setOrigin(32/2, 32/2);
		normalSpriteLeft.setPosition(normalLeftPosX, selectedPosY);
		normalSpriteRight.setPosition(normalRightPosX, selectedPosY);
		window.draw(normalSpriteLeft);
		window.draw(normalSpriteRight);
	}



	
	window.draw(selectedSprite);
	
}