#include "OperBlock.h"
int OperBlock::_count = 0;


OperBlock::OperBlock(Block * block, int hour, int min) :
	_block(block), _hour(hour), _min(min)
{
	_id = _count;
	_count++;
}

OperBlock::OperBlock(Block * block, int hour):
	_block(block), _hour(hour)
{
	_id = _count;
	_count++;
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

void OperBlock::print()
{
	cout << "Oper_" << _id << "\t"<<Util::getTimeStr(Util::getTime(_hour,_min));
	_block->print();
}