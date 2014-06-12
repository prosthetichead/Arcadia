#include "OnScreenKeyboard.h"


OnScreenKeyboard::OnScreenKeyboard(dbHandle &db_ref, assetHandle &ah_ref):
	db(db_ref),
	ah(ah_ref)
{
	normalButtonSize.x = 32;
	normalButtonSize.y = 32;
	spaceBarSize.x = 272;
	spaceBarSize.y = 32;
	sysButtonSize.x = 72;
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
	keySysBackTexture.loadFromFile(db.exe_path + "\\assets\\system\\keySysBack.png");
	keySpaceBackTexture.loadFromFile(db.exe_path + "\\assets\\system\\keySpaceBack.png");
	keyFont.loadFromFile(db.exe_path + "\\assets\\fonts\\ARCADE_PIX.TTF");
	keyText.setFont(keyFont);
	keyText.setCharacterSize(22);
	keyText.setColor(sf::Color::White);
	userEnteredText.setFont(keyFont);
	userEnteredText.setCharacterSize(22);
	userEnteredText.setColor(sf::Color::White);
	userEnteredText.setPosition(posX+10,posY+10);

	keyboardRectangle.setSize(sf::Vector2f(576,288));
	keyboardRectangle.setPosition(posX,posY);
	keyboardRectangle.setOutlineColor(sf::Color::White);
	keyboardRectangle.setFillColor(sf::Color::Black);
	keyboardRectangle.setOutlineThickness(3);
		
	menuNav.addItem("1","", "2","","q","char","1", (posX+normalButtonSize.x*1)+(buttonPaddingSize.x*0)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2) );
	menuNav.addItem("2","1","3","","w","char","2", (posX+normalButtonSize.x*2)+(buttonPaddingSize.x*1)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2) );
	menuNav.addItem("3","2","4","","e","char","3", (posX+normalButtonSize.x*3)+(buttonPaddingSize.x*2)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	menuNav.addItem("4","3","5","","r","char","4", (posX+normalButtonSize.x*4)+(buttonPaddingSize.x*3)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	menuNav.addItem("5","4","6","","t","char","5", (posX+normalButtonSize.x*5)+(buttonPaddingSize.x*4)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	menuNav.addItem("6","5","7","","y","char","6", (posX+normalButtonSize.x*6)+(buttonPaddingSize.x*5)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	menuNav.addItem("7","6","8","","u","char","7", (posX+normalButtonSize.x*7)+(buttonPaddingSize.x*6)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	menuNav.addItem("8","7","9","","i","char","8", (posX+normalButtonSize.x*8)+(buttonPaddingSize.x*7)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	menuNav.addItem("9","8","0","","o","char","9", (posX+normalButtonSize.x*9)+(buttonPaddingSize.x*8)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	menuNav.addItem("0","9","-","","p","char","0", (posX+normalButtonSize.x*10)+(buttonPaddingSize.x*9)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	menuNav.addItem("-","0","+","","[","char","-", (posX+normalButtonSize.x*11)+(buttonPaddingSize.x*10)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	menuNav.addItem("+","-","=","","]","char","+", (posX+normalButtonSize.x*12)+(buttonPaddingSize.x*11)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	menuNav.addItem("=","+","","","\\","char","=", (posX+normalButtonSize.x*13)+(buttonPaddingSize.x*12)+(normalButtonSize.x/2), (posY+normalButtonSize.y*2)+(buttonPaddingSize.x*0)+(normalButtonSize.y/2)  );
	
	menuNav.addItem("q" , "", "w" ,"1","a","char","q", (posX+normalButtonSize.x*1)+(buttonPaddingSize.x*0)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2) );
	menuNav.addItem("w" ,"q", "e" ,"2","s","char","w", (posX+normalButtonSize.x*2)+(buttonPaddingSize.x*1)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2) );
	menuNav.addItem("e" ,"w", "r" ,"3","d","char","e", (posX+normalButtonSize.x*3)+(buttonPaddingSize.x*2)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2) );
	menuNav.addItem("r" ,"e", "t" ,"4","f","char","r", (posX+normalButtonSize.x*4)+(buttonPaddingSize.x*3)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2) );
	menuNav.addItem("t" ,"r", "y" ,"5","g","char","t", (posX+normalButtonSize.x*5)+(buttonPaddingSize.x*4)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2) );
	menuNav.addItem("y" ,"t", "u" ,"6","h","char","y", (posX+normalButtonSize.x*6)+(buttonPaddingSize.x*5)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2) );
	menuNav.addItem("u" ,"y", "i" ,"7","j","char","u", (posX+normalButtonSize.x*7)+(buttonPaddingSize.x*6)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2) );
	menuNav.addItem("i" ,"u", "o" ,"8","k","char","i", (posX+normalButtonSize.x*8)+(buttonPaddingSize.x*7)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2) );
	menuNav.addItem("o" ,"i", "p" ,"9","l","char","o", (posX+normalButtonSize.x*9)+(buttonPaddingSize.x*8)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2) );
	menuNav.addItem("p" ,"o", "(" ,"0",";","char","p", (posX+normalButtonSize.x*10)+(buttonPaddingSize.x*9)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2)  );
	menuNav.addItem("(" ,"p", ")" ,"-","'","char","(", (posX+normalButtonSize.x*11)+(buttonPaddingSize.x*10)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2)  );
	menuNav.addItem(")" ,"(", "\\","+","?","char",")", (posX+normalButtonSize.x*12)+(buttonPaddingSize.x*11)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2)  );
	menuNav.addItem("\\",")",  "" ,"=","!","char","\\", (posX+normalButtonSize.x*13)+(buttonPaddingSize.x*12)+(normalButtonSize.x/2), (posY+normalButtonSize.y*3)+(buttonPaddingSize.x*1)+(normalButtonSize.y/2)  );


	


	menuNav.addItem("a","", "s","q","z","char","a", (posX+normalButtonSize.x*1)+(buttonPaddingSize.x*0)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2) );
	menuNav.addItem("s","a","d","w","x","char","s", (posX+normalButtonSize.x*2)+(buttonPaddingSize.x*1)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2) );
	menuNav.addItem("d","s","f","e","c","char","d", (posX+normalButtonSize.x*3)+(buttonPaddingSize.x*2)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2) );
	menuNav.addItem("f","d","g","r","v","char","f", (posX+normalButtonSize.x*4)+(buttonPaddingSize.x*3)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2) );
	menuNav.addItem("g","f","h","t","b","char","g", (posX+normalButtonSize.x*5)+(buttonPaddingSize.x*4)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2) );
	menuNav.addItem("h","g","j","y","n","char","h", (posX+normalButtonSize.x*6)+(buttonPaddingSize.x*5)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2) );
	menuNav.addItem("j","h","k","u","m","char","j", (posX+normalButtonSize.x*7)+(buttonPaddingSize.x*6)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2) );
	menuNav.addItem("k","j","l","i",",","char","k", (posX+normalButtonSize.x*8)+(buttonPaddingSize.x*7)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2) );
	menuNav.addItem("l","k",";" ,"o",".","char","l",(posX+normalButtonSize.x*9)+(buttonPaddingSize.x*8)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2) );
	menuNav.addItem(";","l","'","p","/","char",";", (posX+normalButtonSize.x*10)+(buttonPaddingSize.x*9)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2)  );
	menuNav.addItem("'",";","?","(","$","char","'", (posX+normalButtonSize.x*11)+(buttonPaddingSize.x*10)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2)  );
	menuNav.addItem("?","'","!",")","@","char","?", (posX+normalButtonSize.x*12)+(buttonPaddingSize.x*11)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2)  );
	menuNav.addItem("!","?", "","\\","&","char","!",(posX+normalButtonSize.x*13)+(buttonPaddingSize.x*12)+(normalButtonSize.x/2), (posY+normalButtonSize.y*4)+(buttonPaddingSize.x*2)+(normalButtonSize.y/2)  );

	menuNav.addItem("z", "", "x","a","shift","char","z",(posX+normalButtonSize.x*1)+(buttonPaddingSize.x*0)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2) );
	menuNav.addItem("x","z","c","s", "shift","char","x",(posX+normalButtonSize.x*2)+(buttonPaddingSize.x*1)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2) );
	menuNav.addItem("c","x","v","d", "space","char","c",(posX+normalButtonSize.x*3)+(buttonPaddingSize.x*2)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2) );
	menuNav.addItem("v","c","b","f", "space","char","v",(posX+normalButtonSize.x*4)+(buttonPaddingSize.x*3)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2) );
	menuNav.addItem("b","v","n","g", "space","char","b",(posX+normalButtonSize.x*5)+(buttonPaddingSize.x*4)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2) );
	menuNav.addItem("n","b","m","h", "space","char","n",(posX+normalButtonSize.x*6)+(buttonPaddingSize.x*5)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2) );
	menuNav.addItem("m","n",",","j", "space","char","m",(posX+normalButtonSize.x*7)+(buttonPaddingSize.x*6)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2) );
	menuNav.addItem(",","m",".","k", "space","char",",",(posX+normalButtonSize.x*8)+(buttonPaddingSize.x*7)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2) );
	menuNav.addItem(".",",","/", "l","space","char",".",(posX+normalButtonSize.x*9)+(buttonPaddingSize.x*8)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2) );
	menuNav.addItem("/",".","$",";", "back_space","char","/", (posX+normalButtonSize.x*10)+(buttonPaddingSize.x*9)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2)  );
	menuNav.addItem("$","/","@","'", "back_space","char","$", (posX+normalButtonSize.x*11)+(buttonPaddingSize.x*10)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2)  );
	menuNav.addItem("@","$","&","?", "enter","char","@", (posX+normalButtonSize.x*12)+(buttonPaddingSize.x*11)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2)  );
	menuNav.addItem("&","@","","!","enter","char","&",(posX+normalButtonSize.x*13)+(buttonPaddingSize.x*12)+(normalButtonSize.x/2), (posY+normalButtonSize.y*5)+(buttonPaddingSize.x*3)+(normalButtonSize.y/2)  );

	menuNav.addItem("shift","","space","z","","sys","Shift",(posX+normalButtonSize.x)+(sysButtonSize.x/2), (posY+normalButtonSize.y*6)+(buttonPaddingSize.x*4)+(normalButtonSize.y/2));
	menuNav.addItem("space","shift","back_space","c","","space","Space Bar",(posX+normalButtonSize.x+sysButtonSize.x)+(buttonPaddingSize.x)+(spaceBarSize.x/2), (posY+normalButtonSize.y*6)+(buttonPaddingSize.x*4)+(normalButtonSize.y/2));
	menuNav.addItem("back_space","space","enter","/","","sys","Back\nSpace",(posX+normalButtonSize.x+sysButtonSize.x+spaceBarSize.x)+(buttonPaddingSize.x*2)+(sysButtonSize.x/2), (posY+normalButtonSize.y*6)+(buttonPaddingSize.x*4)+(normalButtonSize.y/2));
	menuNav.addItem("enter","back_space","","@","","sys","Enter",(posX+normalButtonSize.x+sysButtonSize.x*2+spaceBarSize.x)+(buttonPaddingSize.x*3)+(sysButtonSize.x/2), (posY+normalButtonSize.y*6)+(buttonPaddingSize.x*4)+(normalButtonSize.y/2));



	KeyboardItemRefs = menuNav.getValueRefVector();


}

