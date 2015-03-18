#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"
#include "assetHandle.h"
#include "SkinHandle.h"

class TextChanger
{
private:
	
	assetHandle* ah;

	int changeTimer;
	int changeSpeed;
	int index;

	std::vector<std::string> text;
	SkinHandle::Font_Item font;


public:
	TextChanger(assetHandle* ah_ref);
	~TextChanger(void);

	void TextChanger::setText(std::vector<std::string> textVector, SkinHandle::Font_Item fontItem, int speed);

	void TextChanger::update();
	void TextChanger::draw(sf::RenderWindow &window, sf::Vector2f pos, sf::Vector2f size);

};

