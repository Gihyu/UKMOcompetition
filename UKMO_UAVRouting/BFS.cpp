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

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);


	while (!_ingOperBlocks.empty()  && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
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
						chooseLowestWind(ingOperBlock, cangoto, thisTime,Util::initRatio);
						//chooseHighestWind(ingOperBlock, cangoto, thisTime);
					}
					else if (cangoto->getSituation() == 0)

					//if (cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, Util::initRatio));
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
			//如果原地停留 那么仍需要判断停留的时候是否风速大于initial ratio
			//若大于，则作为坠机处理(假visted处理）
			//所以output的时候对于第一种群式搜索的时候也没关系（因为soln time 对应的风速肯定是小于initial ratio的）
			else
			{	
				if (ingOperBlock->getWind(thisTime / 60) < Util::initRatio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}
				
			}

		}
		//若cangoto为空 那么也是变为visted
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
			cout << endl;
			cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "Wind"  << "\t" << "violations" << endl;
			for (int i =0; i<multiSoln[k].size(); i++)
			{
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - 142) + abs(multiSoln[k][i]->getBlock()->getY() - 328);
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + Util::startTime_BFS > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				//cout << "(" << multiSoln[k][i]->getBlock()->getX() << "," << multiSoln[k][i]->getBlock()->getY() << ")Solntime is" << multiSoln[k][i]->getSolnTime() << "->" ;
				if (multiSoln[k][i]->getSolnTime()!=Util::startTime_BFS)
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getWind((multiSoln[k][i]->getSolnTime()-2) / 60) << "\t" << multiSoln[k][i]->getBlock()->getViolations()<< endl;
				}
				else
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getWind(multiSoln[k][i]->getSolnTime() / 60) << "\t" << multiSoln[k][i]->getBlock()->getViolations()<< endl;
				}
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

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	bool findTheTarget = false;
	int targetSolnTime = Util::maxTime;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
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
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations(ingOperBlock,cangoto,thisTime,windRatio));
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{	

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseLowestWind(ingOperBlock, cangoto, thisTime,windRatio);
							//chooseHighestWind(ingOperBlock, cangoto, thisTime);
						}
						else if (cangoto->getSituation() == 0)
						
						//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, windRatio));
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
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->getWind(thisTime / 60) < windRatio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}
				
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
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
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
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "Wind" <<  "\t" << "violations" << endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - 142) + abs(OperRoute[i]->getBlock()->getY() - 328);
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;
			
			if (OperRoute[i]->getSolnTime()!=Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind((OperRoute[i]->getSolnTime()-2) / 60) << "\t" << OperRoute[i]->getBlock()->getViolations()<< endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime()  / 60) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;
			}
			
		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}


