/*
*	NeuralNet.cpp
*	for CSC 4250, Assignemnt #3
*	by Philip Westrich
*	Friday, December 5, 2014
*
*	This is the implementation of the NeuralNet class, that uses the
*	forward-feeding, backwards-propogation learning stuff.
*/

#include "NeuralNet.h"

using namespace std;

NeuralNet::NeuralNet(double _errorTolerance, double _learningRate, double _multiplier,
					 int _numInputs, int _numOutputs, int _numHiddenLayers, int *_nodesPerLayer){


	//first off, check some inputs
	if (_errorTolerance < 0.0){

		throw new invalid_argument("errorTolerance < 0.0");

	} else if (_learningRate < 0.0){

		throw new invalid_argument("learningRate < 0.0");

	} else if (multiplier < 0.0){

		throw new invalid_argument("multiplier less than 0.0");

	} else if (_numInputs < 1){

		throw new invalid_argument("numInputs below zero");

	} else if (_numOutputs < 1){

		throw new invalid_argument("numOutputs below zero");

	} else if (_numHiddenLayers < 1){

		throw new invalid_argument("numHiddenLayers below zero");

	} else if (_nodesPerLayer == NULL){

		throw new invalid_argument("nodesPerLayer is null");

	}

	//set instance veriables
	errorTolerance 	= _errorTolerance;
	learningRate 	= _learningRate;
	multiplier 		= _multiplier;
	numInputs 		= _numInputs;
	numOutputs 		= _numOutputs;
	nodesPerLayer 	= _nodesPerLayer;
	numHiddenLayers = _numHiddenLayers;
	totalNumNodes 	= 0;

	//now start filling in the layers of nodes
	layers = new NeuralNetNode**[numHiddenLayers + 2];

	for (int i = 0; i < numHiddenLayers + 2; ++i){

		layers[i] = new NeuralNetNode*[nodesPerLayer[i]];

		for (int j = 0; j < nodesPerLayer[i]; ++j){

			layers[i][j] = createNode(totalNumNodes++);
			nodes.push_back(layers[i][j]);

		}

	}

	//now initialize the weights for each node
	weights = new double[totalNumNodes * totalNumNodes];

	for (int i = 0; i < totalNumNodes; ++i){

		for (int j = 0; j < totalNumNodes; ++j){

			weights[index(i, j, totalNumNodes)] = 
				weights[index(j, i, totalNumNodes)] = 
					getRandomDouble(DEFAULT_MIN_WEIGHT, DEFAULT_MAX_WEIGHT);

		}

	}

}

NeuralNet::~NeuralNet(){

	for (int i = 0; i < numHiddenLayers + 2; ++i){

		for (int j = 0; j < nodesPerLayer[i]; ++j){

			delete layers[i][j];

		}

		delete [] layers[i];

	}

	delete [] weights;
	delete layers;

}

double *NeuralNet::test(double *data){

	double *answers = forwardPropogate(data);

	//propogate deltas backwards
	//start with exit nodes
	for (int i = 0; i < nodesPerLayer[numHiddenLayers + 1]; ++i){

		NeuralNetNode *current = layers[numHiddenLayers + 1][i];
		current->gradient = gPrime(current->inputSum) * 
							((data[numInputs + i] / multiplier) - current->activation);

	}

	//for each layer, starting at the end
	for (int i = numHiddenLayers; i >= 0; --i){

		//for each node in that layer, crunch numbers
		for (int j = 0; j < nodesPerLayer[i]; ++j){

			NeuralNetNode *current = layers[i][j];
			double sum = 0.0;

			for (int k = 0; k < nodesPerLayer[i + 1]; ++k){

				NeuralNetNode *next = layers[i + 1][k];
				sum += weights[index(current->index, next->index, totalNumNodes)] * 
							next->gradient;

			}

			current->gradient = gPrime(current->inputSum) * sum;

		}

	}

	//now update the weights
	for (int i = 0; i < totalNumNodes; ++i){

		for (int j = 0; j < totalNumNodes; ++j){
				
			//keep the array mirrored
			weights[index(i, j, totalNumNodes)] = weights[index(j, i, totalNumNodes)] =
												weights[index(i, j, totalNumNodes)] + 
												(learningRate * nodes[j]->gradient *
												nodes[i]->activation);

	}

		//and update the bias weight for this node
		nodes[i]->bias += learningRate * nodes[i]->gradient;

	}

	return answers;

}

double *NeuralNet::forwardPropogate(double *data){

		//the activation of the first layer is just the input
	for (int i = 0; i < numInputs; ++i){

		layers[0][i]->inputSum =
			layers[0][i]->activation =
				 data[i];

	}

	//for every layer
	for (int i = 1; i <= numHiddenLayers + 1; ++i){

		//for every node in that layer, fill in the activation
		for (int j = 0; j < nodesPerLayer[i]; ++j){

			//sum up the activations of every node, multiplied by its weight
			NeuralNetNode *current = layers[i][j];
			current->inputSum = current->bias; //don't forget the bias of this node!

			for (int k = 0; k < nodesPerLayer[i - 1]; ++k){

				NeuralNetNode *previous = layers[i - 1][k];
				current->inputSum += previous->activation * 
									 weights[index(previous->index, current->index, totalNumNodes)];

			}

			//the activation of this node is g(sum of inputs + bias)
			current->activation = g(current->inputSum);

		}

	}

	//now make the array of return values
	double *answers = new double[numOutputs];

	for (int i = 0; i < nodesPerLayer[numHiddenLayers + 1]; ++i){

		answers[i] = (layers[numHiddenLayers + 1][i]->activation * multiplier);

	}

	return answers;

}

double NeuralNet::g(const double &input) const {

	return (1 / (1 + exp(-1 * input)));

}

double NeuralNet::gPrime(const double &input) const {

	double _g = g(input);
	return (_g * (1 - _g));

}

ostream &operator<<(ostream &out, const NeuralNet &net){

	out << "Neural Net:      " 	<< endl;
	out << "Error tolerance: "	<< net.errorTolerance 	<< endl;
	out << "Learning rate:   "	<< net.learningRate 	<< endl;
	out << "Multiplier:      " 	<< net.multiplier 		<< endl;
	out << "Inputs:          " 	<< net.numInputs 		<< endl;
	out << "Outputs:         " 	<< net.numOutputs 		<< endl;
	out << "Total nodes:     "  << net.totalNumNodes 	<< endl;
	out << "Hidden layers:   " 	<< net.numHiddenLayers  << endl << endl;

	for (int i = 0; i < net.numHiddenLayers + 2; ++i){

		out << "Layer #" << i + 1 << ": " << net.nodesPerLayer[i] << " nodes" << endl;

		for (int j = 0; j < net.nodesPerLayer[i]; ++j){

			out << "Node #" << j + 1 << ": " << endl << *(net.layers[i][j]);

		}

		out << endl;

	}

	return out;

}
