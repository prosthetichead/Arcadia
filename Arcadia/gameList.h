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
		dbHandle db;
		assetHandle assets;

		sf::Text selectedText;
		std::vector<dbHandle::gameListItem> listOfItems;
		int selectedItemNum;
		
		int counter; //used for input control
		int normalFontSize;
		int selectedFontSize;

		bool selectedItemChange;


	public:
		GameList();
		~GameList(void);
		void GameList::init(dbHandle& db_obj, assetHandle& icon_obj,  float posX, float posY, int width, float height);
		void GameList::update(inputHandle& ih);
		void GameList::draw(sf::RenderWindow& window);
		void GameList::updateFilter(std::string filterString);
		dbHandle::gameListItem  GameList::getCurrentItem();
};

