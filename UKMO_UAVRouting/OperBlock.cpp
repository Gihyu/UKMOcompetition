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
	//��㶼�÷ɳ�ȥ
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
	int num = numOf_littleWind;
	/*if (numOf_littleWind >= 6)
	{
		num = 6;
	}
	else
	{
		num = 0;
	}*/

	//��Щ��allow 6ʲô��onlyAvg��ʱ����
	//&& target->isItAbadChoice(thisTime, allRratio)
	//&& target->isItAbadChoice(thisTime, allRratio) && _block->isItAbadChoice(thisTime, allRratio)

	//��Щ������modelIndex��ʱ����
	//&& target->getAvgWind(thisTime/60)<avgWindratio
	// && target->getAvgWind(thisTime / 60)<avgWindratio &&_block->getAvgWind(thisTime/60)<avgWindratio

	//allRratio
	double avgWindratio = 14.0;

	//numOf_littleWind

	//��ֹ1258�ϵ�
	if (Util::flyTime + thisTime == Util::maxTime)
	{	
		//if (_block->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->isItSatisfyMultiLevel(thisTime, allRratio) && target->isItAbadChoice(thisTime, allRratio))
		//if (_block->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->isItAbadChoice(thisTime, allRratio))
		//if (_block->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getAvgWind(thisTime / 60)<avgWindratio)
		if (_block->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind(thisTime / 60, allRratio) >= num )
		{
			return true;
		}
		else
			return false;	
	}
	else
	{
		if (_block->getX() == 142 && _block->getY() == 328)
		{
			//��Ӷ���Ϊ�˲����������¸���֮���׹�٣��Լ������㷨�н�Լ����ʱ�䣨������һЩ��
			//if (target->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind((thisTime + Util::flyTime) / 60, allRratio) >= num && target->isItSatisfyMultiLevel(thisTime, allRratio) && target->isItAbadChoice(thisTime, allRratio))
			//if (target->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind((thisTime + Util::flyTime) / 60, allRratio) >= num && target->isItAbadChoice(thisTime, allRratio) )
			//if (target->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind((thisTime + Util::flyTime) / 60, allRratio) >= num && target->getAvgWind(thisTime / 60)<avgWindratio)
			if (target->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind((thisTime + Util::flyTime) / 60, allRratio) >= num )
			{
				return true;
			}
			else
				return false;
		}
		else
		{	
			//if (_block->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind(thisTime / 60, allRratio) >= num  && target->getNumOf_littleWind((thisTime + Util::flyTime) / 60, allRratio) >= num && target->isItSatisfyMultiLevel(thisTime, allRratio) && target->isItSatisfyMultiLevel((thisTime + Util::flyTime), allRratio) && _block->isItSatisfyMultiLevel(thisTime, allRratio) && target->isItAbadChoice(thisTime, allRratio) && target->isItAbadChoice((thisTime + Util::flyTime), allRratio) && _block->isItAbadChoice(thisTime, allRratio))
			//if (_block->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind(thisTime / 60, allRratio) >= num  && target->getNumOf_littleWind((thisTime + Util::flyTime) / 60, allRratio) >= num && target->isItAbadChoice(thisTime, allRratio) && target->isItAbadChoice((thisTime + Util::flyTime), allRratio) && _block->isItAbadChoice(thisTime, allRratio))
			//if (_block->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind((thisTime + Util::flyTime) / 60, allRratio) >= num && target->getAvgWind(thisTime / 60)<avgWindratio &&_block->getAvgWind(thisTime / 60)<avgWindratio)
			if (_block->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind(thisTime / 60, allRratio) >= num && target->getNumOf_littleWind((thisTime + Util::flyTime) / 60, allRratio) >= num )
			{
				return true;
			}
			else
				return false;
		}
	}
	
}

