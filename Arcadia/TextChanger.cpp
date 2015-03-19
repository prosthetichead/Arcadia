#include "TextChanger.h"


TextChanger::TextChanger(assetHandle* ah_ref )
{
	ah = ah_ref;
	changeTimer = 0;
	index = 0;
	
}


TextChanger::~TextChanger(void)
{
}

void TextChanger::setText(std::vector<std::string> textVector, SkinHandle::Skin_Element fontItem, int speed)
{
	text = textVector;
	font = fontItem;
	changeSpeed = speed;
	changeTimer = 0;
	index = 0;
}

void TextChanger::update()
{
	changeTimer++;
	if (changeTimer >= changeSpeed) {
		index++;
		changeTimer = 0;
	}
	if (index > text.size()-1)
		index = 0;
}

void TextChanger::draw(sf::RenderWindow &window)
{	
	if (!text.empty()) {		
		ah->drawText(text.at(index), font, window);
	}
}