std::string OnScreenKeyboard::getEnteredText()
{
	return userEnteredString;
}

bool OnScreenKeyboard::update(inputHandle& ih)
{
	menuNav.selected = false;
	if (ih.inputPress(inputHandle::inputs::start_game))
	{
		if (menuNav.getCurrentType() == "char")
		{
			userEnteredString += menuNav.getCurrentValue();
			userEnteredText.setString(userEnteredString);
		}
		else if (menuNav.getCurrentID() == "space")
		{
			userEnteredString += " ";
			userEnteredText.setString(userEnteredString);
		}
		else if (menuNav.getCurrentID() == "back_space" && userEnteredString.size() != 0)
		{
			userEnteredString.erase(userEnteredString.size()-1);
			userEnteredText.setString(userEnteredString);
		}
		else if (menuNav.getCurrentID() == "enter")
		{
			return true;
		}

		menuNav.selected = true;
	}
	if (ih.inputPress(inputHandle::inputs::left) || ih.inputHold(inputHandle::inputs::left) )
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

	window.draw(keyboardRectangle);
	window.draw(userEnteredText);

	for(auto keyItem : KeyboardItemRefs) 
	{
		sf::Sprite keyBackSprite;
		if (keyItem.type == "char")
		{
			keyBackSprite.setTexture(keyBackTexture);
			keyText.setCharacterSize(24);
		}
		else if (keyItem.type == "sys")
		{			
			keyBackSprite.setTexture(keySysBackTexture);
			keyText.setCharacterSize(10);
		}
		else if (keyItem.type == "space")
		{
			keyBackSprite.setTexture(keySpaceBackTexture);
			keyText.setCharacterSize(10);
		}
		
		keyBackSprite.setPosition(keyItem.posX, keyItem.posY);
		keyBackSprite.setOrigin(keyBackSprite.getLocalBounds().width/2, keyBackSprite.getLocalBounds().height/2);
		
		keyText.setString(keyItem.value);
		keyText.setPosition(keyItem.posX+4, keyItem.posY-5);
		keyText.setOrigin(keyText.getCharacterSize()/2, keyText.getCharacterSize()/2);
		
		if (currentID == keyItem.id && !menuNav.selected)
		{
			keyText.setColor(sf::Color::Yellow);
			if( keyItem.id == "space")
			{
				keyBackSprite.setScale(1.05,1.05);
				keyText.setScale(1.05,1.05);
			}
			else
			{
				keyBackSprite.setScale(1.2,1.2);
				keyText.setScale(1.2,1.2);
			}
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
