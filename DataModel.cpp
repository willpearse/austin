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
DataModel::DataModel(const char *file)
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
			for(int i=0; i<n_explanatory; ++i)
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
			}
		}
	}
}

//Likelihood (toy in this case)
DataModel::log_likelihood(vector<double> parameters)
{
	//Setup
	double log_lik = 0.0;
	double mean = parameters[0];
	double sd = parameters[1];

	//Calculate accumulate log likelihood
	for(int i=0; i<response.size(); ++i)
		log_lik += log(gsl_ran_gaussian_pdf(response[i] - mean, sd));

	return log_lik;
}

