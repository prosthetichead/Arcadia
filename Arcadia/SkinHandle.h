#pragma once
#include "dbHandle.h"
#include <SFML\Graphics.hpp>
#include "tinyxml2.h"

class SkinHandle
{
public:
	
	sf::Vector2f resolution;
	
	struct Font_Item
	{
		int size;
		std::string fontName;
		sf::Color color;
		
		bool shadow;
		sf::Color shadowColor;
		int shadowOffset;

		Font_Item()
		{
			size = 10;
			fontName = "ARCADE_PIX.TTF";
			color = sf::Color::White;

			shadow = false;
			shadowOffset =0;
			shadowColor = sf::Color::Black;
		}
	};

	struct Game_List_Settings
	{
		sf::Vector2f size;
		sf::Vector2f position;
		
		Font_Item selected_font;
		Font_Item normal_font;

		Game_List_Settings()
		{
			size = sf::Vector2f(300,500);
			position = sf::Vector2f(0,0);
		}
	};

	Game_List_Settings game_list_settings;
	SkinHandle();
	~SkinHandle(void);
	void SkinHandle::init(dbHandle& db_ref);

private:
	void SkinHandle::loadLayout();	
	std::string xml_path;
	std::string exe_path;
	SkinHandle::Font_Item SkinHandle::read_font_elem(tinyxml2::XMLElement* elem);

};

