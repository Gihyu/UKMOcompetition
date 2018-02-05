#include"Util.h"
#include"Schedule.h"
#include"IO.h"
#include"PathSolver.h"

#include"BFS.h"
#include"Validation.h"

void print();

int main()
{

//int main(int argc, char * argv[])
//{
	/*cout << "Argv[0]: " << argv[0] << "\t";
	cout << "Argv[1]: " << argv[1] << "\t";
	cout << "Argv[2]: " << argv[2] << endl;*/


	//zhoulei
	Schedule* sche = new Schedule();
	IO* io = new IO();

	int date = 6;//should not 
	//int date = atoi(argv[1]);
	Util::realization =3;//model id

	Util::inputMode = M_Single;//M_Single;M_Multi;M_SingleAndMulti
	cout << "date:" << date << "Util::realization:" << Util::realization << endl;
	print();
	io->input(sche, date);
	//exit(0);
	
	sche->init();

	Block* origin = sche->getCity(0);//the blcok corresponding to origin

	int cityNo = 8;

	//int cityNo = atoi(argv[2]);

	Block* destination = sche->getCity(cityNo);//the blcok corresponding to the destination
	vector<Block*> blocks = sche->getBlockList();//all blocks
	//PathSolver* solver = new PathSolver(origin, destination,blocks);//from origin to destination
	PathSolver* solver = new PathSolver(origin, sche->getCityList(), blocks);//from origin to all destinations


	//solver->solve();//find a path by some method;please call bfs in PathSolver::solve()!!
	//solver->solve_allR_fixWind_changeNum();
	//solver->solve_allR_fixNum_changeWind();
	//solver->solve_allR_justAvg();
	//solver->solve_allR_backtrack();
	//solver->solve_valued_by_allR();

	//solver->solve_singleModel_evabyallR_backtrack();

	//solver->solve_CBNT_backtrack();

	//season 2

	//solver->solve_backtrack_single_rain();
	solver->solve_backtrack_single_rain_logs();



	//io->outputSoln(cityNo,origin,solver->getSoln());
	io->outputMultiSoln(sche->getCityList(), origin, solver->getMultiSoln());//soln is not required exactly
	//zhoulei end

	if (date <= 5)
	{
		Validation* val = new Validation(sche->getCityList());
		val->validate();
	}

	return 0;
}

void print()
{
	cout << "Util::inputMode\t";
	switch (Util::inputMode)
	{
	case M_Single:
		cout << "M_Single";
		break;
	case M_Multi:
		cout << "M_Multi";
		break;
	case M_SingleAndMulti:
		cout << "M_SingleAndMulti";
		break;
	default:
		cout << "NIL";
		break;
	}
	cout << endl;

}