#include "inputHandler.h"


inputHandler::inputHandler(void)
{
	buttonStates["up"] = false;
	buttonStates["down"] = false;
	buttonStates["left"] = false;
	buttonStates["right"] = false;
}


inputHandler::~inputHandler(void)
{

}

void inputHandler::update(void)
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		buttonStates["up"] = true;
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
		buttonStates["up"] = true;
	else
	{
		buttonStates["up"] = false;
		buttonStates["down"] = false;
		buttonStates["left"] = false;
		buttonStates["right"] = false;
	}
}