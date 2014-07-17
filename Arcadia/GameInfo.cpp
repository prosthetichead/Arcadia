#include "GameInfo.h"


GameInfo::GameInfo(dbHandle* db_ref, assetHandle* ah_ref, SkinHandle* sh_ref)
{
	ah = ah_ref;
	db = db_ref;
	sh = sh_ref;
	descriptionOffset = 0;
	resetOffset = false;
	descriptionScroll = false;
	descriptionPause = false;
	descriptionPauseTime = 50;
	descriptionPauseCount = 0;
	gameChangedCounter = 0;
	hasMovieFile = false;
}


GameInfo::~GameInfo(void)
{	
}

void GameInfo::init()
{
	//sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
	
	//rectangleInfo.setSize(sf::Vector2f(width,height));
	//rectangleInfo.setPosition(posX,posY);
	//float movie_width, movie_height = 0;
	//if (desktopMode.height < 1000)
	//{
		//movie_width = 320;
		//movie_height = 240;
	//}/
	//else
	//{
	//	movie_width = 640;
	//	movie_height = 480;
	//}

	rectangleFanArt.setSize(sh->resolution);
	rectangleFanArt.setPosition(0, 0);
	rectangleFanArt.setFillColor(sh->game_info_settings.fanArt_colour);
	rectangleFanArt.setOutlineThickness(0);

	//moviePostion.x = sh->game_info_settings.video_position;
	//moviePostion.y = rectangleInfo.getSize().y/2;

	//movieBorder.setSize(sf::Vector2f(movie_width + 10, movie_height + 10));
	//movieBorder.setOrigin(movieBorder.getLocalBounds().width/2, movieBorder.getLocalBounds().height/2);
	//movieBorder.setPosition(moviePostion.x, moviePostion.y);
	//movieBorder.setOutlineColor(sf::Color::Color(0,102,153,255));
	//movieBorder.setFillColor(sf::Color::Black);
	//movieBorder.setOutlineThickness(4);

	//gameIconsBorderTop.setSize(sf::Vector2f(movie_width,  75));
	//gameIconsBorderTop.setPosition(moviePostion.x - (movieBorder.getSize().x/2), moviePostion.y - (movieBorder.getSize().y/2) - gameIconsBorderTop.getSize().y);
	//gameIconsBorderTop.setOrigin(0,0);
	//gameIconsBorderTop.setFillColor(sf::Color::Color(0,0,0,150));
	//gameIconsBorderTop.setOutlineColor(sf::Color::Color(0,0,0,200));
	//gameIconsBorderTop.setOutlineThickness(1);
	//gameIconsBorderBottom = gameIconsBorderTop;
	//gameIconsBorderBottom.setPosition(moviePostion.x - (movieBorder.getSize().x/2), moviePostion.y + (movieBorder.getSize().y/2));

	//descriptionBorder.setSize(sf::Vector2f(movie_width,240));
	//descriptionBorder.setPosition(500,600);//moviePostion.x - (movieBorder.getSize().x/2), moviePostion.y + (movieBorder.getSize().y/2) + gameIconsBorderTop.getSize().y);
	//descriptionBorder.setOutlineThickness(1);
	
	scrollingTextTexture.create(sh->game_info_settings.description_size.x, sh->game_info_settings.description_size.y);

	//descriptionFont = 
	//descriptionFontSize = sh->game_info_settings.description_font.size;
	descriptionText.setFont(ah->getFontAsset(sh->game_info_settings.description_font.fontName));
	descriptionText.setCharacterSize(sh->game_info_settings.description_font.size);

	//yearFont.loadFromFile(db->exe_path + "\\assets\\fonts\\ARCADE_PIX.TTF");
	yearText.setFont(ah->getFontAsset(sh->game_info_settings.year_font.fontName));
	yearText.setCharacterSize(sh->game_info_settings.year_font.size);

	//playTimeFont.loadFromFile(db->exe_path + "\\assets\\fonts\\ARCADE_PIX.TTF");
	playTimeText.setFont(ah->getFontAsset(sh->game_info_settings.playTime_font.fontName));
	playTimeText.setCharacterSize(sh->game_info_settings.playTime_font.size);
	
	movie = new sfe::Movie;
}

