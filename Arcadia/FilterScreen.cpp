#include "FilterScreen.h"


FilterScreen::FilterScreen(dbHandle &db_ref, assetHandle &ah_ref):
	db(db_ref),
	ah(ah_ref),
	osk(db_ref, ah_ref),
	developerFilter(db_ref,ah_ref),
	publisherFilter(db_ref,ah_ref),
	regionFilter(db_ref,ah_ref),
	genreFilter(db_ref,ah_ref)
{
	filterString = " ";
	showKeyboard = false;
}


FilterScreen::~FilterScreen(void)
{
}

void FilterScreen::updateFilterString()
{
	filterString = developerFilter.getFilterString() + publisherFilter.getFilterString() + regionFilter.getFilterString() + genreFilter.getFilterString();
	std::cout << filterString << std::endl;
}
std::string FilterScreen::getFilterString()
{
	return filterString;
}

void FilterScreen::init(float posX, float posY, int width, float height)
{
	menuRect.setSize(sf::Vector2f(width, height));
	menuRect.setPosition(posX, posY);

	//menuNav
	menuNav.addItem("developer", "" , "publisher", "", "players", "filterList", "none", menuRect.getPosition().x + 103, menuRect.getPosition().y + 90); // DEVELOPER
	menuNav.addItem("publisher", "developer" , "region", "", "co_op", "filterList", "none", menuRect.getPosition().x + 263, menuRect.getPosition().y + 90); // PUBLISHER
	menuNav.addItem("region", "publisher" , "genre", "", "rating", "filterList", "none", menuRect.getPosition().x + 423, menuRect.getPosition().y + 90);
	menuNav.addItem("genre", "region" , "", "", "favorites", "filterList", "none", menuRect.getPosition().x + 583, menuRect.getPosition().y + 90);

	menuNav.addItem("players", "" , "co_op", "developer", "stars", "number", "none", menuRect.getPosition().x + 103, menuRect.getPosition().y + 250);
	menuNav.addItem("co_op", "players" , "rating", "publisher", "year", "bool", "none", menuRect.getPosition().x + 263, menuRect.getPosition().y + 250);
	menuNav.addItem("rating", "co_op" , "favorites", "region", "search", "filterList", "none", menuRect.getPosition().x + 423, menuRect.getPosition().y + 250);
	menuNav.addItem("favorites", "rating" , "", "genre", "search", "bool", "none", menuRect.getPosition().x + 583, menuRect.getPosition().y + 250);

	menuNav.addItem("stars", "" , "year", "players", "cancel", "number", "none", menuRect.getPosition().x + 103, menuRect.getPosition().y + 413);
	menuNav.addItem("year", "stars" , "search", "co_op", "cancel", "year", "none", menuRect.getPosition().x + 263, menuRect.getPosition().y + 413);
	menuNav.addItem("search", "year" , "", "rating", "cancel", "search", "none", menuRect.getPosition().x + 423, menuRect.getPosition().y + 413);

	menuNav.addItem("cancel", "year" , "apply", "search", "", "button", "none", menuRect.getPosition().x + 423, menuRect.getPosition().y + 509);
	menuNav.addItem("apply", "cancel" , "", "search", "", "button", "none", menuRect.getPosition().x + 583, menuRect.getPosition().y + 509);
	
	screenTexture_p = new sf::Texture();
	screenTexture_p->loadFromFile(db.exe_path + "\\assets\\system\\filterMenu.png");
	pointerTexture.loadFromFile(db.exe_path + "\\assets\\system\\pointer.png");
	menuRect.setTexture(screenTexture_p);

	osk.init(300, 500);

	std::vector<dbHandle::filterListItem> listItems = db.getCustomFilterList("developer", "select distinct companies.id, companies.name, companies.icon_id from companies, games where games.developer_id = companies.id and games.active = 1", "games.developer_id");
	developerFilter.init(150,150,0,listItems, "developer");
	
	listItems = db.getCustomFilterList("publisher", "select distinct companies.id, companies.name, companies.icon_id from companies, games where games.publisher_id = companies.id and games.active = 1", "games.publisher_id");
	publisherFilter.init(350,150,0,listItems, "publisher");

	listItems = db.getCustomFilterList("region", "select distinct regions.id, regions.name, regions.icon_id from regions, games where games.region_id = regions.id and games.active = 1", "games.region_id");
	regionFilter.init(500,150,0,listItems, "region");

	listItems = db.getCustomFilterList("genre", "select distinct genres.id, genres.genre_name, genres.icon_id from genres, games where games.genre_id = genres.id and games.active = 1", "games.genre_id");
	genreFilter.init(610,150,0,listItems, "region");
}

