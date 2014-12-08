#include "ControlsScreen.h"


ControlsScreen::ControlsScreen()
{
	rectBackground.setSize(sf::Vector2f(320, 480));
	rectBackground.setFillColor(sf::Color::Color(46,76,106,255));
	rectBackground.setOutlineColor(sf::Color::Black);
	rectBackground.setOutlineThickness(2);

	selectedItem  = 0;
	selected = false;
}


ControlsScreen::~ControlsScreen(void)
{

}

void ControlsScreen::init(dbHandle* db_ref, assetHandle* ah_ref, inputHandle* ih_ref)
{
	db = db_ref;
	ah = ah_ref;
	ih = ih_ref;

	text.setFont(ah->getFontAsset("ARCADE_PIX.TTF"));
	text.setCharacterSize(14);
}

bool ControlsScreen::update()
{
	if (selected) // Lets update be skipped first time so screen can be drawn before waiting for the key
	{
		int numberOfInputs = ih->getInputStates().size();
		ih->waitAndUpdateInput( (inputHandle::inputs)selectedItem );
		++selectedItem;
		sf::sleep(sf::milliseconds(120));
		if (selectedItem == numberOfInputs)
		{
			selectedItem = 0;
			selected = false;
			return true;
		}
	}
	selected = true;
	return false;
}

void ControlsScreen::draw(sf::RenderWindow &window)
{
	window.draw(rectBackground);

	std::vector<dbHandle::inputItem>& inputStates = ih->getInputStates();

	int i=0;
	for (auto &item : inputStates)
	{
		if (i == selectedItem)
			text.setColor(sf::Color::Yellow);
		else
			text.setColor(sf::Color::White);
		// draw Input Name
		text.setPosition(rectBackground.getPosition().x + 10 , rectBackground.getPosition().y + 10 + ((text.getCharacterSize()+5) * i));
		text.setString(item.inputName);
		window.draw(text);
		// draw current input Value
		text.setPosition(rectBackground.getPosition().x + 220, rectBackground.getPosition().y + 10 + ((text.getCharacterSize()+5) * i));
		text.setString(ih->getKeyName(item.key));
		window.draw(text);
		i++;
	}

	
}
