#ifndef TEXTWRITER_H
#define TEXTWRITER_H

#include<GL/glew.h>
#include"DrawUtils.h"
#include<cstdlib>
#include<string>
#include<sstream>
#include<map>

class TextWriter{
public:
	TextWriter(std::string s, std::map<std::string, GLuint>& tMap);
	std::map<std::string, GLuint> strToImageMap;
	std::string text; // the text to be displayed
	int fontWidth;
	int fontHeight;
	int xPosition;
	int yPosition;
	void draw();
	void update(std::string);
	std::string to_string() const;
};

#endif