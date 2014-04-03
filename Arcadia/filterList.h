#pragma once
#include <SFML/Graphics.hpp>
#include "dbHandle.h"

class filterList
{
private:
	sf::RectangleShape rectangle;
	int selectedItemNum;
	
public:
	filterList(void);
	~filterList(void);
};

