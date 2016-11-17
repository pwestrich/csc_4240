
#ifndef NEURAL_NET_ENSEMBLE_H
#define NEURAL_NET_ENSEMBLE_H

#include <cmath>
#include <iostream>
#include <vector>

#include "NeuralNet.h"

using namespace std;

class NeuralNetEnsemble {
	
private:

	//the list of nets
	vector<NeuralNet*> nets;

	//remember all of the constructor arguments for the nets
	double errorTolerance;
	double learningRate;
	double multiplier;
	int numInputs;
	int numOutputs;
	int numHiddenLayers;
	int *nodesPerLayer;

	//trains a neural net with the specified data
	void trainNeuralNet(const vector<double*> &data, NeuralNet *net);

	//normalizes a vector of weights
	void normalize(vector<double> &input) const;

	//classifies a data point based on the number of discrete values it should be
	int classify(const double value);

	vector<double*> pickPointsToTrain(const vector<double*> &points, const vector<double> &pointWeights) const;

	//figures out the weighted majority answer for each data point
	vector<double> weightedMajority(const vector<int*> &hypotheses, const vector<double> &weights, const int numDataPoints);

public:

	//creates the ensemble, first with one net
	NeuralNetEnsemble(double _errorTolerance, double _learningRate, double _multiplier, int _numInputs, 
					  int _numOutputs, int _numHiddenLayers, int *_nodesPerLayer);
	//deletes the ensemble
	~NeuralNetEnsemble();

	//retry
	void train(const vector<double*> &data);

};

#endif
