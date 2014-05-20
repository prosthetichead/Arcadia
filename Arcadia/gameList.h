#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"
#include "inputHandle.h"
#include "assetHandle.h"


class GameList
{
	private:
		sf::RectangleShape rectangle;
		sf::Font selectedFont;
		sf::Font normalFont;
		dbHandle &db;
		assetHandle &ah;

		sf::Text selectedText;
		std::vector<dbHandle::gameListItem> listOfItems;
		int selectedItemNum;
		
		int counter; //used for input control
		int normalFontSize;
		int selectedFontSize;

		bool selectedItemChange;


	public:
		GameList(dbHandle &db_ref, assetHandle &ah_ref);
		~GameList(void);

		void GameList::init(float posX, float posY, int width, float height);
		bool GameList::update(inputHandle& ih);
		void GameList::draw(sf::RenderWindow& window);
		void GameList::updateFilter(std::string filterString);
		dbHandle::gameListItem  GameList::getCurrentItem();
};

