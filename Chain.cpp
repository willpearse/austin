/*
 * Chain.cpp
 *
 *  Created on: 21 Mar 2013
 *      Author: will
 */

#include "Chain.h"
using namespace std;


//Constructor
Chain::Chain(int n_i, int t, DataModel* d_m, vector<double> p_p, long unsigned int seed) : proposal_parameters(p_p), n_iter(n_i), thinner(t), current_iter(-1)
{
	data_model = d_m;
	T = gsl_rng_default;
	rnd_generator = gsl_rng_alloc (T);
	gsl_rng_set (rnd_generator, seed);
}


//Proposal function
std::vector<double> Chain::proposal(void)
{
	//Make proposals
	vector<double> output(proposal_parameters.size());
	for(int i=0; i<proposal_parameters.size(); ++i)
		output[i] += gsl_ran_gaussian(rnd_generator, proposal_parameters[i]);

	return(output);
}

//Run with starting values
void Chain::run(vector<double> start_parameters)
{
	state.push_back(start_parameters);
	iter_log.push_back(0);
	current_params = start_parameters;
	for(int i=1; i < n_iter; ++i)
	{
		vector<double> proposed_params = proposal();
		vector<double> new_state = data_model->calculate(proposed_params);
		vector<double> old_state = data_model->calculate(current_params);
		double ratio = new_state[2] - old_state[2];
		if(ratio > 0.0 || gsl_ran_flat(rnd_generator, 0.0, 1.0) <= ratio)
		{
			current_params = proposed_params;
			if(i % thinner == 0)
			{
				state.push_back(proposed_params);
				prior_state.push_back(new_state[0]);
				log_lik_state.push_back(new_state[1]);
				posterior_state.push_back(new_state[2]);
				iter_log.push_back(i);
			}
		}

		else
			if(i % thinner == 0)
			{
				state.push_back(current_params);
				prior_state.push_back(old_state[0]);
				log_lik_state.push_back(old_state[1]);
				posterior_state.push_back(old_state[2]);
				iter_log.push_back(i);
			}
	}
}

//Write output
void Chain::write_output(const char *file)
{
	//Setup
	ofstream handle;
	handle.open(file);

	//Header row
	for(vector<string>::const_iterator iter = data_model->param_names.begin(); iter != data_model->param_names.end(); ++iter)
		handle << *iter << ",";
	handle << "Step,Prior,LogLikelihood,Posterior" << endl;

	//Each element
	for(int i=0; i<state.size(); ++i)
	{
		for(int j=0; j<data_model->param_names.size(); ++j)
			handle << state[i][j] << ",";
		handle << iter_log[i] << "," << prior_state[i] << "," << log_lik_state[i] << "," << posterior_state[i] << endl;
	}
	handle.close();
}

//Return output (for Master)
vector<vector<double> > Chain::return_output()
{
	vector<vector<double> > output;
	for(int i=0; i<state.size(); ++i)
	{
		int j=0;vector<double> iteration(data_model->param_names.size()+4);
		for(; j<data_model->param_names.size(); ++j)
			iteration[j] = state[i][j];
		iteration[j++] = iter_log[i];
		iteration[j++] = prior_state[i];
		iteration[j++] = log_lik_state[i];
		iteration[j] = posterior_state[i];
		output.push_back(iteration);
	}
	return output;
}
