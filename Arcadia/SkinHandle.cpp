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
	font_item.fontName = elem->Attribute("file");
	font_item.size = atoi(elem->Attribute("size"));
	font_item.color = sf::Color::Color(atoi(elem->Attribute("red")), atoi(elem->Attribute("green")), atoi(elem->Attribute("blue")), atoi(elem->Attribute("alpha")) );
	
	for(tinyxml2::XMLElement* fontElem = elem->FirstChildElement(); fontElem != NULL; fontElem = fontElem->NextSiblingElement())
	{
		std::string elemName = fontElem->Value();
		if (elemName== "shadow")
		{
			font_item.shadow = true;
			font_item.shadowColor = sf::Color::Color(atoi(fontElem->Attribute("red")), atoi(fontElem->Attribute("green")), atoi(fontElem->Attribute("blue")), atoi(fontElem->Attribute("alpha")) );
			font_item.shadowOffset = atoi(fontElem->Attribute("offset"));
		}
	}

	return font_item;
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
				{
					for(tinyxml2::XMLElement* selectedElem = gameElem->FirstChildElement(); selectedElem != NULL; selectedElem = selectedElem->NextSiblingElement())
					{
						std::string selectedElemName = selectedElem->Value();
						if (selectedElemName == "font")
							game_list_settings.selected_font = read_font_elem(selectedElem);						
					}
				}
				if (gameElemName == "normal_item")
				{
					for(tinyxml2::XMLElement* normalElem = gameElem->FirstChildElement(); normalElem != NULL; normalElem = normalElem->NextSiblingElement())
					{
						std::string normalElemName = normalElem->Value();
						if (normalElemName == "font")
							game_list_settings.normal_font = read_font_elem(normalElem);						
					}										
				}
			}
		}
		if (elemName == "game_info")
		{

		}
	}


}