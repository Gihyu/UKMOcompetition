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
	queue<OperBlock *> _ingOperBlocks;

public:
	BFS(Block * sourceBlock);
	bool isInOperVector(Block * test, vector<OperBlock *> testVector);
	bool BFS::isInQueue(Block * test, queue<OperBlock *> testQueue);

	vector<OperBlock *> solve_by_anyCases(Block * target);
	vector<OperBlock *> solve_by_connectedNetwork(Block * target);

	void clearQueue(queue<OperBlock *>& q);

	//test
	void run_the_case();
	vector<OperBlock *> test_BFS(Block * target);
};