void BFS::chooseLowestWind(OperBlock * oper, Block * blo, int thistime,double windRatio)
{	
	if((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	{
		if (getNum_Violations(oper, blo, thistime, windRatio) < blo->getViolations())
		{
			blo->getMyOperBlock()->setFront(oper);
			blo->setViolations(getNum_Violations(oper, blo, thistime, windRatio));
		}
		else if (getNum_Violations(oper, blo, thistime, windRatio) == blo->getViolations())
		{
			if (oper->getWind(thistime / 60) < blo->getMyOperBlock()->getFront()->getWind(thistime / 60))
			{
				//cout << "--------------------------change type2------------------------" << endl;
				blo->getMyOperBlock()->setFront(oper);
			}
		}
		
	}
}


void BFS::chooseHighestWind(OperBlock * oper, Block * blo, int thistime,double windRatio)
{
	if ((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	{
		if (getNum_Violations(oper, blo, thistime, windRatio) < blo->getViolations())
		{
			blo->getMyOperBlock()->setFront(oper);
			blo->setViolations(getNum_Violations(oper, blo, thistime, windRatio));
		}
		else if (getNum_Violations(oper, blo, thistime, windRatio) == blo->getViolations())
		{
			if (oper->getWind(thistime / 60) > blo->getMyOperBlock()->getFront()->getWind(thistime / 60))
			{
				//cout << "--------------------------change type2------------------------" << endl;
				blo->getMyOperBlock()->setFront(oper);
			}
		}

	}
}

void BFS::chooseBestWind_forAllR(OperBlock * oper, Block * blo, int thistime ,int allowNum,double windRatio)
{	
	//先判断时间是否一致
	//if (thistime == blo->getMyOperBlock()->getFront()->getSolnTime() )

	if ( (thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	{	
		//如果大于15.0的时候找点仍然找那些15.0之内rank更高的点，那么这里就不必填windRatio
		int thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
		int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, windRatio);
		if (thisNumof_littleWind > nextNumof_littleWind)
		{
			//cout << "--------------------------change level------------------------" << endl;
			blo->getMyOperBlock()->setFront(oper);
			updateViolations_exchange(oper, blo, thistime, allowNum,windRatio);
			
		}
		else if (thisNumof_littleWind == nextNumof_littleWind)
		{
			if (getNum_Violations_allR(oper, blo, thistime, allowNum, windRatio) < blo->getViolations())
			{	
				//cout << "--------------------------change violations------------------------" << endl;
				blo->getMyOperBlock()->setFront(oper);
				blo->setViolations(getNum_Violations_allR(oper, blo, thistime, allowNum, windRatio));
			}
			else if (getNum_Violations_allR(oper, blo, thistime, allowNum, windRatio) == blo->getViolations())
			{
				double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
				double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);

				//大风与小风的选择
				//if (thisAvgWind > nextAvgWind)
				if (thisAvgWind < nextAvgWind)
				{
					//cout << "--------------------------change wind------------------------" << endl;
					blo->getMyOperBlock()->setFront(oper);
				}
			}
		
		}
	} 



	//only AvgWind
	//if ((thistime + 2) == blo->getMyOperBlock()->getSolnTime())
	//{
	//	double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
	//	double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
	//	if (thisAvgWind < nextAvgWind)
	//	{
	//		//cout << "--------------------------change wind------------------------" << endl;
	//		blo->getMyOperBlock()->setFront(oper);
	//	}
	//}


}

vector<vector<OperBlock *>> BFS::solve_allR_multiTarget()
{
	vector<vector<OperBlock *>> multiSoln;

	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock,Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	while (!_ingOperBlocks.empty()  && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
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
				//如果是fixNum-changeWind，那么一般intial的Num不是10，所以得手动改这里，让它变成10
				//Util::NumOf_littleWindForAllR
				if (ingOperBlock->cangotoThisBlock_allR(cangoto, thisTime ,4 ,Util::initRatio_forAllR))
				{

					//顺序不能乱
					//回撞sourceBlock导致getFront为NULL
					//firstReach就是注释掉

					if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
					{	
						//写11为了在10的时候计数不乱加(fixWind,changeNum的)
						chooseBestWind_forAllR(ingOperBlock, cangoto, thisTime,11,Util::initRatio_forAllR);
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
				if (cangoto->getSituation() == 0)
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
			//如果原地停留 那么仍需要判断停留的时候是否风速大于initial ratio
			//若大于，则作为坠机处理(假visted处理）
			//所以output的时候对于第一种群式搜索的时候也没关系（因为soln time 对应的风速肯定是小于initial ratio的）
			else
			{
				if (ingOperBlock->getBlock()->getNumOf_littleWind(thisTime / 60, Util::initRatio_forAllR) >=Util::NumOf_littleWindForAllR)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

			}

		}
		//若cangoto为空 那么也是变为visted
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<pair<int, int>> destinations;
	pair<int, int> d1(84, 203);
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
	for (int k = 0; k<10; k++)
	{
		if (multiSoln[k].empty())
		{
			cout << endl;
			cout << "can not reach city" << k + 1 << "(" << destinations[k].first << "," << destinations[k].second << endl;
			cout << endl;
		}
		else
		{
			cout << endl;
			cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << destinations[k].first << "," << destinations[k].second << ") is :" << endl;
			cout << endl;
			cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "rank" << "\t" <<"violations" <<endl;
			for (int i = 0; i<multiSoln[k].size(); i++)
			{	
				//开挂的时候就不是 142  328 了
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - _sourceBlock->getX()) + abs(multiSoln[k][i]->getBlock()->getY() - _sourceBlock->getY());
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + Util::startTime_BFS > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				if (multiSoln[k][i]->getSolnTime() != Util::startTime_BFS)
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getAvgWind((multiSoln[k][i]->getSolnTime()- Util::flyTime) / 60) << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind((multiSoln[k][i]->getSolnTime()-Util::flyTime) / 60, Util::initRatio_forAllR) << "\t" << multiSoln[k][i]->getBlock()->getViolations()<< endl;
				}
				else
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getAvgWind(multiSoln[k][i]->getSolnTime() / 60) << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind(multiSoln[k][i]->getSolnTime()  / 60, Util::initRatio_forAllR) << "\t" << multiSoln[k][i]->getBlock()->getViolations()<< endl;
				}
				
			}
			cout << endl;
			cout << endl;
		}
	}

	return multiSoln;
}

vector<OperBlock *> BFS::solve_allR_singleTarget(Block * targetBlock, int allowNumOf_littleWind , double allRratio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
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
				if (ingOperBlock->cangotoThisBlock_allR(cangoto, thisTime, allowNumOf_littleWind, allRratio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{							
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations_allR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind,allRratio));
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;										
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseBestWind_forAllR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind,allRratio);
						}
						else if (cangoto->getSituation() == 0)
							//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							cangoto->setViolations(getNum_Violations_allR(ingOperBlock, cangoto, thisTime, allowNumOf_littleWind, allRratio));
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
				if (cangoto->getSituation() == 0)
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
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->getBlock()->getNumOf_littleWind(thisTime / 60, Util::initRatio_forAllR) >= allowNumOf_littleWind)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

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
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
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
		cout << " shortest path failed ! Can't find the targetBlock when allow " << allowNumOf_littleWind << " !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "rank" << "\t" <<"violations"<< endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{	
			//开挂的时候可能不是142 328
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - _sourceBlock->getX()) + abs(OperRoute[i]->getBlock()->getY() - _sourceBlock->getY());
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
			cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, Util::initRatio_forAllR) << "\t" << OperRoute[i]->getBlock()->getViolations() ;
				if (OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, Util::initRatio_forAllR) == 8 || OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, Util::initRatio_forAllR) == 9)
				{
					vector<int > sayNoIndex = OperRoute[i]->getBlock()->getSayNoModelId((OperRoute[i]->getSolnTime() - Util::flyTime) / 60);
					for (int sni = 0;sni<sayNoIndex.size(); sni++)
					{
						cout << "\t" << sayNoIndex[sni];
					}
				}
				cout << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind(OperRoute[i]->getSolnTime() / 60, Util::initRatio_forAllR) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}

