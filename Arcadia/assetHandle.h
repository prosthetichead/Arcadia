#pragma once
#include <SFML/Graphics.hpp>
#include <boost\unordered_map.hpp>
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
};

