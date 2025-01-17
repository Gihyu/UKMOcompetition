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

	vector<vector<OperBlock *>> solve_by_anyCases_multiTarget();
	vector<OperBlock *> solve_allow_windRatio_singleTarget(Block * targetBlock,double windRatio);

	vector<vector<OperBlock *>> solve_allR_multiTarget();
	vector<OperBlock *>solve_allR_singleTarget(Block * targetBlock, int allowNumOf_littleWind, double allRratio);

	vector<vector<OperBlock *>> solve_multiTarget_valued_by_allR();
	vector<OperBlock *> solve_singleTarget_valued_by_allR(Block * targetBlock, double windRatio);

	void clearQueue(queue<OperBlock *>& q);

	void chooseLowestWind(OperBlock * oper, Block * blo,int thistime,double windRatio);
	void chooseHighestWind(OperBlock * oper, Block * blo, int thistime,double windRatio);
	void chooseBestWind_forAllR(OperBlock * oper, Block * blo, int thistime,int allowNum,double windRatio);

	void updateViolations_exchange(OperBlock * ingOB, Block* cgt, int thisTime, int allowNum,double windRatio);

	int getNum_Violations(OperBlock * ingOB, Block* cgt, int thisTime, double windRatio);
	int getNum_Violations_allR(OperBlock * ingOB, Block* cgt, int thisTime, int allowNum,double windRatio);
};

