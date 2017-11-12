#include "OperBlock.h"
int OperBlock::_count = 0;

OperBlock::OperBlock(Block* block,int hour,int ingTime):
	_block(block), _ingTime(ingTime)
{
	_id = _count;
	_count++;
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

void OperBlock::print()
{
	cout << "Oper_" << _id << "\t"<<Util::getTimeStr(Util::getTime(_solnTime));
	_block->print();
}
