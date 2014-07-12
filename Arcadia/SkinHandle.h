#pragma once
#include "dbHandle.h"
#include <SFML\Graphics.hpp>
#include "tinyxml2.h"

class SkinHandle
{
private:
	void SkinHandle::loadLayout();
	std::string xml_path;
	std::string exe_path;

public:
	
	sf::Vector2f resolution;
	
	struct Game_List_Settings
	{
		sf::Vector2f size;
		sf::Vector2f position;
		sf::Font fontSelected;
		int fontSelectedSize;
		sf::Color fontSelectedColor;
		sf::Font fontNormal;
		int fontNormalSize;
		sf::Color fontNormalColor;
	};

	Game_List_Settings game_list_settings;
	SkinHandle();
	~SkinHandle(void);
	void SkinHandle::init(dbHandle& db_ref);



};

