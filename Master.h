/*
 * Master.h
 *
 *  Created on: 22 Mar 2013
 *      Author: will
 */

#ifndef MASTER_H_
#define MASTER_H_

#include "DataModel.h"
#include "Chain.h"
#include <thread>
#include <algorithm>

class Master{
protected:
	DataModel* data_model;
	int n_chains, n_iter, n_threads, thinner;
	std::vector<unsigned long int> seeds;
	std::vector<double> proposals, chain_heats;
	std::vector<std::vector<double> > starting;
	std::vector<Chain*> chains;
	std::vector<std::string> chain_types;

public:
	Master(int n_c, int n_i, int n, int n_t, DataModel* d_m, std::vector<unsigned long int> s, std::vector<double> p, std::vector<std::vector<double> > s_p);
	Master(int n_i, int t, int n_t, DataModel* d_m, std::vector<unsigned long int> s, std::vector<double> p, std::vector<std::vector<double> > s_p, std::vector<double> c_h);
	void start(void);
	void start_swap(unsigned long int seed);

	void write_output(const char* file);

};


#endif /* MASTER_H_ */
