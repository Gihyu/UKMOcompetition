#pragma once
#include"Util.h"
#include"City.h"
#include"Block.h"

class Schedule
{
private:
	vector<City*> _cityList;
	vector<Block*> _blockList;
public:
	Schedule();


	void setCityList(vector<City*> cities) { _cityList = cities; }
	vector<City*> getCityList() { return _cityList; }

	void setBlockList(vector<Block*> blocks) { _blockList = blocks; }
	vector<Block*> getBlockList() { return _blockList; }
};

