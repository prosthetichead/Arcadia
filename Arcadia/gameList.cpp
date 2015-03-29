#include "gameList.h"

using namespace std;

GameList::GameList(dbHandle* db_ref, assetHandle* ah_ref, SkinHandle* sh_ref)
{	
	ah = ah_ref;
	db = db_ref;
	sh = sh_ref;
	selectedItemNum = 0;
	counter = 0;
	selectedItemChange = false;	
}
GameList::~GameList(void)
{
}

void GameList::updateFilter(std::string filterString1, std::string filterString2 )
{	
	selectedItemNum = 0;
	listOfItems = db->getGamesListQuery(filterString1, filterString2);
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


	return selectedItemChange;
}


void GameList::draw(sf::RenderWindow& window)
{

	sf::RectangleShape listRect = sh->game_list_settings.list.getRectangle();

	//draw selected text
	int selectedLineSpacing = sh->game_list_settings.selected_list_item.line_spacing;
	int selectedIndent = sh->game_list_settings.selected_list_item.indent;
	std::string text = listOfItems.at(selectedItemNum).title;
	sh->game_list_settings.selected_list_item.pos.x = listRect.getPosition().x + selectedIndent; 
	sh->game_list_settings.selected_list_item.pos.y = listRect.getPosition().y + (listRect.getSize().y/2); 
	sf::Text selectedText = ah->getText(text, sh->game_list_settings.selected_list_item);
	ah->trimTextToRectangleWidth(selectedText, listRect);
	ah->drawText(selectedText, sh->game_list_settings.selected_list_item, window );

	
	

	// draw unselected (normal) items
	int normalLineSpacing = sh->game_list_settings.normal_list_item.line_spacing;
	int normalIndent = sh->game_list_settings.normal_list_item.indent;
	
	sf::Text normalText = ah->getText("Place Holder Text", sh->game_list_settings.normal_list_item);

	int numNormalItems = ( listRect.getSize().y - (selectedText.getGlobalBounds().height + selectedLineSpacing)) / (normalText.getGlobalBounds().height + normalLineSpacing) ;
	if (numNormalItems > listOfItems.size())
		numNormalItems = listOfItems.size();

	for(int i=0; i < numNormalItems/2; ++i)
	{
		int itemNum_top = selectedItemNum - i-1;
		if (itemNum_top < 0)
			itemNum_top = listOfItems.size() + itemNum_top;

		int itemNum_bottom = selectedItemNum + i + 1;
		if (itemNum_bottom > listOfItems.size() - 1)
			itemNum_bottom = 0 + (itemNum_bottom - listOfItems.size());
		
		normalText.setString(listOfItems.at(itemNum_top).title);
		normalText.setPosition(listRect.getPosition().x + normalIndent, (selectedText.getPosition().y - (normalText.getGlobalBounds().height/2 + selectedText.getGlobalBounds().height/2 + selectedLineSpacing/2)) - ( i * (normalText.getLocalBounds().height + normalLineSpacing) ) );
		ah->trimTextToRectangleWidth(normalText, listRect);
		ah->drawText(normalText, sh->game_list_settings.normal_list_item, window );

		normalText.setString(listOfItems.at(itemNum_bottom).title);
		normalText.setPosition(listRect.getPosition().x + normalIndent, (selectedText.getPosition().y + (normalText.getGlobalBounds().height/2 + selectedText.getGlobalBounds().height/2 + selectedLineSpacing/2)) + ( i * (normalText.getLocalBounds().height + normalLineSpacing) ));
		ah->trimTextToRectangleWidth(normalText, listRect);
		ah->drawText(normalText, sh->game_list_settings.normal_list_item, window );
	}
}


