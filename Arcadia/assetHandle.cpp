#include "assetHandle.h"


assetHandle::assetHandle(void)
{
}


assetHandle::~assetHandle(void)
{
}

void assetHandle::init(dbHandle& db_obj, std::string skin_id)
{
	//std::vector<dbHandle::assetItem> items = db_obj.getIconPaths();
	//for (int i = 0; i < items.size(); i++)
	//{
	//	sf::Texture texture;
	//	texture.setSmooth(true);
	//	texture.loadFromFile(items.at(i).path);
	//	std::pair<std::string,sf::Texture> pair (items.at(i).id,texture);
	//	textureMap.insert(pair);
	//}

	loadFonts(db_obj.exe_path + "\\assets\\skins\\" + skin_id + "\\fonts");
	iconMap = loadImages(db_obj.exe_path + "\\assets\\icons");
	textureMap = loadImages(db_obj.exe_path + "\\assets\\system");
	staticImagesMap = loadImages(db_obj.exe_path + "\\assets\\skins\\" + skin_id + "\\pngs");
	companiesMap = loadImages(db_obj.exe_path + "\\assets\\companies");
}


boost::unordered_map<std::string, sf::Texture> assetHandle::loadImages(std::string path)
{
	boost::unordered_map<std::string, sf::Texture> returnMap;
	const std::string target_path = path;
	
	boost::filesystem::directory_iterator end_itr; // Default ctor yields past-the-end
	for( boost::filesystem::directory_iterator i( target_path ); i != end_itr; ++i )
	{
	    // Skip if not a file
	    if( !boost::filesystem::is_regular_file( i->status() ) ) continue;
	
	    // Skip if no match
		std::string extension = i->path().extension().string();
		std::string id = i->path().stem().string();

		boost::to_upper(extension);
		boost::to_upper(id);

	    if(extension == ".PNG")
		{
			// File matches, store it
			sf::Texture texture;
			texture.loadFromFile(i->path().string());
			std::pair<std::string,sf::Texture> pair(id, texture);
			returnMap.insert(pair);
			//std::cout << " File ID = " << id << " - " << i->path().string() << std::endl ;
		}
	}
	return returnMap;
}




void assetHandle::loadFonts(std::string path)
{
	const std::string target_path = path;
	
	boost::filesystem::directory_iterator end_itr; // Default ctor yields past-the-end
	for( boost::filesystem::directory_iterator i( target_path ); i != end_itr; ++i )
	{
	    // Skip if not a file
	    if( !boost::filesystem::is_regular_file( i->status() ) ) continue;
	
	    // Skip if no match
		std::string extension = i->path().extension().string();
		boost::to_upper(extension);
	    if( extension != ".TTF" ) continue;
	
	    // File matches, store it
	    sf::Font font;
		font.loadFromFile(i->path().string());

		std::string fontID = i->path().filename().string();
		boost::to_upper(fontID);
		std::pair<std::string,sf::Font> pair(fontID, font);
		fontMap.insert(pair);

		std::cout << i->path().filename().string() << std::endl;
	}

}

sf::Texture& assetHandle::getTextureAsset(std::string id)
{
	boost::to_upper(id);  //Make upper case	
	boost::replace_all(id, ".PNG", "");

	boost::unordered_map<std::string,sf::Texture>::iterator iter = textureMap.find(id);
	if(iter != textureMap.end()) return iter->second;
	else
	{
		//std::cout << "ERROR: texture ID " << id << " not found" << std::endl;
		boost::unordered_map<std::string,sf::Texture>::iterator iter = textureMap.find("ERROR");
		if(iter != textureMap.end()) return iter->second;
	}
}

sf::Texture& assetHandle::getStaticImageAsset(std::string id)
{
	boost::to_upper(id);  //Make upper case	
	boost::replace_all(id, ".PNG", "");

	boost::unordered_map<std::string,sf::Texture>::iterator iter = staticImagesMap.find(id);
	if(iter != staticImagesMap.end()) return iter->second;
	else
	{
		//std::cout << "ERROR: texture ID " << id << " not found" << std::endl;
		boost::unordered_map<std::string,sf::Texture>::iterator iter = textureMap.find("ERROR");
		if(iter != textureMap.end()) return iter->second;
	}
}

sf::Texture& assetHandle::getIconAsset(std::string id)
{
	boost::to_upper(id);  //Make upper case

	boost::unordered_map<std::string,sf::Texture>::iterator iter = iconMap.find(id);
	if(iter != iconMap.end()) return iter->second;
	else
	{
		//std::cout << "ERROR: texture ID " << id << " not found" << std::endl;
		boost::unordered_map<std::string,sf::Texture>::iterator iter = textureMap.find("ERROR");
		if(iter != textureMap.end()) return iter->second;
	}

}

