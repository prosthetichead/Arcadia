#include "MenuNavigation.h"


MenuNavigation::MenuNavigation(void)
{
}


MenuNavigation::~MenuNavigation(void)
{
}

MenuNavigation::menuItem& MenuNavigation::FindMenuItem(std::string id)
{
	return menuTree[id];
	//boost::unordered_map<std::string, menuItem>::iterator iter = menuTree.find(id);
	//if(iter != menuTree.end()) return iter->second;
	//else
	//{
	//	//std::cout << "ERROR: MENU ID DOES NOT EXIST: " << id std::endl;
	//	menuItem newitem;
	//	return newitem;
	//}
}

void MenuNavigation::addItem(std::string id, std::string left_id, std::string right_id, std::string up_id, std::string down_id, std::string type, std::string value, int posX, int posY)
{
	menuItem newItem;
	newItem.id = id;
	newItem.left_id = left_id;
	newItem.right_id = right_id;
	newItem.down_id = down_id;
	newItem.up_id = up_id;
	newItem.type = type;
	newItem.value = value;
	newItem.posX = posX;
	newItem.posY = posY;

	std::pair<std::string, menuItem> pair (id,newItem);
	menuTree.insert(pair);

	if (current_ID.empty())   // do we have a selected item yet? if not set it to this one
		current_ID = id;
}

bool MenuNavigation::move(movements movement)
{
	menuItem& currentItem = menuTree[current_ID]; 

	switch (movement)
	{
	case MenuNavigation::left:
		if(!currentItem.left_id.empty()){  // id is not empty
			current_ID = currentItem.left_id;
			return true;
		}
		else
			return false;

	case MenuNavigation::right:
		if(!currentItem.right_id.empty()){  
			current_ID = currentItem.right_id;
			return true;
		}
		else
			return false;

	case MenuNavigation::up:
		if(!currentItem.up_id.empty()){  
			current_ID = currentItem.up_id;
			return true;
		}
		else
			return false;

	case MenuNavigation::down:
		if(!currentItem.down_id.empty()){ 
			current_ID = currentItem.down_id;
			return true;
		}
		else
			return false;

	default:
		return false;
	}
}

std::vector<MenuNavigation::menuItem> MenuNavigation::getValueRefVector()
{
	std::vector<MenuNavigation::menuItem> valueVector;
		
	for(auto kv : menuTree) 
	{
		std::cout << kv.second.id << " POS X: " << kv.second.posX << " POS Y: " <<  kv.second.posY << std::endl;
		valueVector.push_back(kv.second);
	}

	return valueVector;
}


int MenuNavigation::getCurrentPosX()
{
	menuItem& currentItem = menuTree[current_ID];
	return currentItem.posX;
}
int MenuNavigation::getCurrentPosY()
{
	menuItem& currentItem = menuTree[current_ID];
	return currentItem.posY;
}
std::string MenuNavigation::getCurrentType()
{
	menuItem& currentItem = menuTree[current_ID];
	return currentItem.type;
}
std::string MenuNavigation::getCurrentValue()
{
	menuItem& currentItem = menuTree[current_ID];
	return currentItem.value;
}

std::string MenuNavigation::getCurrentID()
{
	return current_ID;
}