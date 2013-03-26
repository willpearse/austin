/*
 * Chain.h
 *
 *  Created on: 21 Mar 2013
 *      Author: will
 */

#ifndef CHAIN_H_
#define CHAIN_H_


#include <iostream>
#include <vector>
#include <cmath>
#include <valarray>
#include <gsl/gsl_randist.h>
#include <gsl/gsl_rng.h>
#include <functional>
#include <algorithm>
#include <cassert>
#include <vector>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <numeric>
#include "DataModel.h"

class Chain{
protected:
	DataModel* data_model;
	std::vector<std::vector<double> > state;
	std::vector<double> current_params, proposal_parameters, posterior_state, prior_state, log_lik_state;
	std::vector<int> iter_log;
	gsl_rng* rnd_generator;
	const gsl_rng_type * T;
	int n_iter, thinner, curr_iter;

	std::vector<double> proposal(void);
public:
	Chain(int n_i, int t, DataModel* d_m, std::vector<double> p_p, long unsigned int seed, std::vector<double> s_p);

	void run(void);
	int next_step(void);
	void write_output(const char *file);
	std::vector<std::vector<double> > return_output(void);
	double prior(std::vector<double> parameters);
	int swap(Chain* proposal_chain);
	std::vector<double > calculate(void);
	std::vector<double > parameters(void);


};

class ChainHeat: public Chain{
protected:
	double heat;

public:
	ChainHeat(int n_i, int t, double h, DataModel* d_m, std::vector<double> p_p, long unsigned int seed, std::vector<double> s_p);
	void run(void);
	int next_step(void);

	std::vector<std::vector<double> > return_output(void);
};


#endif /* CHAIN_H_ */
