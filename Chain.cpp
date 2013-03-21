/*
 * Chain.cpp
 *
 *  Created on: 21 Mar 2013
 *      Author: will
 */

#include "Chain.h"
using namespace std;


//Constructor
Chain::Chain(int n_i, int t, DataModel* d_m, vector<double> p_p, gsl_rng* r_g) : proposal_parameters(p_p), n_iter(n_i), thinner(t), current_iter(-1)
{
	data_model = d_m;
	rnd_generator = r_g;
}


//Proposal function
std::vector<double> Chain::proposal(void)
{
	vector<double> output(proposal_parameters.size());
	for(int i=0; i<proposal_parameters.size(); ++i)
		output[i] += gsl_ran_gaussian(rnd_generator, proposal_parameters[i]);

	return(output);
}

//Run with no starting values
void Chain::run(vector<double> start_parameters)
{
	state.push_back(start_parameters);
	for(int i=1; i < n_iter; ++i)
	{
		vector<double> proposed_params = proposal();
		vector<double> new_state = data_model->calculate(proposed_params);
		vector<double> old_state = data_model->calculate(state[i-1]);
		double ratio = new_state[2] - old_state[2];
		if(ratio > 0.0 || gsl_ran_flat(rnd_generator, 0.0, 1.0) <= ratio)
		{
			state.push_back(proposed_params);
			prior_state.push_back(new_state[0]);
			log_lik_state.push_back(new_state[1]);
			posterior_state.push_back(new_state[2]);
		}

		else
		{
			state.push_back(state[i-1]);
			prior_state.push_back(old_state[0]);
			log_lik_state.push_back(old_state[1]);
			posterior_state.push_back(old_state[2]);
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
	handle << "Prior,LogLikelihood,Posterior" << endl;

	//Each element
	for(int i=0; i<state.size(); ++i)
	{
		if(i % thinner == 0)
		{
			for(int j=0; j<data_model->param_names.size(); ++j)
				handle << state[i][j] << ",";
			handle << prior_state[i] << "," << log_lik_state[i] << "," << posterior_state[i] << endl;
		}
	}
	handle.close();
}