sf::Texture& assetHandle::getCompanyAsset(std::string id)
{
	boost::to_upper(id);  //Make upper case

	boost::unordered_map<std::string,sf::Texture>::iterator iter = companiesMap.find(id);
	if(iter != companiesMap.end()) return iter->second;
	else
	{
		//std::cout << "ERROR: texture ID " << id << " not found" << std::endl;
		boost::unordered_map<std::string,sf::Texture>::iterator iter = textureMap.find("ERROR");
		if(iter != textureMap.end()) return iter->second;
	}

}

sf::Font& assetHandle::getFontAsset(std::string id)
{
	boost::to_upper(id);
	boost::unordered_map<std::string,sf::Font>::iterator iter = fontMap.find(id);
	if(iter != fontMap.end()) return iter->second;
	else
		return emptyFont;
}


sf::Vector2i assetHandle::resizePreserveRatio(int org_width, int org_height, int new_width, int new_height, bool fit)
{
	sf::Vector2i original_size = sf::Vector2i(org_width, org_height);
	sf::Vector2i wanted_size = sf::Vector2i(new_width, new_height);
	sf::Vector2i new_size;

	float org_ratio = (float)original_size.x / original_size.y;
	float new_ratio = (float)wanted_size.x / wanted_size.y;

	if (org_ratio > new_ratio && fit)
	{
		new_size.x = original_size.x * ((float)wanted_size.x / original_size.x);
		new_size.y = original_size.y * ((float)wanted_size.x / original_size.x);
	}
	else
	{
		new_size.x = original_size.x * ((float)wanted_size.y / original_size.y);
		new_size.y = original_size.y * ((float)wanted_size.y / original_size.y);
	}

	return new_size;
}

void assetHandle::draw5_Stars(double numStars, sf::Color color, int x, int y, sf::RenderWindow& window)
{
	sf::Sprite spriteStarEmpty;
	spriteStarEmpty.setTexture(getTextureAsset("STAR_EMPTY"));
	sf::Sprite spriteStarEmptyHalf;
	spriteStarEmptyHalf.setTexture(getTextureAsset("STAR_EMPTY_HALF"));
	for(int i=0; i < 5; i++)
	{
		spriteStarEmpty.setPosition(x + (20*i), y);
		spriteStarEmpty.setColor(sf::Color::Color(86,86,86,255));
		window.draw(spriteStarEmpty);
		if ((numStars - (i)) == 0.5)
		{
			spriteStarEmptyHalf.setPosition(spriteStarEmpty.getPosition());
			spriteStarEmptyHalf.setColor(color);
			window.draw(spriteStarEmptyHalf);
		}
		if (numStars >= i+1)
		{
			spriteStarEmpty.setColor(color);
			window.draw(spriteStarEmpty);
		}
	}
}

sf::Text assetHandle::getText(std::string text, SkinHandle::Skin_Element& fontItem)
{
	sf::Text returnText;
	returnText.setCharacterSize(fontItem.text_size);
	returnText.setColor(fontItem.text_color);
	returnText.setPosition(fontItem.pos);
	returnText.setString(text);
	returnText.setFont(getFontAsset(fontItem.asset_file));

	sf::FloatRect textRect = returnText.getLocalBounds();	
	returnText.setOrigin( fontItem.getOrigin(textRect) );
	
	

	return returnText;
}

void assetHandle::drawText(std::string text, SkinHandle::Skin_Element& fontItem, sf::RenderWindow& window)
{
	sf::Text main_text = getText(text, fontItem);
	
	if (fontItem.text_shadow)
	{
		sf::Text shadow_text = main_text;
		shadow_text.setPosition(fontItem.pos.x + fontItem.text_shadowOffset, fontItem.pos.y + fontItem.text_shadowOffset);
		shadow_text.setColor(fontItem.text_shadowColor);
		window.draw(shadow_text);
	}

	window.draw(main_text);
}
void assetHandle::drawText(sf::Text text, SkinHandle::Skin_Element& fontItem, sf::RenderWindow& window)
{
	sf::Text main_text = text;
	
	if (fontItem.text_shadow)
	{
		sf::Text shadow_text = main_text;
		shadow_text.setPosition(main_text.getPosition().x + fontItem.text_shadowOffset, main_text.getPosition().y + fontItem.text_shadowOffset);
		shadow_text.setColor(fontItem.text_shadowColor);
		window.draw(shadow_text);
	}

	window.draw(main_text);
}

void assetHandle::trimTextToRectangleWidth(sf::Text &text, sf::RectangleShape &rect)
{
	
	while(true)
	{
		sf::FloatRect floatRect = text.getGlobalBounds();
		if ((floatRect.left + floatRect.width) > rect.getSize().x)
		{
			std::string newString = text.getString();
			newString = newString.substr(0, newString.size()-1);
			text.setString(newString);
		}
		else
			break;
	}
}