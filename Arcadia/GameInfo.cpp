#include "GameInfo.h"


GameInfo::GameInfo(dbHandle* db_ref, assetHandle* ah_ref, SkinHandle* sh_ref)
	: genreChanger(ah_ref), developerChanger(ah_ref), publisherChanger(ah_ref)
{
	ah = ah_ref;
	db = db_ref;
	sh = sh_ref;
	//descriptionOffset = 0;
	//resetOffset = false;
	//descriptionScroll = false;
	//descriptionPause = false;
	//descriptionPauseTime = 50;
	//descriptionPauseCount = 0;
	gameChangedCounter = 0;
	hasMovieFile = false;

	fanArt.create(1,1);
	clearLogo.create(1,1);

	genreChanger.setText(currentGameInfo.genres, 10);
	developerChanger.setText(currentGameInfo.developers, 10);
	publisherChanger.setText(currentGameInfo.publishers, 10);

}


GameInfo::~GameInfo(void)
{	
	
}

void GameInfo::init()
{
	//rectangleFanArt.setSize(sh->game_info_settings.fanArt.size);
	//rectangleFanArt.setPosition(sh->game_info_settings.fanArt.pos);
	//rectangleFanArt.setFillColor(sh->game_info_settings.fanArt.colour);
	//rectangleFanArt.setOutlineThickness(sh->game_info_settings.fanArt.outline_width);
	//rectangleFanArt.setOutlineColor(sh->game_info_settings.fanArt.outline_colour);

	//scrollingTextTexture.create(sh->game_info_settings.description.size.x, sh->game_info_settings.description.size.y);

	//descriptionText.setFont(ah->getFontAsset(sh->game_info_settings.description.text_font));
	//descriptionText.setCharacterSize(sh->game_info_settings.description.text_size);
	//descriptionText.setColor(sh->game_info_settings.description.text_color);
	
	movie = new sfe::Movie;
}

void GameInfo::pauseMovie()
{
	movieStatus = movie->getStatus();
	if (hasMovieFile)
	{
		if (movieStatus == movie->Playing)
			movie->pause();
		if (movieStatus == movie->Paused)
			movie->play();
	}
}

void GameInfo::update(dbHandle::gameListItem gameItem)
{
	if (newGameItem.fileName != gameItem.fileName ||  newGameItem.platformID != gameItem.platformID)
	{
		newGameItem = gameItem;
		gameChangedCounter = 0;
	}
	else
	{
		gameChangedCounter += 1;
	}

	if (gameChangedCounter >= 20 && newGameItem.fileName != currentGameItem.fileName)
	{
		gameChangedCounter = 0;
		newGameInfo(gameItem);
	}
		

	if (hasMovieFile && movie->getStatus() == movie->Playing)
		movie->update();

	genreChanger.update();
}

void GameInfo::newGameInfo(dbHandle::gameListItem gameItem)
{
	currentGameItem = gameItem;
	currentGameInfo = db->getGameInfo(gameItem);

	// Genre roatating text
	genreChanger.setText(currentGameInfo.genres, 100);

	//recreate movie
	delete movie;
	movie = new sfe::Movie;  
	hasMovieFile = false;
	if (currentGameInfo.videoPath != "NULL") // Load movie file if one exists
	{
		hasMovieFile = true;
		movie->openFromFile(currentGameInfo.videoPath);
		movie->play();
	}
	// Fan Art Texture
	fanArt.create(1,1); // Create and empty Texture
	if (currentGameInfo.fanArtPath != "NULL")
		fanArt.loadFromFile(currentGameInfo.fanArtPath);

	// Clear Logo Texture
	if (currentGameInfo.clearLogoPath != "NULL")
	{
		clearLogo.loadFromFile(currentGameInfo.clearLogoPath);
		clearLogo.setSmooth(true);
	}
	else 
		clearLogo.create(1,1); // Create and empty Texture	

	// screen shot texture
	if (currentGameInfo.screenPath != "NULL")
		screenShot.loadFromFile(currentGameInfo.screenPath);
	else
		screenShot.create(1,1); // Create and empty Texture	

	

	// TODO: Move this into a scrolling Text Class??
	// Description Text
	//if (currentGameInfo.description.empty())
	//	descriptionText.setString("No Description Available in Database");
	//else
	//{
	//	std::string new_str;
	//	std::vector<std::string> tokens;
	//	boost::split(tokens, currentGameInfo.description, boost::is_any_of(" "));
	//	int lineLengthPixels = sh->game_info_settings.description.size.x;  // / descriptionFontSize;
	//	int currentLineLength = 0;

	//	sf::Text tempText = descriptionText;
	//	for(std::string& word: tokens)
	//	{
	//		boost::replace_all(word, "\n", "");
	//		word = word + " ";
	//		tempText.setString(word);
	//		int wordLengthPixels = tempText.getLocalBounds().width;
	//		if (currentLineLength + wordLengthPixels <= lineLengthPixels)
	//		{   // its ok write the word to the current line
	//			new_str = new_str + word;
	//			currentLineLength += wordLengthPixels;
	//		}
	//		else
	//		{
	//			new_str = new_str + "\n" + word;
	//			currentLineLength = wordLengthPixels;
	//		}
	//	}
	//	descriptionText.setString(new_str);  
	//	
	//	descriptionOffset = 0;
	//	resetOffset = false;
	//	if (descriptionText.getLocalBounds().height > sh->game_info_settings.description.size.y) {
	//		descriptionRequiredOffset = descriptionText.getLocalBounds().height - sh->game_info_settings.description.size.y;
	//		descriptionScroll = true;
	//		descriptionPause  = true;
	//	}
	//	else {
	//		descriptionRequiredOffset = 0;
	//		descriptionScroll = false;
	//		descriptionPause  = true;
	//	}
	//}
	
}

