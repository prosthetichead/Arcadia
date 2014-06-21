#include "assetHandle.h"


assetHandle::assetHandle(void)
{
}


assetHandle::~assetHandle(void)
{
}

void assetHandle::init(dbHandle& db_obj)
{
	std::vector<dbHandle::assetItem> items = db_obj.getIconPaths();
	for (int i = 0; i < items.size(); i++)
	{
		sf::Texture texture;
		texture.setSmooth(true);
		texture.loadFromFile(items.at(i).path);
		std::pair<std::string,sf::Texture> pair (items.at(i).id,texture);
		textureMap.insert(pair);
	}
}

sf::Texture& assetHandle::getTextureAsset(std::string id)
{
	boost::to_upper(id);  //Make upper case

	boost::unordered_map<std::string,sf::Texture>::iterator iter = textureMap.find(id);
	if(iter != textureMap.end()) return iter->second;
	else
	{
		//std::cout << "ERROR: texture ID " << id << " not found" << std::endl;
		boost::unordered_map<std::string,sf::Texture>::iterator iter = textureMap.find("ERROR");
		if(iter != textureMap.end()) return iter->second;
	}

}


sf::Vector2i assetHandle::resizePreserveRatio(int org_width, int org_height, int new_width, int new_height, bool fit)
{
	sf::Vector2i original_size = sf::Vector2i(org_width, org_height);
	sf::Vector2i wanted_size = sf::Vector2i(new_width, new_height);
	sf::Vector2i new_size;

	float org_ratio = (float)original_size.x / original_size.y;
	float new_ratio = (float)wanted_size.x / wanted_size.y;

	if (org_ratio > new_ratio && fit)
	{
		new_size.x = original_size.x * ((float)wanted_size.x / original_size.x);
		new_size.y = original_size.y * ((float)wanted_size.x / original_size.x);
	}
	else
	{
		new_size.x = original_size.x * ((float)wanted_size.y / original_size.y);
		new_size.y = original_size.y * ((float)wanted_size.y / original_size.y);
	}

	return new_size;
}