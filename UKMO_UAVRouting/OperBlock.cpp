#include "OperBlock.h"
int OperBlock::_count = 0;


OperBlock::OperBlock(Block * block, int hour, int min) :
	_block(block), _hour(hour), _min(min)
{
	_id = _count;
	_count++;
}

OperBlock::OperBlock(Block* block,int hour):
	_block(block), _hour(hour)
{

}

double OperBlock::getWind(int hour)
{
	//need to add something here

	return _wind;
}

bool OperBlock::cangotoThisBlock(Block * target, int thisTime)
{
	// I need to update this rule after my question online is confirmed.
	if (getWind(thisTime) < 20.0 && target->getWind(thisTime + Util::flyTime)<20.0)
	{
		return true;
	}
	else
		return false;
}
