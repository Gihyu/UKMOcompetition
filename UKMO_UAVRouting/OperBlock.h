#pragma once
#include"Util.h"
#include"Block.h"
enum BLOCKTYPE
{
	B_MEASURE = 0,

	B_MODE1 = 1,
	B_MODE2 = 2,
	B_MODE3 = 3,
	B_MODE4 = 4,
	B_MODE5 = 5,
	B_MODE6 = 6,
	B_MODE7 = 7,
	B_MODE8 = 8,
	B_MODE9 = 9,
	B_MODE10 = 10
};
class OperBlock
{
private:
	int _id;
	static int _count;

	Block* _block;

	int _date;
	int _hour;
	double _wind;

	OperBlock * _frontOperBlock;

	BLOCKTYPE _type;

public:
	OperBlock(Block* block,int date,int hour,BLOCKTYPE type,double wind);

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