void GameInfo::pauseMovie()
{
	movieStatus = movie->getStatus();
	if (hasMovieFile)
	{
		if (movieStatus == movie->Playing)
			movie->pause();
		if (movieStatus == movie->Paused)
			movie->play();
	}
}

void GameInfo::update(dbHandle::gameListItem gameItem)
{
	if (newGameItem.fileName != gameItem.fileName ||  newGameItem.platformID != gameItem.platformID)
	{
		newGameItem = gameItem;
		gameChangedCounter = 0;
	}
	else
	{
		gameChangedCounter += 1;
	}

	if (gameChangedCounter >= 20 && newGameItem.fileName != currentGameItem.fileName)
	{
		gameChangedCounter = 0;
		newGameInfo(gameItem);
	}
		

	if (hasMovieFile && movie->getStatus() == movie->Playing)
		movie->update();
}

void GameInfo::newGameInfo(dbHandle::gameListItem gameItem)
{
	currentGameItem = gameItem;
	currentGameInfo = db->getGameInfo(gameItem);

	//recreate movie
	delete movie;
	movie = new sfe::Movie;  
	hasMovieFile = false;
	if (currentGameInfo.videoPath != "NULL") // Load movie file if one exists
	{
		hasMovieFile = true;
		movie->openFromFile(currentGameInfo.videoPath);
		movie->resizeToFrame(sh->game_info_settings.video_position.x , sh->game_info_settings.video_position.y, sh->game_info_settings.video_size.x, sh->game_info_settings.video_size.y,true);
		//movie->setOrigin(movie->getSize().x/2, movie->getSize().y/2);

		movie->play();
	}
	// Fan Art Texture
	fanArt.create(1,1); // Create and empty Texture
	if (currentGameInfo.fanArtPath != "NULL")
		fanArt.loadFromFile(currentGameInfo.fanArtPath);

	// Clear Logo Texture
	if (currentGameInfo.clearLogoPath != "NULL")
		clearLogo.loadFromFile(currentGameInfo.clearLogoPath);
	else 
		clearLogo.create(1,1); // Create and empty Texture	

	// screen shot texture
	if (currentGameInfo.screenPath != "NULL")
		screenShot.loadFromFile(currentGameInfo.screenPath);
	else
		screenShot.create(1,1); // Create and empty Texture	

	// Description Text
	if (currentGameInfo.description.empty())
		descriptionText.setString("No Description Available in Database");
	else
	{
		std::string new_str;
		std::vector<std::string> tokens;
		boost::split(tokens, currentGameInfo.description, boost::is_any_of(" "));
		int lineLengthPixels = sh->game_info_settings.description_size.x;  // / descriptionFontSize;
		int currentLineLength = 0;

		sf::Text tempText = descriptionText;
		for(std::string& word: tokens)
		{
			boost::replace_all(word, "\n", "");
			word = word + " ";
			tempText.setString(word);
			int wordLengthPixels = tempText.getLocalBounds().width;
			if (currentLineLength + wordLengthPixels <= lineLengthPixels)
			{   // its ok write the word to the current line
				new_str = new_str + word;
				currentLineLength += wordLengthPixels;
			}
			else
			{
				new_str = new_str + "\n" + word;
				currentLineLength = wordLengthPixels;
			}
		}
		descriptionText.setString(new_str);  //currentGameInfo.description);
		//descriptionView.reset(sf::FloatRect(0, 0, rectangleFanArt.getSize().x, rectangleFanArt.getSize().y)); //Reset the description view (uses retangle fan art to get full screen size)
		descriptionOffset = 0;
		resetOffset = false;
		if (descriptionText.getLocalBounds().height > sh->game_info_settings.description_size.y)
		{
			descriptionRequiredOffset = descriptionText.getLocalBounds().height - sh->game_info_settings.description_size.y;
			descriptionScroll = true;
			descriptionPause  = true;
		}
		else
		{
			descriptionRequiredOffset = 0;
			descriptionScroll = false;
			descriptionPause  = true;
		}
	}
	
}

