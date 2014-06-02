#include "filterList.h"

filterList::filterList(dbHandle& db_ref, assetHandle& ah_ref): db(db_ref), ah(ah_ref)
{
	newFilter = true;
	
	selectedSpriteSize = 64;
	normalSpriteSize = 32;
	normalPading = 10;
	selectedPadding = 30;

	selectedItemNum = 0;
}


filterList::~filterList(void)
{
}

void filterList::init(float posX, float posY, int width, std::vector<dbHandle::filterListItem> listItems, std::string name)
{
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
	if (listOfItems.at(selectedItemNum).filterString == "NULL")
		return " ";
	else
		return listOfItems.at(selectedItemNum).filterString;
}

void filterList::draw(sf::RenderWindow& window)
{
	sf::Sprite selectedSprite;
	selectedSprite.setTexture(ah.getTextureAsset(listOfItems.at(selectedItemNum).filterIcon), true);


	float selectedPosX = rectangle.getPosition().x + rectangle.getSize().x/2;
	float selectedPosY = rectangle.getPosition().y;

	selectedSprite.setOrigin(selectedSprite.getLocalBounds().width/2, selectedSprite.getLocalBounds().height/2);
	selectedSprite.setPosition(selectedPosX, selectedPosY);	
	
	//Work Out Scale to grow from normal to selected size
	float selectedSpriteScaleX = selectedSpriteSize / selectedSprite.getLocalBounds().width;
	float selectedSpriteScaleY = selectedSpriteSize / selectedSprite.getLocalBounds().height;
	if(newFilter)
	{
		selectedSpriteCurrentScaleX = normalSpriteSize / selectedSprite.getLocalBounds().width;
		selectedSpriteCurrentScaleY = normalSpriteSize / selectedSprite.getLocalBounds().height;
		newFilter = false;
	}	

	if(selectedSpriteCurrentScaleX < selectedSpriteScaleX)
		selectedSpriteCurrentScaleX += .1;
	else
		selectedSpriteCurrentScaleX = selectedSpriteScaleX;

	if(selectedSpriteCurrentScaleY < selectedSpriteScaleY)
		selectedSpriteCurrentScaleY += .1;
	else
		selectedSpriteCurrentScaleY = selectedSpriteScaleY ;

	selectedSprite.scale(selectedSpriteCurrentScaleX, selectedSpriteCurrentScaleY);

	window.draw(selectedSprite);

	if (rectangle.getSize().x != 0)
	{
	
	int numNormalItems = (rectangle.getSize().x - (selectedSpriteSize + (selectedPadding*2) ) ) / (normalSpriteSize + (normalPading*2) );
	float normalLeftPosX = selectedPosX - selectedPadding;
	float normalRightPosX = selectedPosX + selectedPadding;

	if (numNormalItems > listOfItems.size())
		numNormalItems = listOfItems.size();
	for(int i=0; i < numNormalItems/2; ++i)
	{
		normalLeftPosX = normalLeftPosX - ((normalSpriteSize+normalPading));
		normalRightPosX = normalRightPosX + ((normalSpriteSize+normalPading));
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
		
		normalSpriteLeft.setTexture(ah.getTextureAsset(itemLeft.filterIcon));
		normalSpriteRight.setTexture(ah.getTextureAsset(itemRight.filterIcon));
		
		normalSpriteLeft.setOrigin(normalSpriteLeft.getLocalBounds().width/2, normalSpriteLeft.getLocalBounds().height/2);
		normalSpriteRight.setOrigin(normalSpriteRight.getLocalBounds().width/2, normalSpriteRight.getLocalBounds().height/2);
		normalSpriteLeft.setPosition(normalLeftPosX, selectedPosY);
		normalSpriteRight.setPosition(normalRightPosX, selectedPosY);

		normalSpriteLeft.setScale(normalSpriteSize / normalSpriteLeft.getLocalBounds().width, normalSpriteSize / normalSpriteLeft.getLocalBounds().height);
		normalSpriteRight.setScale(normalSpriteSize / normalSpriteRight.getLocalBounds().width, normalSpriteSize / normalSpriteRight.getLocalBounds().height);


		window.draw(normalSpriteLeft);
		window.draw(normalSpriteRight);
	}
	}
	
}