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

	//‘§¥¶¿Ì
	void preProcess();
	void devideFileByDay(bool isForecast);


	void input(Schedule* schedule);

	vector<City*> readCity();
	vector<Block*> readForecast(bool isTraining);
	void readMeasure(vector<Block*> blocks,bool isTraining);

	void tmpAnalyse();

};

