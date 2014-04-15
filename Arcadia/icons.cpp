#include "icons.h"


icons::icons(void)
{
}


icons::~icons(void)
{
}

void icons::init(dbHandle& db_obj)
{
	std::vector<dbHandle::iconItem> items = db_obj.getIconPaths();
	for (int i = 0; i < items.size(); i++)
	{
		sf::Texture texture;
		texture.loadFromFile(items.at(i).path);
		std::pair<std::string,sf::Texture> pair (items.at(i).id,texture);
		iconMap.insert(pair);
	}
}