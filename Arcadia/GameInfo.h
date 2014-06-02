#pragma once
#include <SFML\Graphics.hpp>
#include <sfeMovie\Movie.hpp>
#include <boost\algorithm\string\split.hpp>
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
	sf::RectangleShape gameIconsBorder;
	sf::RectangleShape descriptionBorder;

	sf::View descriptionView;
	
	sf::Font descriptionFont;
	sf::Text descriptionText;
	int descriptionFontSize;
	bool descriptionScroll;
	bool descriptionPause;
	float descriptionPauseTime;
	float descriptionPauseCount;
	float descriptionOffset;
	float descriptionRequiredOffset;
	bool resetOffset;

	sf::Font yearFont;
	sf::Text yearText;
	int yearFontSize;
	
	sf::Vector2i moviePostion;
	
public:
	GameInfo(dbHandle &db_ref, assetHandle &ah_ref);
	~GameInfo(void);
	void GameInfo::init(float posX, float posY, int width, float height);
	void GameInfo::update();
	void GameInfo::newGameInfo(dbHandle::gameListItem gameItem);
	void GameInfo::draw(sf::RenderWindow &window);
	void GameInfo::pauseMovie();
	sf::Vector2i GameInfo::resizePreserveRatio(int org_width, int org_height, int new_width, int new_height, bool fit = true);

};

