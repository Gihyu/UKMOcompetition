#include "City.h"
int City::_count = 0;



City::City(int no, Block * block):_no(no),_block(block)
{
	_id = _count;
	_count++;
}

void City::print()
{
	cout << "City_" << _id << "\tNo:" << _no << "\t";
	_block->print();
}
