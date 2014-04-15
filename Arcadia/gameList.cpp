#include "gameList.h"

using namespace std;

GameList::GameList()
{	
	selectedItemNum = 0;
	counter = 0;
	selectedItemChange = false;	
}
GameList::~GameList(void)
{
}


void GameList::init(dbHandle& db_obj,  float posX, float posY, int width, float height)
{
	//setup Database handeler
	db = db_obj;
		
	//setup rectangle 
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setPosition(posX, posY);
	rectangle.setFillColor(sf::Color::Color(0,0,0,40));
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(1);
	
	selectedFont.loadFromFile(db.exe_path + "\\assets\\fonts\\Teknik-Bold.ttf");
	normalFont.loadFromFile(db.exe_path + "\\assets\\fonts\\Teknik-Bold.ttf");
	normalFontSize = 14;
	selectedFontSize = 20;
	
	selectedText.setFont(selectedFont);
	selectedText.setCharacterSize(selectedFontSize);
	selectedText.setColor(sf::Color::Red);



	flagTexture.loadFromFile(db.exe_path + "\\assets\\icons\\FLAG_NO_FLAG.png");

}

void GameList::updateFilter(std::string filterString)
{
	selectedItemNum = 0;
	listOfItems = db.getGamesListQuery(filterString);

}

dbHandle::gameListItem  GameList::getCurrentItem()
{
	return listOfItems.at(selectedItemNum);
}

void GameList::update(inputHandle::inputState inputStates)
{
	selectedItemChange = false;
	if (inputStates.up_press || inputStates.up_hold)
	{
		selectedItemNum--;
		selectedItemChange = true;
	}
	else if (inputStates.down_press || inputStates.down_hold)
	{
		selectedItemNum++;
		selectedItemChange = true;
	}
	else
		counter = 0;
	
	//Lock selectedItemNum To size of the vector
	if (selectedItemNum < 0)
		selectedItemNum = listOfItems.size() -1;
	else if (selectedItemNum >= listOfItems.size())
		selectedItemNum = 0;

	// Run only if Selected Item is changed
	if (selectedItemChange)
	{
		selectedText.setString(listOfItems.at(selectedItemNum).title);
	}
		
}

void GameList::draw(sf::RenderWindow& window)
{
	int selectedPosX = rectangle.getPosition().x + 30;
	int selectedPosY = (rectangle.getSize().y / 2) + rectangle.getPosition().y - selectedFontSize/2;
	selectedText.setPosition(selectedPosX, selectedPosY);


	
	//sf::Sprite flagSprite;
	//flagSprite.setTexture(flagTexture);
	//flagSprite.setPosition(selectedText.getPosition().x - 20, selectedText.getPosition().y + 6);

	int numNormalItems = (rectangle.getSize().y - selectedFontSize) / normalFontSize;
	if (numNormalItems > listOfItems.size())
		numNormalItems = listOfItems.size();
	float normalPosX = rectangle.getPosition().x, normalPosY = selectedPosY - selectedFontSize;
	
	//window.draw(rectangle);	
	window.draw(selectedText);
	//window.draw(flagSprite);

	for(int i=0; i < numNormalItems/2; ++i)
	{
		int itemNum = selectedItemNum - i-1;
		if (itemNum < 0)
			itemNum = listOfItems.size() + itemNum;

		dbHandle::gameListItem item = listOfItems.at(itemNum);

		sf::Text normalText;

		//flagSprite.setPosition(normalPosX, normalPosY - (i*normalFontSize)+4);
		//window.draw(flagSprite);

		normalText.setFont(normalFont);
		normalText.setCharacterSize(normalFontSize);
		normalText.setColor(sf::Color(sf::Color::White));
		normalText.setString(item.title);
		normalText.setPosition(normalPosX + 20, normalPosY - (i*normalFontSize));
		window.draw(normalText);
	}

	normalPosX = rectangle.getPosition().x;
	normalPosY = selectedPosY + selectedFontSize + 4;
	for(int i=0; i < numNormalItems/2; ++i)
	{
		int itemNum = selectedItemNum + i + 1;
		if (itemNum > listOfItems.size() - 1)
			itemNum = 0 + (itemNum - listOfItems.size());
			
		dbHandle::gameListItem item = listOfItems.at(itemNum);

		sf::Text normalText;

		normalText.setFont(normalFont);
		normalText.setCharacterSize(normalFontSize);
		normalText.setColor(sf::Color(sf::Color::White));
		normalText.setString(item.title);
		normalText.setPosition(normalPosX + 20, normalPosY + (i*normalFontSize));
		window.draw(normalText);
	}

}
