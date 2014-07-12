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
				if (gameElemName == "width")
					game_list_settings.size.x = atof(gameElem->GetText());
				if (gameElemName == "height")
					game_list_settings.size.y = atof(gameElem->GetText());
				if (gameElemName == "pos_x")
					game_list_settings.position.x = atof(gameElem->GetText());
				if (gameElemName == "pos_y")
					game_list_settings.position.y = atof(gameElem->GetText());
				if (gameElemName == "selected_font")
				{
					game_list_settings.fontSelected.loadFromFile(exe_path + "\\assets\\fonts\\" + gameElem->GetText());
					game_list_settings.fontSelectedSize = atoi(gameElem->Attribute("size"));
					game_list_settings.fontSelectedColor = sf::Color::Color(atoi(gameElem->Attribute("red")), atoi(gameElem->Attribute("green")), atoi(gameElem->Attribute("blue")), atoi(gameElem->Attribute("alpha")) );
				}
				if (gameElemName == "normal_font")
				{
					game_list_settings.fontNormal.loadFromFile(exe_path + "\\assets\\fonts\\" + gameElem->GetText());
					game_list_settings.fontNormalSize = atoi(gameElem->Attribute("size"));
					game_list_settings.fontNormalColor = sf::Color::Color(atoi(gameElem->Attribute("red")), atoi(gameElem->Attribute("green")), atoi(gameElem->Attribute("blue")), atoi(gameElem->Attribute("alpha")) );
				}
			}
		}
	}


}