void BFS::updateViolations_exchange(OperBlock * ingOB, Block* cgt, int thisTime, int allowNum,double windRatio)
{
	if (ingOB->getBlock()->getNumOf_littleWind(thisTime / 60, windRatio) ==allowNum || cgt->getNumOf_littleWind(thisTime / 60, windRatio) ==allowNum)
	{
		if (ingOB->getBlock()->getViolations() == cgt->getViolations())
		{	
			//cout << "change type 1" << endl;
			cgt->setViolations(ingOB->getBlock()->getViolations() + 1);
		}
	}
	else
	{
		if (ingOB->getBlock()->getViolations() != cgt->getViolations())
		{	
			//cout << "change type 2" << endl;
			cgt->setViolations(ingOB->getBlock()->getViolations());
		}
	}
}

int BFS::getNum_Violations_allR(OperBlock * ingOB, Block* cgt, int thisTime, int allowNum,double windRatio)
{
	if (ingOB->getBlock()->getNumOf_littleWind(thisTime / 60, windRatio) == allowNum || cgt->getNumOf_littleWind(thisTime / 60, windRatio) == allowNum)
	{
		return (ingOB->getBlock()->getViolations() + 1);
	}
	else
	{
		return ingOB->getBlock()->getViolations();
	}
}

int BFS::getNum_Violations(OperBlock * ingOB, Block* cgt, int thisTime,double windRatio)
{
	if (ingOB->getBlock()->getWind(thisTime / 60) <=(windRatio-0.5) && cgt->getWind(thisTime / 60) <= (windRatio - 0.5))
	{
		return ingOB->getBlock()->getViolations();
	}
	else
	{	
		return (ingOB->getBlock()->getViolations() + 1);	
	}
}

