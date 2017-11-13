#pragma once
#include"Util.h"
#include"OperBlock.h"
class Validation
{
private:
	vector<OperBlock*> _soln;
public:
	Validation(vector<OperBlock*> soln);

	double validate();
};

