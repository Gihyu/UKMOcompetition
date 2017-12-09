#include "Util.h"

//string Util::OutputPath = "C:/Users/zhoulei/Documents/GitHub/UKMOcompetition/output/";
//string Util::InputPath = "C:/Users/zhoulei/Documents/GitHub/UKMOcompetition/input/";

//sjy hp
//string Util::OutputPath = "D:/campus/LaSCO/TianChiCompetition/UKmetoffice/onGitHub/UKMOcompetition/output/";
//string Util::InputPath = "D:/campus/LaSCO/TianChiCompetition/UKmetoffice/onGitHub/UKMOcompetition/input/";

//sjy hasee
string Util::OutputPath = "C:/gihyu/campus/UKMOCompetition/onGithub/UKMOcompetition/output/";
string Util::InputPath = "C:/gihyu/campus/UKMOCompetition/onGithub/UKMOcompetition/input/";

double Util::Epsilon = 0.0001;//0.0001
bool Util::allRealization = true;
int Util::realization = 1;

int Util::flyTime = 2;
int Util::maxTime = 1260;
//fix day10 is 240 ; others are 180.
int Util::startTime_BFS = 240;

//BFS
double Util::initRatio = 15.0;
double Util::initRatio_forAllR = 15.0;
int Util::NumOf_littleWindForAllR = 10;
double Util::WeightOfModels[10] = { 0.956,0.949,0.952,0.947,0.949,0.953,0.960,0.955,0.944,0.950 };


//ACO
int Util::maxIterationACO = 100;

int Util::maxX = 548;
int Util::maxY = 421;
int Util::maxIndex = 548*421-1;

//print
time_t Util::startTime = 0;

string Util::getTimeStr(time_t t)
{
	struct tm timeinfo;
	char buffer[80];
	localtime_s(&timeinfo, &t);
	strftime(buffer, 80, "%R", &timeinfo);
	//strftime(buffer, 80, "%Y/%m/%d %R", &timeinfo);
	return buffer;
}

time_t Util::getTime(int hour, int min)
{
	time_t t = Util::startTime + hour * 3600 + min * 60;
	return t;
}

time_t Util::getTime(int min)
{
	time_t t = Util::startTime + min * 60;
	return t;
}

void Util::printCurTime()
{
	time_t t = time(NULL);
	cout << ">>> PRINT TIME: " << Util::getTimeStr(t) << endl;
}

