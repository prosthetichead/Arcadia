#pragma once
#include <SFML/Graphics.hpp>
#include <sfeMovie\Movie.hpp>
#include "dbHandle.h"
#include "assetHandle.h"


class GameInfo
{
private:
	dbHandle &db;
	assetHandle &ah;
	dbHandle::gameListItem currentGameItem;
	dbHandle::gameInfoItem currentGameInfo;

	bool hasMovieFile;
	int movieStatus;
	
	sf::Texture fanArt;
	sf::Texture clearLogo;
	sf::Texture boxShoot;
	sf::Texture screenShot;
	sf::Texture developerLogo;
	sf::Texture publisherLogo;

	sfe::Movie* movie;

	sf::RectangleShape rectangleInfo;	//Rectangle of just the Info Area
	sf::RectangleShape rectangleFanArt;	//Full Screen Rectangle
	sf::RectangleShape movieBorder;		
	
	sf::Text description;

	int refresh_counter;
	
public:
	GameInfo(dbHandle &db_ref, assetHandle &ah_ref);
	~GameInfo(void);
	void GameInfo::init(float posX, float posY, int width, float height);
	void GameInfo::update();
	void GameInfo::newGameInfo(dbHandle::gameListItem gameItem);
	void GameInfo::draw(sf::RenderWindow &window);
	void GameInfo::pauseMovie();

};

