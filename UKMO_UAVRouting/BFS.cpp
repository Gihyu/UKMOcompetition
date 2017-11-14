#include "BFS.h"

BFS::BFS(Block * sourceBlock):
_sourceBlock(sourceBlock)
{

}



bool BFS::isInOperVector(Block * test, vector<OperBlock *> testVector)
{
	bool whetherIn = false;
	if (!testVector.empty())
	{
		int x = test->getX();
		int y = test->getY();
		for (auto & operBlock : testVector)
		{
			if (x == operBlock->getX() && y == operBlock->getY())
			{
				whetherIn = true;
				break;
			}
		}
	}	
	return whetherIn;
}

//maybe add the attribute to the Block-Class is well;

bool BFS::isInQueue(Block * test, queue<OperBlock *> testQueue)
{
	bool whetherIn = false;
	if (!testQueue.empty())
	{
		int x = test->getX();
		int y = test->getY();
		int size = testQueue.size();
		for (int i = 1; i <= size; i++)
		{
			OperBlock * testOperBlock = testQueue.front();
			if (x == testOperBlock->getX() && y == testOperBlock->getY())
			{
				whetherIn = true;
			}
			testQueue.pop();
			testQueue.push(testOperBlock);			
		}
	}
	return whetherIn;
}
// use for debug
//vector<OperBlock *> BFS::solve_by_anyCases(Block * targetBlock)
//{	
//	_vistedOperBlocks.clear();
//	clearQueue(_ingOperBlocks);
//
//	// 540 means start from 9:00
//	_sourceBlock->setSituation(1);
//	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock,540);
//	sourceOperBlock->setFront(NULL);
//	_ingOperBlocks.push(sourceOperBlock);
//
//	//int targetX = targetBlock->getX();
//	//int targetY = targetBlock->getY();
//
//	int targetX = 297;
//	int targetY = 63;
//
//	bool findTheTarget = false;
//	OperBlock * targetOperBlock = NULL;
//
//	// if we don't need the analysis for the actual steops even >1260, use this "while" sentence
//
//	while (!_ingOperBlocks.empty() && !findTheTarget && _ingOperBlocks.front()->getSolnTime()<Util::maxTime)
//	{
//		OperBlock * ingOperBlock = _ingOperBlocks.front();
//		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();
//
//		if (!cangoToBlocks.empty())
//		{
//			int thisTime = ingOperBlock->getIngTime();
//
//			//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and time is " << thisTime <<" and wind here is "<< ingOperBlock->getBlock()->getWind(thisTime/60)<< endl;
//			cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and solnTime is " << ingOperBlock->getSolnTime() << endl;
//			bool allCangoToOper = true;
//			for (auto & cangoto : cangoToBlocks)
//			{
//				if (ingOperBlock->cangotoThisBlock(cangoto, thisTime))
//				{
//					if (cangoto->getX() == targetX && cangoto->getY() == targetY)
//					{	
//						//maybe here need to set Situation
//						targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
//						targetOperBlock->setFront(ingOperBlock);
//						findTheTarget = true;
//						break;
//					}
//					else
//					{
//						//if (!isInOperVector(cangoto, _vistedOperBlocks) && !isInQueue(cangoto, _ingOperBlocks))
//						if (cangoto->getSituation()==0 )
//						{	
//							cangoto->setSituation(1);
//							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
//							cangotoOperBlock->setFront(ingOperBlock);
//							_ingOperBlocks.push(cangotoOperBlock);
//						}
//
//					}
//				}
//				//in order to assure the function for the whole system , the " break " should write after all the cangotoOperBlocls.
//				// version 2017-11-12
//			}
//
//			for (auto & cangoto : cangoToBlocks)
//			{
//				//if (!isInOperVector(cangoto, _vistedOperBlocks) && !isInQueue(cangoto, _ingOperBlocks))
//				if (cangoto->getSituation() == 0)
//				{
//					allCangoToOper = false;
//				}
//			}
//
//			if (allCangoToOper)
//			{
//				_vistedOperBlocks.push_back(ingOperBlock);
//				ingOperBlock->getBlock()->setSituation(2);
//				_ingOperBlocks.pop();
//			}
//			else
//				// stay for next 2 minutes
//			{	
//
//				//cout << endl;
//				//cout<< "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!now is waited!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//				//cout<< "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and IngTime is " << thisTime << " and wind here is " << ingOperBlock->getBlock()->getWind(thisTime / 60) << endl;
//				ingOperBlock->setIngTime(thisTime + Util::flyTime);
//				//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!after reset waited!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//				//cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and IngTime is " << ingOperBlock->getIngTime() <<" and solnTime is "<< ingOperBlock->getSolnTime() <<endl;
//				_ingOperBlocks.pop();
//				_ingOperBlocks.push(ingOperBlock);
//				//cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!end waited!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//				//cout << endl;
//
//
//				// maybe here is a function to plus not 2 but the number which let the wind changed
//			}
//
//		}
//		else
//		{
//			_vistedOperBlocks.push_back(ingOperBlock);
//			ingOperBlock->getBlock()->setSituation(2);
//			_ingOperBlocks.pop();
//		}
//	}
//
//	vector<OperBlock *> OperRoute;
//	if (findTheTarget)
//	{
//		// when print don't forget to inverted order		
//		OperRoute.push_back(targetOperBlock);
//		OperBlock * front = targetOperBlock->getFront();
//		while (front != NULL)
//		{
//			OperRoute.push_back(front);
//			front = front->getFront();
//		}
//	}
//	
//	//print function
//	if (OperRoute.empty())
//	{
//		cout << " shortest path failed ! Can't find the targetBlock !" << endl;
//	}
//	else
//	{
//		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
//		for (int i = OperRoute.size() - 1; i >= 0; i--)
//		{	
//			int absPlus = abs(OperRoute[i]->getBlock()->getX() - 142) + abs(OperRoute[i]->getBlock()->getY() - 328);
//			bool findBug = true;
//			while (findBug)
//			{
//				if (absPlus * 2 + 540 != OperRoute[i]->getSolnTime())
//				{
//					vector<Block * > BugCanGotos = OperRoute[i]->getBlock()->getCangoToBlocks();
//					for (auto & bug : BugCanGotos)
//					{
//						cout << bug->getX() << "," << bug->getY() << endl;
//					}
//					cout << "!!!!!!!!!!bug is here !!!!!!!!!" << endl;
//					findBug = false;
//				}
//			}
//			
//			cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " IngTime is " << OperRoute[i]->getIngTime()<< "->"<<endl;
//			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")time is" << OperRoute[i]->getSolnTime() << " and wind here is " << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->";
//		}
//		cout << endl;
//		cout << endl;
//	}
//
//	//if (sourceOperBlock != NULL)
//	//{
//	//	delete sourceOperBlock;
//	//	sourceOperBlock = NULL;
//	//}
//
//	//if (targetOperBlock != NULL)
//	//{
//	//	delete targetOperBlock;
//	//	targetOperBlock = NULL;
//	//}
//
//	return OperRoute;
//}


