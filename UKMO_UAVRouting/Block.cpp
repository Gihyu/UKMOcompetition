#include "Block.h"



Block::Block(int id, int x, int y):
	_id(id),_x(x),_y(y)
{
}

pair<int, int> Block::getCoordinate()
{
	pair<int, int> p;
	p.first = _x;
	p.second = _y;
	return p;
}

double Block::getWind(int thisTime)
{
	//need to add something here

	return _wind;
}
