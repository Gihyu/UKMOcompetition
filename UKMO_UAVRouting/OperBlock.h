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
	//int _date;//date由block得
	int _hour;
	int _min;

	double _wind;//oper 不应该有wind！！！！
	OperBlock * _frontOperBlock;

public:
	OperBlock(Block* block,int hour,int min);

	//add by sjy 2017-11-7
	void setFront(OperBlock * frontOperBlock) { _frontOperBlock = frontOperBlock; }
	OperBlock * getFront() { return _frontOperBlock; }

	OperBlock(Block * block, int hour);

	int getTime() { return _hour; }
	void setTime(int hour) { _hour = hour; }

	Block * getBlock() { return _block; }
	int getX() { return _block->getX(); }
	int getY() { return _block->getY(); }
	double getWind(int hour);
	bool cangotoThisBlock(Block * target, int thisTime);

};

