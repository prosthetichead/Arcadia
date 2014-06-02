#include "FilterScreen.h"


FilterScreen::FilterScreen(dbHandle &db_ref, assetHandle &ah_ref):
	db(db_ref),
	ah(ah_ref),
	developerFilter(db_ref,ah_ref)
{
}


FilterScreen::~FilterScreen(void)
{
}

void FilterScreen::init(float posX, float posY, int width, float height)
{
	//menuNav
	menuNav.addItem("developer", "" , "publisher", "", "", "filterList", "none", 400, 500);
	menuNav.addItem("publisher", "developer" , "", "", "", "filterList", "none", 600, 500);

	screenTexture.loadFromFile(db.exe_path + "\\assets\\system\\filterMenu.png");
	pointerTexture.loadFromFile(db.exe_path + "\\assets\\system\\pointer.png");

	std::vector<dbHandle::filterListItem> listItems = db.getCustomFilterList("developer", "select distinct companies.id, companies.name, companies.icon_id from companies, games where games.developer_id = companies.id and games.active = 1", "games.developer_id");
	developerFilter.init(150,150,0,listItems, "developers");

}

void FilterScreen::update(inputHandle& ih)
{
	if (ih.inputPress(inputHandle::inputs::up) || ih.inputHold(inputHandle::inputs::up))
		menuNav.move(MenuNavigation::movements::up);
	else if (ih.inputPress(inputHandle::inputs::down) || ih.inputHold(inputHandle::inputs::down))
		menuNav.move(MenuNavigation::movements::down);
	else if (ih.inputPress(inputHandle::inputs::left) || ih.inputHold(inputHandle::inputs::left))
		menuNav.move(MenuNavigation::movements::left);
	else if (ih.inputPress(inputHandle::inputs::right) || ih.inputHold(inputHandle::inputs::right))
		menuNav.move(MenuNavigation::movements::right);

}

void FilterScreen::draw(sf::RenderWindow& window)
{
	sf::Sprite pointerSprite;
	pointerSprite.setTexture(pointerTexture);
	pointerSprite.setPosition(menuNav.getSelectedPosX(), menuNav.getSelectedPosY());
	
	sf::Sprite screenSprite;
	screenSprite.setTexture(screenTexture);

	window.draw(screenSprite);
	window.draw(pointerSprite);
	developerFilter.draw(window);
}