vector<OperBlock *> BFS::solve_by_anyCases_singleTarget(Block * targetBlock)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	// 540 means start from 9:00
	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, 540);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	bool findTheTarget = false;
	OperBlock * targetOperBlock = NULL;

	
	while (!_ingOperBlocks.empty() && !findTheTarget && _ingOperBlocks.front()->getSolnTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime <<" and SolnTime is "<< ingOperBlock ->getSolnTime()<< endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock(cangoto, thisTime))
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
						if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
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

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock !" << endl;
	}
	else
	{	
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		
		for (int i = OperRoute.size() - 1; i >= 0; i--)
		{	
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - 142) + abs(OperRoute[i]->getBlock()->getY() - 328);
			if (absPlus * 2 + 540 < OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}
			if (absPlus * 2 + 540 > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

		cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() <<" and the wind that time is"<< OperRoute [i]->getBlock()->getWind(OperRoute[i]->getSolnTime()/60)<<  "->" << endl;
		}
		cout << endl;
		cout << endl;
	}

	//invalid memory control but it's helpful to stop the printFile in IO when we just want to analysis the route.
	if (sourceOperBlock != NULL)
	{
		delete sourceOperBlock;
		sourceOperBlock = NULL;
	}
	if (targetOperBlock != NULL)
	{
		delete targetOperBlock;
		targetOperBlock = NULL;
	}

	return OperRoute;
}

