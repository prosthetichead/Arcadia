#pragma once
#include <iostream>
#include <map>
#include <string>
#include <SFML/Window.hpp>

class inputHandler
{

public:
	inputHandler(void);
	~inputHandler(void);
	void update(void);
	std::map<std::string, bool> buttonStates; //up, down, left, right, A, B, C, D, E, F, G, H
};