bool FilterScreen::update(inputHandle& ih)
{
	if(showKeyboard)
	{
		if(osk.update(ih))
		{
			// returned true, close keyboard and get the search string
			showKeyboard = false;
		}
	}
	else
	{

		if (menuNav.selected) // Selected  = True
		{
			if (ih.inputPress(inputHandle::inputs::start_game))
				menuNav.selected = false;

			if (menuNav.getCurrentID() == "developer") //Developer Menu Item is selected do our thing with the developer filter
			{
				 if (ih.inputPress(inputHandle::inputs::left))
					 developerFilter.update(-1);
				 else if (ih.inputPress(inputHandle::inputs::right))
					 developerFilter.update(1);
			}		
			if (menuNav.getCurrentID() == "publisher")
			{
				 if (ih.inputPress(inputHandle::inputs::left))
					 publisherFilter.update(-1);
				 else if (ih.inputPress(inputHandle::inputs::right))
					 publisherFilter.update(1);
			}
			if (menuNav.getCurrentID() == "region") 
			{
				 if (ih.inputPress(inputHandle::inputs::left))
					 regionFilter.update(-1);
				 else if (ih.inputPress(inputHandle::inputs::right))
					 regionFilter.update(1);
			}
			if (menuNav.getCurrentID() == "genre") 
			{
				 if (ih.inputPress(inputHandle::inputs::left))
					 genreFilter.update(-1);
				 else if (ih.inputPress(inputHandle::inputs::right))
					 genreFilter.update(1);
			}
			if (menuNav.getCurrentID() == "search")
			{
				showKeyboard = true;
			}
			if (menuNav.getCurrentID() == "apply")
			{
				menuNav.selected = false;
				updateFilterString();
				return true;  // Close the menu now
			}
			if (menuNav.getCurrentID() == "cancel")
			{
				menuNav.selected = false;
				return true; // Close the menu now
			}
		}
		else // Selected  = False
		{
			if (ih.inputPress(inputHandle::inputs::up) || ih.inputHold(inputHandle::inputs::up))
				menuNav.move(MenuNavigation::movements::up);
			else if (ih.inputPress(inputHandle::inputs::down) || ih.inputHold(inputHandle::inputs::down))
				menuNav.move(MenuNavigation::movements::down);
			else if (ih.inputPress(inputHandle::inputs::left) || ih.inputHold(inputHandle::inputs::left))
				menuNav.move(MenuNavigation::movements::left);
			else if (ih.inputPress(inputHandle::inputs::right) || ih.inputHold(inputHandle::inputs::right))
				menuNav.move(MenuNavigation::movements::right);
			else if (ih.inputPress(inputHandle::inputs::start_game) && !menuNav.selected)
			{
				menuNav.selected = true;			
			}
		}
	}

	return false; // Dont close the menu this update!

}

void FilterScreen::draw(sf::RenderWindow& window)
{
	
	window.draw(menuRect);

	if(!menuNav.selected) // Nothing is selected yet, show the pointer
	{
		sf::Sprite pointerSprite;
		pointerSprite.setTexture(pointerTexture);
		pointerSprite.setOrigin(32,12);
		pointerSprite.setPosition(menuNav.getCurrentPosX(), menuNav.getCurrentPosY());
		window.draw(pointerSprite);
	}

	developerFilter.draw(window); //Draw the developer filter list.
	publisherFilter.draw(window);
	regionFilter.draw(window);
	genreFilter.draw(window);
	if(showKeyboard)
	{
		osk.draw(window);
	}
}