#pragma once
#include"Ant.h"
#include"Util.h"
#include"Block.h"
#include"OperBlock.h"


// total name of this head file is Ant Colony Optimization.
class ACO
{
private:
	int _iteration;
	double _avoidRepeat;

	Block * _sourceBlock;
	
	double fitness[ANTAMOUNT];        // it is decided by ant's path reducedcost

public:
	ACO(Block * sourceBlock);
	
	vector<OperBlock *> solve(Block * targetBlock);
		
	void updatePheromone(vector<Ant *> colony);

	bool isInOperVector(Block * test, vector<OperBlock *> testVector);

};