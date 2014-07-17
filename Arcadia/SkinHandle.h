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
	struct Game_Info_Settings
	{
		sf::Color fanArt_colour;

		sf::Vector2f clearLogo_position;
		sf::Vector2f clearLogo_size;
				
		sf::Vector2f description_position;
		sf::Vector2f description_size;
		Font_Item description_font;

		sf::Vector2f video_position;
		sf::Vector2f video_size;

		sf::Vector2f screenshot_position;
		sf::Vector2f screenshot_size;

		sf::Vector2f genreIcon_position;
		sf::Vector2f genreIcon_size;

		sf::Vector2f companyLogos_position;
		sf::Vector2f companyLogos_size;

		sf::Vector2f platformIcon_position;
		sf::Vector2f platformIcon_size;

		sf::Vector2f players_position;
		sf::Vector2f players_size;

		sf::Vector2f year_position;
		Font_Item year_font;

		sf::Vector2f playTimeTitle_position;
		Font_Item playTimeTitle_font;
		sf::Vector2f playTime_position;
		Font_Item playTime_font;
		sf::Vector2f lastPlayedTitle_position;
		Font_Item lastPlayedTitle_font;
		sf::Vector2f lastPlayed_position;
		Font_Item lastPlayed_font;

		Game_Info_Settings()
		{
			clearLogo_position = sf::Vector2f(300,500);
			clearLogo_size = sf::Vector2f(300,500);
			description_position = sf::Vector2f(300,500);
			description_size = sf::Vector2f(300,500);
			video_position =  sf::Vector2f(300,500);
			video_size =  sf::Vector2f(300,500);
			screenshot_position = sf::Vector2f(300,500);
			screenshot_size = sf::Vector2f(300,500);
			genreIcon_position = sf::Vector2f(300,500); 
			genreIcon_size = sf::Vector2f(300,500);
			companyLogos_position = sf::Vector2f(300,500);
			companyLogos_size = sf::Vector2f(300,500);
			platformIcon_position = sf::Vector2f(300,500);
			platformIcon_size = sf::Vector2f(300,500);
			players_position = sf::Vector2f(300,500);
			players_size = sf::Vector2f(300,500);
			year_position  = sf::Vector2f(300,500);
			playTimeTitle_position = sf::Vector2f(300,500);
			playTime_position = sf::Vector2f(300,500);
			lastPlayedTitle_position = sf::Vector2f(300,500);
			lastPlayed_position = sf::Vector2f(300,500);
		}

	};

	Game_List_Settings game_list_settings;
	Game_Info_Settings game_info_settings;
	SkinHandle();
	~SkinHandle(void);
	void SkinHandle::init(dbHandle& db_ref);

private:
	void SkinHandle::loadLayout();	
	std::string xml_path;
	std::string exe_path;
	SkinHandle::Font_Item SkinHandle::read_font_elem(tinyxml2::XMLElement* elem);

};

