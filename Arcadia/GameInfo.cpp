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

	gameIconsBorderTop.setSize(sf::Vector2f(640,  75));
	gameIconsBorderTop.setPosition(moviePostion.x - (movieBorder.getSize().x/2), moviePostion.y - (movieBorder.getSize().y/2) - gameIconsBorderTop.getSize().y);
	gameIconsBorderTop.setOrigin(0,0);
	gameIconsBorderTop.setFillColor(sf::Color::Color(0,0,0,150));
	gameIconsBorderTop.setOutlineColor(sf::Color::Color(0,0,0,200));
	gameIconsBorderTop.setOutlineThickness(1);
	gameIconsBorderBottom = gameIconsBorderTop;
	gameIconsBorderBottom.setPosition(moviePostion.x - (movieBorder.getSize().x/2), moviePostion.y + (movieBorder.getSize().y/2));

	descriptionBorder.setSize(sf::Vector2f(640,240));
	descriptionBorder.setPosition(moviePostion.x - (movieBorder.getSize().x/2), moviePostion.y + (movieBorder.getSize().y/2) + gameIconsBorderTop.getSize().y);
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
	if (currentGameInfo.description.empty())
		descriptionText.setString("No Description Available in Database");
	else
	{
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
	
}

void GameInfo::draw(sf::RenderWindow& window)
{


	sf::Sprite spriteFanArt;
	spriteFanArt.setTexture(fanArt);
	spriteFanArt.setOrigin(spriteFanArt.getLocalBounds().width/2, spriteFanArt.getLocalBounds().height/2);
	spriteFanArt.setPosition(rectangleFanArt.getSize().x/2, rectangleFanArt.getSize().y/2);
	sf::Vector2i newSize = ah.resizePreserveRatio(spriteFanArt.getLocalBounds().width, spriteFanArt.getLocalBounds().height, rectangleFanArt.getSize().x, rectangleFanArt.getSize().y, false);
	spriteFanArt.setScale(newSize.x/spriteFanArt.getLocalBounds().width, newSize.y/spriteFanArt.getLocalBounds().height);

	window.draw(spriteFanArt);
	window.draw(rectangleFanArt);
	//window.draw(gameIconsBorderTop);
	//window.draw(descriptionBorder);
	
	 // Set back to original view

	sf::Sprite spriteClearLogo;
	spriteClearLogo.setTexture(clearLogo);
	spriteClearLogo.setOrigin(spriteClearLogo.getGlobalBounds().width/2, 0); // Set origin to top center
	spriteClearLogo.setPosition(rectangleInfo.getPosition().x + rectangleInfo.getSize().x/2, 10); 
	window.draw(spriteClearLogo);
	
	sf::Sprite spriteDeveloper;
	spriteDeveloper.setTexture(ah.getTextureAsset(currentGameInfo.developerIconID));
	spriteDeveloper.setOrigin(spriteDeveloper.getLocalBounds().width/2, spriteDeveloper.getLocalBounds().height);  //Set Origin to bottom center
	spriteDeveloper.setPosition(gameIconsBorderTop.getPosition().x + gameIconsBorderTop.getSize().x/2, gameIconsBorderTop.getPosition().y + gameIconsBorderTop.getSize().y - 10);
	if (currentGameInfo.developerIconID != "ERROR") // icon id of error means it does not have an icon so we should show the text insted.
		window.draw(spriteDeveloper);

	sf::Sprite spriteGenre;
	spriteGenre.setTexture(ah.getTextureAsset(currentGameInfo.genreIconID));
	spriteGenre.setPosition(gameIconsBorderTop.getPosition().x,gameIconsBorderTop.getPosition().y);
	if (currentGameInfo.genreIconID != "ERROR")
		window.draw(spriteGenre);

	sf::Sprite spritePlayer;
	spritePlayer.setTexture(ah.getTextureAsset("PLAYERS"));
	for(int i=0; i < currentGameInfo.players; i++)
	{
		spritePlayer.setPosition(gameIconsBorderTop.getPosition().x + gameIconsBorderTop.getSize().x - ((spritePlayer.getLocalBounds().width+2) * i), gameIconsBorderTop.getPosition().y + 25);
		window.draw(spritePlayer);
	}
	
	
	// Draw Stars
	sf::Sprite spriteStarEmpty;
	spriteStarEmpty.setTexture(ah.getTextureAsset("STAR_EMPTY"));
	sf::Sprite spriteStarEmptyHalf;
	spriteStarEmptyHalf.setTexture(ah.getTextureAsset("STAR_EMPTY_HALF"));
	for(int i=0; i < 5; i++)
	{
		spriteStarEmpty.setPosition(gameIconsBorderBottom.getPosition().x + 20*i, gameIconsBorderBottom.getPosition().y+10);
		spriteStarEmpty.setColor(sf::Color::Color(86,86,86,255));
		window.draw(spriteStarEmpty);
		if ((currentGameInfo.user_stars - (i)) == 0.5)
		{
			spriteStarEmptyHalf.setPosition(spriteStarEmpty.getPosition());
			spriteStarEmptyHalf.setColor(sf::Color::Color(246,235,20,255));
			window.draw(spriteStarEmptyHalf);
		}
		if (currentGameInfo.user_stars >= i+1)
		{
			spriteStarEmpty.setColor(sf::Color::Color(246,235,20,255));
			window.draw(spriteStarEmpty);
		}
		

		spriteStarEmpty.setPosition(gameIconsBorderBottom.getPosition().x + 20*i, gameIconsBorderBottom.getPosition().y+30);
		spriteStarEmpty.setColor(sf::Color::Color(86,86,86,255));
		window.draw(spriteStarEmpty);
		if ((currentGameInfo.online_stars - (i)) == 0.5)
		{
			spriteStarEmptyHalf.setPosition(spriteStarEmpty.getPosition());
			spriteStarEmptyHalf.setColor(sf::Color::Color(105,205,255,255));
			window.draw(spriteStarEmptyHalf);
		}
		if (currentGameInfo.online_stars >= i+1)
		{
			spriteStarEmpty.setColor(sf::Color::Color(105,205,255,255));
			window.draw(spriteStarEmpty);
		}
	}

	window.draw(movieBorder);
	if (hasMovieFile)
	{	
		window.draw(*movie);
	}
	else if (currentGameInfo.screenPath != "NULL")
	{
		sf::Sprite spriteScreenShot;
		spriteScreenShot.setTexture(screenShot);
		spriteScreenShot.setPosition(moviePostion.x , moviePostion.y);	
		sf::Vector2i new_size = ah.resizePreserveRatio(spriteScreenShot.getLocalBounds().width, spriteScreenShot.getLocalBounds().height, 640, 480, true);
		spriteScreenShot.setScale(new_size.x/spriteScreenShot.getLocalBounds().width, new_size.y/spriteScreenShot.getLocalBounds().height);
		spriteScreenShot.setOrigin(spriteScreenShot.getLocalBounds().width/2, spriteScreenShot.getLocalBounds().height/2);
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



