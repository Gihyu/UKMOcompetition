#include"Util.h"
#include"Schedule.h"
#include"IO.h"
#include"PathSolver.h"

#include"BFS.h"

int main()
{
	////sjy
	//Block * testBlock = new Block(100,100,100);
	//BFS * testBFS = new BFS(testBlock);
	//testBFS->run_the_case();
	////sjy end

	//zhoulei
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
	solver->solve();//find a path by some method;please call bfs in PathSolver::solve()!!
	io->outputSoln(cityNo,origin,solver->getSoln());
	//zhoulei end



	return 0;
}