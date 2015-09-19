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
	//work out which layout to use. for now lets just use skins/main.xml
	xml_path = db.exe_path + "\\skins\\main.xml";
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
		return_element.origin_code = elem->Attribute("origin");	
	if(elem->Attribute("png_file") != NULL)
		return_element.asset_file = elem->Attribute("png_file");

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
			if(rectElem->Attribute("line_spacing") != NULL)
				return_element.line_spacing = atoi(rectElem->Attribute("line_spacing"));
			if(rectElem->Attribute("indent") != NULL)
				return_element.indent = atoi(rectElem->Attribute("indent"));

			return_element.asset_file = rectElem->Attribute("file");

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
		}

	}

	return return_element;
}

void SkinHandle::loadLayout()
{
	tinyxml2::XMLDocument doc;
	tinyxml2::XMLNode *rootnode;
	
	doc.LoadFile(xml_path.c_str());
	rootnode = doc.RootElement();
	
	if (rootnode->ToElement()->Attribute("res_width") != NULL && rootnode->ToElement()->Attribute("res_height") != NULL) {
		resolution.x = atof(rootnode->ToElement()->Attribute("res_width"));
		resolution.y = atof(rootnode->ToElement()->Attribute("res_height"));
		skin_id = rootnode->ToElement()->Attribute("id");
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
		Skin_Element se = read_skin_elem(elem);

		if (elemName == "game_list")
		{
			se.type = element_type::game_list;
			game_list_settings.list = se;			
			skin_elements.push_back(game_list_settings.list);

			for(tinyxml2::XMLElement* gameElem = elem->FirstChildElement(); gameElem != NULL; gameElem = gameElem->NextSiblingElement())
			{
				std::string gameElemName = gameElem->Value();
				if (gameElemName == "game_list_selected_item")
				{
					game_list_settings.selected_list_item = read_skin_elem(gameElem);	
					game_list_settings.selected_list_item.origin_code = "CL";
				}
				if (gameElemName == "game_list_normal_item")
				{
					game_list_settings.normal_list_item = read_skin_elem(gameElem);	
					game_list_settings.normal_list_item.origin_code = "CL";
				}
			}
		}
		else if (elemName == "static_rectangle") {				
			se.type = element_type::static_rectangle;
			skin_elements.push_back(se);
		}
		else if (elemName == "static_text") {				
			se.type = element_type::static_text;
			skin_elements.push_back(se);
		}
		else if (elemName == "static_image") {				
			se.type = element_type::static_image;
			skin_elements.push_back(se);
		}
		else if (elemName == "fanart") {				
			se.type = element_type::fanart;	
			skin_elements.push_back(se);
		}
		else if (elemName == "clear_logo") {
			se.type = element_type::clear_logo;
			skin_elements.push_back(se);
		}
		else if (elemName == "description") {
			se.type = element_type::description;
			skin_elements.push_back(se);
		}
		else if (elemName == "platform_icon") {
			se.type = element_type::platform_icon;
			skin_elements.push_back(se);
		}
		else if (elemName == "genres") {
			se.type = element_type::genres;
			skin_elements.push_back(se);
		}
		else if (elemName == "screenshot") {
			se.type = element_type::screenshot;
			skin_elements.push_back(se);
		}
		else if (elemName == "video") {
			se.type = element_type::video;
			skin_elements.push_back(se);
		}
		else if (elemName == "developers") {
			se.type = element_type::developers;
			skin_elements.push_back(se);
		}
		else if (elemName == "publishers") {
			se.type = element_type::publishers;
			skin_elements.push_back(se);
		}
		else if (elemName == "year") {
			se.type = element_type::year;
			skin_elements.push_back(se);
		}
		else if (elemName == "play_time") {
			se.type = element_type::play_time;
			skin_elements.push_back(se);
		}
		else if (elemName == "last_played") {
			se.type = element_type::last_played;
			skin_elements.push_back(se);
		}
		else if (elemName == "players") {
			se.type = element_type::players;
			skin_elements.push_back(se);
		}
	}

	
}