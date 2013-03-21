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
private:
	DataModel* data_model;
	std::vector<std::vector<double> > state;
	std::vector<double> posterior_state, prior_state, log_lik_state;
	std::vector<double> proposal_parameters;
	gsl_rng* rnd_generator;
	int n_iter, thinner;
	int current_iter;

	std::vector<double> proposal(void);
public:
	Chain(int n_i, int t, DataModel* d_m, std::vector<double> p_p, gsl_rng* r_g);

	void run(std::vector<double> start_parameters);
	void write_output(const char *file);
	double prior(std::vector<double> parameters);

};


#endif /* CHAIN_H_ */
