#include "FilterScreen.h"


FilterScreen::FilterScreen(dbHandle &db_ref, assetHandle &ah_ref):
	db(db_ref),
	ah(ah_ref),
	osk(db_ref, ah_ref),
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
	userStarsSelected = false;

	filterDescription = "";
}


FilterScreen::~FilterScreen(void)
{
}

void FilterScreen::updateFilterString()
{
	filterDescription = "";

	filterString = regionFilter.getFilterString() + genreFilter.getFilterString();

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
	menuNav.addItem("titlelike", "" , "", "", "developer", "textInput", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 16);
	menuNav.addItem("developer", "", "", "titlelike", "region", "textInput", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 48);
	menuNav.addItem("region","" , "", "developer", "genre", "filterList", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 80);
	menuNav.addItem("genre", "" , "", "region", "players", "filterList", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 112);
	menuNav.addItem("players", "" , "", "genre", "co_op", "number", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 144);
	menuNav.addItem("co_op", "" , "", "players", "rating", "bool", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 176);
	menuNav.addItem("rating", "" , "", "co_op", "favourite", "filterList", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 208);
	menuNav.addItem("favourite", "" , "", "rating", "stars", "bool", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 240);
	menuNav.addItem("stars", "" , "", "favourite", "year", "number", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 272);
	menuNav.addItem("year", "" , "","stars", "cancel", "year", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 304);
	menuNav.addItem("cancel", "" , "", "year", "apply", "button", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 336);
	menuNav.addItem("apply", "" , "", "cancel", "", "button", "none", menuRect.getPosition().x + 16, menuRect.getPosition().y + 368);

	//screenTexture_p = new sf::Texture();
	//screenTexture_p->loadFromFile(db.exe_path + "\\assets\\system\\filterMenu.png");
	//pointerTexture.loadFromFile();
	//menuRect.setTexture(screenTexture_p);
	menuRect.setFillColor(sf::Color::Color(46,76,106,255));

	osk.init(300, 500);

	std::vector<dbHandle::filterListItem> listItems;

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

	fontItem.color = sf::Color::White;
	fontItem.fontName = "ARCADE_PIX.TTF";
	fontItem.size = 18;


	maxPlayers = db.getMaxPlayers();
}

FilterScreen::update_retern FilterScreen::update(inputHandle& ih)
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
			}		
			else if (menuNav.getCurrentID() == "publisher")
			{
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
				if (userStarsSelected)
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
					userStarsSelected = true;
				else if (ih.inputPress(inputHandle::inputs::down))
					userStarsSelected = false;
			}

			else if (menuNav.getCurrentID() == "search")
			{
				showKeyboard = true;
			}

			if (menuNav.getCurrentID() == "apply")
			{
				menuNav.selected = false;
				updateFilterString();
				return update_retern::apply;  // Close the menu now
			}
			if (menuNav.getCurrentID() == "cancel")
			{
				menuNav.selected = false;
				return update_retern::cancel; // Close the menu now
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

	return update_retern::open; // Dont close the menu this update!

}