// valued by allR , added 12-23
vector<vector<OperBlock *>> BFS::solve_multiTarget_valued_by_allR()
{
	vector<vector<OperBlock *>> multiSoln;

	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
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
				if (ingOperBlock->cangotoThisBlock_valued_by_allR(cangoto, thisTime,Util::initRatio,Util::NumOf_littleWindForAllR))
				{

					//顺序不能乱
					//回撞sourceBlock导致getFront为NULL
					//firstReach就是注释掉

					if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
					{
						chooseLowestWind(ingOperBlock, cangoto, thisTime, Util::initRatio);
					}
					else if (cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, Util::initRatio));
						OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						cangotoOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(cangotoOperBlock);
						_ingOperBlocks.push(cangotoOperBlock);
					}
				}
			}

			for (auto & cangoto : cangoToBlocks)
			{
				if (cangoto->getSituation() == 0)
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
			//如果原地停留 那么仍需要判断停留的时候是否风速大于initial ratio
			//若大于，则作为坠机处理(假visted处理）
			//所以output的时候对于第一种群式搜索的时候也没关系（因为soln time 对应的风速肯定是小于initial ratio的）
			else
			{
				if (ingOperBlock->getWind(thisTime / 60) < Util::initRatio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

			}

		}
		//若cangoto为空 那么也是变为visted
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<pair<int, int>> destinations;
	pair<int, int> d1(84, 203);
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
	for (int k = 0; k<10; k++)
	{
		if (multiSoln[k].empty())
		{
			cout << endl;
			cout << "can not reach city" << k + 1 << "(" << destinations[k].first << "," << destinations[k].second << endl;
			cout << endl;
		}
		else
		{
			cout << endl;
			cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << destinations[k].first << "," << destinations[k].second << ") is :" << endl;
			cout << endl;
			cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "Wind" << "\t" << "violations" <<"\t"<<"votes"<< endl;
			for (int i = 0; i<multiSoln[k].size(); i++)
			{
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - 142) + abs(multiSoln[k][i]->getBlock()->getY() - 328);
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + Util::startTime_BFS > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				//cout << "(" << multiSoln[k][i]->getBlock()->getX() << "," << multiSoln[k][i]->getBlock()->getY() << ")Solntime is" << multiSoln[k][i]->getSolnTime() << "->" ;
				if (multiSoln[k][i]->getSolnTime() != Util::startTime_BFS)
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getWind((multiSoln[k][i]->getSolnTime() - 2) / 60) << "\t" << multiSoln[k][i]->getBlock()->getViolations() << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind((multiSoln[k][i]->getSolnTime() - 2) / 60,Util::initRatio) << endl;
				}
				else
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getWind(multiSoln[k][i]->getSolnTime() / 60) << "\t" << multiSoln[k][i]->getBlock()->getViolations() << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind(multiSoln[k][i]->getSolnTime() / 60,Util::initRatio) << endl;
				}
			}
			cout << endl;
			cout << endl;
		}
	}

	return multiSoln;
}

vector<OperBlock *> BFS::solve_singleTarget_valued_by_allR(Block * targetBlock, double windRatio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	bool findTheTarget = false;
	int targetSolnTime = Util::maxTime;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
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
				if (ingOperBlock->cangotoThisBlock_valued_by_allR(cangoto, thisTime, windRatio,Util::NumOf_littleWindForAllR))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, windRatio));
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseLowestWind(ingOperBlock, cangoto, thisTime, windRatio);
							//chooseHighestWind(ingOperBlock, cangoto, thisTime);
						}
						else if (cangoto->getSituation() == 0)

							//if (cangoto->getSituation() == 0)
						{
							cangoto->setSituation(1);
							cangoto->setViolations(getNum_Violations(ingOperBlock, cangoto, thisTime, windRatio));
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
				if (cangoto->getSituation() == 0)
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
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->getWind(thisTime / 60) < windRatio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

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
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
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
		cout << " shortest path failed ! Can't find the targetBlock when allow " << windRatio << " !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "Wind" << "\t" << "violations" << "\t"<<"votes"<<endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - 142) + abs(OperRoute[i]->getBlock()->getY() - 328);
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
			cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind((OperRoute[i]->getSolnTime() - 2) / 60) << "\t" << OperRoute[i]->getBlock()->getViolations() << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - 2) / 60,windRatio) << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getViolations() << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind(OperRoute[i]->getSolnTime() / 60,windRatio) << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}

void BFS::chooseLowestForAllRjustAvg(OperBlock * oper, Block * blo, int thistime,double allRratio)
{

	/*if ((thistime + Util::flyTime) == blo->getMyOperBlock()->getSolnTime())
	{
		int thisNumof_littleWind = oper->getBlock()->getNumOf_littleWind(thistime / 60, allRratio);
		int nextNumof_littleWind = blo->getMyOperBlock()->getFront()->getBlock()->getNumOf_littleWind(thistime / 60, allRratio);
		if (thisNumof_littleWind > nextNumof_littleWind)
		{
			blo->getMyOperBlock()->setFront(oper);

		}
		else if (thisNumof_littleWind == nextNumof_littleWind)
		{	
			double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
			double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
			if (thisAvgWind < nextAvgWind)
			{
				blo->getMyOperBlock()->setFront(oper);
			}
		}
	}*/



	//only AvgWind
	if ((thistime + 2) == blo->getMyOperBlock()->getSolnTime())
	{
		double thisAvgWind = oper->getBlock()->getAvgWind(thistime / 60);
		double nextAvgWind = blo->getMyOperBlock()->getFront()->getBlock()->getAvgWind(thistime / 60);
		if (thisAvgWind < nextAvgWind)
		{
			//cout << "--------------------------change wind------------------------" << endl;
			blo->getMyOperBlock()->setFront(oper);
		}
	}
}

