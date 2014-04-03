#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"


class GameList
{
	private:
		sf::RectangleShape rectangle;
		sf::Font selectedFont;
		sf::Font normalFont;
		dbHandle db;
		sf::Text selectedText;
		std::vector<dbHandle::listItem> listOfItems;
		std::vector<dbHandle::listItem> filteredListOfItems;
		int selectedItemNum;
		int counter;
		bool repeat;
		int normalFontSize;
		int selectedFontSize;
	public:
		GameList();
		~GameList(void);
		void GameList::init(dbHandle& db_obj, float posX, float posY, int width, float height);
		void GameList::update();
		void GameList::draw(sf::RenderWindow& window);
		void GameList::setPlatform(std::string platformID);
};

