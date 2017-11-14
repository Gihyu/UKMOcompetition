#pragma once
#include"Util.h"
#include"Block.h"
#include"OperBlock.h"
#include"BFS.h"
class PathSolver
{
private:
	Block* _origin;
	Block* _destination;
	vector<Block*> _blockList;

	vector<OperBlock*> _soln;

	vector<vector<OperBlock*>> _multiSoln;
public:
	PathSolver(Block* origin, Block* destination,vector<Block*> blocks);

	void solve();//TODO

	vector<OperBlock*> getSoln() { return _soln; }
};

