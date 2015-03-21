#pragma once
#include <SFML/Graphics.hpp>
#include "SkinHandle.h"
#include <boost\algorithm\string\split.hpp>
#include <boost\algorithm\string.hpp>

class TextScrolling
{
private:
	
	std::string text;
	bool descriptionScroll;
	bool descriptionPause;
	float descriptionPauseTime;
	float descriptionPauseCount;
	float descriptionOffset;
	float descriptionRequiredOffset;

public:

	void TextScrolling::update();
	void TextScrolling::draw(sf::Window &window, SkinHandle::Skin_Element &se);
	void TextScrolling::setText(std::string new_text);

	TextScrolling(void);

	~TextScrolling(void);
};

