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