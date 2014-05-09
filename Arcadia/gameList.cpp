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


void GameList::init(dbHandle &db_obj, assetHandle &asset_obj,  float posX, float posY, int width, float height)
{
	//setup Database handeler
	db = db_obj;
	assets = asset_obj;
		
	//setup rectangle 
	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setPosition(posX, posY);
	rectangle.setFillColor(sf::Color::Color(0,0,0,150));
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(0);
	
	selectedFont.loadFromFile(db.exe_path + "\\assets\\fonts\\Teknik-bold.ttf");
	normalFont.loadFromFile(db.exe_path + "\\assets\\fonts\\Teknik-Regular.ttf");
	normalFontSize = 18;
	selectedFontSize = 24;
	
	selectedText.setFont(selectedFont);
	selectedText.setCharacterSize(selectedFontSize);
	selectedText.setColor(sf::Color::White);
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
		
	}

	selectedText.setString(listOfItems.at(selectedItemNum).title);
		
}

void GameList::draw(sf::RenderWindow& window)
{
	//window.draw(rectangle);

	int selectedFontPadding = selectedFontSize+20;
	int normalFontSize_Padding = normalFontSize+10;
	float selectedFontSizeHalf = selectedFontSize/2;
	int selectedPosX = rectangle.getPosition().x + 30;
	int selectedPosY = (rectangle.getSize().y / 2) + rectangle.getPosition().y;
	selectedText.setPosition(selectedPosX, selectedPosY);
	selectedText.setOrigin(0,selectedFontSize/2);
	window.draw(selectedText);

	sf::RectangleShape line(sf::Vector2f(rectangle.getSize().x, 2));
	line.setPosition(selectedPosX - 10, selectedPosY - (selectedFontSizeHalf + selectedFontPadding/4));
	window.draw(line);
	line.setPosition(selectedPosX - 10, selectedPosY + (selectedFontSizeHalf + selectedFontPadding/4));
	window.draw(line);

	


	//sf::Sprite flagSprite;
	//sf::Texture empty;
	//empty.create(16,11);
	//flagSprite.setTexture(empty);
	//flagSprite.setTexture(assets.getTextureAsset(listOfItems.at(selectedItemNum).region));
	//flagSprite.setPosition(selectedText.getPosition().x - 20, selectedText.getPosition().y + 6);

	int numNormalItems = (rectangle.getSize().y - (selectedFontSize + selectedFontPadding)) / (normalFontSize_Padding);
	if (numNormalItems > listOfItems.size())
		numNormalItems = listOfItems.size();
	
	float normalPosX = rectangle.getPosition().x + 10;
	float normalPosY = selectedPosY - (normalFontSize + selectedFontPadding);
		
	
	//window.draw(flagSprite);

	int numItemsHalf = numNormalItems/2;

	for(int i=0; i < numItemsHalf; ++i)
	{
		int itemNum = selectedItemNum - i-1;
		if (itemNum < 0)
			itemNum = listOfItems.size() + itemNum;

		dbHandle::gameListItem item = listOfItems.at(itemNum);

		sf::Text normalText;

		//flagSprite.setTexture(assets.getTextureAsset(item.region));
		//flagSprite.setPosition(normalPosX, normalPosY - (i*normalFontSize)+4);
		//window.draw(flagSprite);

		normalText.setFont(normalFont);
		normalText.setCharacterSize(normalFontSize);
		normalText.setColor(sf::Color(sf::Color::Color(255,255,255,255 - i * (255/numItemsHalf))));
		normalText.setString(item.title);
		normalText.setPosition(normalPosX, normalPosY - (i*normalFontSize_Padding));
		window.draw(normalText);
	}

	
	normalPosY = selectedPosY + (selectedFontPadding);
	for(int i=0; i < numItemsHalf; ++i)
	{
		int itemNum = selectedItemNum + i + 1;
		if (itemNum > listOfItems.size() - 1)
			itemNum = 0 + (itemNum - listOfItems.size());
			
		dbHandle::gameListItem item = listOfItems.at(itemNum);

		sf::Text normalText;

		//flagSprite.setTexture(assets.getTextureAsset(item.region));
		//flagSprite.setPosition(normalPosX, normalPosY + (i*normalFontSize)+4);
		//window.draw(flagSprite);

		normalText.setFont(normalFont);
		normalText.setCharacterSize(normalFontSize);
		normalText.setColor(sf::Color(sf::Color::Color(255,255,255,255 - i * (255/numItemsHalf))));
		normalText.setString(item.title);
		normalText.setPosition(normalPosX, normalPosY + (i*normalFontSize_Padding));
		window.draw(normalText);
	}

}
