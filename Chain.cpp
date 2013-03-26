/*
 * Chain.cpp
 *
 *  Created on: 21 Mar 2013
 *      Author: will
 */

#include "Chain.h"
using namespace std;

////////////////////
//CHAIN/////////////
////////////////////

//Constructor
Chain::Chain(int n_i, int t, DataModel* d_m, vector<double> p_p, long unsigned int seed, vector<double>s_p) :
		state(0), current_params(s_p), proposal_parameters(p_p), posterior_state(0), prior_state(0), log_lik_state(0), n_iter(n_i), thinner(t), curr_iter(0)
{
	data_model = d_m;
	T = gsl_rng_default;
	rnd_generator = gsl_rng_alloc (T);
	gsl_rng_set (rnd_generator, seed);
	state.push_back(current_params);
	iter_log.push_back(0);
}


//Proposal function
std::vector<double> Chain::proposal(void)
{
	//Make proposals
	vector<double> output(proposal_parameters.size());
	for(unsigned int i=0; i<proposal_parameters.size(); ++i)
		output[i] += gsl_ran_gaussian(rnd_generator, proposal_parameters[i]);

	return(output);
}

//Run
void Chain::run(void)
{
	for(curr_iter=1; curr_iter<n_iter; ++curr_iter)
	{
		vector<double> proposed_params = proposal();
		vector<double> new_state = data_model->calculate(proposed_params);
		vector<double> old_state = data_model->calculate(current_params);
		double ratio = new_state[2] - old_state[2];
		if(ratio > 0.0 || gsl_ran_flat(rnd_generator, 0.0, 1.0) <= pow(10.0, ratio))
		{
			current_params = proposed_params;
			if(curr_iter % thinner == 0)
			{
				state.push_back(proposed_params);
				prior_state.push_back(new_state[0]);
				log_lik_state.push_back(new_state[1]);
				posterior_state.push_back(new_state[2]);
				iter_log.push_back(curr_iter);
			}
		}

		else
			if(curr_iter % thinner == 0)
			{
				state.push_back(current_params);
				prior_state.push_back(old_state[0]);
				log_lik_state.push_back(old_state[1]);
				posterior_state.push_back(old_state[2]);
				iter_log.push_back(curr_iter);
			}
	}
}

//One iteration to allow swapping
int Chain::next_step(void)
{
	++curr_iter;
	vector<double> proposed_params = proposal();
	vector<double> new_state = data_model->calculate(proposed_params);
	vector<double> old_state = data_model->calculate(current_params);
	double ratio = new_state[2] - old_state[2];
	if(ratio > 0.0 || gsl_ran_flat(rnd_generator, 0.0, 1.0) <= pow(10.0, ratio))
	{
		current_params = proposed_params;
		if(curr_iter % thinner == 0)
		{
			state.push_back(proposed_params);
			prior_state.push_back(new_state[0]);
			log_lik_state.push_back(new_state[1]);
			posterior_state.push_back(new_state[2]);
			iter_log.push_back(curr_iter);
		}
	}

	else
		if(curr_iter % thinner == 0)
		{
			state.push_back(current_params);
			prior_state.push_back(old_state[0]);
			log_lik_state.push_back(old_state[1]);
			posterior_state.push_back(old_state[2]);
			iter_log.push_back(curr_iter);
		}
	if(curr_iter == n_iter)
		return 1;
	else
		return 0;
}

//Return current state
vector<double> Chain::calculate(void)
{
	return data_model->calculate(current_params);
}

//Return current parameters
vector<double> Chain::parameters(void)
{
	return current_params;
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
	for(unsigned int i=0; i<state.size(); ++i)
	{
		for(unsigned int j=0; j<data_model->param_names.size(); ++j)
			handle << state[i][j] << ",";
		handle << iter_log[i] << "," << prior_state[i] << "," << log_lik_state[i] << "," << posterior_state[i] << endl;
	}
	handle.close();
}

//Return output (for Master)
vector<vector<double> > Chain::return_output()
{
	vector<vector<double> > output;
	for(unsigned int i=0; i<state.size(); ++i)
	{
		unsigned int j=0;vector<double> iteration(data_model->param_names.size()+4);
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

//Consider proposal from another chain
int Chain::swap(Chain* proposal_chain)
{
	++curr_iter;
	vector<double> new_state = proposal_chain->calculate();
	vector<double> old_state = data_model->calculate(current_params);
	double ratio = new_state[2] - old_state[2];
	if(ratio > 0.0 || gsl_ran_flat(rnd_generator, 0.0, 1.0) <= pow(10.0, ratio))
	{
		current_params = proposal_chain->parameters();
		if(curr_iter % thinner == 0)
		{
			state.push_back(current_params);
			prior_state.push_back(new_state[0]);
			log_lik_state.push_back(new_state[1]);
			posterior_state.push_back(new_state[2]);
			iter_log.push_back(curr_iter);
		}
		return 1;
	}

	else
		if(curr_iter % thinner == 0)
		{
			state.push_back(current_params);
			prior_state.push_back(old_state[0]);
			log_lik_state.push_back(old_state[1]);
			posterior_state.push_back(old_state[2]);
			iter_log.push_back(curr_iter);
		}
	return 0;
}

////////////////////
//ChainHeat//////////
////////////////////

//Constructor
ChainHeat::ChainHeat(int n_i, int t, double h, DataModel* d_m, vector<double> p_p, long unsigned int seed, vector<double>s_p) :
		Chain(n_i, t, d_m, p_p, seed, s_p), heat(h)
{
	data_model = d_m;
	T = gsl_rng_default;
	rnd_generator = gsl_rng_alloc (T);
	gsl_rng_set (rnd_generator, seed);
}

//Run
//Note: heated chains don't store any output, because it's of no use!
void ChainHeat::run(void)
{
	for(curr_iter=1; curr_iter < n_iter; ++curr_iter)
	{
		vector<double> proposed_params = proposal();
		vector<double> new_state = data_model->calculate(proposed_params);
		vector<double> old_state = data_model->calculate(current_params);
		double ratio = heat * (new_state[2] - old_state[2]);
		if(ratio > 0.0 || gsl_ran_flat(rnd_generator, 0.0, 1.0) <= pow(10.0, ratio))
			current_params = proposed_params;
	}
}

//One iteration to allow swapping
int ChainHeat::next_step(void)
{
	++curr_iter;
	vector<double> proposed_params = proposal();
	vector<double> new_state = data_model->calculate(proposed_params);
	vector<double> old_state = data_model->calculate(current_params);
	double ratio = heat * (new_state[2] - old_state[2]);
	if(ratio > 0.0 || gsl_ran_flat(rnd_generator, 0.0, 1.0) <= pow(10.0, ratio))
		current_params = proposed_params;
	if(curr_iter == n_iter)
		return 1;
	else
		return 0;
}

//Return output (for Master)
vector<vector<double> > ChainHeat::return_output(void)
{
	vector<double> final(data_model->param_names.size()+4);
	vector<vector<double> > output;
	unsigned int i=0;
	for(; i<data_model->param_names.size(); ++i)
		final[i] = current_params[i];
	vector<double> stats = calculate();
	final[i++] = -1.0;
	final[i++] = stats[0];
	final[i++] = stats[1];
	final[i] = stats[2];
	output.push_back(final);
	return output;
}
