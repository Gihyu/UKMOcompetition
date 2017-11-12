#include"Util.h"
#include"Schedule.h"
#include"IO.h"
#include"PathSolver.h"

int main()
{
	Schedule* sche = new Schedule();
	IO* io = new IO();
	int date = 1;
	io->input(sche, date);
	
	sche->init();


	Block* origin = sche->getCity(0);//the blcok corresponding to origin
	int cityNo = 1;
	Block* destination = sche->getCity(cityNo);//the blcok corresponding to the destination
	vector<Block*> blocks = sche->getBlockList();//all blocks
	PathSolver* solver = new PathSolver(origin, destination,blocks);//from origin to destination
	solver->solve();//find a path by some method
	io->outputSoln(cityNo,origin,solver->getSoln());

	return 0;
}