#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"
#include "inputHandle.h"
#include "assetHandle.h"
#include "SkinHandle.h"


class GameList
{
	private:
		
		SkinHandle::Game_List_Settings *settings;
		
		sf::RectangleShape rectangle;
		
		dbHandle &db;
		assetHandle &ah;

		sf::Text selectedText;
		sf::Text normalText;
		std::vector<dbHandle::gameListItem> listOfItems;
		
		int selectedItemNum;
		int counter; //used for input control
		
		bool selectedItemChange;
		void GameList::nextLetter(int direction);

	public:
		GameList(dbHandle &db_ref, assetHandle &ah_ref);
		~GameList(void);

		void GameList::init(SkinHandle& sh);//float posX, float posY, int width, float height);
		bool GameList::update(inputHandle& ih);
		void GameList::draw(sf::RenderWindow& window);
		void GameList::updateFilter(std::string filterString1, std::string filterString2);
		dbHandle::gameListItem  GameList::getCurrentItem();
};

