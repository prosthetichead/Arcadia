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
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.left_press = true;
			counter = 0;
		}
		else
		{
			inputStates.left_press = false;
			counter++;
			if (counter > 10)
				inputStates.left_hold = true;
		}	
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.right_press = true;
			counter = 0;
		}
		else
		{
			inputStates.right_press = false;
			counter++;
			if (counter > 10)
				inputStates.right_hold = true;
		}	
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))					 //   btn_1
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.btn_1_press = true;
			counter = 0;
		}
		else
		{
			inputStates.btn_1_press = false;
			counter++;
			if (counter > 10)
				inputStates.btn_1_hold = true;
		}	
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))				//		Exit
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.exit_press = true;
			counter = 0;
		}
		else
		{
			inputStates.exit_press = false;
			counter++;
			if (counter > 10)
				inputStates.exit_hold = true;
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
	inputStates.exit_press = false;
	inputStates.exit_hold = false;
	repeat = false;
	counter = 0;
}

