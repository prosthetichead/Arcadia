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
	searchString = "";
	co_op = -1;
	favourite = -1;
	year_operation = -1;
	year = 2000;
	userStars = -0.5;
	onlineStars = -0.5;
	userSelected = false;

	filterDescription = "";
}


FilterScreen::~FilterScreen(void)
{
}

void FilterScreen::updateFilterString()
{
	filterDescription = "";

	filterString = developerFilter.getFilterString() + publisherFilter.getFilterString() + regionFilter.getFilterString() + genreFilter.getFilterString();
	if (developerFilter.getFilterString() != "")
		filterDescription = "Developer: " + developerFilter.getSelectedTitle();

	if (!searchString.empty())
		filterString += " and games.name like '%" + searchString + "%' ";
	if (co_op != -1)
		filterString += " and games.co_op = " + std::to_string(co_op) + " ";
	if (favourite != -1)
		filterString += " and games.favourite = " + std::to_string(favourite) + " ";
	if (players != 0)
		filterString += " and games.players = " + std::to_string(players) + " ";

	switch (year_operation)
	{
		case 0:
			filterString += " and games.release_year > " + std::to_string(year) + " ";
			break;
		case 1:
			filterString += " and games.release_year < " + std::to_string(year) + " ";
			break;
		case 2:
			filterString += " and games.release_year = " + std::to_string(year) + " ";
			break;
		case 3:
			filterString += " and games.release_year <> " + std::to_string(year) + " ";
			break;
		default:
			break;
	}
	if (onlineStars >= 0)
		filterString += " and games.gamedb_stars >= " + std::to_string(onlineStars) + " ";
	if (userStars >= 0)
		filterString += " and games.users_stars >= " + std::to_string(userStars) + " ";	
	

	std::cout << filterString << std::endl;
	std::cout << filterDescription << std::endl;
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
	menuNav.addItem("genre", "region" , "", "", "favourite", "filterList", "none", menuRect.getPosition().x + 583, menuRect.getPosition().y + 90);

	menuNav.addItem("players", "" , "co_op", "developer", "stars", "number", "none", menuRect.getPosition().x + 103, menuRect.getPosition().y + 250);
	menuNav.addItem("co_op", "players" , "rating", "publisher", "year", "bool", "none", menuRect.getPosition().x + 263, menuRect.getPosition().y + 250);
	menuNav.addItem("rating", "co_op" , "favourite", "region", "search", "filterList", "none", menuRect.getPosition().x + 423, menuRect.getPosition().y + 250);
	menuNav.addItem("favourite", "rating" , "", "genre", "search", "bool", "none", menuRect.getPosition().x + 583, menuRect.getPosition().y + 250);

	menuNav.addItem("stars", "" , "year", "players", "cancel", "number", "none", menuRect.getPosition().x + 103, menuRect.getPosition().y + 413);
	menuNav.addItem("year", "stars" , "search", "co_op", "cancel", "year", "none", menuRect.getPosition().x + 263, menuRect.getPosition().y + 413);
	menuNav.addItem("search", "year" , "", "rating", "cancel", "search", "none", menuRect.getPosition().x + 423, menuRect.getPosition().y + 413);

	menuNav.addItem("cancel", "year" , "apply", "search", "", "button", "none", menuRect.getPosition().x + 423, menuRect.getPosition().y + 509);
	menuNav.addItem("apply", "cancel" , "", "search", "", "button", "none", menuRect.getPosition().x + 583, menuRect.getPosition().y + 509);
	
	screenTexture_p = new sf::Texture();
	screenTexture_p->loadFromFile(db.exe_path + "\\assets\\system\\filterMenu.png");
	//pointerTexture.loadFromFile();
	menuRect.setTexture(screenTexture_p);

	osk.init(300, 500);

	std::vector<dbHandle::filterListItem> listItems = db.getCustomFilterList("developer", "select distinct companies.id, companies.name, companies.icon_id from companies, games where games.developer_id = companies.id and games.active = 1", "games.developer_id");
	developerFilter.init(menuNav.getPosX("developer")+55, menuNav.getPosY("developer")+60,0, listItems, "developer");
	developerFilter.setSelectedSize(85);

	listItems = db.getCustomFilterList("publisher", "select distinct companies.id, companies.name, companies.icon_id from companies, games where games.publisher_id = companies.id and games.active = 1", "games.publisher_id");
	publisherFilter.init(menuNav.getPosX("publisher")+55, menuNav.getPosY("publisher")+60, 0, listItems, "publisher");
	publisherFilter.setSelectedSize(85);

	listItems = db.getCustomFilterList("region", "select distinct regions.id, regions.name, regions.icon_id from regions, games where games.region_id = regions.id and games.active = 1", "games.region_id");
	regionFilter.init(menuNav.getPosX("region")+55, menuNav.getPosY("region")+60, 0, listItems, "region");
	regionFilter.setSelectedSize(85);

	listItems = db.getCustomFilterList("genre", "select distinct genres.id, genres.genre_name, genres.icon_id from genres, games where games.genre_id = genres.id and games.active = 1", "games.genre_id");
	genreFilter.init(menuNav.getPosX("genre")+55, menuNav.getPosY("genre")+60,0,listItems, "region");
	genreFilter.setSelectedSize(85);

	textFont.loadFromFile(db.exe_path + "\\assets\\fonts\\ARCADE_PIX.TTF");
	largeText.setFont(textFont);
	largeText.setCharacterSize(18);
	searchStringText.setFont(textFont);
	searchStringText.setPosition(menuRect.getPosition().x + 427, menuRect.getPosition().y + 436);
	searchStringText.setCharacterSize(16);
	searchStringText.setString("nothing");

	maxPlayers = db.getMaxPlayers();
}

