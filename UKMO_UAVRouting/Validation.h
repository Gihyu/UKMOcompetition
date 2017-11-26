#pragma once
#include"Util.h"
#include"OperBlock.h"
#include"City.h"
class Validation
{
private:
	vector<OperBlock*> _soln;
	vector<City*> _cityList;
	const double _wind = 20.0;
	const int _maxCost = 24*60;//delay 1 day
	const int _beginTime = 9 * 60;//9:00

public:
	Validation(vector<OperBlock*> soln);
	Validation(vector<City*> cities);

	double validate();
	double validate(vector<OperBlock*> soln);

	void printInfo(Block* block,int min);
};

