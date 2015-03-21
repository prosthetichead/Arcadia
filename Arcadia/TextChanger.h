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

public:
	TextChanger(assetHandle* ah_ref);
	~TextChanger(void);

	void TextChanger::setText(std::vector<std::string> textVector, int speed);

	void TextChanger::update();
	void TextChanger::draw(sf::RenderWindow &window, SkinHandle::Skin_Element &skin_item);

};
