#include "OperBlock.h"
int OperBlock::_count = 0;


OperBlock::OperBlock(Block* block, int date, int hour, BLOCKTYPE type, double wind)
	:_block(block),_date(date),_hour(hour),_type(type),_wind(wind)
{
	_id = _count;
	_count++;
}

OperBlock::OperBlock(Block* block,int hour,int ingTime):
	_block(block), _hour(hour),_ingTime(ingTime)
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
	if (getWind(thisTime) < 20.0 && target->getWind(thisTime)<20.0)
	{
		return true;
	}
	else
		return false;
}

bool OperBlock::test_cangotoThisBlock(Block * target, int thisTime)
{
	if (_block->testgetWind(thisTime)< 20.0 && target->testgetWind(thisTime)<20.0)
	{
		return true;
	}
	else
		return false;
}



