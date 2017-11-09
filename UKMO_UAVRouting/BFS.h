#pragma once
#include"Util.h"
#include"Block.h"
#include"OperBlock.h"
#include<queue>

//class OperBlock
//class Block;
class OperBlock;

class BFS
{
private:
	Block * _sourceBlock;
	vector<OperBlock *> _vistedOperBlocks;
	queue<OperBlock *> _ingBlocks;

public:
	BFS(Block * sourceBlock);
	bool isInOperVector(Block * test, vector<OperBlock *> testVector);

	vector<OperBlock *> solve_all_valid(Block * target);
};

