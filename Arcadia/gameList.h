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
		
		int counter; //used for input control
		int normalFontSize;
		int selectedFontSize;

		sf::Texture flagTexture;
		bool selectedItemChange;

	public:
		GameList();
		~GameList(void);
		void GameList::init(dbHandle& db_obj, float posX, float posY, int width, float height);
		void GameList::update(inputHandle::inputState inputStates);
		void GameList::draw(sf::RenderWindow& window);
		void GameList::updateFilter(std::string filterString);
		dbHandle::gameListItem  GameList::getCurrentItem();
};

