#include "filterList.h"

filterList::filterList(void)
{
	//newFilter = true;
	selectedScale = 2;
	selectedItemNum = 0;
}


filterList::~filterList(void)
{
}

void filterList::init(dbHandle& db_obj, assetHandle& asset_obj, float posX, float posY, int width, std::vector<dbHandle::filterListItem> listItems, std::string name)
{
	//setup Database handeler
	db = db_obj;
	assets= asset_obj;
	
	filterListName = name;

	listOfItems = listItems; 
		
	//setup rectangle 
	rectangle.setSize(sf::Vector2f(width, 0));
	rectangle.setPosition(posX, posY);
	rectangle.setFillColor(sf::Color::Color(0,0,0,40));
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(1);
}

void filterList::update(int move)
{
	
	newFilter = false;
	if (move != 0)
	{
		std::cout << filterListName << " - Moved " << move << std::endl;
		selectedItemNum = selectedItemNum + move;
		newFilter = true;
	}

	//Lock selectedItemNum To size of the vector
	if (selectedItemNum < 0)
		selectedItemNum = listOfItems.size() -1;
	else if (selectedItemNum >= listOfItems.size())
		selectedItemNum = 0;
}

std::string filterList::getFilterString()
{
	return listOfItems.at(selectedItemNum).filterString;
}

void filterList::draw(sf::RenderWindow& window)
{
	//window.draw(rectangle);

	sf::Sprite selectedSprite;

	dbHandle::filterListItem selectedItem = listOfItems.at(selectedItemNum);

	selectedSprite.setTexture(assets.getTextureAsset(listOfItems.at(selectedItemNum).filterIcon));
	float selectedPosX = rectangle.getSize().x/2;
	float selectedPosY = rectangle.getPosition().y;

	if (selectedScale < 2) 
		selectedScale = selectedScale + .4; 
	else 
		selectedScale = 2;
	if (newFilter)
	{
		selectedScale = 1;
		newFilter = false;
	}


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
		sf::Sprite normalSpriteRight;
		
		normalSpriteLeft.setTexture(assets.getTextureAsset(itemLeft.filterIcon));
		normalSpriteRight.setTexture(assets.getTextureAsset(itemRight.filterIcon));
		
		normalSpriteLeft.setOrigin(32/2, 32/2);
		normalSpriteRight.setOrigin(32/2, 32/2);
		normalSpriteLeft.setPosition(normalLeftPosX, selectedPosY);
		normalSpriteRight.setPosition(normalRightPosX, selectedPosY);
		window.draw(normalSpriteLeft);
		window.draw(normalSpriteRight);
	}



	
	window.draw(selectedSprite);
	
}