#pragma once
#include"Util.h"
class OperBlock;
class Block
{
private:
	int _id;

	//coordinate
	int _x;
	int _y;

	//weather
	vector<OperBlock*> _forecastOperBlockList;
	vector<OperBlock*> _measureOperBlockList;

	// maybe it's not necessary
	//should take a discuss with ZL
	double _wind;
	vector<Block* >_cangotoBlocks;


	//add by sjy to use a case test ;17-11-11
	vector<pair<int, double>> _time_wind;


public:
	Block(int id,int x,int y);

	pair<int, int> getCoordinate();
	int getX() { return _x; }
	int getY() { return _y; }

	void pushForecastOperBlock(OperBlock* operBlock) { _forecastOperBlockList.push_back(operBlock); }
	void pushMeasureOperBlock(OperBlock* operBlock) { _measureOperBlockList.push_back(operBlock); }

	//add by sjy 2017-11-7
	double getWind(int hour);
	vector<Block*> getCangoToBlocks() { return _cangotoBlocks; }
	void pushCangoToBlocks(Block * cangoto) { _cangotoBlocks.push_back(cangoto); }



	//add by sjy to use a case test ;17-11-11
	int getId() { return _id; }
	double testgetWind(int thisTime);
	void testPushTimeWind(pair<int, double> time_wind) { _time_wind.push_back(time_wind); }
};

