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

vector<OperBlock *> BFS::solve_all_valid(Block * targetBlock)
{
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, 0);
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
			int thisTime = ingOperBlock->getTime();
			bool allCangoToOper = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock(cangoto, thisTime))
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
						if (!isInOperVector(cangoto, _vistedOperBlocks))
						{
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + Util::flyTime);
							cangotoOperBlock->setFront(ingOperBlock);
							_ingOperBlocks.push(cangotoOperBlock);
						}

					}
				}
				//in order to assure the function for the whole system , the " break " should write after all the cangotoOperBlocls.
				// version 2017-11-9
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
				// stay for next 2 minutess
			{
				ingOperBlock->setTime(thisTime + Util::flyTime);
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
		// when print don't forget to inverted order		
		OperRoute.push_back(targetOperBlock);
		OperBlock * front = targetOperBlock->getFront();
		while (front->getFront() != NULL)
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
		cout << "The shortestPath from ("<<_sourceBlock->getX()<<","<< _sourceBlock->getY()<<") to ("<<targetBlock->getX()<<","<<targetBlock->getY()<<") is :"<< endl;
		for (int i = OperRoute.size() - 1; i = 0; i--)
		{
			cout << "("<<OperRoute[i]->getX()<<","<<OperRoute[i]->getY()<< ")->";
		}
		cout << endl;
	}

	delete sourceOperBlock;
	sourceOperBlock = NULL;

	delete targetOperBlock;
	targetOperBlock = NULL;

	//I don't do memory management for vistedOperBlocks and ingBlocks because this function maybe used for other destinations.

	return OperRoute;
}
