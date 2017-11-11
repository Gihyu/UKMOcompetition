#pragma once
#include"Util.h"
class OperBlock;
class Block
{
private:
	int _id;
	static int _count;

	//coordinate
	int _x;
	int _y;

	//time
	int _date;

	//wind
	array<double, 12> _windArr;//9:00-20:00
	array<double, 12> _measureWindArr;//9:00-20:00

	// maybe it's not necessary
	vector<Block* >_cangotoBlocks;

	static const int _basicHour = 9;

public:
	Block(int x,int y,int date, array<double, 12> windArr);

	pair<int, int> getCoordinate();
	int getX() { return _x; }
	int getY() { return _y; }

	double getWind(int hour) { return _windArr[hour - _basicHour]; }

	void setMeasureWindArr(array<double, 12> windArr) { _measureWindArr = windArr; }
	double getMeasureWind(int hour){return _measureWindArr[hour - _basicHour];}

	vector<Block*> getCangoToBlocks() { return _cangotoBlocks; }


	bool equal(Block* block);
	bool equal(int x, int y);

	void print();
};

