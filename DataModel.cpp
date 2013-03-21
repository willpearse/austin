/*
 * DataModel.cpp
 *
 *  Created on: 20 Mar 2013
 *      Author: Will Pearse
 */


#include "DataModel.h"

using namespace std;

//Housekeeping
static double string_to_double(string var)
{
    double var_i;
    stringstream convert;
    convert << var;
    convert >> var_i;
    return var_i;
}


//Constructor from file
DataModel::DataModel(const char *file, vector<string> p_n) : param_names(p_n)
{
	//Setup
	ifstream str(file);
	int n_explanatory;
	int processed_header = 0;
	string line,var;

	//For each line
	while(getline(str, line))
	{
		stringstream stream(line);
		if(processed_header)
		{
			getline(stream, var, ',');
			response.push_back(string_to_double(var));
			for(int i=0; i<explanatories.size(); ++i)
			{
				getline(stream, var, ',');
				explanatories[i].push_back(string_to_double(var));
			}
		}
		else
		{
			getline(stream, var, ',');
			response_name = var;
			while(getline(stream, var, ','))
			{
				explanatory_names.push_back(var);
				explanatories.push_back(response);
			}
			processed_header = 1;

		}
	}
}

//Likelihood (toy in this case)
double DataModel::log_likelihood(vector<double> parameters)
{
	//Setup
	double log_lik = 0.0;
	double mean = parameters[0];
	double sd = parameters[1];

	//Calculate accumulated log likelihood
	for(int i=0; i<response.size(); ++i)
		log_lik += log(gsl_ran_gaussian_pdf(response[i] - mean, sd));

	return log_lik;
}

//Sample from prior
double DataModel::prior(vector<double> parameters)
{
	//Setup
	double log_lik = 0.0;
	double mean = parameters[0];
	double sd = parameters[1];

	//Sum log_likelihoods of parameters under prior
	log_lik = log(gsl_ran_gaussian_pdf(mean, 5.0)) + log(gsl_ran_gaussian_pdf(sd, 5.0));

	return log_lik;
}

//Sample from posterior
double DataModel::posterior(vector<double> parameters)
{
	return (log_likelihood(parameters) + prior(parameters));
}

vector<double> DataModel::calculate(std::vector<double> parameters)
{
	vector<double> output(3);
	output[0] = prior(parameters);
	output[1] = log_likelihood(parameters);
	output[2] = output[0] + output[1];
	return output;
}

//Write output
void DataModel::print_data(void)
{
	//Header row
	cout << response_name;
	for(vector<string>::const_iterator iter = explanatory_names.begin(); iter != explanatory_names.end(); ++iter)
		cout << " " << setw(10) << *iter;
	cout << endl;
	for(int i=0; i<response.size(); ++i)
	{
		cout << setw(10) << response[i];
		for(int j=0; j<explanatories.size(); ++j)
			cout << " " << setw(10) << explanatories[j][i];
		cout << endl;
	}
}
