#include "inputHandle.h"

inputHandle::inputHandle(void)
{
	clear(); //resets the inputStates 
	repeat_time  = 15;
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
			if (counter > repeat_time)
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
			if (counter > repeat_time)
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
			if (counter > repeat_time)
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
			if (counter > repeat_time)
				inputStates.right_hold = true;
		}	
	}

	//----------------------------------
	// PLATFORM FILTER LEFT AND RIGHT
	//----------------------------------
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))					 //   Platform Filter Left
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.platform_filter_left_press = true;
			counter = 0;
		}
		else
		{
			inputStates.platform_filter_left_press = false;
			counter++;
			if (counter > repeat_time)
				inputStates.platform_filter_left_hold = true;
		}	
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))					 //   Platform Filter Right
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.platform_filter_right_press = true;
			counter = 0;
		}
		else
		{
			inputStates.platform_filter_right_press = false;
			counter++;
			if (counter > repeat_time)
				inputStates.platform_filter_right_hold = true;
		}	
	}
	//----------------------------------
	// USER FILTER LEFT AND RIGHT
	//----------------------------------
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))					 //   user Filter Left
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.user_filter_left_press = true;
			counter = 0;
		}
		else
		{
			inputStates.user_filter_left_press = false;
			counter++;
			if (counter > repeat_time)
				inputStates.user_filter_left_hold = true;
		}	
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))					 //   user Filter Right
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.user_filter_right_press = true;
			counter = 0;
		}
		else
		{
			inputStates.user_filter_right_press = false;
			counter++;
			if (counter > repeat_time)
				inputStates.user_filter_right_hold = true;
		}	
	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))				//		START GAME
	{
		if (!repeat)
		{
			repeat = true;
			inputStates.start_game_press = true;
			counter = 0;
		}
		else
		{
			inputStates.start_game_press = false;
			counter++;
			if (counter > repeat_time)
				inputStates.start_game_hold = true;
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
			if (counter > repeat_time)
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

	inputStates.platform_filter_left_press = false;
	inputStates.platform_filter_right_press = false;
	inputStates.platform_filter_left_hold = false;
	inputStates.platform_filter_right_hold = false;

	inputStates.user_filter_left_press = false;
	inputStates.user_filter_right_press = false;
	inputStates.user_filter_left_hold = false;
	inputStates.user_filter_right_hold = false;

	inputStates.start_game_press = false;
	inputStates.start_game_hold = false;

	inputStates.exit_press = false;
	inputStates.exit_hold = false;

	repeat = false;
	counter = 0;
}