vector<vector<OperBlock *>> BFS::solve_by_anyCases_multiTarget(Block * farmostTarget)
{
	vector<vector<OperBlock *>> multiSoln;

	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	// 540 means start from 9:00
	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, 540);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int farmostX = farmostTarget->getX();
	int farmostY = farmostTarget->getY();

	bool findTheFarmost = false;

	while (!_ingOperBlocks.empty() && !findTheFarmost && _ingOperBlocks.front()->getSolnTime()<Util::maxTime)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			cout << "now is (" << ingOperBlock->getX() << "," << ingOperBlock->getY() << ") and Ingtime is " << thisTime << " and SolnTime is " << ingOperBlock->getSolnTime() << endl;
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock(cangoto, thisTime))
				{
					if (cangoto->getX() == farmostX && cangoto->getY() == farmostY)
					{
						cangoto->setSituation(2);
						OperBlock * farmostOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						farmostOperBlock->setFront(ingOperBlock);
						_vistedOperBlocks.push_back( farmostOperBlock);
						findTheFarmost = true;
						break;
					}
					else
					{
						if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
				{
					allCangoToOper = false;
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
				//int absPlus = abs(OperRoute[i]->getBlock()->getX() - 142) + abs(OperRoute[i]->getBlock()->getY() - 328);
				//if (absPlus * 2 + 540 < OperRoute[i]->getSolnTime())
				//{
				//	cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				//}
				//if (absPlus * 2 + 540 > OperRoute[i]->getSolnTime())
				//{
				//	cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				//}

				cout << "(" << multiSoln[k][i]->getBlock()->getX() << "," << multiSoln[k][i]->getBlock()->getY() << ")Solntime is" << multiSoln[k][i]->getSolnTime() << "->" ;
			}
			cout << endl;
			cout << endl;
		}
	}

	return multiSoln;
}


vector<OperBlock *> BFS::solve_by_connectedNetwork(Block * targetBlock)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, 0);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();
	bool findTheTarget = false;
	OperBlock * targetOperBlock = NULL;

	// if we don't need the analysis for the actual steops even > 360 , use this "while" sentence
	//while(!ingBlocks.empty() && !findTheTarget && ingBlock->getTime()<360)

	while (!_ingOperBlocks.empty() && !findTheTarget)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();		
			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getX() == targetX && cangoto->getY() == targetY)
				{
					targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
					targetOperBlock->setFront(ingOperBlock);
					findTheTarget = true;
					break;
				}
				else
				{
					if (!isInOperVector(cangoto, _vistedOperBlocks) && !isInQueue(cangoto, _ingOperBlocks))
					{
						OperBlock * cangotoOperBlock = new OperBlock(cangoto,  thisTime + Util::flyTime);
						cangotoOperBlock->setFront(ingOperBlock);
						_ingOperBlocks.push(cangotoOperBlock);
					}

				}
				//in order to assure the function for the whole system , the " break " should write after all the cangotoOperBlocls.
				// version 2017-11-12
			}
			_vistedOperBlocks.push_back(ingOperBlock);
			_ingOperBlocks.pop();
		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
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

	//print function
	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock !" << endl;
	}
	else
	{
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		for (int i = OperRoute.size() - 1; i >= 0; i--)
		{

			cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")time is" << OperRoute[i]->getSolnTime() << "->";
		}
		cout << endl;
		cout << endl;
	}

	delete sourceOperBlock;
	sourceOperBlock = NULL;

	if (targetOperBlock != NULL)
	{
		delete targetOperBlock;
		targetOperBlock = NULL;
	}

	return OperRoute;
}

