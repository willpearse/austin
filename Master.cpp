/*
 * Master.cpp
 *
 *  Created on: 22 Mar 2013
 *      Author: will
 */

#include "Master.h"

using namespace std;

Master::Master(int n_c, int n_i, int n_t, int t, DataModel* d_m, vector<unsigned long int> s, vector<double> p, vector<vector<double> > s_p) : n_chains(n_c), n_iter(n_i), n_threads(n_t), thinner(t), seeds(s), proposals(p), starting(s_p)
{
	data_model = d_m;

	//Initialise the chains
	for(int i=0; i<n_chains; ++i)
	{
		Chain chain(1000000, 1000, data_model, proposals, seeds[i]);
		chains.push_back(chain);
	}
}

void Master::start(void)
{
	if(n_threads > 1)
	{
		vector<thread> threads;
		int running_threads = 0;
		for(int i=0; i<n_chains; ++i)
		{
			if(running_threads < n_threads)
				threads.push_back(thread(&Chain::run, &chains[i], starting[i]));
			else
			{
				for(int j=0; j<threads.size(); ++j)
					threads[j].join();
				threads.clear();
			}
		}
		if(threads.size() > 0)
			for(int j=0; j<threads.size(); ++j)
				threads[j].join();
	}
	else
	{
		for(int i=0; i<n_chains; ++i)
			chains[i].run(starting[i]);
	}
}

void Master::write_output(const char* file)
{
	//Setup
	ofstream handle;
	handle.open(file);

	//Header row
	for(vector<string>::const_iterator iter = data_model->param_names.begin(); iter != data_model->param_names.end(); ++iter)
		handle << *iter << ",";
	handle << "Step,Prior,LogLikelihood,Posterior,Chain" << endl;

	for(int i=0; i<n_chains; ++i)
	{
		vector<vector<double> > chain_output = chains[i].return_output();
		for(int j=0; j<chain_output.size(); ++j)
		{
			for(int k=0; k<chain_output[j].size(); ++k)
				handle << chain_output[j][k] << ",";
			handle << i << endl;
		}
	}
	handle.close();
}
