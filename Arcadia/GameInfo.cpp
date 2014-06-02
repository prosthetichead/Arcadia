#include "GameInfo.h"


GameInfo::GameInfo(dbHandle &db_ref, assetHandle &ah_ref):db(db_ref), ah(ah_ref)
{
	descriptionOffset = 0;
	resetOffset = false;
	descriptionScroll = false;
	descriptionPause = false;
	descriptionPauseTime = 500;
	descriptionPauseCount = 0;
}


GameInfo::~GameInfo(void)
{	
}

void GameInfo::init(float posX, float posY, int width, float height)
{
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

	hasMovieFile = false;

	rectangleInfo.setSize(sf::Vector2f(width,height));
	rectangleInfo.setPosition(posX,posY);

	rectangleFanArt.setSize(sf::Vector2f(desktopMode.width, desktopMode.height));
	rectangleFanArt.setPosition(0, 0);
	rectangleFanArt.setFillColor(sf::Color::Color(0,0,0,210));
	rectangleFanArt.setOutlineColor(sf::Color::White);
	rectangleFanArt.setOutlineThickness(0);

	moviePostion.x = rectangleInfo.getPosition().x + rectangleInfo.getSize().x/2;
	moviePostion.y = rectangleInfo.getSize().y/2;

	movieBorder.setSize(sf::Vector2f(640 + 10, 480 + 10));
	movieBorder.setOrigin(movieBorder.getLocalBounds().width/2, movieBorder.getLocalBounds().height/2);
	movieBorder.setPosition(moviePostion.x, moviePostion.y);
	movieBorder.setOutlineColor(sf::Color::Color(0,102,153,255));
	movieBorder.setFillColor(sf::Color::Black);
	movieBorder.setOutlineThickness(4);

	gameIconsBorder.setSize(sf::Vector2f(640,  75));
	gameIconsBorder.setPosition(moviePostion.x - (movieBorder.getSize().x/2), moviePostion.y - (movieBorder.getSize().y/2) - gameIconsBorder.getSize().y);
	gameIconsBorder.setOrigin(0,0);
	gameIconsBorder.setFillColor(sf::Color::Color(0,0,0,150));
	gameIconsBorder.setOutlineColor(sf::Color::Color(0,0,0,200));
	gameIconsBorder.setOutlineThickness(1);

	descriptionBorder.setSize(sf::Vector2f(640,240));
	descriptionBorder.setPosition(moviePostion.x - (movieBorder.getSize().x/2), moviePostion.y + (movieBorder.getSize().y/2) + gameIconsBorder.getSize().y);
	descriptionBorder.setOutlineThickness(1);
	descriptionView.reset(sf::FloatRect(0, 0, desktopMode.width, desktopMode.height));
	descriptionView.setViewport(sf::FloatRect(descriptionBorder.getPosition().x / desktopMode.width, descriptionBorder.getPosition().y / desktopMode.height, 1, 1));
	
	descriptionFont.loadFromFile(db.exe_path + "\\assets\\fonts\\ARCADE_PIX.TTF");
	descriptionFontSize = 18;
	descriptionText.setFont(descriptionFont);
	descriptionText.setCharacterSize(descriptionFontSize);

	yearFont.loadFromFile(db.exe_path + "\\assets\\fonts\\ARCADE_PIX.TTF");
	yearFontSize = 24;
	yearText.setFont(yearFont);

	
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

void GameInfo::update()
{
	if (hasMovieFile && movie->getStatus() == movie->Playing)
		movie->update();
}

void GameInfo::newGameInfo(dbHandle::gameListItem gameItem)
{
	currentGameItem = gameItem;
	currentGameInfo = db.getGameInfo(gameItem);

	//recreate movie
	delete movie;
	movie = new sfe::Movie;  
	hasMovieFile = false;
	if (currentGameInfo.videoPath != "NULL") // Load movie file if one exists
	{
		hasMovieFile = true;
		movie->openFromFile(currentGameInfo.videoPath);
		movie->resizeToFrame(moviePostion.x , moviePostion.y, 640,480,true);
		movie->setOrigin(movie->getSize().x/2, movie->getSize().y/2);

		movie->play();
	}
	// Fan Art Texture
	if (currentGameInfo.fanArtPath != "NULL")
		fanArt.loadFromFile(currentGameInfo.fanArtPath);
	else
		fanArt.create(1,1); // Create and empty Texture
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
	std::string new_str;
    std::vector<std::string> tokens;
    boost::split(tokens, currentGameInfo.description, boost::is_any_of(" "));
	int lineLengthPixels = descriptionBorder.getSize().x;  // / descriptionFontSize;
	int currentLineLength = 0;

	sf::Text tempText;
	tempText.setFont(descriptionFont);
	tempText.setCharacterSize(descriptionFontSize);
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
	descriptionView.reset(sf::FloatRect(0, 0, rectangleFanArt.getSize().x, rectangleFanArt.getSize().y)); //Reset the description view (uses retangle fan art to get full screen size)
	descriptionOffset = 0;
	resetOffset = false;
	if (descriptionText.getLocalBounds().height > descriptionBorder.getSize().y)
	{
		descriptionRequiredOffset = descriptionText.getLocalBounds().height - descriptionBorder.getSize().y;
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

void GameInfo::draw(sf::RenderWindow& window)
{


	sf::Sprite spriteFanArt;
	spriteFanArt.setTexture(fanArt);
	spriteFanArt.setOrigin(spriteFanArt.getLocalBounds().width/2, spriteFanArt.getLocalBounds().height/2);
	spriteFanArt.setPosition(rectangleFanArt.getSize().x/2, rectangleFanArt.getSize().y/2);
	sf::Vector2i newSize = resizePreserveRatio(spriteFanArt.getLocalBounds().width, spriteFanArt.getLocalBounds().height, rectangleFanArt.getSize().x, rectangleFanArt.getSize().y, false);
	spriteFanArt.setScale(newSize.x/spriteFanArt.getLocalBounds().width, newSize.y/spriteFanArt.getLocalBounds().height);

	window.draw(spriteFanArt);
	window.draw(rectangleFanArt);
	//window.draw(gameIconsBorder);
	//window.draw(descriptionBorder);
	
	 // Set back to original view

	sf::Sprite spriteClearLogo;
	spriteClearLogo.setTexture(clearLogo);
	spriteClearLogo.setOrigin(spriteClearLogo.getGlobalBounds().width/2, 0); // Set origin to top center
	spriteClearLogo.setPosition(rectangleInfo.getPosition().x + rectangleInfo.getSize().x/2, 10); 
	window.draw(spriteClearLogo);
	
	sf::Sprite spriteDeveloper;
	spriteDeveloper.setTexture(ah.getTextureAsset(currentGameInfo.developerIconID));
	spriteDeveloper.setOrigin(spriteDeveloper.getLocalBounds().width/2, spriteDeveloper.getLocalBounds().height/2);  //Set Origin to center
	spriteDeveloper.setPosition(gameIconsBorder.getPosition().x + gameIconsBorder.getSize().x/2, gameIconsBorder.getPosition().y + gameIconsBorder.getSize().y/2);
	if (currentGameInfo.developerIconID != "ERROR") // icon id of error means it does not have an icon so we should show the text insted.
		window.draw(spriteDeveloper);

	sf::Sprite spriteGenre;
	spriteGenre.setTexture(ah.getTextureAsset(currentGameInfo.genreIconID));
	spriteGenre.setPosition(gameIconsBorder.getPosition().x,gameIconsBorder.getPosition().y);
	if (currentGameInfo.genreIconID != "ERROR")
		window.draw(spriteGenre);

	sf::Sprite spritePlayer;
	spritePlayer.setTexture(ah.getTextureAsset("PLAYERS"));
	for(int i=0; i < currentGameInfo.players; i++)
	{
		spritePlayer.setPosition(gameIconsBorder.getPosition().x + gameIconsBorder.getSize().x - ((spritePlayer.getLocalBounds().width+2) * i), gameIconsBorder.getPosition().y + 25);
		window.draw(spritePlayer);
	}

	if (hasMovieFile)
	{		
		window.draw(movieBorder);
		window.draw(*movie);
	}
	else if (currentGameInfo.screenPath != "NULL")
	{
		sf::Sprite spriteScreenShot;
		spriteScreenShot.setTexture(screenShot);
		spriteScreenShot.setPosition(moviePostion.x , moviePostion.y);	
		sf::Vector2i new_size = resizePreserveRatio(spriteScreenShot.getLocalBounds().width, spriteScreenShot.getLocalBounds().height, 640, 480);
		spriteScreenShot.setScale(new_size.x/spriteScreenShot.getLocalBounds().width, new_size.y/spriteScreenShot.getLocalBounds().height);
		spriteScreenShot.setOrigin(spriteScreenShot.getLocalBounds().width/2, spriteScreenShot.getLocalBounds().height/2);

		window.draw(movieBorder);
		window.draw(spriteScreenShot);
	}

	if (descriptionScroll)
	{
		if (!descriptionPause)
		{
			if (descriptionOffset < descriptionRequiredOffset && !resetOffset)
			{
				descriptionView.move(0, .1);
				descriptionOffset += .1;
			}
			else if (descriptionOffset >= descriptionRequiredOffset && !resetOffset)
			{
				resetOffset = true;
				descriptionPause = true;
				descriptionPauseCount = 0;
				descriptionView.move(0, -.1);
				descriptionOffset -= .1;
			}
			else if (resetOffset && descriptionOffset > 0)
			{
				descriptionView.move(0, -.1);
				descriptionOffset -= .1;
			}
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



	window.setView(descriptionView);
	descriptionText.setPosition(0,0);//descriptionBorder.getPosition().x, descriptionBorder.getPosition().y); 
	window.draw(descriptionText);
	window.setView(window.getDefaultView());

}


sf::Vector2i GameInfo::resizePreserveRatio(int org_width, int org_height, int new_width, int new_height, bool fit)
{
	sf::Vector2i original_size = sf::Vector2i(org_width, org_height);
	sf::Vector2i wanted_size = sf::Vector2i(new_width, new_height);
	sf::Vector2i new_size;

	float org_ratio = (float)original_size.x / original_size.y;
	float new_ratio = (float)wanted_size.x / wanted_size.y;

	if (org_ratio > new_ratio && fit)
	{
		new_size.x = original_size.x * ((float)wanted_size.x / original_size.x);
		new_size.y = original_size.y * ((float)wanted_size.x / original_size.x);
	}
	else
	{
		new_size.x = original_size.x * ((float)wanted_size.y / original_size.y);
		new_size.y = original_size.y * ((float)wanted_size.y / original_size.y);
	}

	return new_size;
}
