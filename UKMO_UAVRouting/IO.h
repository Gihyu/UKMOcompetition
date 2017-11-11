#pragma once
#include"Util.h"
#include"City.h"
#include"Block.h"
#include"OperBlock.h"
#include"Schedule.h"
class IO
{
public:
	void input(Schedule* sche, bool isTraining);

	void readForecast(Schedule* sche,bool isTraining);
	void readCity(Schedule* sche);
	void readMeasure(Schedule* sche);

};

