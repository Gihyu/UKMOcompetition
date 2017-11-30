#include "BFS.h"

BFS::BFS(Block * sourceBlock):
_sourceBlock(sourceBlock)
{

}

void BFS::clearQueue(queue<OperBlock *>& q) {
	queue<OperBlock*> empty;
	swap(empty, q);
}

vector<vector<OperBlock *>> BFS::solve_by_anyCases_multiTarget()
{
	vector<vector<OperBlock *>> multiSoln;

	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	// 540 means start from 9:00
	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, 180);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	bool findTheFarmost = false;

	while (!_ingOperBlocks.empty() && !findTheFarmost && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			int allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock(cangoto, thisTime))
				{	

				    //顺序不能乱
					//回撞sourceBlock导致getFront为NULL
					//firstReach就是注释掉

					if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
					{				
						chooseLowestWind(ingOperBlock, cangoto, thisTime);
						//chooseHighestWind(ingOperBlock, cangoto, thisTime);
					}
					else if (cangoto->getSituation() == 0)

					//if (cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						cangotoOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(cangotoOperBlock);
						_ingOperBlocks.push(cangotoOperBlock);
					}											
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{	
				if (cangoto->getSituation() == 0 )
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			else
			{
				ingOperBlock->setIngTime(thisTime + Util::flyTime);
				_ingOperBlocks.pop();
				_ingOperBlocks.push(ingOperBlock);
			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<pair<int, int>> destinations;
	pair<int,int> d1(84, 203);
	pair<int, int> d2(199, 371);
	pair<int, int> d3(140, 234);
	pair<int, int> d4(236, 241);
	pair<int, int> d5(315, 281);
	pair<int, int> d6(358, 207);
	pair<int, int> d7(363, 237);
	pair<int, int> d8(423, 266);
	pair<int, int> d9(125, 375);
	pair<int, int> d10(189, 274);

	destinations.push_back(d1);
	destinations.push_back(d2);
	destinations.push_back(d3);
	destinations.push_back(d4);
	destinations.push_back(d5);
	destinations.push_back(d6);
	destinations.push_back(d7);
	destinations.push_back(d8);
	destinations.push_back(d9);
	destinations.push_back(d10);

	for (int k = 0; k < 10; k++)
	{
		int destinationX = destinations[k].first;
		int destinationY = destinations[k].second;
		vector<OperBlock * > OperRoute;
		OperBlock * front = NULL;
		// maybe some is soln but it's ingTime is 1258 so it will be in the ingBlock
		bool findTheDes = false;
		for (auto & visted : _vistedOperBlocks)
		{
			if (visted->getBlock()->getX() == destinationX && visted->getBlock()->getY() == destinationY)
			{
				OperRoute.push_back(visted);
				front = visted->getFront();
				findTheDes = true;
				break;
			}			
		}
		if (!findTheDes)
		{
			int size = _ingOperBlocks.size();
			for (int i = 1; i <= size; i++)
			{
				OperBlock * testOperBlock = _ingOperBlocks.front();
				if (destinationX == testOperBlock->getX() && destinationY == testOperBlock->getY())
				{	
					OperRoute.push_back(testOperBlock);
					front = testOperBlock->getFront();
					findTheDes = true;
					break;
				}
				_ingOperBlocks.pop();
				_ingOperBlocks.push(testOperBlock);
			}
		}
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
		sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);
		multiSoln.push_back(OperRoute);
	}

	//print function
	for (int k = 0;k<10;k++)
	{
		if (multiSoln[k].empty())
		{
			cout << endl;
			cout << "can not reach city" << k+1 << "(" << destinations[k].first << "," << destinations[k].second << endl;
			cout << endl;
		}
		else
		{
			cout << endl;
			cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << destinations[k].first << "," << destinations[k].second << ") is :" << endl;

			for (int i =0; i<multiSoln[k].size(); i++)
			{
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - 142) + abs(multiSoln[k][i]->getBlock()->getY() - 328);
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + 180 > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				//cout << "(" << multiSoln[k][i]->getBlock()->getX() << "," << multiSoln[k][i]->getBlock()->getY() << ")Solntime is" << multiSoln[k][i]->getSolnTime() << "->" ;
				cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getWind(multiSoln[k][i]->getSolnTime() / 60) << endl;
			}
			cout << endl;
			cout << endl;
		}
	}

	return multiSoln;
}


vector<OperBlock *> BFS::solve_allow_windRatio_singleTarget(Block * targetBlock,double windRatio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	// 540 means start from 9:00
	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, 180);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	bool findTheTarget = false;
	OperBlock * targetOperBlock = NULL;


	while (!_ingOperBlocks.empty() && !findTheTarget && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock_allow_ratio(cangoto, thisTime, windRatio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY)
					{
						cangoto->setSituation(2);
						targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						_vistedOperBlocks.push_back(targetOperBlock);
						findTheTarget = true;
						break;
					}
					else
					{	

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseLowestWind(ingOperBlock, cangoto, thisTime);
							//chooseHighestWind(ingOperBlock, cangoto, thisTime);
						}
						else if (cangoto->getSituation() == 0)
						
						//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							cangoto->setMyOperBlock(cangotoOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0 )
				{
					allCangoToOper = false;
					break;
				}
			}

			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				ingOperBlock->getBlock()->setSituation(2);
				_ingOperBlocks.pop();
			}
			else
			{
				ingOperBlock->setIngTime(thisTime + Util::flyTime);
				_ingOperBlocks.pop();
				_ingOperBlocks.push(ingOperBlock);
			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{
		// when print don't forget to inverted order		
		OperRoute.push_back(targetOperBlock);
		OperBlock * front = targetOperBlock->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		}
	}

	sort(OperRoute.begin(), OperRoute.end(), OperBlock::cmpBySolnTime);

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock when allow "<<windRatio<<" !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;

		for (int i = 0; i <OperRoute.size(); i++)
		{
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - 142) + abs(OperRoute[i]->getBlock()->getY() - 328);
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + 180 > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;
			
			if (OperRoute[i]->getSolnTime()<1260)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << endl;
			}
			
		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}


void BFS::chooseLowestWind(OperBlock * oper, Block * blo, int thistime)
{	
	if (oper->getWind(thistime / 60) < blo->getMyOperBlock()->getFront()->getWind(thistime / 60))
	{	
		//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ing solntime " << oper->getSolnTime() << " myOperSolnTime " << blo->getMyOperBlock()->getFront()->getSolnTime() << " !!!!!!!!!!!!!!!" << endl;
		if (thistime == blo->getMyOperBlock()->getFront()->getSolnTime())
		{
			//cout << "--------------------------change------------------------" << endl;
			blo->getMyOperBlock()->setFront(oper);
		}

	}
}


void BFS::chooseHighestWind(OperBlock * oper, Block * blo, int thistime)
{
	if (oper->getWind(thistime / 60) > blo->getMyOperBlock()->getFront()->getWind(thistime / 60))
	{
		//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ing solntime " << oper->getSolnTime() << " myOperSolnTime " << blo->getMyOperBlock()->getFront()->getSolnTime() << " !!!!!!!!!!!!!!!" << endl;
		if (thistime == blo->getMyOperBlock()->getFront()->getSolnTime())
		{
			//cout << "--------------------------change------------------------" << endl;
			blo->getMyOperBlock()->setFront(oper);
		}

	}
}

