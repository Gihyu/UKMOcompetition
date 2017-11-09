#include "City.h"

City::City(int id, int no, int x, int y)
	:_id(id),_no(no),_x(x),_y(y)
{
}

void City::print()
{
	cout << "City_" << _id << "\tNo:" << _no << "\tx:" << _x << "\ty:" << _y << endl;
}
