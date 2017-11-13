#include "PathSolver.h"

PathSolver::PathSolver(Block * origin, Block* destination, vector<Block*> blocks):
	_origin(origin),_destination(destination),_blockList(blocks)
{
	cout << ">>> PathSolver create:" << endl;
	cout << "\tfrom:\t";
	_origin->print();
	cout << "\tto:\t";
	_destination->print();
	cout << endl;
}

void PathSolver::solve()
{	
	for (auto & block : _blockList)
	{
		block->setSituation(0);
	}

	BFS* bfs = new BFS(_origin);//should be origin and destination!!
	_soln = bfs->solve_by_anyCases(_destination);

	//OperBlock* opb1 = new OperBlock(_blockList[0], 9, 30);
	//opb1->setSolnTime(9*60+30);//9:30
	//OperBlock* opb2 = new OperBlock(_blockList[1], 10, 4);
	//opb2->setSolnTime(10*60+4);//10:04
	//_soln.push_back(opb1);
	//_soln.push_back(opb2);

}
