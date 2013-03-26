/*
 * Master.cpp
 *
 *  Created on: 22 Mar 2013
 *      Author: will
 */

#include "Master.h"

using namespace std;

//Constructor for (MC)MCMC
Master::Master(int n_c, int n_i, int t, int n_t, DataModel* d_m, vector<unsigned long int> s, vector<double> p, vector<vector<double> > s_p) :
		n_chains(n_c), n_iter(n_i), n_threads(n_t), thinner(t), seeds(s), proposals(p), starting(s_p)
{
	data_model = d_m;

	//Initialise the chains
	for(int i=0; i<n_chains; ++i)
	{
		chains.push_back(new Chain(n_iter, thinner, data_model, proposals, seeds[i], starting[i]));
		chain_types.push_back("standard");
	}

}

//Constructor for heated (MC)MCMC
Master::Master(int n_i, int t, int n_t, DataModel* d_m, vector<unsigned long int> s, vector<double> p, vector<vector<double> > s_p, vector<double> c_h) :
		 n_iter(n_i), n_threads(n_t), thinner(t), seeds(s), proposals(p), chain_heats(c_h), starting(s_p)
{
	data_model = d_m;
	n_chains = chain_heats.size() + 1;

	//Initialise the chains
	chains.push_back(new Chain(n_iter, thinner, data_model, proposals, seeds[0], starting[0]));
	chain_types.push_back("standard");
	for(int i=1; i<n_chains; ++i)
	{
		chains.push_back(new ChainHeat(n_iter, thinner, chain_heats[i-1], data_model, proposals, seeds[i], starting[i]) );
		chain_types.push_back("heated");
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
				threads.push_back(thread(&Chain::run, chains[i]));
			else
			{
				for(unsigned int j=0; j<threads.size(); ++j)
					threads[j].join();
				threads.clear();
			}
		}
		if(threads.size() > 0)
			for(unsigned int j=0; j<threads.size(); ++j)
				threads[j].join();
	}
	else
	{
		for(int i=0; i<n_chains; ++i)
			chains[i]->run();
	}
}

void Master::start_swap(unsigned long int seed)
{
	//Setup random draws
	gsl_rng* rnd_generator;
	const gsl_rng_type * T;
	T = gsl_rng_default;
	rnd_generator = gsl_rng_alloc (T);
	gsl_rng_set (rnd_generator, seed);

	//Setup chain finished information
	vector<int> finished(n_chains, 0);
	while(!finished[0])
	{
		//Run each unfinished chain
		for(int i=0; i<n_chains; ++i)
			if(!finished[i])
			{
				if(chain_types[i]=="standard")
					finished[i] = chains[i]->next_step();
				else
					finished[i] = static_cast<ChainHeat*>(chains[i])->next_step();
			}



		//Select two random chains
		// - allow swaps from 'finished' chains as this is all rather arbitrary anyway
		int donor = gsl_rng_uniform_int(rnd_generator, n_chains);
		int receiver = gsl_rng_uniform_int(rnd_generator, n_chains-1);
		if(receiver == donor)
			++receiver;

		//Swap
		chains[receiver]->swap(chains[donor]);
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
		vector<vector<double> > chain_output;
		if(chain_types[i]=="standard")
			chain_output = chains[i]->return_output();
		else
			chain_output = static_cast<ChainHeat*>(chains[i])->return_output();
		//Heated chains have no return information
		for(unsigned int j=0; j<chain_output.size(); ++j)
		{
			for(unsigned int k=0; k<chain_output[j].size(); ++k)
				handle << chain_output[j][k] << ",";
			handle << i << endl;
		}
	}
	handle.close();
}
