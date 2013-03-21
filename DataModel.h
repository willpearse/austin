/*
 * DataModel.h
 *
 *  Created on: 15 Mar 2013
 *      Author: Will Pearse
 */

#ifndef DATAMODEL_H_
#define DATAMODEL_H_

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

class DataModel{
private:
	std::vector<std::vector<double> > explanatories;
	std::vector<double> response;
	std::string response_name;
	std::vector<std::string> explanatory_names;
public:
	DataModel(const char *file, std::vector<std::string> p_n);

	double log_likelihood(std::vector<double> parameters);
	double likelihood(std::vector<double> parameters);
	double prior(std::vector<double> parameters);
	double posterior(std::vector<double> parameters);
	std::vector<double> calculate(std::vector<double> parameters);

	std::vector<std::string> param_names;
	void print_data(void);
};


#endif /* DATAMODEL_H_ */
