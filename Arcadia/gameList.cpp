#include "gameList.h"

using namespace std;

GameList::GameList(dbHandle &db_ref, assetHandle &ah_ref): db(db_ref), ah(ah_ref)
{	
	selectedItemNum = 0;
	counter = 0;
	selectedItemChange = false;	
}
GameList::~GameList(void)
{
}


void GameList::init(SkinHandle& sh)//float posX, float posY, int width, float height)
{
	settings = &sh.game_list_settings;

	//setup rectangle 
	rectangle.setSize(settings->size);
	rectangle.setPosition(settings->position);
	
	//setup fonts
	selectedText.setFont( ah.getFontAsset(settings->selected_font.fontName) );
	selectedText.setCharacterSize(settings->selected_font.size);
	normalText.setFont( ah.getFontAsset(settings->normal_font.fontName) );
	normalText.setCharacterSize(settings->normal_font.size);
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

bool GameList::update(inputHandle& ih)
{
	selectedItemChange = false;
	if (ih.inputPress(inputHandle::inputs::up) || ih.inputHold(inputHandle::inputs::up))
	{
		selectedItemNum--;
		selectedItemChange = true;
	}
	else if (ih.inputPress(inputHandle::inputs::down) || ih.inputHold(inputHandle::inputs::down))
	{
		selectedItemNum++;
		selectedItemChange = true;
	}
	else if (ih.inputPress(inputHandle::inputs::right))
	{
		std::string firstLetter = listOfItems.at(selectedItemNum).title;
		firstLetter = firstLetter.at(0);
		for (int i=selectedItemNum; i < listOfItems.size(); i++)
		{
			std::string newFirstLetter = listOfItems.at(i).title;
			newFirstLetter = newFirstLetter.at(0);
			if (firstLetter != newFirstLetter)
			{
				selectedItemNum = i;
				break;
			}
		}
		selectedItemChange = true;
	}
	else if (ih.inputPress(inputHandle::inputs::left))
	{
		std::string firstLetter = listOfItems.at(selectedItemNum).title;
		firstLetter = firstLetter.at(0);
		for (int i=selectedItemNum; i >= 0; i--)
		{
			std::string newFirstLetter = listOfItems.at(i).title;
			newFirstLetter = newFirstLetter.at(0);
			if (firstLetter != newFirstLetter)
			{
				selectedItemNum = i;
				break;
			}
		}
		selectedItemChange = true;
	}
	
	//Lock selectedItemNum To size of the vector
	if (selectedItemNum < 0)
		selectedItemNum = listOfItems.size() -1;
	else if (selectedItemNum >= listOfItems.size())
		selectedItemNum = 0;

	// Run only if Selected Item is changed
	if (selectedItemChange)
	{
		
	}

	selectedText.setString(listOfItems.at(selectedItemNum).title.substr(0,50));

	return selectedItemChange;
}


void GameList::draw(sf::RenderWindow& window)
{
	//window.draw(rectangle);

	int selectedFontPadding = selectedText.getCharacterSize() + 20;
	int normalFontSize_Padding = normalText.getCharacterSize() + 10;

	float selectedFontSizeHalf = selectedText.getCharacterSize() / 2;
	int selectedPosX = rectangle.getPosition().x + 30;
	int selectedPosY = ((rectangle.getSize().y / 2) + rectangle.getPosition().y);
	
	
	selectedText.setOrigin(0, selectedFontSizeHalf );
	selectedText.setPosition(selectedPosX , selectedPosY );
	selectedText.setColor(sf::Color::Color(0,102,153,255));
	window.draw(selectedText);
	selectedText.setPosition(selectedPosX -2, selectedPosY -2);
	selectedText.setColor(settings->selected_font.color);
	window.draw(selectedText);

	sf::RectangleShape line(sf::Vector2f(rectangle.getSize().x, 2));
	line.setFillColor(sf::Color::Color(0,102,153,255));
	line.setPosition(selectedPosX - 10, selectedPosY - (selectedFontSizeHalf + selectedFontPadding/4));
	window.draw(line);
	line.setPosition(selectedPosX - 10, selectedPosY + (selectedFontSizeHalf + selectedFontPadding/4));
	window.draw(line);

	int numNormalItems = (rectangle.getSize().y - (selectedText.getCharacterSize() + selectedFontPadding)) / (normalFontSize_Padding);
	if (numNormalItems > listOfItems.size())
		numNormalItems = listOfItems.size();
	
	float normalPosX = rectangle.getPosition().x + 10;
	float normalPosY = selectedPosY - (normalText.getCharacterSize() + selectedFontPadding);
		
	int numItemsHalf = numNormalItems/2;

	for(int i=0; i < numItemsHalf; ++i)
	{
		int itemNum = selectedItemNum - i-1;
		if (itemNum < 0)
			itemNum = listOfItems.size() + itemNum;

		dbHandle::gameListItem item = listOfItems.at(itemNum);

		sf::Color newNormalColor = settings->normal_font.color;
		newNormalColor.a = 255 - i * (255/numItemsHalf);
		normalText.setColor(newNormalColor);
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

		sf::Color newNormalColor = settings->normal_font.color;
		newNormalColor.a = 255 - i * (255/numItemsHalf);
		normalText.setColor(newNormalColor);
		normalText.setString(item.title);
		normalText.setPosition(normalPosX, normalPosY + (i*normalFontSize_Padding));
		window.draw(normalText);
	}

}