//test 2017-11-12
void BFS::run_the_case()
{
	array<double, 12> windArr = { 10,10,10,10,10,10,10,10,10,10,10,10 };
	Block * b1 = new Block(1,99,164, windArr);
	Block * b2 = new Block(2, 100, 164, windArr);
	Block * b3 = new Block(3, 101, 164, windArr);
	Block * b4 = new Block(4, 102, 164, windArr);
	Block * b5 = new Block(5, 99, 163, windArr);
	Block * b6 = new Block(6, 100, 163, windArr);
	Block * b7 = new Block(7, 101, 163, windArr);
	Block * b8 = new Block(8, 102, 163, windArr);
	Block * b9 = new Block(9, 99, 162, windArr);
	Block * b10 = new Block(10, 100, 162, windArr);
	Block * b11 = new Block(11, 101, 162, windArr);
	Block * b12 = new Block(12, 102, 162, windArr);
	
	b1->pushCangoToBlocks(b2);
	b1->pushCangoToBlocks(b5);

	b2->pushCangoToBlocks(b1);
	b2->pushCangoToBlocks(b6);
	b2->pushCangoToBlocks(b3);

	b3->pushCangoToBlocks(b2);
	b3->pushCangoToBlocks(b7);
	b3->pushCangoToBlocks(b4);

	b4->pushCangoToBlocks(b3);
	b4->pushCangoToBlocks(b8);

	b5->pushCangoToBlocks(b1);
	b5->pushCangoToBlocks(b6);
	b5->pushCangoToBlocks(b9);

	b6->pushCangoToBlocks(b2);
	b6->pushCangoToBlocks(b5);
	b6->pushCangoToBlocks(b7);
	b6->pushCangoToBlocks(b10);

	b7->pushCangoToBlocks(b3);
	b7->pushCangoToBlocks(b6);
	b7->pushCangoToBlocks(b8);
	b7->pushCangoToBlocks(b11);

	b8->pushCangoToBlocks(b4);
	b8->pushCangoToBlocks(b7);
	b8->pushCangoToBlocks(b12);

	b9->pushCangoToBlocks(b5);
	b9->pushCangoToBlocks(b10);

	b10->pushCangoToBlocks(b6);
	b10->pushCangoToBlocks(b9);
	b10->pushCangoToBlocks(b11);

	b11->pushCangoToBlocks(b7);
	b11->pushCangoToBlocks(b10);
	b11->pushCangoToBlocks(b12);

	b12->pushCangoToBlocks(b8);
	b12->pushCangoToBlocks(b11);   

	pair<int,double> p0(0, 30.0);
	pair<int, double> p2(2, 30.0);
	pair<int, double> p4(4, 30.0);
	pair<int, double> p6(6, 30.0);
	pair<int, double> p8(8, 30.0);

	b2->testPushTimeWind(p0);
	b2->testPushTimeWind(p8);

	b3->testPushTimeWind(p2);
	b3->testPushTimeWind(p4);
	b3->testPushTimeWind(p6);

	b5->testPushTimeWind(p0);

	b7->testPushTimeWind(p6);
	b7->testPushTimeWind(p8);

	b8->testPushTimeWind(p2); 
	b8->testPushTimeWind(p4);

	b10->testPushTimeWind(p2);

	_sourceBlock = b6;
	test_BFS(b1);
	test_BFS(b2);
	test_BFS(b3);
	test_BFS(b4);
	test_BFS(b5);	
	test_BFS(b7);
	test_BFS(b8);
	test_BFS(b9);
	test_BFS(b10);
	test_BFS(b11);
	test_BFS(b12);
}

vector<OperBlock *> BFS::test_BFS(Block * targetBlock)
{	
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);


	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock,0);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();
	bool findTheTarget = false;
	OperBlock * targetOperBlock;

	// if we don't need the analysis for the actual steops even > 360 , use this "while" sentence
	//while(!ingBlocks.empty() && !findTheTarget && ingBlock->getTime()<360)

	while (!_ingOperBlocks.empty() && !findTheTarget)
	{
		OperBlock * ingOperBlock = _ingOperBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getIngTime();
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->test_cangotoThisBlock(cangoto, thisTime))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY)
					{
						targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						findTheTarget = true;
						break;
					}
					else
					{
						if (!isInOperVector(cangoto, _vistedOperBlocks) && !isInQueue(cangoto, _ingOperBlocks))
						{
							OperBlock * cangotoOperBlock = new OperBlock(cangoto,  thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (!isInOperVector(cangoto, _vistedOperBlocks) && !isInQueue(cangoto, _ingOperBlocks))
				{
					allCangoToOper = false;
				}
			}
			if (allCangoToOper)
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				_ingOperBlocks.pop();
			}
			else
				// stay for next 2 minutes
			{
				ingOperBlock->setIngTime(thisTime + Util::flyTime);
				_ingOperBlocks.push(ingOperBlock);
				_ingOperBlocks.pop();
				// maybe here is a function to plus not 2 but the number which let the wind changed
			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			_ingOperBlocks.pop();
		}
	}

	vector<OperBlock *> OperRoute;
	if (findTheTarget)
	{	
		OperRoute.push_back(targetOperBlock);
		OperBlock * front = targetOperBlock->getFront();
		while (front != NULL)
		{
			OperRoute.push_back(front);
			front = front->getFront();
		} 	
	}

	if (OperRoute.empty())
	{
		cout << " shortest path failed ! Can't find the targetBlock !" << endl;
	}
	else
	{
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		for (int i = OperRoute.size() - 1; i >= 0; i--)
		{	

			cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")time is"<< OperRoute[i]->getSolnTime() <<"->";
		}
		cout << endl;
		cout << endl;
	}	
	return OperRoute;
}


void BFS::clearQueue(queue<OperBlock *>& q) {
	queue<OperBlock*> empty;
	swap(empty, q);
}
