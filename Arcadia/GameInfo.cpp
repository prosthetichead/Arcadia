#include "GameInfo.h"


GameInfo::GameInfo(void)
{


}


GameInfo::~GameInfo(void)
{
}

void GameInfo::init(dbHandle &db_obj, float posX, float posY, int width, float height)
{
	db = db_obj;
	sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();

	hasMovieFile = false;

	rectangleInfo.setSize(sf::Vector2f(width,height));
	rectangleInfo.setPosition(posX,posY);

	rectangleFanArt.setSize(sf::Vector2f(desktopMode.width, desktopMode.height));
	rectangleFanArt.setPosition(0, 0);
	rectangleFanArt.setFillColor(sf::Color::White);
	rectangleFanArt.setOutlineColor(sf::Color::White);
	rectangleFanArt.setOutlineThickness(0);

	rectangleFanArtDarken = rectangleFanArt;
	rectangleFanArtDarken.setFillColor(sf::Color::Color(0,0,0,200));

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

//void GameInfo::update(dbHandle::gameListItem gameItem)
//{	
//	movieStatus = movie->getStatus();
//	if (currentGameItem.platformID != gameItem.platformID || currentGameItem.fileName != gameItem.fileName)    // New Game Selected
//	{
//		delete movie;
//		movie = new sfe::Movie;  // recreate movie
//
//		currentGameItem = gameItem;
//		hasMovieFile = false;
//
//		// Load movie file if one exists
//		if (currentGameItem.videoPath != "NULL")
//		{
//			hasMovieFile = true;
//			movie->openFromFile(currentGameItem.videoPath);
//			movie->resizeToFrame(rectangleFanArt.getPosition().x + rectangleFanArt.getSize().x/2 , rectangleFanArt.getPosition().y + rectangleFanArt.getSize().y/2, 640,480,true);
//			movieBorder.setPosition(movie->getPosition().x - 3,movie->getPosition().y - 3);
//			movieBorder.setSize(sf::Vector2f(movie->getSize().x + 3 , movie->getSize().y + 3));
//			movieBorder.setScale(movie->getScale());
//			movieBorder.setFillColor(sf::Color::Black);
//			movie->play();
//		}
//
//		// Reload Textures
//
//		delete fanArt;
//		fanArt = new sf::Texture;
//		if (currentGameItem.fanArtPath != "NULL") {
//			fanArt->loadFromFile(currentGameItem.fanArtPath);
//			rectangleFanArt.setTexture(fanArt);
//			rectangleFanArt.setFillColor(sf::Color::Color(255,255,255,255));
//		}
//		else {
//			rectangleFanArt.setTexture(NULL);
//			rectangleFanArt.setFillColor(sf::Color::Color(0,0,0,255));
//		}
//
//		if (currentGameItem.clearLogoPath != "NULL")
//		{
//			std::cout << currentGameItem.clearLogoPath << std::endl;
//			clearLogo.loadFromFile(currentGameItem.clearLogoPath);
//		}
//		else 
//			clearLogo.create(1,1); // Create and empty Texture
//
//		// chanage text
//	}
//
//	if (hasMovieFile && movieStatus == movie->Playing)
//	{
//		movie->update();
//	}
//	//if (hasMovieFile && movieStatus == movie->Stopped)
//	//{
//	//	movie->play();
//	//	movie->update();
//	//}
//}

void GameInfo::draw(sf::RenderWindow& window)
{
	sf::Sprite spriteClearLogo;
	spriteClearLogo.setTexture(clearLogo);
	spriteClearLogo.setPosition(rectangleInfo.getPosition().x + rectangleInfo.getSize().x/2, 100);
	spriteClearLogo.setOrigin(spriteClearLogo.getGlobalBounds().width/2, spriteClearLogo.getGlobalBounds().height/2);


	window.draw(rectangleFanArt);
	window.draw(rectangleFanArtDarken);
	window.draw(spriteClearLogo);

	if (hasMovieFile)
	{
		window.draw(movieBorder);
		window.draw(*movie);
	}
}
