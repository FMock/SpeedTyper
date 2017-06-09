#include"TextWriter.h"

TextWriter::TextWriter(std::string s, std::map<std::string, GLuint>& tMap):strToImageMap(tMap){
	text = s;
	fontWidth = 23;
	fontHeight = 25;
	xPosition = 25;
	yPosition = 540;
}

void TextWriter::draw(){
	int textSize = text.size();
	// For each character of text, get it's corresponding image from the map and draw it
	int offset = 0;
	for(std::string::size_type i = 0; i < textSize; ++i) {
		// convert char to string
		std::stringstream ss;
		ss << text[i];
		std::string s;
		ss >> s; // s is a key in strToImageMap
		glDrawSprite(strToImageMap[s],  xPosition + offset, yPosition, fontWidth, fontHeight);
		offset += fontWidth;
	}
}

void TextWriter::update(std::string s){
	text = s;
}

std::string TextWriter::to_string() const{
	std::ostringstream oss;

	oss << "text = " << text << "\n";
	return oss.str();
}