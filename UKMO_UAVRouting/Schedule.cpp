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
	Block* tmpBlock = _blockList[getBlockIndex(296,421)];
	vector<Block*> tmps = tmpBlock->getCangoToBlocks();
	tmpBlock->print();
	for (auto&tmp : tmps)
	{
		cout << "\t";
		tmp->print();
	}
	tmpBlock = _blockList[getBlockIndex(297,1)];
	tmps = tmpBlock->getCangoToBlocks();
	tmpBlock->print();
	for (auto&tmp : tmps)
	{
		cout << "\t";
		tmp->print();
	}

	tmpBlock = _blockList[getBlockIndex(297,47)];
	tmps = tmpBlock->getCangoToBlocks();
	tmpBlock->print();
	for (auto&tmp : tmps)
	{
		cout << "\t";
		tmp->print();
	}

	tmpBlock = _blockList[getBlockIndex(1,421)];
	tmps = tmpBlock->getCangoToBlocks();
	tmpBlock->print();
	for (auto&tmp : tmps)
	{
		cout << "\t";
		tmp->print();
	}

	cout << endl;
}

int Schedule::getBlockIndex(int x, int y)
{
	if (x <= 0 || x > Util::maxX)//x:1-548
	{
		return -1;
	}
	if (y <= 0 || y > Util::maxY)//y:1-421
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

