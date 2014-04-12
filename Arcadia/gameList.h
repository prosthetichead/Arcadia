#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"
#include "inputHandle.h"


class GameList
{
	private:
		sf::RectangleShape rectangle;
		sf::Font selectedFont;
		sf::Font normalFont;
		dbHandle db;
		sf::Text selectedText;
		std::vector<dbHandle::gameListItem> listOfItems;
		int selectedItemNum;
		bool repeat; // used for input control (maybe moved to a input handeler?)
		int counter; //used for input control (maybe moved to a input handeler?)
		int normalFontSize;
		int selectedFontSize;
	public:
		GameList();
		~GameList(void);
		void GameList::init(dbHandle& db_obj, float posX, float posY, int width, float height);
		void GameList::update(inputHandle::inputState inputStates);
		void GameList::draw(sf::RenderWindow& window);
		void GameList::updateFilter(std::string filterString);
		dbHandle::gameListItem  GameList::getCurrentItem();
};

