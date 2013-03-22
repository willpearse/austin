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

class Master{
private:
	DataModel* data_model;
	int n_chains, n_iter, n_threads, thinner;
	std::vector<unsigned long int> seeds;
	std::vector<Chain> chains;
	std::vector<double> proposals;
	std::vector<std::vector<double> > starting;

public:
	Master(int n_c, int n_i, int n_t, int t, DataModel* d_m, std::vector<unsigned long int> s, std::vector<double> p, std::vector<std::vector<double> > s_p);
	void start(void);

	void write_output(const char* file);

};


#endif /* MASTER_H_ */
