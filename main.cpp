//============================================================================
// Name        : austin.cpp
// Author      : Will Pearse
// Version     :
// Copyright   : GNU GPL 3, additionally required to cite author (contact for details)
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <vector>
#include <cmath>
#include <valarray>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include "DataModel.h"
#include "Chain.h"
#include "Master.h"

using namespace std;

int main(int argc, const char * argv[]) {
	//Setup
	vector<string> names(2);names[0]="Mean";names[1]="SD";
	DataModel test_data("/Users/will/Documents/code/Austin/src/test_data.txt", names);
	vector<double> starting(2);starting[0]=0.8; starting[1]=0.2;
	vector<vector<double> > starting_vector(2);
	starting_vector[0]=starting;starting_vector[1]=starting;
	vector<double> proposals(2);proposals[0]=0.2; proposals[1]=0.2;
	vector<long unsigned int> seeds(2);seeds[0]=123456;seeds[1]=456789;

	//Initialising
	Master test_master(2, 1000000, 1000, 1, &test_data, seeds, proposals, starting_vector);
	test_master.start();
	test_master.write_output("combined.txt");
	return 0;
}
