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


SkinHandle::Skin_Element SkinHandle::read_rectangle_elem(tinyxml2::XMLElement* elem)
{
	SkinHandle::Skin_Element return_element;
	if (elem->Attribute("pos_x") != NULL && elem->Attribute("pos_y") != NULL)
		return_element.pos = sf::Vector2f(atof(elem->Attribute("pos_x")), atof(elem->Attribute("pos_y")));	
	if (elem->Attribute("size_x") != NULL && elem->Attribute("size_y") != NULL)
		return_element.size = sf::Vector2f(atof(elem->Attribute("size_x")), atof(elem->Attribute("size_y")));
	if(elem->Attribute("origin") != NULL)
	{
		return_element.origin_code = elem->Attribute("origin");
		
	}
	if (elem->Attribute("resize_fit") != NULL)
	{
		std::string resize_fit = elem->Attribute("resize_fit");
		if (resize_fit == "false")	
			return_element.resize_fit = false;		
		else
			return_element.resize_fit = true;
	}
	for(tinyxml2::XMLElement* rectElem = elem->FirstChildElement(); rectElem != NULL; rectElem = rectElem->NextSiblingElement())
	{
		std::string rectElemName = rectElem->Value();
		if(rectElemName == "colour")
			return_element.colour = sf::Color::Color(atoi(rectElem->Attribute("red")), atoi(rectElem->Attribute("green")), atoi(rectElem->Attribute("blue")), atoi(rectElem->Attribute("alpha")) );
		if(rectElemName == "outline")
		{
			return_element.outline_width = atoi(rectElem->Attribute("width"));
			return_element.outline_colour = sf::Color::Color(atoi(rectElem->Attribute("red")), atoi(rectElem->Attribute("green")), atoi(rectElem->Attribute("blue")), atoi(rectElem->Attribute("alpha")) );
		}
		if(rectElemName == "font")
		{
			return_element.text_font = rectElem->Attribute("file");

			return_element.text_size = atoi(rectElem->Attribute("size"));
			return_element.text_color = sf::Color::Color(atoi(rectElem->Attribute("red")), atoi(rectElem->Attribute("green")), atoi(rectElem->Attribute("blue")), atoi(rectElem->Attribute("alpha")) );
			for(tinyxml2::XMLElement* fontElem = rectElem->FirstChildElement(); fontElem != NULL; fontElem = fontElem->NextSiblingElement())
			{
				std::string elemName = fontElem->Value();
				if (elemName== "shadow")
				{
					return_element.text_shadow = true;
					return_element.text_shadowColor = sf::Color::Color(atoi(fontElem->Attribute("red")), atoi(fontElem->Attribute("green")), atoi(fontElem->Attribute("blue")), atoi(fontElem->Attribute("alpha")) );
					return_element.text_shadowOffset = atoi(fontElem->Attribute("offset"));
				}
			}
		}

	}

	return return_element;
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
			for(tinyxml2::XMLElement* gameElem = elem->FirstChildElement(); gameElem != NULL; gameElem = gameElem->NextSiblingElement())
			{
				std::string gameElemName = gameElem->Value();
				if (gameElemName == "list")
					game_list_settings.list = read_rectangle_elem(gameElem);
				if (gameElemName == "selected_item")
					game_list_settings.selected_list_item = read_rectangle_elem(gameElem);						
				if (gameElemName == "normal_item")
					game_list_settings.normal_list_item = read_rectangle_elem(gameElem);																	
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
				
				if (gameElemName == "description") 
					game_info_settings.description = read_rectangle_elem(gameElem);		
				
				if (gameElemName == "genres") 
					game_info_settings.genres = read_rectangle_elem(gameElem);
				
				if (gameElemName == "video")
					game_info_settings.video  = read_rectangle_elem(gameElem);
				
				if (gameElemName == "screenshot")
					game_info_settings.screenshot = read_rectangle_elem(gameElem);
				
				if (gameElemName == "company_logos")
					game_info_settings.companyLogos = read_rectangle_elem(gameElem);
				
				if (gameElemName == "platform_icon")
					game_info_settings.platformIcon = read_rectangle_elem(gameElem);
				
				if (gameElemName == "year")
					game_info_settings.year = read_rectangle_elem(gameElem);
				
				if (gameElemName == "play_time_title")
					game_info_settings.playTimeTitle  = read_rectangle_elem(gameElem);

				if (gameElemName == "play_time")
					game_info_settings.playTime = read_rectangle_elem(gameElem);

				if (gameElemName == "last_played_title")
					game_info_settings.lastPlayedTitle = read_rectangle_elem(gameElem);
				
				if (gameElemName == "last_played")
					game_info_settings.lastPlayed = read_rectangle_elem(gameElem);

				if (gameElemName == "game_info_border")
					game_info_settings.gameInfoBorder = read_rectangle_elem(gameElem);

				if (gameElemName == "players")
					game_info_settings.players = read_rectangle_elem(gameElem);
			}

		}
	}


}