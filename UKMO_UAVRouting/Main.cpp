#include"Util.h"
#include"Schedule.h"
#include"IO.h"
#include"PathSolver.h"

#include"BFS.h"

int main()
{

//int main(int argc, char * argv[])
//{
	/*cout << "Argv[0]: " << argv[0] << "\t";
	cout << "Argv[1]: " << argv[1] << "\t";
	cout << "Argv[2]: " << argv[2] << endl;*/

	////sjy testCase for BFS
	//Block * testBlock = new Block(100,100,100);
	//BFS * testBFS = new BFS(testBlock);
	//testBFS->run_the_case();
	////sjy end

	//zhoulei
	Schedule* sche = new Schedule();
	IO* io = new IO();

	int date = 7;

	//int date = atoi(argv[1]);

	io->input(sche, date);
	
	sche->init();

	Block* origin = sche->getCity(0);//the blcok corresponding to origin

	int cityNo = 8;

	//int cityNo = atoi(argv[2]);

	Block* destination = sche->getCity(cityNo);//the blcok corresponding to the destination
	vector<Block*> blocks = sche->getBlockList();//all blocks
	//PathSolver* solver = new PathSolver(origin, destination,blocks);//from origin to destination
	PathSolver* solver = new PathSolver(origin, sche->getCityList(), blocks);//from origin to all destinations
	solver->solve();//find a path by some method;please call bfs in PathSolver::solve()!!
	//io->outputSoln(cityNo,origin,solver->getSoln());
	io->outputMultiSoln(sche->getCityList(), origin, solver->getMultiSoln());//soln is not required exactly
	//zhoulei end



	return 0;
}