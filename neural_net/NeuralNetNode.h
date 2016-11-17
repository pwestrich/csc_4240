
#ifndef NEURAL_NET_NODE_H
#define NEURAL_NET_NODE_H

#define DEFAULT_MIN_WEIGHT -0.1
#define DEFAULT_MAX_WEIGHT  0.1

#include <iostream>

#include "utilities.h"

using namespace std;

//this is a node for the neural net
struct NeuralNetNode {

	int index;
	double activation;
	double bias;
	double inputSum;
	double gradient;

};

//returns an initialized node pointer
NeuralNetNode *createNode(const int index);

//prints a node to an output stream, for debugging and such
ostream &operator<<(ostream &out, const NeuralNetNode &node);

#endif
