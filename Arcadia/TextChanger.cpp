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

void TextChanger::setText(std::vector<std::string> textVector, int speed)
{
	text = textVector;	
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

void TextChanger::draw(sf::RenderWindow &window, SkinHandle::Skin_Element &skin_item)
{	
	if (!text.empty()) {		
		ah->drawText(text.at(index), skin_item, window);
	}
}