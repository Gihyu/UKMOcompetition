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
	if (Util::allRealization)
	{
		printWindMatrix();
	}
	else
	{
		printWindArr();
	}
}

void Block::printWindArr()
{
	for (auto&wind : _windArr)
	{
		cout << "\t" << wind;
	}
	cout << endl;
}

void Block::printWindMatrix()
{
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
