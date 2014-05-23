#include "GameInfo.h"


GameInfo::GameInfo(dbHandle &db_ref, assetHandle &ah_ref):db(db_ref), ah(ah_ref)
{
	
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
	rectangleFanArt.setFillColor(sf::Color::Color(0,0,0,200));
	rectangleFanArt.setOutlineColor(sf::Color::White);
	rectangleFanArt.setOutlineThickness(0);

	moviePostion.x = rectangleInfo.getPosition().x + rectangleInfo.getSize().x/2;
	moviePostion.y = rectangleInfo.getSize().y/2;

	movieBorder.setSize(sf::Vector2f(640 + 10, 480 + 10));
	movieBorder.setOrigin(movieBorder.getLocalBounds().width/2, movieBorder.getLocalBounds().height/2);
	movieBorder.setPosition(moviePostion.x, moviePostion.y);
	movieBorder.setOutlineColor(sf::Color::White);
	movieBorder.setFillColor(sf::Color::Black);
	movieBorder.setOutlineThickness(1);

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
}

void GameInfo::draw(sf::RenderWindow& window)
{
	sf::Sprite spriteFanArt;
	spriteFanArt.setTexture(fanArt);
	spriteFanArt.setOrigin(spriteFanArt.getLocalBounds().width/2, spriteFanArt.getLocalBounds().height/2);
	spriteFanArt.setPosition(rectangleFanArt.getSize().x/2, rectangleFanArt.getSize().y/2);
	sf::Vector2i newSize = resizePreserveRatio(spriteFanArt.getLocalBounds().width, spriteFanArt.getLocalBounds().height, rectangleFanArt.getSize().x, rectangleFanArt.getSize().y);
	spriteFanArt.setScale(newSize.x/spriteFanArt.getLocalBounds().width, newSize.y/spriteFanArt.getLocalBounds().height);

	window.draw(spriteFanArt);
	window.draw(rectangleFanArt);

	sf::Sprite spriteClearLogo;
	spriteClearLogo.setTexture(clearLogo);
	spriteClearLogo.setOrigin(spriteClearLogo.getGlobalBounds().width/2, 0); // Set origin to top center
	spriteClearLogo.setPosition(rectangleInfo.getPosition().x + rectangleInfo.getSize().x/2, 40); 
	window.draw(spriteClearLogo);
	
	sf::Sprite spriteDeveloper;
	spriteDeveloper.setTexture(ah.getTextureAsset(currentGameInfo.developerIconID));
	spriteDeveloper.setOrigin(0, spriteDeveloper.getLocalBounds().height);  //Set Origin to bottom left
	spriteDeveloper.setPosition(moviePostion.x + 5, movieBorder.getPosition().y - 5);
	if (currentGameInfo.developerIconID != "ERROR") // icon id of error means it does not have an icon so we should show the text insted.
		window.draw(spriteDeveloper);

	sf::Sprite spriteGenre;
	spriteGenre.setTexture(ah.getTextureAsset(currentGameInfo.genreIconID));
	spriteGenre.setOrigin(0, spriteGenre.getLocalBounds().height); //Set Origin to bottom left
	spriteGenre.setPosition(spriteDeveloper.getPosition().x + spriteDeveloper.getLocalBounds().width + 5, movieBorder.getPosition().y);
	if (currentGameInfo.genreIconID != "ERROR")
		window.draw(spriteGenre);
	


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
}


sf::Vector2i GameInfo::resizePreserveRatio(int org_width, int org_height, int new_width, int new_height)
{
	sf::Vector2i original_size = sf::Vector2i(org_width, org_height);
	sf::Vector2i wanted_size = sf::Vector2i(new_width, new_height);
	sf::Vector2i new_size;

	float org_ratio = (float)original_size.x / original_size.y;
	float new_ratio = (float)wanted_size.x / wanted_size.y;

	if (org_ratio > new_ratio)
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
