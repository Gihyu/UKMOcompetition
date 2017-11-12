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
	Block* getCity(int cityNo) { return _cityList[cityNo]->getBlock(); }

	void setBlockList(vector<Block*> blocks) { _blockList = blocks; }
	vector<Block*> getBlockList() { return _blockList; }

	void init();

	void initAdjBlock();
	static int getBlockIndex(int x, int y);
};

