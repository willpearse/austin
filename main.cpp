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
using namespace std;

int main(int argc, const char * argv[]) {
	//Testing likelihood

	//Setup random number generator
	//Change environmental variable 'GSL_RNG_SEED' to change the seed
	const gsl_rng_type * T;
	gsl_rng * r;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);

	//Setup
	vector<string> names(2);names[0]="Mean";names[1]="SD";
	DataModel test_data("/Users/will/Documents/code/Austin/src/test_data.txt", names);
	test_data.print_data();
	vector<double> starting(2);starting[0]=3.0; starting[1]=0.2;
	vector<double> proposals(2);proposals[0]=0.2; proposals[1]=0.2;

	Chain test_chain(10000000, 10000, &test_data, proposals, r);
	test_chain.run(starting);
	test_chain.write_output("/Users/will/Documents/code/Austin/src/test_output.txt");

	//Free random number generator
	gsl_rng_free(r);
	return 0;
}
