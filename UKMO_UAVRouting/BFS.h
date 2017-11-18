#pragma once
#include"Util.h"
#include"Block.h"
#include"OperBlock.h"
#include<queue>


class BFS
{
private:
	Block * _sourceBlock;
	vector<OperBlock *> _vistedOperBlocks;
	queue<OperBlock *> _ingOperBlocks;

	vector<OperBlock*> _soln;//!!可以不存，但要返回

public:
	BFS(Block * sourceBlock);

	vector<OperBlock*> getSoln() { return _soln; }//TODO

	bool isInOperVector(Block * test, vector<OperBlock *> testVector);
	bool BFS::isInQueue(Block * test, queue<OperBlock *> testQueue);

	vector<OperBlock *> solve_by_anyCases_singleTarget(Block * target);
	vector<vector<OperBlock *>> solve_by_anyCases_multiTarget();
	vector<OperBlock *> solve_allow_windRatio_singleTarget(Block * targetBlock,double windRatio);

	vector<OperBlock *> solve_by_connectedNetwork(Block * target);

	void clearQueue(queue<OperBlock *>& q);

	void chooseLowestWind(OperBlock * oper, Block * blo, int thistime);
	//test
	void run_the_case();
	vector<OperBlock *> test_BFS(Block * target);
};