void FilterScreen::draw(sf::RenderWindow& window)
{
	
	window.draw(menuRect);
	sf::Sprite pointerSprite;
	pointerSprite.setTexture(ah.getTextureAsset("POINTER"));
	pointerSprite.setOrigin(32,1);
	pointerSprite.setPosition(menuNav.getCurrentPosX(), menuNav.getCurrentPosY());
	if(!menuNav.selected) // Nothing is selected yet, show the pointer
		window.draw(pointerSprite);

// Title Like
	fontItem.pos = sf::Vector2f(menuNav.getPosX("titlelike"), menuNav.getPosY("titlelike"));
	ah.drawText("Title Like:",  fontItem, window);

// Developer
	fontItem.pos = sf::Vector2f(menuNav.getPosX("developer"), menuNav.getPosY("developer"));
	ah.drawText("Developer / Publisher:",  fontItem, window);

// Region	
	fontItem.pos = sf::Vector2f(menuNav.getPosX("region"), menuNav.getPosY("region"));
	ah.drawText("Region:",  fontItem, window);

// genre
	fontItem.pos = sf::Vector2f(menuNav.getPosX("genre"), menuNav.getPosY("genre"));
	ah.drawText("Genre:",  fontItem, window);

// Players Filter
	fontItem.pos = sf::Vector2f(menuNav.getPosX("players"), menuNav.getPosY("players"));
	ah.drawText("Number of Players:",  fontItem, window);
	if (players == 0)
		largeText.setString("All Players");
	else if (players == 1)
		largeText.setString(std::to_string(players) + " Player");
	else
		largeText.setString(std::to_string(players) + " Players");
	largeText.setPosition(menuNav.getPosX("players") + 250,menuNav.getPosY("players"));	
	window.draw(largeText);

// Co-Op Filter
	fontItem.pos = sf::Vector2f(menuNav.getPosX("co_op"), menuNav.getPosY("co_op"));
	ah.drawText("Co-Op:", fontItem, window);
	if (co_op == -1)
		largeText.setString("No Filter");		
	else if (co_op == 1)
		largeText.setString("Co-Op");
	else
		largeText.setString("Not Co-Op");
	largeText.setPosition(menuNav.getPosX("co_op") + 100,menuNav.getPosY("co_op"));
	window.draw(largeText);

// Rating
	fontItem.pos = sf::Vector2f(menuNav.getPosX("rating"), menuNav.getPosY("rating"));
	ah.drawText("Rating:", fontItem, window);

// Favourite Filter
	fontItem.pos = sf::Vector2f(menuNav.getPosX("favourite"), menuNav.getPosY("favourite"));
	ah.drawText("Favourites:",  fontItem, window);
	if (favourite == -1)
		largeText.setString("No Filter");
	else if (favourite == 1)
		largeText.setString("Only in Favourites");
	else
		largeText.setString("Not in Favourites");
	largeText.setPosition(menuNav.getPosX("favourite") +150 ,menuNav.getPosY("favourite"));
	window.draw(largeText);
	if (menuNav.selected && menuNav.getCurrentID()=="favourite"){
		pointerSprite.setPosition(menuNav.getPosX("favourite") +150, menuNav.getPosY("favourite"));
		window.draw(pointerSprite);
	}

// Year Filter
	fontItem.pos = sf::Vector2f(menuNav.getPosX("year"), menuNav.getPosY("year"));
	ah.drawText("Year",  fontItem, window);
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
	fontItem.pos = sf::Vector2f(menuNav.getPosX("stars"), menuNav.getPosY("stars"));
	ah.drawText("Stars:",  fontItem, window);
	if (userStars > -0.5)
		ah.draw5_Stars(userStars, sf::Color::Color(246,235,20,255), menuNav.getPosX("stars") + 100, menuNav.getPosY("stars")-8, window);
	else {
		largeText.setString("No Filter");
		largeText.setPosition(menuNav.getPosX("stars") + 100, menuNav.getPosY("stars")-8);
		window.draw(largeText);
	}
	if(onlineStars > -0.5)
		ah.draw5_Stars(onlineStars, sf::Color::Color(105,205,255,255), menuNav.getPosX("stars") + 100, menuNav.getPosY("stars")+8, window);
	else {
		largeText.setString("No Filter");
		largeText.setPosition(menuNav.getPosX("stars") + 100, menuNav.getPosY("stars") + 8);
		window.draw(largeText);
	}

	if (!userStarsSelected && menuNav.selected && menuNav.getCurrentID()=="stars"){
		pointerSprite.setPosition(menuNav.getPosX("stars") + 100, menuNav.getPosY("stars") + 8);
		window.draw(pointerSprite);
	}
	else if (userStarsSelected && menuNav.selected && menuNav.getCurrentID()=="stars"){
		pointerSprite.setPosition(menuNav.getPosX("stars") + 100, menuNav.getPosY("stars") - 8);
		window.draw(pointerSprite);
	}

//fliter lists
	//regionFilter.draw(window);
	//genreFilter.draw(window);
	window.draw(searchStringText);
	if(showKeyboard)
	{
		osk.draw(window);
	}
}