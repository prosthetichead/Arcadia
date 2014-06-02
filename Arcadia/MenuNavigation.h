#pragma once
#include <boost\unordered_map.hpp>

class MenuNavigation
{
	private:
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

		std::string selected_ID;

		boost::unordered_map<std::string, menuItem> menuTree;

		menuItem& MenuNavigation::FindMenuItem(std::string id);

	public:
		enum movements{left,right,up,down};

		MenuNavigation(void);
		~MenuNavigation(void);
		
		bool MenuNavigation::move(movements movement);
		void MenuNavigation::addItem(std::string id, std::string left_id, std::string right_id, std::string up_id, std::string down_id, std::string type, std::string value, int posX, int posY);	

		std::string MenuNavigation::getSelectedType();
		std::string MenuNavigation::getSelectedValue();
		std::string MenuNavigation::getSelectedID();
		int MenuNavigation::getSelectedPosX();
		int MenuNavigation::getSelectedPosY();

		std::string MenuNavigation::getType(std::string id);
		std::string MenuNavigation::getValue(std::string id);
		int MenuNavigation::getPosX(std::string id);
		int MenuNavigation::getPosY(std::string id);
		
};

