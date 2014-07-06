#pragma once
#include <boost\unordered_map.hpp>
#include <vector>
#include <iostream>

class MenuNavigation
{
	public:
		struct menuItem
		{
			std::string id;
			std::string left_id;
			std::string right_id;
			std::string up_id;
			std::string down_id;

			std::string type;
			std::string value;

			int posX;
			int posY;
		};

		enum movements{left,right,up,down};

		bool selected;

		MenuNavigation(void);
		~MenuNavigation(void);
		
		bool MenuNavigation::move(movements movement);
		void MenuNavigation::addItem(std::string id, std::string left_id, std::string right_id, std::string up_id, std::string down_id, std::string type, std::string value, int posX, int posY);	
		std::vector<MenuNavigation::menuItem> MenuNavigation::getVector();
		std::vector<std::string> MenuNavigation::getIDVector();


		std::string MenuNavigation::getCurrentType();
		std::string MenuNavigation::getCurrentValue();
		std::string MenuNavigation::getCurrentID();
		int MenuNavigation::getCurrentPosX();
		int MenuNavigation::getCurrentPosY();


		int MenuNavigation::getPosX(std::string id);
		int MenuNavigation::getPosY(std::string id);
		menuItem& MenuNavigation::getMenuItem(std::string id);

	private:
		std::string current_ID;

		boost::unordered_map<std::string, menuItem> menuTree;

		menuItem& MenuNavigation::FindMenuItem(std::string id);
		
};

