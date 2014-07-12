#include "SettingsScreen.h"


SettingsScreen::SettingsScreen(dbHandle &db_ref, assetHandle &ah_ref):
	db(db_ref),
	ah(ah_ref)
{
	editGameVisible = false;
}


SettingsScreen::~SettingsScreen(void)
{
}


void SettingsScreen::init(float posX, float posY)
{
	menuRect.setSize(sf::Vector2f(256,320));
	menuRect.setPosition(posX - 256/2, posY - 512/2);
	menuRect.setFillColor(sf::Color::Color(46,76,106,255));
	menuRect.setOutlineColor(sf::Color::Black);
	menuRect.setOutlineThickness(2);

	menuButtonRect.setSize(sf::Vector2f(224,32));
	menuButtonRect.setFillColor(sf::Color::Color(0,153,204,255));
	menuButtonRect.setOutlineColor(sf::Color::Black);
	menuButtonRect.setOutlineThickness(1);

	menuFont.loadFromFile(db.exe_path + "\\assets\\fonts\\ARCADE_PIX.TTF");
	menuText.setFont(menuFont);
	menuText.setCharacterSize(20);
	menuShadowText = menuText;


	menuNav.addItem("close", "", "", "","editGame", "none", "Close Menu", menuRect.getPosition().x+16, menuRect.getPosition().y + 16);
	menuNav.addItem("editGame", "" , "", "close", "platformOptions", "none", "Edit Game Data", menuRect.getPosition().x+16, menuRect.getPosition().y + (16 * 2) + 32);
	menuNav.addItem("platformOptions", "", "", "editGame", "controls", "none", "Platform Options", menuRect.getPosition().x+16, menuRect.getPosition().y + (16 * 3) + (32 * 2));
	menuNav.addItem("controls", "", "", "platformOptions", "exit", "none", "Controls", menuRect.getPosition().x+16, menuRect.getPosition().y + (16 * 4) + (32 * 3));
	menuNav.addItem("exit", "", "", "controls", "", "none", "Exit Arcadia", menuRect.getPosition().x+16, menuRect.getPosition().y + (16 * 5) + (32 * 4));

	menuIDs = menuNav.getIDVector();
}

void SettingsScreen::setCurrentGameListItem(dbHandle::gameListItem gameItem)
{
	currentGameListItem = gameItem;
}

bool SettingsScreen::update(inputHandle& ih)
{
	menuNav.selected = false;
	if (ih.inputPress(inputHandle::inputs::up) || ih.inputHold(inputHandle::inputs::up))
		menuNav.move(MenuNavigation::movements::up);
	else if (ih.inputPress(inputHandle::inputs::down) || ih.inputHold(inputHandle::inputs::down))
		menuNav.move(MenuNavigation::movements::down);
	else if (ih.inputPress(inputHandle::inputs::left) || ih.inputHold(inputHandle::inputs::left))
		menuNav.move(MenuNavigation::movements::left);
	else if (ih.inputPress(inputHandle::inputs::right) || ih.inputHold(inputHandle::inputs::right))
		menuNav.move(MenuNavigation::movements::right);
	else if (ih.inputPress(inputHandle::inputs::start_game) && !menuNav.selected)
		menuNav.selected = true;			
	


	if (menuNav.selected) // Selected  = True
	{
		if (menuNav.getCurrentID() == "editGame")
		{
			if (!editGameVisible) //we havent displayed the edit game screen yet, so load up any details we need.
			{
				//gameDB_FindResults = db.findGame_onGameDb(currentGameListItem);
				//for (auto &item : gameDB_FindResults)
				//{

				//}
			}
		}
		else if (menuNav.getCurrentID() == "close")
			return true;
	}


	return false;
}
void SettingsScreen::draw(sf::RenderWindow& window)
{
	window.draw(menuRect);

	for(auto menuID : menuIDs) 
	{
		MenuNavigation::menuItem menuItem = menuNav.getMenuItem(menuID);
		menuButtonRect.setPosition( menuItem.posX,  menuItem.posY);
		window.draw(menuButtonRect);

		menuText.setColor(sf::Color::White);
		menuText.setString(menuItem.value);
		menuText.setPosition((menuButtonRect.getPosition().x + menuButtonRect.getLocalBounds().width/2) - menuText.getLocalBounds().width/2  , menuButtonRect.getPosition().y + 3);
		menuShadowText = menuText;
		menuShadowText.setColor(sf::Color::Black);
		menuShadowText.setPosition(menuText.getPosition().x + 2  , menuText.getPosition().y + 2);
		window.draw(menuShadowText);
		window.draw(menuText);
	}

	




	// draw the hand pointer
		sf::Sprite pointerSprite;
		pointerSprite.setTexture(ah.getTextureAsset("POINTER"));
		pointerSprite.setOrigin(32,12);
		pointerSprite.setPosition(menuNav.getCurrentPosX() + 10, menuNav.getCurrentPosY()+ 10);
		window.draw(pointerSprite);

}