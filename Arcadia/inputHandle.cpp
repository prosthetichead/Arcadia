#include "inputHandle.h"

inputHandle::inputHandle()
{ 
	repeat_time  = 15;

}

void inputHandle::init(dbHandle &db_obj)
{
	dbHandle::inputItem item;
	item = db_obj.getInputItem(inputs::up);
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::down);
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::left); 
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::right);
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::start_game);
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::platform_filter_left);
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::platform_filter_right);
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::filter_menu);
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::edit_game);
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::set_fav);
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::settings);
	inputStates.push_back(item);
	item = db_obj.getInputItem(inputs::exit);
	inputStates.push_back(item);

	clear(); //resets all the inputStates
}


inputHandle::~inputHandle(void)
{

}

void inputHandle::update()
{
	for (auto &item : inputStates)
	{
		if (sf::Keyboard::isKeyPressed(item.key))
		{
			if (!item.repeat)
			{
				item.repeat = true;
				item.press = true;
				item.counter = 0;
			}
			else
			{
				item.press = false;
				item.counter++;
				if(item.counter > repeat_time)
					item.hold = true;
			}

		}
		else
		{
			item.press = false;
			item.hold = false;
			item.repeat = false;
			item.counter = 0;	
		}
	}
}

bool inputHandle::inputPress(inputs input)
{
	return inputStates.at((int)input).press;
}
bool inputHandle::inputHold(inputs input)
{
	return inputStates.at((int)input).hold;
}

void inputHandle::clear()
{
	for (auto &item : inputStates) {		
		item.press = false;
		item.hold = false;
		item.repeat = false;
		item.counter = 0;		
	}
}

