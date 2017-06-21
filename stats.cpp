#include"stats.h"

Stats::Stats():score(0),totalCount(0),correctCount(0){

}

void Stats::increaseScore(int x){
	if(x >= 0)
		score += x;
}

int Stats::getScore(){
	return score;
}

void Stats::increaseTotalCount(int x){
	if(x >= 0)
		totalCount += x;
}

int Stats::getTotalCount(){
	return totalCount;
}

void Stats::increaseCorrectCount(int x){
	if(correctCount >=0)
		correctCount += x;
}

int Stats::getCorrectCount(){
	return correctCount;
}

void Stats::reset(){
	score = 0;
	totalCount = 0;
	correctCount = 0;
}

std::string Stats::to_string() const{
	std::ostringstream oss;
	oss << "Stats ************\n"
		<< "score = " << score << "\n"
		<< "totalCount = " << totalCount << "\n"
		<< "correctCount = " << correctCount << "\n";
	return oss.str();
}