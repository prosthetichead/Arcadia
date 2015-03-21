#include "TextScrolling.h"


TextScrolling::TextScrolling(void)
{
}


TextScrolling::~TextScrolling(void)
{
}

void TextScrolling::setText(std::string new_text)
{
	if (new_text.empty())
		text = "No Description Available in Database";
	else
	{
		std::string new_str;
		std::vector<std::string> tokens;
		boost::split(tokens, new_text, boost::is_any_of(" "));
		
	}






		
		
		
		int currentLineLength = 0;

		
		for(std::string& word: tokens)
		{
			boost::replace_all(word, "\n", "");
			word = word + " ";
			tempText.setString(word);
			int wordLengthPixels = tempText.getLocalBounds().width;
			if (currentLineLength + wordLengthPixels <= lineLengthPixels)
			{   // its ok write the word to the current line
				new_str = new_str + word;
				currentLineLength += wordLengthPixels;
			}
			else
			{
				new_str = new_str + "\n" + word;
				currentLineLength = wordLengthPixels;
			}
		}
		descriptionText.setString(new_str);  
		
		descriptionOffset = 0;
		resetOffset = false;
		if (descriptionText.getLocalBounds().height > sh->game_info_settings.description.size.y) {
			descriptionRequiredOffset = descriptionText.getLocalBounds().height - sh->game_info_settings.description.size.y;
			descriptionScroll = true;
			descriptionPause  = true;
		}
		else {
			descriptionRequiredOffset = 0;
			descriptionScroll = false;
			descriptionPause  = true;
		}
	}
}

void TextScrolling::update()
{
}