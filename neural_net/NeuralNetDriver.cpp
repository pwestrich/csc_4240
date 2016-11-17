/*
*	NeuralNetDriver.cpp
*	for CSC 4250, Assignemnt #3
*	by Philip Westrich
*	Friday, December 5, 2014
*
*	This is the driver for the single NeuralNet. It reads in data, tests each
*	point for the specified number of times in one of the input files, then 
* 	reports the results back to the user.
* 	
*	The input file is specified at the command line. It is formatted as follows:
*		1	Name_of_data_file
*		2 	Num_hidden_layers
* 		3 	Num_nodes_in_layer_1
* 		...
*		n+2 Num_nodes_in_layer_n
*		n+3 learning_rate
* 		n+4 error_tolerance
*
*	The error tolerance is the number of seconds the test loop is to run. 
* 	I have included a few input files.
*	The data file is formatted as follows:
*		1 	multiplier
*		2 	num_data_points  		//I ADDED THIS, makes input reading a whole lot easier
*		3 	num_inputs,num_outputs
*		4 	in_1,in_2,...,in_i,out_1,out_2,...,out_j //data point #1
*		...
*		k+3 data_point_k
*
*		I added the line telling you how many data points ar in the file. It made life a whole lot easier.
*/

#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "getRealTime.h"
#include "NeuralNet.h"
#include "utilities.h"

using namespace std;

int main(int argc, char *argv[]){

	//check argument count, need at least one
	if (argc < 2){

		cerr << "Error: Too few arguments." << endl;
		exit(EXIT_FAILURE);

	}

	fstream inFile(argv[1]); //attempt to to open file

	if (!inFile){

		cerr << "Error opening file: " << argv[1] << endl;
		exit(EXIT_FAILURE);

	}

	//read the name of the datafile from the input file, make sure it worked
	string buffer;
	getline(inFile, buffer, '\n');
	fstream dataFile(buffer.c_str());

	if (!dataFile){

		cerr << "Error opening data file: " << buffer << endl;
		exit(EXIT_FAILURE);

	}

	//next is the number of hidden layers
	int numHiddenLayers;
	inFile >> numHiddenLayers;
	int nodesPerLayer[numHiddenLayers + 2];

	//then the next several lines are the number of nodes per layer
	for (int i = 1; i <= numHiddenLayers; ++i){

		inFile >> nodesPerLayer[i];

	}

	//the next two lines: learning rate and error tolerance
	double learningRate;
	double errorTolerance;
	inFile >> learningRate >> errorTolerance;

	inFile.close(); //done with you

	//now read in data from the datafile, first up: multiplier
	double multiplier;
	int numDataPoints;
	dataFile >> multiplier >> numDataPoints;

	//next is: numInputs,numOutputs
	getline(dataFile, buffer, ',');
	int numInputs = atoi(buffer.c_str());
	nodesPerLayer[0] = numInputs;

	getline(dataFile, buffer);
	int numOutputs = atoi(buffer.c_str());
	nodesPerLayer[numHiddenLayers + 1] = numOutputs;

	double data[numDataPoints][numInputs + numOutputs];

	for (int i = 0; i < numDataPoints; ++i){

		getline(dataFile, buffer);
		vector<string> temp = explode(",", buffer);

		for (int j = 0; j < numInputs + numOutputs; ++j){

			data[i][j] = atof(temp[j].c_str());

		}

	}

	dataFile.close(); //done with you too

	try {

		cout << "Testing..." << endl;

		NeuralNet net(errorTolerance, learningRate, multiplier, numInputs, numInputs, numHiddenLayers, nodesPerLayer);
		
		int runNumber = 0;
		double *results[numDataPoints];

		for (int i = 0; i < numDataPoints; ++i){

			results[i] = new double[1];

		}

		double startTime = getRealTime();

		do {

			for (int i = 0; i < numDataPoints; ++i){

				delete [] results[i];
				results[i] = net.test(data[i]);

			}

			++runNumber;

		} while ((getRealTime() - startTime) < errorTolerance);

		cout << "Results after " << runNumber << " runs:" << endl;

		for (int i = 0; i < numDataPoints; ++i){

			cout << "Point #" << i + 1 << ": " << endl;
			cout << "Inputs:        ";

			for (int j = 0; j < numInputs; ++j){

				cout << " " << data[i][j];

			}

			cout << endl << "Test outputs:  ";

			for (int j = 0; j < numOutputs; ++j){

				cout << " " << results[i][j];

			}

			cout << endl << "Actual outputs:";

			for (int j = 0; j < numOutputs; ++j){

				cout << " " << data[i][numInputs + j];

			}

			cout << endl << endl;

		}

		for (int i = 0; i < numOutputs; ++i){

			delete [] results[i];

		}

	} catch (invalid_argument *it){

		cerr << "Exception: " << it->what() << endl;
		exit(EXIT_FAILURE);

	}

	return 0;

}
