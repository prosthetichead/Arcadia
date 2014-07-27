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
	return_rect.pos = sf::Vector2f(atof(elem->Attribute("pos_x")), atof(elem->Attribute("pos_y")));	
	return_rect.size = sf::Vector2f(atof(elem->Attribute("size_x")), atof(elem->Attribute("size_y")));
	if(elem->Attribute("origin") != NULL)
		return_rect.origin_code = elem->Attribute("origin");

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
					game_info_settings.fanArt_colour = sf::Color::Color(atoi(gameElem->Attribute("red")), atoi(gameElem->Attribute("green")), atoi(gameElem->Attribute("blue")), atoi(gameElem->Attribute("alpha")) );
				if (gameElemName == "description")
				{
					game_info_settings.description_position.x = atof(gameElem->Attribute("pos_x"));
					game_info_settings.description_position.y = atof(gameElem->Attribute("pos_y"));
					game_info_settings.description_size.x = atof(gameElem->Attribute("size_x"));
					game_info_settings.description_size.y = atof(gameElem->Attribute("size_y"));
					
					game_info_settings.description_font = read_font_elem(gameElem);
				}
				if (gameElemName == "video")
				{
					game_info_settings.video  = read_rectangle_elem(gameElem);
				}
				if (gameElemName == "screenshot")
				{
					game_info_settings.screenshot_position.x = atof(gameElem->Attribute("pos_x"));
					game_info_settings.screenshot_position.y = atof(gameElem->Attribute("pos_y"));
					game_info_settings.screenshot_size.x = atof(gameElem->Attribute("size_x"));
					game_info_settings.screenshot_size.y = atof(gameElem->Attribute("size_y"));
				}
				if (gameElemName == "genre_icon")
				{
					game_info_settings.genreIcon  = read_rectangle_elem(gameElem);
				}
				if (gameElemName == "company_logos")
				{
					game_info_settings.companyLogos = read_rectangle_elem(gameElem);
				}

			}
		}
	}


}