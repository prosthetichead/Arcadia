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
		platform_filter_left,
		platform_filter_right,
		filter_menu,
		edit_game,
		set_fav,
		settings,
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

