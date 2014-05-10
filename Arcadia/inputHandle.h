#pragma once
#include <SFML/Window.hpp>
#include <iostream>
#include <string>
#include "dbHandle.h"

class inputHandle
{
public:
	enum inputs
	{
		up,
		down,
		left,
		right,
		start_game,
		filter_1_left,
		filter_1_right,
		filter_2_left,
		filter_2_right,
		show_favs,
		set_fav,
		exit
	};
	inputHandle();	
	~inputHandle(void);
	void inputHandle::init(dbHandle &db_obj);
	void inputHandle::update();
	bool inputHandle::inputPress(inputs input);
	bool inputHandle::inputHold(inputs input);

private:
	int repeat_time;

	void inputHandle::clear();
	std::vector<dbHandle::inputItem> inputStates;

};

