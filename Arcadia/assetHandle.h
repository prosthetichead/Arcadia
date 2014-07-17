#pragma once
#include <SFML/Graphics.hpp>
#include <boost\unordered_map.hpp>
#include <boost\algorithm\string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include "dbHandle.h"
#include "SkinHandle.h"

class assetHandle
{
private:
	boost::unordered_map<std::string, sf::Texture> textureMap;
	boost::unordered_map<std::string, sf::Font> fontMap;
	
	void assetHandle::loadFonts(std::string path);

	sf::Font emptyFont;

public:
	assetHandle(void);
	~assetHandle(void);
	void assetHandle::init(dbHandle& db_obj);
	sf::Texture& assetHandle::getTextureAsset(std::string id);
	sf::Font& assetHandle::getFontAsset(std::string id);
	sf::Vector2i assetHandle::resizePreserveRatio(int org_width, int org_height, int new_width, int new_height, bool fit);
	
	void assetHandle::draw5_Stars(double numStars, sf::Color color, int x, int y, sf::RenderWindow& window);
	void assetHandle::drawFont(sf::Text& text, SkinHandle::Font_Item& fontItem, sf::RenderWindow& window);
	
};

