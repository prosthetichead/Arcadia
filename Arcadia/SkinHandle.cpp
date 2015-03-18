#include "SkinHandle.h"


SkinHandle::SkinHandle()
{	
}


SkinHandle::~SkinHandle(void)
{
}

void SkinHandle::init(dbHandle& db)
{
	exe_path = db.exe_path;
	//work out which layout to use. for now lets just use /assets/layouts/main.xml
	xml_path = db.exe_path + "\\assets\\layouts\\main.xml";
	std::cout << xml_path << std::endl;

	resolution.x = 1600;
	resolution.y = 1200;

	//load gamelist settings from xml
	loadLayout();
}

SkinHandle::Font_Item SkinHandle::read_font_elem(tinyxml2::XMLElement* elem)
{
	SkinHandle::Font_Item font_item;

	if (elem->Attribute("pos_x") != NULL)
		font_item.pos.x = atof(elem->Attribute("pos_x"));
	if (elem->Attribute("pos_y") != NULL)
		font_item.pos.y = atof(elem->Attribute("pos_y"));

	for(tinyxml2::XMLElement* fontRootElem = elem->FirstChildElement(); fontRootElem != NULL; fontRootElem = fontRootElem->NextSiblingElement())
	{
		std::string fontRootElemName = fontRootElem->Value();
		 
			if (fontRootElemName == "font")
			{	
				font_item.fontName = fontRootElem->Attribute("file");
				font_item.size = atoi(fontRootElem->Attribute("size"));
				font_item.color = sf::Color::Color(atoi(fontRootElem->Attribute("red")), atoi(fontRootElem->Attribute("green")), atoi(fontRootElem->Attribute("blue")), atoi(fontRootElem->Attribute("alpha")) );
	
				for(tinyxml2::XMLElement* fontElem = fontRootElem->FirstChildElement(); fontElem != NULL; fontElem = fontElem->NextSiblingElement())
				{
					std::string elemName = fontElem->Value();
					if (elemName== "shadow")
					{
						font_item.shadow = true;
						font_item.shadowColor = sf::Color::Color(atoi(fontElem->Attribute("red")), atoi(fontElem->Attribute("green")), atoi(fontElem->Attribute("blue")), atoi(fontElem->Attribute("alpha")) );
						font_item.shadowOffset = atoi(fontElem->Attribute("offset"));
					}
				}
			}
	}

	return font_item;
}

SkinHandle::Rectangle_Item SkinHandle::read_rectangle_elem(tinyxml2::XMLElement* elem)
{
	SkinHandle::Rectangle_Item return_rect;
	if (elem->Attribute("pos_x") != NULL && elem->Attribute("pos_y") != NULL)
		return_rect.pos = sf::Vector2f(atof(elem->Attribute("pos_x")), atof(elem->Attribute("pos_y")));	
	if (elem->Attribute("size_x") != NULL && elem->Attribute("size_y") != NULL)
		return_rect.size = sf::Vector2f(atof(elem->Attribute("size_x")), atof(elem->Attribute("size_y")));
	if(elem->Attribute("origin") != NULL)
	{
		return_rect.origin_code = elem->Attribute("origin");
		
	}
	if (elem->Attribute("resize_fit") != NULL)
	{
		std::string resize_fit = elem->Attribute("resize_fit");
		if (resize_fit == "false")	
			return_rect.resize_fit = false;		
		else
			return_rect.resize_fit = true;
	}
	for(tinyxml2::XMLElement* rectElem = elem->FirstChildElement(); rectElem != NULL; rectElem = rectElem->NextSiblingElement())
	{
		std::string rectElemName = rectElem->Value();
		if(rectElemName == "colour")
			return_rect.colour = sf::Color::Color(atoi(rectElem->Attribute("red")), atoi(rectElem->Attribute("green")), atoi(rectElem->Attribute("blue")), atoi(rectElem->Attribute("alpha")) );
		if(rectElemName == "outline")
		{
			return_rect.outline_width = atoi(rectElem->Attribute("width"));
			return_rect.outline_colour = sf::Color::Color(atoi(rectElem->Attribute("red")), atoi(rectElem->Attribute("green")), atoi(rectElem->Attribute("blue")), atoi(rectElem->Attribute("alpha")) );
		}
	}

	return return_rect;
}

void SkinHandle::loadLayout()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(xml_path.c_str());
	tinyxml2::XMLNode *rootnode = doc.RootElement();

	resolution.x = atof(rootnode->ToElement()->Attribute("res_width"));
	resolution.y = atof(rootnode->ToElement()->Attribute("res_height"));

	for(tinyxml2::XMLElement* elem = rootnode->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string elemName = elem->Value();
		if (elemName == "game_list")
		{
			game_list_settings.size.x = atof(elem->Attribute("size_x"));
			game_list_settings.size.y = atof(elem->Attribute("size_y"));
			game_list_settings.position.x = atof(elem->Attribute("pos_x"));
			game_list_settings.position.y = atof(elem->Attribute("pos_y"));

			for(tinyxml2::XMLElement* gameElem = elem->FirstChildElement(); gameElem != NULL; gameElem = gameElem->NextSiblingElement())
			{
				std::string gameElemName = gameElem->Value();

				if (gameElemName == "selected_item")
					game_list_settings.selected_font = read_font_elem(gameElem);						
				if (gameElemName == "normal_item")
					game_list_settings.normal_font = read_font_elem(gameElem);																	
			}
		}
		if (elemName == "game_info")
		{
			for(tinyxml2::XMLElement* gameElem = elem->FirstChildElement(); gameElem != NULL; gameElem = gameElem->NextSiblingElement())
			{
				std::string gameElemName = gameElem->Value();
				if (gameElemName == "fanart")
					game_info_settings.fanArt = read_rectangle_elem(gameElem);

				if (gameElemName == "clear_logo")
					game_info_settings.clearLogo = read_rectangle_elem(gameElem);	
				
				if (gameElemName == "description") {
					game_info_settings.description = read_rectangle_elem(gameElem);					
					game_info_settings.description_font = read_font_elem(gameElem);
				}

				if (gameElemName == "genres") {
					game_info_settings.genres = read_rectangle_elem(gameElem);
					game_info_settings.genres_font = read_font_elem(gameElem);
				}

				if (gameElemName == "video")
					game_info_settings.video  = read_rectangle_elem(gameElem);
				
				if (gameElemName == "screenshot")
					game_info_settings.screenshot = read_rectangle_elem(gameElem);
				
				if (gameElemName == "company_logos")
					game_info_settings.companyLogos = read_rectangle_elem(gameElem);
				
				if (gameElemName == "platform_icon")
					game_info_settings.platformIcon = read_rectangle_elem(gameElem);
				
				if (gameElemName == "year")
					game_info_settings.year_font = read_font_elem(gameElem);
				
				if (gameElemName == "play_time_title")
					game_info_settings.playTimeTitle_font = read_font_elem(gameElem);

				if (gameElemName == "play_time")
					game_info_settings.playTime_font = read_font_elem(gameElem);

				if (gameElemName == "last_played_title")
					game_info_settings.lastPlayedTitle_font = read_font_elem(gameElem);
				
				if (gameElemName == "last_played")
					game_info_settings.lastPlayed_font = read_font_elem(gameElem);

				if (gameElemName == "game_info_border")
					game_info_settings.gameInfoBorder = read_rectangle_elem(gameElem);

				if (gameElemName == "players")
					game_info_settings.players = read_rectangle_elem(gameElem);
			}

		}
	}


}