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
	//BFS* bfs = new BFS(_origin);//should be origin and destination!!
	//_soln = bfs->getSoln();

	OperBlock* opb1 = new OperBlock(_blockList[0], 9, 30);
	OperBlock* opb2 = new OperBlock(_blockList[1], 10, 4);
	_soln.push_back(opb1);
	_soln.push_back(opb2);

}
