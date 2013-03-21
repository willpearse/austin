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
using namespace std;

//Calculate log likelihood
double log_likelihood(std::vector<double> parameters, std::vector<double> data)
{
	//Setup
	double log_lik = 0.0;
	double mean = parameters[0];
	double sd = parameters[1];

	//Calculate accumulate log likelihood
	for(int i=0; i<data.size(); ++i)
	{
		log_lik += log(gsl_ran_gaussian_pdf(data[i] - mean, sd));
	}

	return log_lik;
}

//Proposal function
std::vector<double> proposal(std::vector<double> parameters, std::vector<double> sd, gsl_rng * rnd_generator)
{
	//Generate proposed new values
	for(int i=0; i<parameters.size(); ++i)
	{
		parameters[0] += gsl_ran_gaussian(rnd_generator, sd[i]);
	}

	//Cleanup and return
	return(parameters);

}

int main() {
	//Testing likelihood function
	vector<double> test_data(5);
	test_data[0]=1.0;test_data[1]=1.0;test_data[2]=1.0;test_data[3]=1.0;test_data[4]=1.0;
	vector<double> test_param(2);
	test_param[0]=2.0;test_param[1]=1.0;
	cout << log_likelihood(test_param, test_data) << endl;
	vector<double> test_prop_sd(2);
	test_prop_sd[0]=0.2;test_prop_sd[1]=0.2;

	//Setup random number generator
	//Change environmental variable 'GSL_RNG_SEED' to change the seed
	const gsl_rng_type * T;
	gsl_rng * r;
	gsl_rng_env_setup();
	T = gsl_rng_default;
	r = gsl_rng_alloc (T);
	vector<double> test_proposal = proposal(test_prop_sd, test_data, r);
	cout << test_proposal[0] << "," << test_proposal[1] << endl;

	//Do an MCMC
	vector<vector<double> > chain(10000);
	chain[0] = test_param;
	for(int i=1; i < 10000; ++i)
	{
		vector<double> proposed_params = proposal(test_prop_sd, test_data, r);
		//Accept the new state if it's more likely, and sometimes take a bad step
		double ratio = log_likelihood(proposed_params, test_data) - log_likelihood(chain[i-1], test_data);
		if(ratio > 0.0 || gsl_ran_flat(r, 0.0, 1.0) <= ratio)
			chain[i] = proposed_params;
		else
			chain[i] = chain[i-1];
	}

	cout << "Final value:" << chain[9999][0] << "," << chain[9999][1] << endl;

	//Free random number generator
	gsl_rng_free(r);
	return 0;
}
