
#include "NeuralNetNode.h"

using namespace std;

//returns a newly initialized pointer to a node
NeuralNetNode *createNode(const int index){

	NeuralNetNode *temp = new NeuralNetNode;

	temp->index = index;
	temp->activation = 0.0;
	temp->inputSum = 0.0;
	temp->gradient = 0.0;
	temp->bias = getRandomDouble(DEFAULT_MIN_WEIGHT, DEFAULT_MAX_WEIGHT);

	return temp;

}

//prints a node to an output stream
ostream &operator<<(ostream &out, const NeuralNetNode &node){

	out << "Index:      " << node.index 	 << endl;
	out << "Activation: " << node.activation << endl;
	out << "InputSum:   " << node.inputSum 	 << endl;
	out << "Bias:       " << node.bias 		 << endl;
	out << "Gradient:   " << node.gradient 	 << endl;

	return out;

}
