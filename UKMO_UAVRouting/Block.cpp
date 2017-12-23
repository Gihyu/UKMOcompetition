#include "Block.h"
int Block::_count = 0;

Block::Block(int x, int y, int date, array<double, Util::hourCount> windArr):
	_x(x),_y(y),_date(date),_windArr(windArr)
{
	_id = _count;
	_count++;
}

Block::Block(int x, int y, int date, array<array<double, 10>, Util::hourCount> windMatrix):
	_x(x), _y(y), _date(date), _windMatrix(windMatrix)
{
	_id = _count;
	_count++;
}

pair<int, int> Block::getCoordinate()
{
	pair<int, int> p;
	p.first = _x;
	p.second = _y;
	return p;
}

bool Block::equal(Block * block)
{
	if (block->getX() != _x)
	{
		return false;
	}

	if (block->getY() != _y)
	{
		return false;
	}

	return true;
}

bool Block::equal(int x, int y)
{
	if ( x != _x)
	{
		return false;
	}

	if ( y != _y)
	{
		return false;
	}

	return true;
}

void Block::print()
{
	cout << "Block_" << _id << "\tx:" << _x << "\ty:" << _y << "\tdate:" << _date << endl;
	switch (Util::inputMode)
	{
	case M_Single:
		printWindArr();
		break;
	case M_Multi:
		printWindMatrix();
		break;
	case M_SingleAndMulti:
		printWindArr();
		printWindMatrix();
		break;
	}
}

void Block::printWindArr()
{
	cout << "windArr:";
	for (auto&wind : _windArr)
	{
		cout << "\t" << wind;
	}
	cout << endl;
}

void Block::printWindMatrix()
{
	cout << "windMatrix:\n";
	for (auto&arr : _windMatrix)
	{
		for (auto&wind : arr)
		{
			cout << "\t" << wind;
		}
		cout << endl;
	}
}

double Block::testgetWind(int thisTime)
{
	if(_time_wind.empty())
	{
		return 0.0;
	}
	else
	{	
		for (auto & time_wind : _time_wind)
		{
			if (thisTime == time_wind.first)
			{
				return time_wind.second;
			}
		}
		return 0.0;
	}
}



double Block::getAvgWind(int hour)
{
	array<double, 10> thisTimeWindArr = getWindAllRealization(hour);
	double sum = 0.0;
	double max = 0.0;
	double min = 100.0;
	for (int i = 0; i < 10; i++)
	{
		sum += thisTimeWindArr[i];
		if (thisTimeWindArr[i] > max)
		{
			max = thisTimeWindArr[i];
		}
		else if (thisTimeWindArr[i] < min)
		{
			min = thisTimeWindArr[i];
		}
	}
	double windAvg = (sum - max - min) / 8;
	return windAvg;
}

double Block::getWeightedAvgWind(int hour)
{	
	array<double, 10> thisTimeWindArr = getWindAllRealization(hour);

	double max = 0.0;
	double min = 100.0;
	int maxIndex;
	int minIndex;
	for (int i = 0; i < 10; i++)
	{
		if (thisTimeWindArr[i] > max)
		{
			max = thisTimeWindArr[i];
			maxIndex = i;
		}
		else if (thisTimeWindArr[i] < min)
		{
			min = thisTimeWindArr[i];
			minIndex = i;
		}
	}
	double sum = 0.0;
	double weightSum = 0.0;
	for (int j = 0; j < 10; j++)
	{
		if (j != maxIndex && j != minIndex)
		{
			sum += thisTimeWindArr[j] * Util::WeightOfModels[j];
			weightSum+= Util::WeightOfModels[j];
		}
	}
	double weightedAvgWind = sum / weightSum;
	return weightedAvgWind;
}

int Block::getNumOf_littleWind(int hour)
{
	int countNumOf_littleWind = 0;
	array<double, 10> thisTimeWindArr = getWindAllRealization(hour);
	for (int i = 0; i < 10; i++)
	{
		if (thisTimeWindArr[i] <Util::initRatio_forAllR)
		{
			countNumOf_littleWind++;
		}
	}
	return countNumOf_littleWind;
}

vector<int > Block::getSayNoModelId(int hour)
{
	vector<int > vioIndex;
	array<double, 10> thisTimeWindArr = getWindAllRealization(hour);
	for (int i = 0; i < 10; i++)
	{
		if (thisTimeWindArr[i] >=Util::initRatio_forAllR)
		{	
			//加1转换为真实的model号
			int  index = i+1;
			vioIndex.push_back(index);
		}
	}
	return vioIndex;
}
