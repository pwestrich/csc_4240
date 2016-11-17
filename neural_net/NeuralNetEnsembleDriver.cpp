
#include <cstdlib>
#include <exception>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "getRealTime.h"
#include "NeuralNetEnsemble.h"
#include "utilities.h"

using namespace std;

int main(int argc, char *argv[]){
	
	//a lot of this is identical to NeuralNetDriver.cpp
	//check argument count, need at least one
	if (argc < 2){

		cerr << "Error: Too few arguments." << endl;
		exit(EXIT_FAILURE);

	}

	cout << "Opening input file..." << endl;
	fstream inFile(argv[1]); //attempt to to open file

	if (!inFile){

		cerr << "Error opening file: " << argv[1] << endl;
		exit(EXIT_FAILURE);

	}

	cout << "Opening data file..." << endl;

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

	cout << "Reading data..." << endl;

	vector<double*> data;

	for (int i = 0; i < numDataPoints; ++i){

		getline(dataFile, buffer);
		vector<string> temp = explode(",", buffer);

		double *temp2 = new double[numInputs + numOutputs];

		for (int j = 0; j < numInputs + numOutputs; ++j){

			temp2[j] = atof(temp[j].c_str());

		}

		data.push_back(temp2);

	}

	dataFile.close(); //done with you too

	try {

		cout << "Creating ensemble..." << endl;
		
		NeuralNetEnsemble ensemble(errorTolerance, learningRate, multiplier, numInputs, 
									numOutputs, numHiddenLayers, nodesPerLayer);

		ensemble.train(data);

	} catch (invalid_argument *it){

		cerr << "Exception: " << it->what() << endl;
		exit(EXIT_FAILURE);

	}

	return 0;

}
