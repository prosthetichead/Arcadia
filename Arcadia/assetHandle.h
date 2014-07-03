#pragma once
#include <SFML/Graphics.hpp>
#include <boost\unordered_map.hpp>
#include <boost\algorithm\string.hpp>
#include "dbHandle.h"

class assetHandle
{
private:
	boost::unordered_map<std::string, sf::Texture> textureMap;

public:
	assetHandle(void);
	~assetHandle(void);
	void assetHandle::init(dbHandle& db_obj);
	sf::Texture& assetHandle::getTextureAsset(std::string id);
	sf::Vector2i assetHandle::resizePreserveRatio(int org_width, int org_height, int new_width, int new_height, bool fit);
	void assetHandle::draw5_Stars(double numStars, sf::Color color, int x, int y, sf::RenderWindow& window);
};

