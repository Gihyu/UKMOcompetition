#pragma once
#include"Util.h"
#include"Block.h"
#include"OperBlock.h"
#include"BFS.h"
#include"City.h"
class PathSolver
{
private:
	Block* _origin;
	Block* _destination;
	vector<Block*> _blockList;

	vector<City*> _desCityList;

	vector<OperBlock*> _soln;

	vector<vector<OperBlock*>> _multiSoln;
public:
	PathSolver(Block* origin, Block* destination,vector<Block*> blocks);
	PathSolver(Block* origin, vector<City*> desCities, vector<Block*> blocks);


	void solve();//TODO

	vector<OperBlock*> getSoln() { return _soln; }
	vector<vector<OperBlock*>> getMultiSoln() { return _multiSoln; }

};

