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
		filter_left,
		filter_right,
		settings,
		extra,
		exit_game
	};
	inputHandle();	
	~inputHandle(void);
	void inputHandle::init(dbHandle *db_obj, sf::Window *window_ref);
	void inputHandle::update();
	bool inputHandle::inputPress(inputs input);
	bool inputHandle::inputHold(inputs input);
	void inputHandle::checkEventForInput(sf::Event event);
	//dbHandle::inputItem inputHandle::waitForInput(inputs input);
	const char *inputHandle::getKeyName( const sf::Keyboard::Key key );
	dbHandle::inputItem& inputHandle::getInputItem(inputs input);
	std::vector<dbHandle::inputItem>& inputHandle::getInputStates();
	void inputHandle::waitAndUpdateInput(inputs input);


private:
	sf::Window *window;
	dbHandle *db;
	int repeat_time;
	void inputHandle::clear();
	int lastInputID;
	int lastInputCounter;
	bool lastInput;
	std::vector<dbHandle::inputItem> inputStates;

};

