#pragma once
#include"Util.h"
#include"Block.h"
class OperBlock
{
private:
	int _id;
	static int _count;

	Block* _block;

	//time
	int _solnTime;//mins from 00:00

	OperBlock * _frontOperBlock;

	int _ingTime;

public:
	OperBlock(Block* block,int firstReachTime);

	//add by sjy 2017-11-7
	void setFront(OperBlock * frontOperBlock) { _frontOperBlock = frontOperBlock; }
	OperBlock * getFront() { return _frontOperBlock; }

	int getSolnTime() { return _solnTime; }
	void setSolnTime(int time) { _solnTime = time; }

	int getIngTime() { return _ingTime; }
	void setIngTime(int ingTime) { _ingTime = ingTime; }

	Block * getBlock() { return _block; }

	int getX() { return _block->getX(); }
	int getY() { return _block->getY(); }

	double getWind(int hour){ return _block->getWind(hour); }

	void print();
	static bool cmpBySolnTime(OperBlock* a, OperBlock* b);

	bool cangotoThisBlock(Block * target, int thisTime);
	bool cangotoThisBlock_allow_ratio(Block * target, int thisTime, double windRatio);
	bool cangotoThisBlock_allR(Block * target, int thisTime, int numOf_littleWind,double allRratio);
	bool cangotoThisBlock_CBNT(Block * target, int thisTime, int numOf_littleWind, double allRratio);
	bool cangotoThisBlock_valued_by_allR(Block * target, int thisTime, double windRatio, int numOf_littleWind);

	bool cangotoThisBlock_allRjustAvg(Block * target, int thisTime, double allRratio);

	//backtrack ava by allR
	bool cangotoThisBlock_backtrack_evabyallR(Block * target, int thisTime, int numOf_littleWind, double allRratio,double singleRatio);

	//season 2 

	bool cangotoThisBlock_backtrack_single_rain(Block * target, int thisTime, double singleWindRatio, double singleRainRatio);

	bool cangotoThisBlock_backtrack_all_rain(Block * target, int thisTime, double allWindRatio, double allRainRatio,int windvote,int rainvote);
	//test case
	bool OperBlock::test_cangotoThisBlock(Block * target, int thisTime);
	

};