vector<vector<OperBlock *>> BFS::solve_allRjustAvg_multi()
{
	vector<vector<OperBlock *>> multiSoln;

	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
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
				if (ingOperBlock->cangotoThisBlock_allRjustAvg(cangoto, thisTime, Util::allRjustAvgRatio))
				{
					if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
					{
						chooseLowestForAllRjustAvg(ingOperBlock, cangoto, thisTime,Util::allRjustAvgRatio);
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
				if (cangoto->getSituation() == 0)
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
			//如果原地停留 那么仍需要判断停留的时候是否风速大于initial ratio
			//若大于，则作为坠机处理(假visted处理）
			//所以output的时候对于第一种群式搜索的时候也没关系（因为soln time 对应的风速肯定是小于initial ratio的）
			else
			{
				if (ingOperBlock->getBlock()->getAvgWind(thisTime/60)<Util::allRjustAvgRatio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

			}

		}
		//若cangoto为空 那么也是变为visted
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			ingOperBlock->getBlock()->setSituation(2);
			_ingOperBlocks.pop();
		}
	}

	vector<pair<int, int>> destinations;
	pair<int, int> d1(84, 203);
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
	for (int k = 0; k<10; k++)
	{
		if (multiSoln[k].empty())
		{
			cout << endl;
			cout << "can not reach city" << k + 1 << "(" << destinations[k].first << "," << destinations[k].second << endl;
			cout << endl;
		}
		else
		{
			cout << endl;
			cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
			cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << destinations[k].first << "," << destinations[k].second << ") is :" << endl;
			cout << endl;
			cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "rank" << "\t" << "violations" << endl;
			for (int i = 0; i<multiSoln[k].size(); i++)
			{
				//开挂的时候就不是 142  328 了
				int absPlus = abs(multiSoln[k][i]->getBlock()->getX() - _sourceBlock->getX()) + abs(multiSoln[k][i]->getBlock()->getY() - _sourceBlock->getY());
				/*if (absPlus * 2 + 180 < multiSoln[k][i]->getSolnTime())
				{
				cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
				}*/
				if (absPlus * 2 + Util::startTime_BFS > multiSoln[k][i]->getSolnTime())
				{
					cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
				}

				if (multiSoln[k][i]->getSolnTime() != Util::startTime_BFS)
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getAvgWind((multiSoln[k][i]->getSolnTime() - Util::flyTime) / 60) << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind((multiSoln[k][i]->getSolnTime() - Util::flyTime) / 60, Util::initRatio_forAllR) << "\t" << multiSoln[k][i]->getBlock()->getViolations() << endl;
				}
				else
				{
					cout << multiSoln[k][i]->getBlock()->getX() << "\t" << multiSoln[k][i]->getBlock()->getY() << "\t" << multiSoln[k][i]->getSolnTime() << "\t" << multiSoln[k][i]->getBlock()->getAvgWind(multiSoln[k][i]->getSolnTime() / 60) << "\t" << multiSoln[k][i]->getBlock()->getNumOf_littleWind(multiSoln[k][i]->getSolnTime() / 60, Util::initRatio_forAllR) << "\t" << multiSoln[k][i]->getBlock()->getViolations() << endl;
				}

			}
			cout << endl;
			cout << endl;
		}
	}

	return multiSoln;
}

