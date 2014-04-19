#include "GameInfo.h"


GameInfo::GameInfo(void)
{


}


GameInfo::~GameInfo(void)
{
}

void GameInfo::init(dbHandle &db_obj, float posX, float posY, int width, float height)
{
	db = db_obj;

	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setPosition(posX, posY);
	rectangle.setFillColor(sf::Color::Color(45,33,100,255));
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(0);
}

void GameInfo::update(dbHandle::gameListItem gameItem)
{
	if (currentGameItem.gameID != gameItem.gameID)    // New Game Selected
	{
		currentGameItem = gameItem;

		std::cout << currentGameItem.title << std::endl;
		// Reload Textures
		// chanage text

	}
}

void GameInfo::draw(sf::RenderWindow& window)
{
	window.draw(rectangle);
}
