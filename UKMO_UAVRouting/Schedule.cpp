#include "Schedule.h"



Schedule::Schedule()
{
}

void Schedule::init()
{
	initAdjBlock();
}

void Schedule::initAdjBlock()
{
	for (auto&block : _blockList)
	{
		int x = block->getX();
		int y = block->getY();

		int index = getBlockIndex(x - 1, y);
		if (index != -1)
		{
			block->pushCanGoToBlock(_blockList[index]);
		}

		index = getBlockIndex(x + 1, y);
		if (index != -1)
		{
			block->pushCanGoToBlock(_blockList[index]);
		}

		index = getBlockIndex(x, y-1);
		if (index != -1)
		{
			block->pushCanGoToBlock(_blockList[index]);
		}

		index = getBlockIndex(x, y+1);
		if (index != -1)
		{
			block->pushCanGoToBlock(_blockList[index]);
		}
	}

	cout << ">>> finish initAdjBlock" << endl;
	cout << ">>> test print" << endl;
	vector<Block*> tmps = _blockList.front()->getCangoToBlocks();
	_blockList.front()->print();
	for (auto&tmp : tmps)
	{
		cout << "\t";
		tmp->print();
	}
	tmps = _blockList[2]->getCangoToBlocks();
	_blockList[2]->print();
	for (auto&tmp : tmps)
	{
		cout << "\t";
		tmp->print();
	}
	cout << endl;
}

int Schedule::getBlockIndex(int x, int y)
{
	if (x <= 0)//x:1-548
	{
		return -1;
	}
	if (y <= 0)//y:1-421
	{
		return -1;
	}
	int index = (x - 1)* Util::maxY + (y - 1);
	if (index > Util::maxIndex)
	{
		return -1;
	}
	return index;
}

