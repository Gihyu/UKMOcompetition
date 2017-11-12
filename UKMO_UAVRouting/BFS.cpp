#include "BFS.h"

BFS::BFS(Block * sourceBlock):
_sourceBlock(sourceBlock)
{

}



bool BFS::isInOperVector(Block * test, vector<OperBlock *> testVector)
{
	bool whetherIn = false;
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
	return whetherIn;
}

vector<OperBlock *> BFS::solve_all_valid(Block * targetBlock)
{
	OperBlock * sourceOperBlock = new OperBlock(_sourceBlock, 0);
	sourceOperBlock->setFront(NULL);
	_ingBlocks.push(sourceOperBlock);

	int targetX = targetBlock->getX();
	int targetY = targetBlock->getY();
	bool findTheTarget = false;
	OperBlock * targetOperBlock = NULL;

	// if we don't need the analysis for the actual steops even > 360 , use this "while" sentence
	//while(!ingBlocks.empty() && !findTheTarget && ingBlock->getTime()<360)

	while (!_ingBlocks.empty() && !findTheTarget)
	{
		OperBlock * ingOperBlock = _ingBlocks.front();
		vector<Block *> cangoToBlocks = ingOperBlock->getBlock()->getCangoToBlocks();

		if (!cangoToBlocks.empty())
		{
			int thisTime = ingOperBlock->getTime();
			bool allcantGoto = true;
			for (auto & cangoto : cangoToBlocks)
			{
				if (ingOperBlock->cangotoThisBlock(cangoto, thisTime))
				{
					allcantGoto = false;
					if (cangoto->getX() == targetX && cangoto->getY() == targetY)
					{
						targetOperBlock = new OperBlock(cangoto, thisTime + 2);
						targetOperBlock->setFront(ingOperBlock);
						findTheTarget = true;
						break;
					}
					else
					{
						if (!isInOperVector(cangoto, _vistedOperBlocks) || ingOperBlock->getBlock() == cangoto)
						{
							OperBlock * cangotoOperBlock = new OperBlock(cangoto, thisTime + 2);
							cangotoOperBlock->setFront(ingOperBlock);
							_ingBlocks.push(cangotoOperBlock);
						}

					}
				}
				//in order to assure the function for the whole system , the " break " should write after all the cangotoOperBlocls.
				// version 2017-11-7
			}

			// stay for next 2 minutes
			if (allcantGoto)
			{
				ingOperBlock->setTime(thisTime + 2);
				_ingBlocks.push(ingOperBlock);
				// maybe here is a function to plus not 2 but the number which let the wind changed
			}
			else
			{
				_vistedOperBlocks.push_back(ingOperBlock);
				_ingBlocks.pop();
			}

		}
		else
		{
			_vistedOperBlocks.push_back(ingOperBlock);
			_ingBlocks.pop();
		}


	}

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

	//I don't do memory management for vistedOperBlocks and ingBlocks because this function maybe used for other destinations.


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
	return OperRoute;
}
