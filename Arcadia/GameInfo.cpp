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

	if (currentGameItem.gameID != gameItem.gameID)    // New Game Selected
	{
		delete movie;
		movie = new sfe::Movie;

		currentGameItem = gameItem;
		hasMovieFile = false;

		// Load movie file if one exists
		if (currentGameItem.videoPath != "NULL")
		{
			hasMovieFile = true;
			movie->openFromFile(currentGameItem.videoPath);
			movie->resizeToFrame(rectangle.getPosition().x + rectangle.getSize().x/2 , rectangle.getPosition().y + rectangle.getSize().y/2, 640,480,true);
			
			movie->play();
		}

		// Reload Textures	
		// chanage text
	}

	if (hasMovieFile && movieStatus == movie->Playing)
	{
		movie->update();
	}
}

void GameInfo::draw(sf::RenderWindow& window)
{
	window.draw(rectangle);

	if (hasMovieFile && movieStatus == movie->Playing)
	{
		window.draw(*movie);
	}
}