bool FilterScreen::update(inputHandle& ih)
{
	if(showKeyboard)
	{
		if(osk.update(ih))
		{
			// returned true, close keyboard and get the search string
			showKeyboard = false;
			menuNav.selected = false;
			searchString = osk.getEnteredText();
			if (!searchString.empty())
				searchStringText.setString(searchString);
			else
				searchStringText.setString("nothing");
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
			else if (menuNav.getCurrentID() == "publisher")
			{
				 if (ih.inputPress(inputHandle::inputs::left))
					 publisherFilter.update(-1);
				 else if (ih.inputPress(inputHandle::inputs::right))
					 publisherFilter.update(1);
			}
			else if (menuNav.getCurrentID() == "region") 
			{
				 if (ih.inputPress(inputHandle::inputs::left))
					 regionFilter.update(-1);
				 else if (ih.inputPress(inputHandle::inputs::right))
					 regionFilter.update(1);
			}
			else if (menuNav.getCurrentID() == "genre") 
			{
				 if (ih.inputPress(inputHandle::inputs::left))
					 genreFilter.update(-1);
				 else if (ih.inputPress(inputHandle::inputs::right))
					 genreFilter.update(1);
			}
			else if (menuNav.getCurrentID() == "players") 
			{
				 if (ih.inputPress(inputHandle::inputs::left))
					 players--;
				 else if (ih.inputPress(inputHandle::inputs::right))
					 players++;

				 if (players < 0)
					 players = 0;
				 if (players > maxPlayers)
					 players = maxPlayers;
			}
			else if (menuNav.getCurrentID() == "co_op")
			{
				if (ih.inputPress(inputHandle::inputs::left))
					co_op--;
				else if (ih.inputPress(inputHandle::inputs::right))
					co_op++;
				
				// -1 No Filter, 1 True, 0 False
				// Lock it to -1 to 1 
				if (co_op < -1)
					co_op = -1;
				if (co_op > 1)
					co_op = 1;
			}
			else if (menuNav.getCurrentID() == "favourite")
			{
				if (ih.inputPress(inputHandle::inputs::left))
					favourite--;
				else if (ih.inputPress(inputHandle::inputs::right))
					favourite++;
				
				// -1 No Filter, 1 True, 0 False
				// Lock it to -1 to 1 
				if (favourite < -1)
					favourite = -1;
				if (favourite > 1)
					favourite = 1;
			}
			else if (menuNav.getCurrentID() == "year")
			{
				if (ih.inputPress(inputHandle::inputs::left))
					year_operation--;
				if (ih.inputPress(inputHandle::inputs::right))
					year_operation++;
				if (year_operation < -1)
					year_operation = -1;
				if (year_operation > 3)
					year_operation = 3;

				if (ih.inputPress(inputHandle::inputs::up) && year_operation != -1)
					year++;
				if (ih.inputPress(inputHandle::inputs::down) && year_operation != -1)
					year--;
			}
			else if (menuNav.getCurrentID() == "stars")
			{
				if (userSelected)
				{				
					if (ih.inputPress(inputHandle::inputs::left))
						userStars -= 0.5;
					if (ih.inputPress(inputHandle::inputs::right))
						userStars += 0.5;
					if (userStars < -0.5)
						userStars = -0.5;
					if (userStars > 5)
						userStars = 5;
				}
				else
				{
					if (ih.inputPress(inputHandle::inputs::left))
						onlineStars -= 0.5;
					if (ih.inputPress(inputHandle::inputs::right))
						onlineStars += 0.5;
					if (onlineStars < -0.5)
						onlineStars = -0.5;
					if (onlineStars > 5)
						onlineStars = 5;
				}
				if (ih.inputPress(inputHandle::inputs::up))
					userSelected = true;
				else if (ih.inputPress(inputHandle::inputs::down))
					userSelected = false;
			}

			else if (menuNav.getCurrentID() == "search")
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
		pointerSprite.setTexture(ah.getTextureAsset("POINTER"));
		pointerSprite.setOrigin(32,12);
		pointerSprite.setPosition(menuNav.getCurrentPosX(), menuNav.getCurrentPosY());
		window.draw(pointerSprite);
	}

// Players Filter
	for(int i = 0; i < players ; i++)
	{
		sf::Vector2i playerPos(menuNav.getPosX("players"),menuNav.getPosY("players"));
		sf::Sprite playerSprite;
		playerSprite.setTexture(ah.getTextureAsset("PLAYERS"));
		playerSprite.setPosition(playerPos.x + 25 + (playerSprite.getLocalBounds().width * i),playerPos.y + 40);
		window.draw(playerSprite);
	}
	if (players == 0)
	{
		largeText.setString("  All\nPlayers");
		largeText.setPosition(menuNav.getPosX("players") + 10,menuNav.getPosY("players") + 40);
		window.draw(largeText);
	}

// Co-Op Filter
	if (co_op == -1)
	{
		largeText.setString("No Filter");
		largeText.setPosition(menuNav.getPosX("co_op"),menuNav.getPosY("co_op") + 40);
	}
	else if (co_op == 1)
	{
		largeText.setString("Co-Op");
		largeText.setPosition(menuNav.getPosX("co_op") + 10,menuNav.getPosY("co_op") + 40);	
	}
	else
	{
		largeText.setString("Not Co-Op");
		largeText.setPosition(menuNav.getPosX("co_op"),menuNav.getPosY("co_op") + 40);
	}
	window.draw(largeText);

// Favourite Filter
	if (favourite == -1)
	{
		largeText.setString("No Filter");
		largeText.setPosition(menuNav.getPosX("favourite"),menuNav.getPosY("favourite") + 40);
	}
	else if (favourite == 1)
	{
		largeText.setString("Only\nFavourites");
		largeText.setPosition(menuNav.getPosX("favourite") + 10,menuNav.getPosY("favourite") + 40);	
	}
	else
	{
		largeText.setString("Not\nFavourites");
		largeText.setPosition(menuNav.getPosX("favourite"),menuNav.getPosY("favourite") + 40);
	}
	window.draw(largeText);

// Year Filter
	if (year_operation == -1)
	{
		largeText.setString("No Filter");
		largeText.setPosition(menuNav.getPosX("year"),menuNav.getPosY("year") + 40);
	}
	else if (year_operation == 0)
	{
		largeText.setString("Greater Then");
		largeText.setPosition(menuNav.getPosX("year"),menuNav.getPosY("year") + 40);
	}
	else if (year_operation == 1)
	{
		largeText.setString("Less Then");
		largeText.setPosition(menuNav.getPosX("year"),menuNav.getPosY("year") + 40);
	}
	else if (year_operation == 2)
	{
		largeText.setString("Equals");
		largeText.setPosition(menuNav.getPosX("year"),menuNav.getPosY("year") + 40);
	}
	else if (year_operation == 3)
	{
		largeText.setString("Not Equal To");
		largeText.setPosition(menuNav.getPosX("year"),menuNav.getPosY("year") + 40);
	}
	window.draw(largeText);
	if (year_operation != -1)
	{
		largeText.setString(std::to_string(year));
		largeText.setPosition(menuNav.getPosX("year"),menuNav.getPosY("year") + 60);
		window.draw(largeText);
	}

// Draw Stars
	if (userStars > -0.5)
		ah.draw5_Stars(userStars, sf::Color::Color(246,235,20,255), menuNav.getPosX("stars")+10, menuNav.getPosY("stars")+40, window);
	else
	{
		largeText.setString("No Filter");
		largeText.setPosition(menuNav.getPosX("stars")+10, menuNav.getPosY("stars")+40);
		window.draw(largeText);
	}
	if(onlineStars > -0.5)
		ah.draw5_Stars(onlineStars, sf::Color::Color(105,205,255,255), menuNav.getPosX("stars")+10, menuNav.getPosY("stars")+60, window);
	else
	{
		largeText.setString("No Filter");
		largeText.setPosition(menuNav.getPosX("stars")+10, menuNav.getPosY("stars")+60);
		window.draw(largeText);
	}

//fliter lists
	developerFilter.draw(window); //Draw the developer filter list.
	publisherFilter.draw(window);
	regionFilter.draw(window);
	genreFilter.draw(window);
	window.draw(searchStringText);
	if(showKeyboard)
	{
		osk.draw(window);
	}
}