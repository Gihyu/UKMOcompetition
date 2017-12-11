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
	array<double, Util::hourCount> _windArr;//3:00-20:00
	array<array<double, 10>, Util::hourCount> _windMatrix;//3:00-20:00£»18h * 10realization
	array<double, Util::hourCount> _measureWindArr;//3:00-20:00
	static const int _basicHour = 3;

	// mark the situation of the Block ( BFS ) in order to run faster
	// 0 means notFound ; 1 means Searching ; 2 means visted
	// initial is 0
	int _situation;
	int _violations;

	vector<Block* >_cangotoBlocks;
	
	OperBlock * _myOperBlock;

	//add by sjy to use a case test ;17-11-11
	vector<pair<int, double>> _time_wind;

public:
	Block(int x,int y,int date, array<double, Util::hourCount> windArr);
	Block(int x, int y, int date, array<array<double, 10>, Util::hourCount> windMatrix);

	pair<int, int> getCoordinate();

	int getX() { return _x; }
	int getY() { return _y; }

	int getDate() { return _date; }

	double getWind(int hour) { return _windArr[hour - _basicHour]; }
	double getAvgWind(int hour);
	double getWeightedAvgWind(int hour);
	int getNumOf_littleWind(int hour);

	vector<int > getSayNoModelId(int hour);

	array<double, 10> getWindAllRealization(int hour) { return _windMatrix[hour - _basicHour]; }

	void setMeasureWindArr(array<double, Util::hourCount> windArr) { _measureWindArr = windArr; }
	double getMeasureWind(int hour){return _measureWindArr[hour - _basicHour];}

	void pushCanGoToBlock(Block* block) { _cangotoBlocks.push_back(block); }
	vector<Block*> getCangoToBlocks() { return _cangotoBlocks; }
	void pushCangoToBlocks(Block * cangoto) { _cangotoBlocks.push_back(cangoto); }

	void setSituation(int a) { _situation = a; }
	int getSituation() { return _situation; }

	void setViolations(int vio) { _violations = vio; }
	int getViolations() { return _violations; }

	void setMyOperBlock(OperBlock * ob) { _myOperBlock = ob; }
	OperBlock * getMyOperBlock() { return _myOperBlock; }

	//add by sjy to use a case test ;17-11-11
	int getId() { return _id; }
	double testgetWind(int thisTime);
	void testPushTimeWind(pair<int, double> time_wind) { _time_wind.push_back(time_wind); }

	bool equal(Block* block);
	bool equal(int x, int y);

	void print();
	void printWindArr();
	void printWindMatrix();
};

