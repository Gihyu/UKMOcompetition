#include "OperBlock.h"
#include "math.h"
int OperBlock::_count = 0;

OperBlock::OperBlock(Block* block,int firstReachTime):
	_block(block), _solnTime(firstReachTime),_ingTime(firstReachTime)
{
	_id = _count;
	_count++;
}

bool OperBlock::cangotoThisBlock(Block * target, int thisTime)
{	
	//起点都让飞出去
	// I need to update this rule after my question online is confirmed.
	if (_block->getX() == 142 && _block->getY() == 328)
	{
		if ( target->getWind(thisTime / 60)< Util::initRatio)
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		if (getWind(thisTime / 60) < Util::initRatio && target->getWind(thisTime / 60)< Util::initRatio)
		{
			return true;
		}
		else
			return false;
	}
	
}

bool OperBlock::cangotoThisBlock_allow_ratio(Block * target, int thisTime,double windRatio)
{
	// I need to update this rule after my question online is confirmed.
	if (_block->getX() == 142 && _block->getY() == 328)
	{
		if (target->getWind(thisTime / 60)<windRatio)
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		if (getWind(thisTime / 60) < windRatio && target->getWind(thisTime / 60)<windRatio)
		{
			return true;
		}
		else
			return false;
	}

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
	cout << "Oper_" << _id << "\t" << Util::getTimeStr(Util::getTime(_solnTime)) << "\t";
	_block->print();
}

bool OperBlock::cmpBySolnTime(OperBlock * a, OperBlock * b)
{
	return a->getSolnTime() < b->getSolnTime();
}

bool OperBlock::cangotoThisBlock_allR(Block * target, int thisTime, int numOf_littleWind, double allRratio)
{	
	int justAvgUsed;
	if (numOf_littleWind >= 6)
	{
		justAvgUsed = 6;
	}
	else
	{
		justAvgUsed = 0;
	}

	//
	//


	//numOf_littleWind
	if (_block->getX() == 142 && _block->getY() == 328)
	{	
		if ( target->getNumOf_littleWind(thisTime / 60, allRratio) >= justAvgUsed && target->isItAbadChoice(thisTime, allRratio))
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		if (_block->getNumOf_littleWind(thisTime / 60, allRratio) >= justAvgUsed && target->getNumOf_littleWind(thisTime / 60, allRratio) >= justAvgUsed && target->isItAbadChoice(thisTime, allRratio) && _block->isItAbadChoice(thisTime, allRratio))
		{
			return true;
		}
		else
			return false;
	}
	
}

//valued by allR , added 12-23
bool OperBlock::cangotoThisBlock_valued_by_allR(Block * target, int thisTime, double windRatio, int numOf_littleWind)
{
	if (_block->getX() == 142 && _block->getY() == 328)
	{
		if (target->getWind(thisTime / 60)<windRatio && target->getNumOf_littleWind(thisTime /60,windRatio)>=numOf_littleWind)
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		if (getWind(thisTime / 60) < windRatio && target->getWind(thisTime / 60)<windRatio && _block->getNumOf_littleWind(thisTime / 60, windRatio) >= numOf_littleWind && target->getNumOf_littleWind(thisTime / 60, windRatio) >= numOf_littleWind)
		{
			return true;
		}
		else
			return false;
	}
}