void GameInfo::draw(sf::RenderWindow& window,  SkinHandle::Skin_Element& se)
{
		switch (se.type)
		{
		case SkinHandle::element_type::fanart:
			window.draw(se.getSprite(fanArt));
			break;
		case SkinHandle::element_type::clear_logo:
			window.draw(se.getSprite( clearLogo ));
			break;
		case SkinHandle::element_type::description:
			ah->drawText(currentGameInfo.description, se, window);
			break;
		case SkinHandle::element_type::platform_icon:
			window.draw(se.getSprite( ah->getIconAsset(currentGameInfo.platformIconID) ));
			break;
		case SkinHandle::element_type::genres:
			genreChanger.draw(window, se);
			break;
		case SkinHandle::element_type::screenshot:
			if (currentGameInfo.screenPath != "NULL")
			{
				window.draw(se.getSprite(screenShot));
			}
			break;
		case SkinHandle::element_type::video:
			if (currentGameInfo.videoPath != "NULL") // Load movie file if one exists
			{
				movie->resizeToFrame(se.pos.x , se.pos.y, se.size.x, se.size.y,true);
				movie->setOrigin(se.getOrigin(movie->getSize().x,movie->getSize().y));
				window.draw(*movie);
			}
			break;
		case SkinHandle::element_type::year:			
			ah->drawText(currentGameInfo.release_year, se, window);
			break;
		case SkinHandle::element_type::play_time:
			ah->drawText(currentGameInfo.playTime, se, window);
			break;
		case SkinHandle::element_type::last_played:
			ah->drawText(currentGameInfo.lastPlayed, se, window);
			break;

		default:
			break;
		}
	
	

	

	
	
	//sf::Sprite spritePlayer;
	//spritePlayer.setTexture(ah->getTextureAsset("PLAYERS"));
	//for(int i=0; i < currentGameInfo.players; i++)
	//{
	//	spritePlayer.setPosition(sh->game_info_settings.players.pos.x + (spritePlayer.getLocalBounds().width * i), sh->game_info_settings.players.pos.y);
	//	window.draw(spritePlayer);
	//}

	// Draw Stars
	//ah->draw5_Stars(currentGameInfo.user_stars, sf::Color::Color(246,235,20,255), gameIconsBorderBottom.getPosition().x, gameIconsBorderBottom.getPosition().y+10, window);
	//ah->draw5_Stars(currentGameInfo.online_stars, sf::Color::Color(105,205,255,255), gameIconsBorderBottom.getPosition().x, gameIconsBorderBottom.getPosition().y+30, window);
	
	//playtime infomation.	
	//ah->drawText("", sh->game_info_settings.playTimeTitle, window);
	//ah->drawText(currentGameInfo.playTime, sh->game_info_settings.playTime, window);
	//ah->drawText("Last Played", sh->game_info_settings.lastPlayedTitle, window);
	//ah->drawText(currentGameInfo.lastPlayed, sh->game_info_settings.lastPlayed, window);

	//draw year information
	


	/*
	scrollingTextTexture.clear(sf::Color::Transparent);
	if (descriptionScroll)
	{
		if (!descriptionPause)
		{
			if (descriptionOffset < descriptionRequiredOffset && !resetOffset)
				descriptionOffset += .1;
			else if (descriptionOffset >= descriptionRequiredOffset && !resetOffset)
			{
				resetOffset = true;
				descriptionPause = true;
				descriptionPauseCount = 0;
				descriptionOffset -= .1;
			}
			else if (resetOffset && descriptionOffset > 0)
				descriptionOffset -= .1;
			else if (resetOffset &&  descriptionOffset <= 0)
			{
				resetOffset = false;
				descriptionPause = true;
				descriptionPauseCount = 0;
			}
		}
		else
		{
			descriptionPauseCount++;
			if (descriptionPauseCount >= descriptionPauseTime)
				descriptionPause = false;
		}
		
	}
	descriptionText.setPosition(0,0 - descriptionOffset);
	scrollingTextTexture.draw(descriptionText);
	scrollingTextTexture.display();
	sf::Sprite scrollingTextSprite(scrollingTextTexture.getTexture());
	scrollingTextSprite.setPosition(sh->game_info_settings.description.pos);
	window.draw(scrollingTextSprite);
	*/
}
 


