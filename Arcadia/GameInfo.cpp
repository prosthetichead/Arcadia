#include "GameInfo.h"


GameInfo::GameInfo(dbHandle &db_ref, assetHandle &ah_ref):db(db_ref), ah(ah_ref)
{
	refresh_counter = 0;
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
		movie->resizeToFrame(rectangleFanArt.getPosition().x + rectangleFanArt.getSize().x/2 , rectangleFanArt.getPosition().y + rectangleFanArt.getSize().y/2, 640,480,true);
		movieBorder.setPosition(movie->getPosition().x - 3,movie->getPosition().y - 3);
		movieBorder.setSize(sf::Vector2f(movie->getSize().x + 3 , movie->getSize().y + 3));
		movieBorder.setScale(movie->getScale());
		movieBorder.setFillColor(sf::Color::Black);
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
}

void GameInfo::draw(sf::RenderWindow& window)
{
	sf::Sprite spriteFanArt;
	spriteFanArt.setTexture(fanArt);
	spriteFanArt.setPosition(0,0);
	window.draw(spriteFanArt);
	window.draw(rectangleFanArt);

	sf::Sprite spriteClearLogo;
	spriteClearLogo.setTexture(clearLogo);
	spriteClearLogo.setOrigin(spriteClearLogo.getGlobalBounds().width/2, 0); // Set origin to top center
	spriteClearLogo.setPosition(rectangleInfo.getPosition().x + rectangleInfo.getSize().x/2, 40); 
	window.draw(spriteClearLogo);
	
	if (currentGameInfo.developerIconID != "ERROR") // icon id of error means it does not have an icon so we should show the text insted.
	{
	sf::Sprite spriteDeveloper;
	spriteDeveloper.setTexture(ah.getTextureAsset(currentGameInfo.developerIconID));
	spriteDeveloper.setOrigin(0, spriteDeveloper.getGlobalBounds().height);  //Set Origin to bottom left
	spriteDeveloper.setPosition(movieBorder.getPosition().x + 10, movieBorder.getPosition().y - 10);

	window.draw(spriteDeveloper);
	}

	if (currentGameInfo.genreIconID != "ERROR")
	{
		sf::Sprite spriteGenre;
		spriteGenre.setTexture(ah.getTextureAsset(currentGameInfo.genreIconID));
		spriteGenre.setOrigin(0,0);
		spriteGenre.setPosition(300,300);

		window.draw(spriteGenre);
	}


	if (hasMovieFile)
	{
		window.draw(movieBorder);
		window.draw(*movie);
	}

}
