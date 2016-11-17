/*
*	NeuralNet.h
*	for CSC 4250, Assignemnt #3
*	by Philip Westrich
*	Friday, December 5, 2014
*
*	This is the specification for the NeuralNet class, that uses the
*	forward-feeding, backwards-propogation learning stuff.
*/

#ifndef NEURAL_NET_H
#define NEURAL_NET_H

#include <stdexcept>
#include <exception>
#include <cmath>
#include <iostream>
#include <vector>

#include "NeuralNetNode.h"
#include "utilities.h"

using namespace std;

class NeuralNet {
	
private:

	//the nodes
	NeuralNetNode ***layers;
	vector<NeuralNetNode*> nodes;
	int *nodesPerLayer;
	int numHiddenLayers;

	//how many inputs and outputs?
	int numInputs;
	int numOutputs;
	int totalNumNodes;

	double errorTolerance;
	double learningRate;
	double multiplier;

	//the weights between each two nodes
	double *weights;

	//the functions for updating things
	double g(const double &input) const;
	double gPrime(const double &input) const;

public:

	//constructs a new neural net
	NeuralNet(double _errorTolerance, double _learningRate, double _multiplier,
			  int _numInputs, int _numOutputs, int _numHiddenLayers, int *_nodesPerLayer);
	
	//deletes the internal nodes, NOT the arguments to the constructor
	~NeuralNet();

	//runs a single data point through the neural net
	//returns an array of results
	double *test(double *data);

	//does not do the backwards propogation
	double *forwardPropogate(double *data);

	//prints the neural net, for debugging
	friend ostream &operator<<(ostream &out, const NeuralNet &net);

};

#endif
