#pragma once
#include <SFML/Graphics.hpp>
#include <sfeMovie\Movie.hpp>
#include "dbHandle.h"


class GameInfo
{
private:
	dbHandle db;
	dbHandle::gameListItem currentGameItem;

	bool hasMovieFile;
	int movieStatus;
	
	sf::Texture* fanArt;
	sf::Texture boxShoot;
	sf::Texture screenShot;

	sfe::Movie* movie;

	sf::RectangleShape rectangle;
	sf::RectangleShape movieBorder;

	sf::Text description;
	
public:
	GameInfo(void);
	~GameInfo(void);
	void GameInfo::init(dbHandle &db_obj, float posX, float posY, int width, float height);
	void GameInfo::update(dbHandle::gameListItem gameItem);
	void GameInfo::draw(sf::RenderWindow& window);
	void GameInfo::pauseMovie();

};