vector<OperBlock *> BFS::solve_allRjustAvg_single(Block * targetBlock, double allRratio)
{
	_vistedOperBlocks.clear();
	clearQueue(_ingOperBlocks);

	_sourceBlock->setSituation(1);
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, Util::startTime_BFS);
	sourceOperBlock->setFront(NULL);
	_ingOperBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();

	int targetSolnTime = Util::maxTime;
	bool findTheTarget = false;


	while (!_ingOperBlocks.empty() && _ingOperBlocks.front()->getIngTime()<targetSolnTime && _ingOperBlocks.front()->getIngTime()<Util::maxTime)
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
				if (ingOperBlock->cangotoThisBlock_allRjustAvg(cangoto, thisTime, allRratio))
				{
					if (cangoto->getX() == targetX && cangoto->getY() == targetY && cangoto->getSituation() == 0)
					{
						cangoto->setSituation(1);
						OperBlock * targetOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
						targetOperBlock->setFront(ingOperBlock);
						cangoto->setMyOperBlock(targetOperBlock);
						_ingOperBlocks.push(targetOperBlock);
						targetSolnTime = thisTime + Util::flyTime;
						findTheTarget = true;
					}
					else
					{

						if (cangoto->getSituation() == 1 && cangoto != _sourceBlock)
						{
							chooseLowestForAllRjustAvg(ingOperBlock, cangoto, thisTime,allRratio);
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
				if (cangoto->getSituation() == 0)
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
			//判断停留的时候是否不坠机
			else
			{
				if (ingOperBlock->getBlock()->getAvgWind(thisTime / 60)<allRratio)
				{
					ingOperBlock->setIngTime(thisTime + Util::flyTime);
					_ingOperBlocks.pop();
					_ingOperBlocks.push(ingOperBlock);
				}
				else
				{
					_vistedOperBlocks.push_back(ingOperBlock);
					ingOperBlock->getBlock()->setSituation(2);
					_ingOperBlocks.pop();
				}

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
		OperBlock * targetOB = targetBlock->getMyOperBlock();
		OperRoute.push_back(targetOB);
		OperBlock * front = targetOB->getFront();
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
		cout << " shortest path failed ! Can't find the targetBlock when allow " << allRratio << " !" << endl;
	}
	else
	{
		cout << endl;
		cout << "!!!!!!!!!!!!!!!!!!!!!!!! find the soln route !!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
		cout << "The shortestPath from (" << _sourceBlock->getX() << "," << _sourceBlock->getY() << ") to (" << targetBlock->getX() << "," << targetBlock->getY() << ") is :" << endl;
		cout << endl;
		cout << "x" << "\t" << "y" << "\t" << "time" << "\t" << "AvgWind" << "\t" << "rank" << "\t" << "violations" << endl;
		for (int i = 0; i <OperRoute.size(); i++)
		{
			//开挂的时候可能不是142 328
			int absPlus = abs(OperRoute[i]->getBlock()->getX() - _sourceBlock->getX()) + abs(OperRoute[i]->getBlock()->getY() - _sourceBlock->getY());
			/*if (absPlus * 2 + 180 < OperRoute[i]->getSolnTime())
			{
			cout << "!!!!!!!!!!loop is here !!!!!!!!!" << endl;
			}*/
			if (absPlus * 2 + Util::startTime_BFS > OperRoute[i]->getSolnTime())
			{
				cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!bug is here !!!!!!!!!!!!!!!!!!!!!!" << endl;
			}

			//cout << "(" << OperRoute[i]->getBlock()->getX() << "," << OperRoute[i]->getBlock()->getY() << ")Solntime is" << OperRoute[i]->getSolnTime() << " and the wind that time is" << OperRoute[i]->getBlock()->getWind(OperRoute[i]->getSolnTime() / 60) << "->" << endl;

			if (OperRoute[i]->getSolnTime() != Util::startTime_BFS)
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, Util::initRatio_forAllR) << "\t" << OperRoute[i]->getBlock()->getViolations();
				if (OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, Util::initRatio_forAllR) == 8 || OperRoute[i]->getBlock()->getNumOf_littleWind((OperRoute[i]->getSolnTime() - Util::flyTime) / 60, Util::initRatio_forAllR) == 9)
				{
					vector<int > sayNoIndex = OperRoute[i]->getBlock()->getSayNoModelId((OperRoute[i]->getSolnTime() - Util::flyTime) / 60);
					for (int sni = 0; sni<sayNoIndex.size(); sni++)
					{
						cout << "\t" << sayNoIndex[sni];
					}
				}
				cout << endl;
			}
			else
			{
				cout << OperRoute[i]->getBlock()->getX() << "\t" << OperRoute[i]->getBlock()->getY() << "\t" << OperRoute[i]->getSolnTime() << "\t" << OperRoute[i]->getBlock()->getAvgWind(OperRoute[i]->getSolnTime() / 60) << "\t" << OperRoute[i]->getBlock()->getNumOf_littleWind(OperRoute[i]->getSolnTime() / 60, Util::initRatio_forAllR) << "\t" << OperRoute[i]->getBlock()->getViolations() << endl;
			}

		}
		cout << endl;
		cout << endl;
	}


	return OperRoute;
}