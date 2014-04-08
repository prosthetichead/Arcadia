#pragma once
#include <SFML/Window.hpp>
#include <iostream>
#include <string>

class inputHandle
{
public:
	struct inputState
	{
		bool up_press;
		bool up_hold;
		bool down_press;
		bool down_hold;
		bool left_press;
		bool left_hold;
		bool right_press;
		bool right_hold;
		bool btn_1_press;
		bool btn_1_hold;
		bool btn_2_press;
		bool btn_2_hold;
		bool exit_press;
		bool exit_hold;
	};

	inputHandle(void);
	~inputHandle(void);
	inputState inputHandle::update();

private:
	bool repeat;
	int counter;
	void inputHandle::clear();
	inputHandle::inputState inputStates;
};

