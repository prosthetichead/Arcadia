#include "inputHandle.h"

inputHandle::inputHandle(void)
{
	clear(); //resets the inputStates 
}


inputHandle::~inputHandle(void)
{

}

inputHandle::inputState inputHandle::update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.up_press = true;
			counter = 0;
		}
		else
		{
			inputStates.up_press = false;
			counter++;
			if (counter > 10)
				inputStates.up_hold = true;
		}	
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.down_press = true;
			counter = 0;
		}
		else
		{
			inputStates.down_press = false;
			counter++;
			if (counter > 10)
				inputStates.down_hold = true;
		}	
	}
	else
	{
		clear();
	}
	return inputStates;
}

void inputHandle::clear()
{
	inputStates.up_press = false;
	inputStates.up_hold = false;
	inputStates.down_press = false;
	inputStates.down_hold = false;
	inputStates.left_press = false;
	inputStates.left_hold = false;
	inputStates.right_press = false;
	inputStates.right_hold = false;
	inputStates.btn_1_press = false;
	inputStates.btn_1_hold = false;
	inputStates.btn_2_press = false;
	inputStates.btn_2_hold = false;
	repeat = false;
	counter = 0;
}

