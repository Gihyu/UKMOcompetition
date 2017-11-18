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

PathSolver::PathSolver(Block * origin, vector<City*> desCities, vector<Block*> blocks):
	_origin(origin), _desCityList(desCities), _blockList(blocks)
{
	_destination = NULL;
	cout << ">>> PathSolver create:" << endl;
	cout << "\tfrom:\t";
	_origin->print();
	cout << "\tto:\t";
	for (auto&city:_desCityList)
	{
		city->print();
	}
	cout << endl;
}

void PathSolver::solve()
{	
	for (auto & block : _blockList)
	{
		block->setSituation(0);
	}

	BFS* bfs = new BFS(_origin);//should be origin and destination!!
	//_soln = bfs->solve_by_anyCases_singleTarget(_destination);
	//sort(_soln.begin(), _soln.end(), OperBlock::cmpBySolnTime);

	_multiSoln = bfs->solve_by_anyCases_multiTarget();
	for (int i = 1; i < _desCityList.size(); ++i)
	{	
		if (_multiSoln[i - 1].empty())
		{	
			double windratio = Util::initRatio;
			vector<OperBlock *> ratioSoln;
			while (ratioSoln.empty())
			{	
				windratio += 1.0;
				cout << "!!!!!!!!!Let's start to allow " << windratio << endl;
				cout << "!!!!!!!!!Let's start to allow "<<windratio<<" fo city" << i << "(" << _desCityList[i]->getBlock()->getX() << "," << _desCityList[i]->getBlock()->getY() << ")!!!!!!!!!" << endl;
				cout << "!!!!!!!!!Let's start to allow " << windratio << endl;
				OperBlock * nullOper = NULL;
				for (auto & block : _blockList)
				{						
					block->setSituation(0);
					block->setMyOperBlock(nullOper);
				}
				ratioSoln=bfs->solve_allow_windRatio_singleTarget(_desCityList[i]->getBlock(), windratio);
			}
			_desCityList[i]->setSoln(ratioSoln);
		}
		else
		{
			_desCityList[i]->setSoln(_multiSoln[i - 1]);//#city = 11; #soln = 10
		}
		
	}

	//zhoulei debug
	//OperBlock* opb1 = new OperBlock(_blockList[0], 9, 30);
	//opb1->setSolnTime(9*60+30);//9:30
	//OperBlock* opb2 = new OperBlock(_blockList[1], 10, 4);
	//opb2->setSolnTime(10*60+4);//10:04
	//_soln.push_back(opb1);
	//_soln.push_back(opb2);

}