void GameInfo::draw(sf::RenderWindow& window)
{


	sf::Sprite spriteFanArt;
	spriteFanArt.setTexture(fanArt);
	spriteFanArt.setOrigin(spriteFanArt.getLocalBounds().width/2, spriteFanArt.getLocalBounds().height/2);
	spriteFanArt.setPosition(rectangleFanArt.getSize().x/2, rectangleFanArt.getSize().y/2);
	sf::Vector2i newSize = ah->resizePreserveRatio(spriteFanArt.getLocalBounds().width, spriteFanArt.getLocalBounds().height, rectangleFanArt.getSize().x, rectangleFanArt.getSize().y, false);
	spriteFanArt.setScale(newSize.x/spriteFanArt.getLocalBounds().width, newSize.y/spriteFanArt.getLocalBounds().height);

	window.draw(spriteFanArt);
	window.draw(rectangleFanArt);
	//window.draw(gameIconsBorderTop);
	//window.draw(descriptionBorder);
	
	 // Set back to original view

	sf::Sprite spriteClearLogo;
	spriteClearLogo.setTexture(clearLogo);
	//spriteClearLogo.setOrigin(spriteClearLogo.getGlobalBounds().width/2, 0); // Set origin to top center
	spriteClearLogo.setPosition(sh->game_info_settings.clearLogo_position); 
	window.draw(spriteClearLogo);
	
	sf::Sprite spriteDeveloper;
	spriteDeveloper.setTexture(ah->getTextureAsset(currentGameInfo.developerIconID));
	//spriteDeveloper.setOrigin(spriteDeveloper.getLocalBounds().width/2, spriteDeveloper.getLocalBounds().height);  //Set Origin to bottom center
	spriteDeveloper.setPosition(sh->game_info_settings.companyLogos_position);
	if (currentGameInfo.developerIconID != "ERROR") // icon id of error means it does not have an icon so we should show the text insted.
		window.draw(spriteDeveloper);

	sf::Sprite spriteGenre;
	spriteGenre.setTexture(ah->getTextureAsset(currentGameInfo.genreIconID));
	spriteGenre.setPosition(sh->game_info_settings.genreIcon_position);
	newSize = ah->resizePreserveRatio(spriteGenre.getLocalBounds().width, spriteGenre.getLocalBounds().height, sh->game_info_settings.genreIcon_size.x, sh->game_info_settings.genreIcon_size.y, true);
	spriteGenre.setScale(newSize.x/spriteGenre.getLocalBounds().width, newSize.y/spriteGenre.getLocalBounds().height);
	if (currentGameInfo.genreIconID != "ERROR")
		window.draw(spriteGenre);

	sf::Sprite spritePlatform;
	spritePlatform.setTexture(ah->getTextureAsset(currentGameInfo.platformIconID));
	newSize = ah->resizePreserveRatio(spritePlatform.getLocalBounds().width, spritePlatform.getLocalBounds().height, sh->game_info_settings.platformIcon_size.x, sh->game_info_settings.platformIcon_size.y, true);
	spritePlatform.setScale(newSize.x/spritePlatform.getLocalBounds().width, newSize.y/spritePlatform.getLocalBounds().height);
	spritePlatform.setOrigin(0, spritePlatform.getLocalBounds().height);
	spritePlatform.setPosition(sh->game_info_settings.platformIcon_position);
	window.draw(spritePlatform);

	sf::Sprite spritePlayer;
	spritePlayer.setTexture(ah->getTextureAsset("PLAYERS"));
	for(int i=0; i < currentGameInfo.players; i++)
	{
		spritePlayer.setPosition(sh->game_info_settings.players_position.x * i, sh->game_info_settings.players_position.y);
		window.draw(spritePlayer);
	}

	// Draw Stars
	//ah->draw5_Stars(currentGameInfo.user_stars, sf::Color::Color(246,235,20,255), gameIconsBorderBottom.getPosition().x, gameIconsBorderBottom.getPosition().y+10, window);
	//ah->draw5_Stars(currentGameInfo.online_stars, sf::Color::Color(105,205,255,255), gameIconsBorderBottom.getPosition().x, gameIconsBorderBottom.getPosition().y+30, window);
	
	//playtime infomation.
	playTimeText.setColor(sf::Color::Color(0,102,153,255));
	playTimeText.setString("You've Played");	
	playTimeText.setPosition(sh->game_info_settings.playTimeTitle_position);
	window.draw(playTimeText);
	playTimeText.setColor(sf::Color::White);
	playTimeText.setString(currentGameInfo.playTime);
	playTimeText.setPosition(sh->game_info_settings.playTime_position);
	window.draw(playTimeText);

	playTimeText.setColor(sf::Color::Color(0,102,153,255));
	playTimeText.setString("Last Played");
	playTimeText.setPosition(sh->game_info_settings.playTimeTitle_position);
	window.draw(playTimeText);
	playTimeText.setColor(sf::Color::White);
	playTimeText.setString(currentGameInfo.lastPlayed);
	playTimeText.setPosition(sh->game_info_settings.playTimeTitle_position);
	window.draw(playTimeText);

	//draw year information
	yearText.setString(currentGameInfo.release_year);
	yearText.setPosition(sh->game_info_settings.year_position);
	window.draw(yearText);

	//window.draw(movieBorder);
	if (hasMovieFile)
	{	
		window.draw(*movie);
	}
	else if (currentGameInfo.screenPath != "NULL")
	{
		sf::Sprite spriteScreenShot;
		spriteScreenShot.setTexture(screenShot);
		spriteScreenShot.setPosition(sh->game_info_settings.screenshot_position);	
		sf::Vector2i new_size = ah->resizePreserveRatio(spriteScreenShot.getLocalBounds().width, spriteScreenShot.getLocalBounds().height, 640, 480, true);
		spriteScreenShot.setScale(new_size.x/spriteScreenShot.getLocalBounds().width, new_size.y/spriteScreenShot.getLocalBounds().height);
		spriteScreenShot.setOrigin(spriteScreenShot.getLocalBounds().width/2, spriteScreenShot.getLocalBounds().height/2);
		window.draw(spriteScreenShot);
	}
	
	scrollingTextTexture.clear(sf::Color::Transparent);
	if (descriptionScroll)
	{
		if (!descriptionPause)
		{
			if (descriptionOffset < descriptionRequiredOffset && !resetOffset)
				descriptionOffset += .1;
			else if (descriptionOffset >= descriptionRequiredOffset && !resetOffset)
			{
				resetOffset = true;
				descriptionPause = true;
				descriptionPauseCount = 0;
				descriptionOffset -= .1;
			}
			else if (resetOffset && descriptionOffset > 0)
				descriptionOffset -= .1;
			else if (resetOffset &&  descriptionOffset <= 0)
			{
				resetOffset = false;
				descriptionPause = true;
				descriptionPauseCount = 0;
			}
		}
		else
		{
			descriptionPauseCount++;
			if (descriptionPauseCount >= descriptionPauseTime)
				descriptionPause = false;
		}
		
	}
	descriptionText.setPosition(0,0 - descriptionOffset);
	scrollingTextTexture.draw(descriptionText);
	scrollingTextTexture.display();
	sf::Sprite scrollingTextSprite(scrollingTextTexture.getTexture());
	scrollingTextSprite.setPosition(sh->game_info_settings.description_position);
	window.draw(scrollingTextSprite);
}