//ComBiNaTion 1-15
bool OperBlock::cangotoThisBlock_CBNT(Block * target, int thisTime, int numOf_littleWind, double allRratio)
{
	int num = numOf_littleWind;


	//��ֹ1258�ϵ�
	if (Util::flyTime + thisTime == Util::maxTime)
	{	
		//if (_block->getCBNTvotes(thisTime / 60, allRratio) >= num && target->getCBNTvotes(thisTime / 60, allRratio) >= num)
		if (_block->getCBNTavgWind(thisTime / 60) < allRratio && target->getCBNTavgWind(thisTime / 60) <allRratio)
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		if (_block->getX() == 142 && _block->getY() == 328)
		{
			//��Ӷ���Ϊ�˲����������¸���֮���׹�٣��Լ������㷨�н�Լ����ʱ�䣨������һЩ��
			//if (target->getCBNTvotes(thisTime / 60, allRratio) >= num && target->getCBNTvotes((thisTime + Util::flyTime) / 60, allRratio) >= num)
			if (target->getCBNTavgWind(thisTime / 60) < allRratio && target->getCBNTavgWind((thisTime + Util::flyTime) / 60) < allRratio)
			{
				return true;
			}
			else
				return false;
		}
		else
		{	
			//if (_block->getCBNTvotes(thisTime / 60, allRratio) >= num && target->getCBNTvotes(thisTime / 60, allRratio) >= num && target->getCBNTvotes((thisTime + Util::flyTime) / 60, allRratio) >= num)
			if (_block->getCBNTavgWind(thisTime / 60) < allRratio && target->getCBNTavgWind(thisTime / 60)< allRratio && target->getCBNTavgWind((thisTime + Util::flyTime) / 60) < allRratio)
			{
				return true;
			}
			else
				return false;
		}
	}

}

//eva by allR , added 12-23
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

//backtrack ava by allR 1-11
bool OperBlock::cangotoThisBlock_backtrack_evabyallR(Block * target, int thisTime, int numOf_littleWind, double allRratio,double singleRatio)
{
	int num = numOf_littleWind;

	double avgWindratio = allRratio;


	//��ֹ1258�ϵ�,һ��������avg��һ��������vote
	if (Util::flyTime + thisTime == Util::maxTime)
	{
		
		//if (_block->getWind(thisTime / 60) < singleRatio && _block->getAvgWind(thisTime/60)<avgWindratio && target->getWind(thisTime / 60) < singleRatio && target->getAvgWind(thisTime / 60)<avgWindratio)
		if (_block->getWind(thisTime / 60) < singleRatio && _block->getNumOf_littleWind(thisTime / 60,avgWindratio)>=num && target->getWind(thisTime / 60) < singleRatio && target->getNumOf_littleWind(thisTime / 60,avgWindratio)>=num)
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		if (_block->getX() == 142 && _block->getY() == 328)
		{
			//��Ӷ���Ϊ�˲����������¸���֮���׹�٣��Լ������㷨�н�Լ����ʱ�䣨������һЩ��
			//if (target->getWind(thisTime / 60) < singleRatio && target->getWind((thisTime + Util::flyTime) / 60) <singleRatio && target->getAvgWind(thisTime / 60)<avgWindratio && target->getAvgWind((thisTime + Util::flyTime) / 60)<avgWindratio)
			if (target->getWind(thisTime / 60) < singleRatio && target->getWind((thisTime + Util::flyTime) / 60) <singleRatio && target->getNumOf_littleWind(thisTime / 60,avgWindratio)>=num && target->getNumOf_littleWind((thisTime + Util::flyTime) / 60,avgWindratio)>=num)
			{
				return true;
			}
			else
				return false;
		}
		else
		{
			//if (_block->getWind(thisTime / 60) <singleRatio && _block->getAvgWind(thisTime/60)<avgWindratio && target->getWind(thisTime / 60) <singleRatio && target->getWind((thisTime + Util::flyTime) / 60)<singleRatio && target->getAvgWind(thisTime / 60)<avgWindratio && target->getAvgWind((thisTime + Util::flyTime) / 60)<avgWindratio)
			if (_block->getWind(thisTime / 60) <singleRatio && _block->getNumOf_littleWind(thisTime / 60,avgWindratio)>=num && target->getWind(thisTime / 60) <singleRatio && target->getWind((thisTime + Util::flyTime) / 60)<singleRatio && target->getNumOf_littleWind(thisTime / 60,avgWindratio)>=num && target->getNumOf_littleWind((thisTime + Util::flyTime) / 60,avgWindratio)>=num)
			{
				return true;
			}
			else
				return false;
		}
	}

}

bool OperBlock::cangotoThisBlock_allRjustAvg(Block * target, int thisTime, double allRratio)
{
	// I need to update this rule after my question online is confirmed.
	if (_block->getX() == 142 && _block->getY() == 328)
	{
		if (target->getAvgWind(thisTime / 60)<allRratio)
		{
			return true;
		}
		else
			return false;
	}
	else
	{
		if (_block->getAvgWind(thisTime / 60) < allRratio && target->getAvgWind(thisTime / 60)<allRratio)
		{
			return true;
		}
		else
			return false;
	}

}
