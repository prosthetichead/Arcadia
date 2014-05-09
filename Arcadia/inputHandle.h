#pragma once
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

class inputHandle
{
public:
	struct inputState
	{
		sf::Keyboard::Key up_key;
		bool up_press;
		bool up_hold;

		sf::Keyboard::Key down_key;
		bool down_press;
		bool down_hold;
		
		sf::Keyboard::Key left_key;
		bool left_press;
		bool left_hold;
		
		bool right_press;
		bool right_hold;

		bool start_game_press;
		bool start_game_hold;

		bool platform_filter_left_press;
		bool platform_filter_left_hold;
		
		bool platform_filter_right_press;
		bool platform_filter_right_hold;

		bool user_filter_left_press;
		bool user_filter_left_hold;
		
		bool user_filter_right_press;
		bool user_filter_right_hold;

		bool show_favs_press;
		bool show_favs_hold;
		
		bool set_fav_press;
		bool set_fav_hold;

		bool exit_press;
		bool exit_hold;
	};
	enum inputs
	{
		up = 1,
		down = 2,
		left = 3,
		right = 4,

	};

	struct inputItem
	{
		std::string inputName;
		std::string inputID;

		std::string inputType;  // joystick, keyboard, mouse
		sf::Keyboard::Key key;
		int buttonNumber;
		bool press;
		bool hold;
		bool repeat;
		int counter;
		int repeat_time;
	};

	
	inputHandle(void);
	~inputHandle(void);
	inputState inputHandle::update();

private:
	bool repeat;
	int counter;
	int repeat_time;

	void inputHandle::clear();
	inputHandle::inputState inputStates;

};

