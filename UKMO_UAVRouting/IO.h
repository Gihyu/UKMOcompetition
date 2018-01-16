#pragma once
#include"Util.h"
#include"City.h"
#include"Block.h"
#include"OperBlock.h"
#include"Schedule.h"

class IO
{
public:
	IO();

	void input(Schedule* sche,int date);

	void readForecast(Schedule* sche,string inFile);
	void readForecastMatrix(Schedule* sche, string inFile);
	void readCity(Schedule* sche);
	void readMeasure(Schedule* sche,string inFile);

	void printSoln(int destinationCityNo, Block* origin, vector<OperBlock*> soln);
	void outputSoln(int destinationCityNo,Block* origin,vector<OperBlock*> soln);
	void outputMultiSoln(vector<City*> cities, Block* origin, vector<vector<OperBlock*>> multiSoln);

	void readInfo(Schedule* sche, int maxFileNum);
	void readInfo(Schedule* sche,string inFile);
	double getBlockAvgWindByCoordinate(Schedule* sche,int x, int y, int mins);

};

