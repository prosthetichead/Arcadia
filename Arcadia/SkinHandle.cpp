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
	windowStyle = sf::Style::Default;

	//load gamelist settings from xml
	loadLayout();
}


SkinHandle::Skin_Element SkinHandle::read_skin_elem(tinyxml2::XMLElement* elem)
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
		if (rectElemName == "text")
		{
			return_element.text = rectElem->GetText();
			printf(return_element.text.c_str());
		}

	}

	return return_element;
}

void SkinHandle::loadLayout()
{
	tinyxml2::XMLDocument doc;
	doc.LoadFile(xml_path.c_str());
	tinyxml2::XMLNode *rootnode = doc.RootElement();

	if (rootnode->ToElement()->Attribute("res_width") != NULL && rootnode->ToElement()->Attribute("res_height") != NULL) {
		resolution.x = atof(rootnode->ToElement()->Attribute("res_width"));
		resolution.y = atof(rootnode->ToElement()->Attribute("res_height"));
	}
	if (rootnode->ToElement()->Attribute("screen_mode") != NULL){
		std::string screen_mode = rootnode->ToElement()->Attribute("screen_mode");
		if (screen_mode == "fullscreen")
			windowStyle = sf::Style::Fullscreen;
		if (screen_mode == "none")
			windowStyle = sf::Style::None;
		if (screen_mode == "window")
			windowStyle = sf::Style::Default;
	}


	for(tinyxml2::XMLElement* elem = rootnode->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement())
	{
		std::string elemName = elem->Value();
		if (elemName == "game_list")
		{
			for(tinyxml2::XMLElement* gameElem = elem->FirstChildElement(); gameElem != NULL; gameElem = gameElem->NextSiblingElement())
			{
				std::string gameElemName = gameElem->Value();
				if (gameElemName == "list")
					game_list_settings.list = read_skin_elem(gameElem);
				if (gameElemName == "selected_item")
					game_list_settings.selected_list_item = read_skin_elem(gameElem);						
				if (gameElemName == "normal_item")
					game_list_settings.normal_list_item = read_skin_elem(gameElem);																	
			}
		}
		if (elemName == "game_info")
		{
			for(tinyxml2::XMLElement* gameElem = elem->FirstChildElement(); gameElem != NULL; gameElem = gameElem->NextSiblingElement())
			{
				std::string gameElemName = gameElem->Value();
				Skin_Element se = read_skin_elem(gameElem);

				if (gameElemName == "static_rectangle") {				
					se.type = element_type::static_rectangle;
					game_info_elements.push_back(se);
				}

				if (gameElemName == "static_text") {				
					se.type = element_type::static_text;
					game_info_elements.push_back(se);
				}
			
				else if (gameElemName == "fanart") {				
					se.type = element_type::fanart;	
					game_info_elements.push_back(se);
				}

				else if (gameElemName == "clear_logo") {
					se.type = element_type::clear_logo;
					game_info_elements.push_back(se);
				}
				
				else if (gameElemName == "description") {
					se.type = element_type::description;
					game_info_elements.push_back(se);
				}

				else if (gameElemName == "platform_icon") {
					se.type = element_type::platform_icon;
					game_info_elements.push_back(se);
				}

				else if (gameElemName == "genres") {
					se.type = element_type::genres;
					game_info_elements.push_back(se);
				}

				else if (gameElemName == "screenshot") {
					se.type = element_type::screenshot;
					game_info_elements.push_back(se);
				}

				else if (gameElemName == "video") {
					se.type = element_type::video;
					game_info_elements.push_back(se);
				}
				
				else if (gameElemName == "developers") {
					se.type = element_type::developers;
					game_info_elements.push_back(se);
				}

				else if (gameElemName == "publishers") {
					se.type = element_type::publishers;
					game_info_elements.push_back(se);
				}
				
				else if (gameElemName == "year") {
					se.type = element_type::year;
					game_info_elements.push_back(se);
				}
				
				else if (gameElemName == "play_time") {
					se.type = element_type::play_time;
					game_info_elements.push_back(se);
				}

				else if (gameElemName == "last_played") {
					se.type = element_type::last_played;
					game_info_elements.push_back(se);
				}

				else if (gameElemName == "players") {
					se.type = element_type::players;
					game_info_elements.push_back(se);
				}
			}
		}

		
	}


}