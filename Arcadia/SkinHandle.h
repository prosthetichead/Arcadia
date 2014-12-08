#pragma once
#include "dbHandle.h"
#include <SFML\Graphics.hpp>
#include "tinyxml2.h"
//#include "assetHandle.h"

class SkinHandle
{
public:
	
	sf::Vector2f resolution;
	
	struct Font_Item
	{
		int size;
		std::string fontName;
		sf::Color color;

		sf::Vector2f pos;
		
		bool shadow;
		sf::Color shadowColor;
		int shadowOffset;

		Font_Item()
		{
			size = 10;
			fontName = "ARCADE_PIX.TTF";
			color = sf::Color::White;

			pos = sf::Vector2f(0,0);

			shadow = false;
			shadowOffset =0;
			shadowColor = sf::Color::Black;
		}
	};
	
	// Used by rectangles and sprites to position them.
	struct Rectangle_Item{
		sf::Vector2f size;
		sf::Vector2f pos;
		std::string origin_code;
		sf::Color colour;
		int outline_width;
		sf::Color outline_colour; 
		bool resize_fit;

		Rectangle_Item()
		{
			size = sf::Vector2f(100,100);
			pos = sf::Vector2f(100,100);
			origin_code = "TL";
			colour = sf::Color::White;
			outline_colour = sf::Color::White;
			outline_width = 0;
			resize_fit = true;
		}

		sf::RectangleShape getRectangle()
		{
			sf::RectangleShape returnRect(size);
			returnRect.setPosition(pos);
			returnRect.setOrigin(getOrigin(size.x, size.y));
			returnRect.setFillColor(colour);
			returnRect.setOutlineColor(outline_colour);
			returnRect.setOutlineThickness(outline_width);

			return returnRect;		
		}

		sf::Sprite getSprite(sf::Texture &texture)
		{
			sf::Sprite returnSprite(texture);
			returnSprite.setOrigin(getOrigin(returnSprite.getLocalBounds().width, returnSprite.getLocalBounds().height));
			returnSprite.setPosition(pos);
			returnSprite.setScale(newScale(returnSprite.getLocalBounds().width, returnSprite.getLocalBounds().height, size.x, size.y, resize_fit));
			returnSprite.setColor(colour);
			return returnSprite;
		}

		sf::Vector2f getOrigin(int og_size_x, int og_size_y){			
			if (origin_code == "TL")
				return sf::Vector2f(0,0);
			else if (origin_code == "TC")
				return sf::Vector2f(og_size_x/2,0);
			else if (origin_code == "TR")
				return sf::Vector2f(og_size_x,0);
			else if (origin_code == "CL")
				return sf::Vector2f(0,og_size_y/2);
			else if (origin_code == "C")
				return sf::Vector2f(og_size_x/2,og_size_y/2);
			else if (origin_code == "CR")
				return sf::Vector2f(og_size_x,og_size_y/2);
			else if (origin_code == "BL")
				return sf::Vector2f(0,og_size_y);
			else if (origin_code == "BC")
				return sf::Vector2f(og_size_x/2,og_size_y);
			else if (origin_code == "BR")
				return sf::Vector2f(og_size_x,og_size_y);
		}

		sf::Vector2f newScale(int org_width, int org_height, int new_width, int new_height, bool fit)
		{
			sf::Vector2f original_size = sf::Vector2f(org_width, org_height);
			sf::Vector2f wanted_size = sf::Vector2f(new_width, new_height);
			sf::Vector2f new_size(1,1);
			sf::Vector2f new_scale(0,0);

			float org_ratio = original_size.x / original_size.y;
			float new_ratio = wanted_size.x / wanted_size.y;

			if (org_ratio > new_ratio && fit)
			{
				new_size.x = original_size.x * (wanted_size.x / original_size.x);
				new_size.y = original_size.y * (wanted_size.x / original_size.x);
			}
			else
			{
				new_size.x = original_size.x * (wanted_size.y / original_size.y);
				new_size.y = original_size.y * (wanted_size.y / original_size.y);
			}

			new_scale = sf::Vector2f(new_size.x/original_size.x, new_size.y/original_size.y);

			return new_scale;
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
		Rectangle_Item fanArt;

		Rectangle_Item clearLogo;
				
		Rectangle_Item description;
		Font_Item description_font;

		Rectangle_Item video;
		Rectangle_Item gameInfoBorder;

		Rectangle_Item screenshot;

		Rectangle_Item genreIcon;

		Rectangle_Item companyLogos;

		Rectangle_Item platformIcon;

		Rectangle_Item players;

		Font_Item year_font;

		Font_Item playTimeTitle_font;
		
		Font_Item playTime_font;
		
		Font_Item lastPlayedTitle_font;
		
		Font_Item lastPlayed_font;

		Game_Info_Settings()
		{		

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
	SkinHandle::Rectangle_Item SkinHandle::read_rectangle_elem(tinyxml2::XMLElement* elem);


};

