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

	hasMovieFile = false;

	rectangle.setSize(sf::Vector2f(width, height));
	rectangle.setPosition(posX, posY);
	rectangle.setFillColor(sf::Color::Color(45,33,100,255));
	rectangle.setOutlineColor(sf::Color::White);
	rectangle.setOutlineThickness(0);

	
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
	
	movieStatus = movie->getStatus();
	std::cout << movieStatus << std::endl;
	if (currentGameItem.platformID != gameItem.platformID || currentGameItem.fileName != gameItem.fileName)    // New Game Selected
	{
		delete movie;
		movie = new sfe::Movie;  // recreate movie

		currentGameItem = gameItem;
		hasMovieFile = false;

		// Load movie file if one exists
		if (currentGameItem.videoPath != "NULL")
		{
			hasMovieFile = true;
			movie->openFromFile(currentGameItem.videoPath);
			movie->resizeToFrame(rectangle.getPosition().x + rectangle.getSize().x/2 , rectangle.getPosition().y + rectangle.getSize().y/2, 640,480,true);
			movieBorder.setPosition(movie->getPosition().x - 3,movie->getPosition().y - 3);
			movieBorder.setSize(sf::Vector2f(movie->getSize().x + 3 , movie->getSize().y + 3));
			movieBorder.setScale(movie->getScale());
			movieBorder.setFillColor(sf::Color::Black);
			movie->play();
		}

		// Reload Textures

		delete fanArt;
		fanArt = new sf::Texture;
		if (currentGameItem.fanArtPath != "NULL")
		{
			fanArt->loadFromFile(currentGameItem.fanArtPath);
			rectangle.setTexture(fanArt);
			rectangle.setFillColor(sf::Color::White);
		}
		else
		{
			rectangle.setTexture(NULL);
			rectangle.setFillColor(sf::Color::Color(51,51,102,255));
		}
		
		// chanage text
	}

	if (hasMovieFile && movieStatus == movie->Playing)
	{
		movie->update();
	}
	//if (hasMovieFile && movieStatus == movie->Stopped)
	//{
	//	movie->play();
	//	movie->update();
	//}
}

void GameInfo::draw(sf::RenderWindow& window)
{
	window.draw(rectangle);
	
	if (hasMovieFile)
	{
		window.draw(movieBorder);
		window.draw(*movie);
	}
}
