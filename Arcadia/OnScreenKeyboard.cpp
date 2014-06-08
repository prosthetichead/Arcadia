#include "OnScreenKeyboard.h"


OnScreenKeyboard::OnScreenKeyboard(dbHandle &db_ref, assetHandle &ah_ref):
	db(db_ref),
	ah(ah_ref)
{
	normalButtonSize.x = 32;
	normalButtonSize.y = 32;
	spaceBarSize.x = 128;
	spaceBarSize.y = 32;
	sysButtonSize.x = 64;
	sysButtonSize.y = 32;
	buttonPaddingSize.x = 8;
	buttonPaddingSize.y = 8;

	userEnteredString = "";
}


OnScreenKeyboard::~OnScreenKeyboard(void)
{
}

void OnScreenKeyboard::init(float posX, float posY)
{
	keyBackTexture.loadFromFile(db.exe_path + "\\assets\\system\\keyBack.png");
	keyFont.loadFromFile(db.exe_path + "\\assets\\fonts\\ARCADE_PIX.TTF");
	keyText.setFont(keyFont);
	keyText.setCharacterSize(22);
	keyText.setColor(sf::Color::White);
	userEnteredText.setFont(keyFont);
	userEnteredText.setCharacterSize(22);
	userEnteredText.setColor(sf::Color::White);
	userEnteredText.setPosition(0,0);
		
	menuNav.addItem("1","", "2","","q","char","1",posX+(buttonPaddingSize.x+normalButtonSize.x)*1,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("2","1","3","","w","char","2",posX+(buttonPaddingSize.x+normalButtonSize.x)*2,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("3","2","4","","e","char","3",posX+(buttonPaddingSize.x+normalButtonSize.x)*3,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("4","3","5","","r","char","4",posX+(buttonPaddingSize.x+normalButtonSize.x)*4,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("5","4","6","","t","char","5",posX+(buttonPaddingSize.x+normalButtonSize.x)*5,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("6","5","7","","y","char","6",posX+(buttonPaddingSize.x+normalButtonSize.x)*6,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("7","6","8","","u","char","7",posX+(buttonPaddingSize.x+normalButtonSize.x)*7,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("8","7","9","","i","char","8",posX+(buttonPaddingSize.x+normalButtonSize.x)*8,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("9","8","0","","o","char","9",posX+(buttonPaddingSize.x+normalButtonSize.x)*9,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("0","9", "-","","p","char","0",posX+(buttonPaddingSize.x+normalButtonSize.x)*10,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("-","0", "=","","[","char","-",posX+(buttonPaddingSize.x+normalButtonSize.x)*11,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("=","-", "back_space","","]","char","=",posX+(buttonPaddingSize.x+normalButtonSize.x)*12,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);
	menuNav.addItem("back_space","=","","","\\","back_space","back_space",posX+(buttonPaddingSize.x+normalButtonSize.x)*13,posY+(buttonPaddingSize.y+normalButtonSize.y)*1);


	menuNav.addItem("q", "","w","1","a","char","q",posX+(buttonPaddingSize.x+normalButtonSize.x)*1,posY+(buttonPaddingSize.y+normalButtonSize.y)*2);
	menuNav.addItem("w","q","e","2","s","char","w",posX+(buttonPaddingSize.x+normalButtonSize.x)*2,posY+(buttonPaddingSize.y+normalButtonSize.y)*2);
	menuNav.addItem("e","w","r","3","d","char","e",posX+(buttonPaddingSize.x+normalButtonSize.x)*3,posY+(buttonPaddingSize.y+normalButtonSize.y)*2);
	menuNav.addItem("r","e","t","4","f","char","r",posX+(buttonPaddingSize.x+normalButtonSize.x)*4,posY+(buttonPaddingSize.y+normalButtonSize.y)*2);
	menuNav.addItem("t","r","y","5","g","char","t",posX+(buttonPaddingSize.x+normalButtonSize.x)*5,posY+(buttonPaddingSize.y+normalButtonSize.y)*2);
	menuNav.addItem("y","t","u","6","h","char","y",posX+(buttonPaddingSize.x+normalButtonSize.x)*6,posY+(buttonPaddingSize.y+normalButtonSize.y)*2);
	menuNav.addItem("u","y","i","7","j","char","u",posX+(buttonPaddingSize.x+normalButtonSize.x)*7,posY+(buttonPaddingSize.y+normalButtonSize.y)*2);
	menuNav.addItem("i","u","o","8","k","char","i",posX+(buttonPaddingSize.x+normalButtonSize.x)*8,posY+(buttonPaddingSize.y+normalButtonSize.y)*2);
	menuNav.addItem("o","i","p","9","l","char","o",posX+(buttonPaddingSize.x+normalButtonSize.x)*9,posY+(buttonPaddingSize.y+normalButtonSize.y)*2);
	menuNav.addItem("p","o","","0",  "","char","p",posX+(buttonPaddingSize.x+normalButtonSize.x)*10,posY+(buttonPaddingSize.y+normalButtonSize.y)*2);

	menuNav.addItem("a","", "s","q","z","char","a",posX+(buttonPaddingSize.x+normalButtonSize.x)*1,posY+(buttonPaddingSize.y+normalButtonSize.y)*3);
	menuNav.addItem("s","a","d","w","x","char","s",posX+(buttonPaddingSize.x+normalButtonSize.x)*2,posY+(buttonPaddingSize.y+normalButtonSize.y)*3);
	menuNav.addItem("d","s","f","e","c","char","d",posX+(buttonPaddingSize.x+normalButtonSize.x)*3,posY+(buttonPaddingSize.y+normalButtonSize.y)*3);
	menuNav.addItem("f","d","g","r","v","char","f",posX+(buttonPaddingSize.x+normalButtonSize.x)*4,posY+(buttonPaddingSize.y+normalButtonSize.y)*3);
	menuNav.addItem("g","f","h","t","b","char","g",posX+(buttonPaddingSize.x+normalButtonSize.x)*5,posY+(buttonPaddingSize.y+normalButtonSize.y)*3);
	menuNav.addItem("h","g","j","y","n","char","h",posX+(buttonPaddingSize.x+normalButtonSize.x)*6,posY+(buttonPaddingSize.y+normalButtonSize.y)*3);
	menuNav.addItem("j","h","k","u","m","char","j",posX+(buttonPaddingSize.x+normalButtonSize.x)*7,posY+(buttonPaddingSize.y+normalButtonSize.y)*3);
	menuNav.addItem("k","j","l","i", "","char","k",posX+(buttonPaddingSize.x+normalButtonSize.x)*8,posY+(buttonPaddingSize.y+normalButtonSize.y)*3);
	menuNav.addItem("l","k","", "o", "","char","l",posX+(buttonPaddingSize.x+normalButtonSize.x)*9,posY+(buttonPaddingSize.y+normalButtonSize.y)*3);

	menuNav.addItem("z", "", "x","a","","char","z",posX+(buttonPaddingSize.x+normalButtonSize.x)*1,posY+(buttonPaddingSize.y+normalButtonSize.y)*4);
	menuNav.addItem("x","z","c","s", "","char","x",posX+(buttonPaddingSize.x+normalButtonSize.x)*2,posY+(buttonPaddingSize.y+normalButtonSize.y)*4);
	menuNav.addItem("c","x","v","d", "","char","c",posX+(buttonPaddingSize.x+normalButtonSize.x)*3,posY+(buttonPaddingSize.y+normalButtonSize.y)*4);
	menuNav.addItem("v","c","b","f", "","char","v",posX+(buttonPaddingSize.x+normalButtonSize.x)*4,posY+(buttonPaddingSize.y+normalButtonSize.y)*4);
	menuNav.addItem("b","v","n","g", "","char","b",posX+(buttonPaddingSize.x+normalButtonSize.x)*5,posY+(buttonPaddingSize.y+normalButtonSize.y)*4);
	menuNav.addItem("n","b","m","h", "","char","n",posX+(buttonPaddingSize.x+normalButtonSize.x)*6,posY+(buttonPaddingSize.y+normalButtonSize.y)*4);
	menuNav.addItem("m","n",",","j", "","char","m",posX+(buttonPaddingSize.x+normalButtonSize.x)*7,posY+(buttonPaddingSize.y+normalButtonSize.y)*4);
	menuNav.addItem(",","m",".","k", "","char",",",posX+(buttonPaddingSize.x+normalButtonSize.x)*8,posY+(buttonPaddingSize.y+normalButtonSize.y)*4);
	menuNav.addItem(".",",","", "l", "","char",".",posX+(buttonPaddingSize.x+normalButtonSize.x)*9,posY+(buttonPaddingSize.y+normalButtonSize.y)*4);

	KeyboardItemRefs = menuNav.getValueRefVector();


}

bool OnScreenKeyboard::update(inputHandle& ih)
{
	if (ih.inputPress(inputHandle::inputs::start_game))
	{
		if (menuNav.getCurrentType() == "char")
		{
			userEnteredString += menuNav.getCurrentValue();
			userEnteredText.setString(userEnteredString);
		}
	}
	if (ih.inputPress(inputHandle::inputs::left))
		menuNav.move(MenuNavigation::movements::left);
	if (ih.inputPress(inputHandle::inputs::right))
		menuNav.move(MenuNavigation::movements::right);
	if (ih.inputPress(inputHandle::inputs::up))
		menuNav.move(MenuNavigation::movements::up);
	if (ih.inputPress(inputHandle::inputs::down))
		menuNav.move(MenuNavigation::movements::down);
	
	return false;
}

void OnScreenKeyboard::draw(sf::RenderWindow& window)
{
	std::string currentID = menuNav.getCurrentID();

	window.draw(userEnteredText);

	for(auto keyItem : KeyboardItemRefs) 
	{
		if (keyItem.type == "char")
		{
			sf::Sprite keyBackSprite;
			keyBackSprite.setTexture(keyBackTexture);
			keyBackSprite.setPosition(keyItem.posX, keyItem.posY);
			keyBackSprite.setOrigin(keyBackSprite.getLocalBounds().width/2, keyBackSprite.getLocalBounds().height/2);

			keyText.setString(keyItem.id);
			keyText.setOrigin(0,0);
			keyText.setPosition(keyItem.posX - keyBackSprite.getLocalBounds().width/4, keyItem.posY - keyBackSprite.getLocalBounds().height/2);
			if (currentID == keyItem.id)
			{
				keyText.setColor(sf::Color::Cyan);
				//keyText.setScale(1.2,1.2);
				keyBackSprite.setScale(1.3,1.3);
			}
			else
			{
				keyText.setColor(sf::Color::White);
				keyText.setScale(1,1);
				keyBackSprite.setScale(1,1);
			}		
			window.draw(keyBackSprite);
			window.draw(keyText);
		}
	}
}
