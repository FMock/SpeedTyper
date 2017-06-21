#ifndef STATS_H
#define STATS_H

#include<sstream>

class Stats{
public:
	Stats();
	int score;
	int totalCount;
	int correctCount;
	void increaseScore(int x);
	int getScore();
	void increaseTotalCount(int x);
	int getTotalCount();
	void increaseCorrectCount(int x);
	int getCorrectCount();
	void reset();
	std::string to_string() const;
}; 
#endif