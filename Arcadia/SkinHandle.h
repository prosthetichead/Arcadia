#pragma once
#include "dbHandle.h"
#include <SFML\Graphics.hpp>
#include "tinyxml2.h"
//#include "assetHandle.h"

class SkinHandle
{
public:
	
	std::string skin_id;
	sf::Vector2f resolution;
	int windowStyle;


	enum class element_type
	{ static_text, static_rectangle, static_image, video, screenshot, fanart,
	clear_logo, description, genres, developers, publishers, platform_icon, year,
	players, play_time, last_played, game_list  };

	
	struct Skin_Element{
		element_type type;
		bool enabled;
		sf::Vector2f size;
		sf::Vector2f pos;
		std::string origin_code;
		sf::Color colour;
		int outline_width;
		sf::Color outline_colour; 
		bool resize_fit;

		std::string asset_file;

		bool text_shadow;
		int text_size;
		sf::Color text_color;
		sf::Color text_shadowColor;
		int text_shadowOffset;
		std::string text;
		int line_spacing;
		int indent;

		Skin_Element()
		{
			enabled = false;
			size = sf::Vector2f(100,100);
			pos = sf::Vector2f(100,100);
			origin_code = "TL";
			colour = sf::Color::White;
			outline_colour = sf::Color::White;
			outline_width = 0;
			resize_fit = true;

			text_shadow = false;			
			text_size = 10;
			text_shadowOffset = 0;
			text_shadowColor = sf::Color::White;
			text_color = sf::Color::White;
			asset_file = "ARCADE_PIX.TTF";
			text = "";
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
		sf::Vector2f getOrigin(sf::FloatRect floatRect){			
			if (origin_code == "TL")
				return sf::Vector2f(0,0);
			else if (origin_code == "TC")
				return sf::Vector2f(floatRect.left + floatRect.width/2.0f, 0);
			else if (origin_code == "TR")
				return sf::Vector2f(floatRect.left + floatRect.width, 0);
			else if (origin_code == "CL")
				return sf::Vector2f(0, floatRect.top + floatRect.height/2.0f);
			else if (origin_code == "C")
				return sf::Vector2f(floatRect.left + floatRect.width/2.0f,  floatRect.top  + floatRect.height/2.0f);
			else if (origin_code == "CR")
				return sf::Vector2f(floatRect.left + floatRect.width,  floatRect.top  + floatRect.height/2.0f);
			else if (origin_code == "BL")
				return sf::Vector2f(0, floatRect.top  + floatRect.height);
			else if (origin_code == "BC")
				return sf::Vector2f(floatRect.left + floatRect.width/2.0f, floatRect.top  + floatRect.height);
			else if (origin_code == "BR")
				return sf::Vector2f(floatRect.left + floatRect.width, floatRect.top  + floatRect.height);
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
		Skin_Element list;		
		Skin_Element selected_list_item;
		Skin_Element normal_list_item;

		Game_List_Settings()
		{
			list.size = sf::Vector2f(300,500);
			list.pos = sf::Vector2f(0,0);
		}
	};

	std::vector<Skin_Element> skin_elements;
	Game_List_Settings game_list_settings;
	SkinHandle();
	~SkinHandle(void);
	void SkinHandle::init(dbHandle& db_ref);
	
private:
	void SkinHandle::loadLayout();	
	std::string xml_path;
	std::string exe_path;
	//SkinHandle::Font_Item SkinHandle::read_font_elem(tinyxml2::XMLElement* elem);
	SkinHandle::Skin_Element SkinHandle::read_skin_elem(tinyxml2::XMLElement* elem);
	


};

