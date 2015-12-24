#include "ControlsScreen.h"


ControlsScreen::ControlsScreen()
{
	rectBackground.setSize(sf::Vector2f(320, 480));
	rectBackground.setFillColor(sf::Color::Color(46,76,106,255));
	rectBackground.setOutlineColor(sf::Color::Black);
	rectBackground.setOutlineThickness(2);
	rectBackground.setPosition(50, 50);

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

	//setup menu nav for input states
	std::vector<dbHandle::inputItem>& inputStates = ih->getInputStates();
	int index = 0;
	std::string prevItemName = "";
	for (auto &item : inputStates)
	{
		std::string nextItemName;
		if (index < inputStates.size() - 1)
			nextItemName = std::to_string(inputStates[index + 1].inputID);
		else
			nextItemName = "reset";
			
		menuNav.addItem(std::to_string(item.inputID), "", "", prevItemName, nextItemName, "input", item.inputName, rectBackground.getPosition().x + 10, rectBackground.getPosition().y + 5 + (30 * index));
		prevItemName = std::to_string(item.inputID);
		index++;
	}
	menuNav.addItem("reset", "", "", prevItemName,  "save", "none", "Reset To Defaults", rectBackground.getPosition().x + 10, rectBackground.getPosition().y + 5 + (30 * (index + 1)));
	menuNav.addItem("save", "", "", "reset", "cancel", "none", "Save", rectBackground.getPosition().x + 10, rectBackground.getPosition().y + 5 + (30 * (index + 2)));
	menuNav.addItem("cancel", "", "", "save", "", "none", "Cancel", rectBackground.getPosition().x + 10, rectBackground.getPosition().y + 5 + (30 * (index + 3)));
	
}

bool ControlsScreen::update()
{
	if (menuNav.selected){
		if (menuNav.getCurrentType() == "input"){
			
			printf(menuNav.getCurrentID().c_str());
		}
	}
	else {
		if (ih->inputPress(inputHandle::inputs::up) || ih->inputHold(inputHandle::inputs::up))
			menuNav.move(MenuNavigation::movements::up);

		else if (ih->inputPress(inputHandle::inputs::down) || ih->inputHold(inputHandle::inputs::down))
			menuNav.move(MenuNavigation::movements::down);

		else if (ih->inputPress(inputHandle::inputs::left) || ih->inputHold(inputHandle::inputs::left))
			menuNav.move(MenuNavigation::movements::left);

		else if (ih->inputPress(inputHandle::inputs::right) || ih->inputHold(inputHandle::inputs::right))
			menuNav.move(MenuNavigation::movements::right);

		else if (ih->inputPress(inputHandle::inputs::start_game) && !menuNav.selected)
			menuNav.selected = true;
	}
	

	//if (selected) // Lets update be skipped first time so screen can be drawn before waiting for the key
	//{
	//	int numberOfInputs = ih->getInputStates().size();
	//	ih->waitAndUpdateInput( (inputHandle::inputs)selectedItem );
	//	++selectedItem;
	//	sf::sleep(sf::milliseconds(120));
	//	if (selectedItem == numberOfInputs)
	//	{
	//		selectedItem = 0;
	//		selected = false;
	//		return true;
	//	}
	//}
	//selected = true;
	return false;
}

void ControlsScreen::draw(sf::RenderWindow &window)
{
	window.draw(rectBackground);
	std::vector<std::string> menuIDs = menuNav.getIDVector();
	for (std::string menuID : menuIDs){
		MenuNavigation::menuItem menuItem = menuNav.getMenuItem(menuID);
		text.setPosition(menuItem.posX, menuItem.posY);
		text.setString(menuItem.value);
		window.draw(text);
		//This is a input item so also draw its current key alication
		if (menuItem.type == "input"){
			int inputID = std::stoi(menuID);
			text.setPosition(menuItem.posX + 250, menuItem.posY);
			text.setString(ih->getKeyName( ih->getInputItem((inputHandle::inputs)inputID).key));
			window.draw(text);
		}
		
	}
	// draw the hand pointer
	sf::Sprite pointerSprite;
	pointerSprite.setTexture(ah->getTextureAsset("POINTER"));
	pointerSprite.setOrigin(32, 12);
	pointerSprite.setPosition(menuNav.getCurrentPosX(), menuNav.getCurrentPosY() + 5);
	window.draw(pointerSprite);

	//std::vector<dbHandle::inputItem>& inputStates = ih->getInputStates();

	//int i=0;
	//for (auto &item : inputStates)
	//{
	//	if (i == selectedItem)
	//		text.setColor(sf::Color::Yellow);
	//	else
	//		text.setColor(sf::Color::White);
	//	// draw Input Name
	//	text.setPosition(rectBackground.getPosition().x + 10 , rectBackground.getPosition().y + 10 + ((text.getCharacterSize()+5) * i));
	//	text.setString(item.inputName);
	//	window.draw(text);
	//	// draw current input Value
	//	text.setPosition(rectBackground.getPosition().x + 220, rectBackground.getPosition().y + 10 + ((text.getCharacterSize()+5) * i));
	//	text.setString(ih->getKeyName(item.key));
	//	window.draw(text);
	//	i++;
	//}

	
}
