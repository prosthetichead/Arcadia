#pragma once
#include <SFML/Graphics.hpp>
#include "MenuNavigation.h"
#include "inputHandle.h"
#include "assetHandle.h"
#include "DBHandle.h"


class OnScreenKeyboard
{
private:
	dbHandle &db;
	assetHandle &ah;

	MenuNavigation menuNav;
	
	sf::Vector2f normalButtonSize;
	sf::Vector2f spaceBarSize;
	sf::Vector2f sysButtonSize;
	sf::Vector2f buttonPaddingSize;

	sf::Texture	keyBackTexture;
	sf::Texture	keySysBackTexture;
	sf::Texture	keySpaceBackTexture;

	std::vector<MenuNavigation::menuItem> KeyboardItemRefs;
	std::string userEnteredString;
	sf::Font keyFont;
	sf::Text keyText;
	sf::Text userEnteredText;

	sf::RectangleShape keyboardRectangle;

public:
	OnScreenKeyboard(dbHandle &db_ref, assetHandle &ah_ref);
	~OnScreenKeyboard(void);
	void OnScreenKeyboard::init(float posX, float posY);
	bool OnScreenKeyboard::update(inputHandle& ih);
	void OnScreenKeyboard::draw(sf::RenderWindow& window);
	std::string OnScreenKeyboard::getEnteredText();

};

