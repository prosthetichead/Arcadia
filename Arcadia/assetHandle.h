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
	boost::unordered_map<std::string, sf::Texture> iconMap;
	boost::unordered_map<std::string, sf::Texture> companiesMap;

	void assetHandle::loadFonts(std::string path);
	boost::unordered_map<std::string, sf::Texture> assetHandle::loadImages(std::string path);

	sf::Font emptyFont;

public:
	assetHandle(void);
	~assetHandle(void);
	void assetHandle::init(dbHandle& db_obj);
	sf::Texture& assetHandle::getTextureAsset(std::string id);
	sf::Texture& assetHandle::getIconAsset(std::string id);
	sf::Texture& assetHandle::getCompanyAsset(std::string id);

	sf::Font& assetHandle::getFontAsset(std::string id);
	
	sf::Vector2i assetHandle::resizePreserveRatio(int org_width, int org_height, int new_width, int new_height, bool fit);
	
	void assetHandle::draw5_Stars(double numStars, sf::Color color, int x, int y, sf::RenderWindow& window);
	sf::Text assetHandle::getText(std::string text, SkinHandle::Skin_Element& fontItem);
	void assetHandle::drawText(std::string text, SkinHandle::Skin_Element& fontItem, sf::RenderWindow& window);
};

