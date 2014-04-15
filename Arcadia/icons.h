#pragma once
#include <SFML/Graphics.hpp>
#include <boost\unordered_map.hpp>
#include "dbHandle.h"

class icons
{
private:
	boost::unordered_map<std::string, sf::Texture> iconMap;

public:
	icons(void);
	~icons(void);
	void icons::init(dbHandle& db_obj);
	bool icons::drawIcon(int x, int y, std::string id);
};

