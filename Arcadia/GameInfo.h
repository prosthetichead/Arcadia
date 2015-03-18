#pragma once
#include <SFML\Graphics.hpp>
#include <sfeMovie\Movie.hpp>
#include <boost\algorithm\string\split.hpp>
#include "dbHandle.h"
#include "assetHandle.h"
#include "SkinHandle.h"
#include "TextChanger.h"


class GameInfo
{
private:
	dbHandle* db;
	assetHandle* ah;
	SkinHandle* sh;

	TextChanger genreChanger;

	dbHandle::gameListItem currentGameItem;
	dbHandle::gameInfoItem currentGameInfo;
	dbHandle::gameListItem newGameItem;

	bool hasMovieFile;
	int movieStatus;
	int gameChangedCounter;
	
	sf::Texture fanArt;
	sf::Texture clearLogo;
	sf::Texture boxShoot;
	sf::Texture screenShot;
	sf::Texture developerLogo;
	sf::Texture publisherLogo;
	sf::RenderTexture scrollingTextTexture;


	sfe::Movie* movie;

	
	sf::RectangleShape rectangleFanArt;	//Full Screen Rectangle

	sf::Text descriptionText;
	int descriptionFontSize;
	bool descriptionScroll;
	bool descriptionPause;
	float descriptionPauseTime;
	float descriptionPauseCount;
	float descriptionOffset;
	float descriptionRequiredOffset;
	
	bool resetOffset;

	//sf::Font yearFont;
	sf::Text yearText;
	int yearFontSize;

	//sf::Font playTimeFont;
	sf::Text playTimeText;
	
	//sf::Vector2i moviePostion;

	void GameInfo::newGameInfo(dbHandle::gameListItem gameItem);
	
public:
	GameInfo(dbHandle* db_ref, assetHandle* ah_ref, SkinHandle* sh_ref);
	~GameInfo(void);
	void GameInfo::init(); //void GameInfo::init(float posX, float posY, float width, float height);
	void GameInfo::update(dbHandle::gameListItem gameItem);
	
	void GameInfo::draw(sf::RenderWindow &window);
	void GameInfo::pauseMovie();
};

