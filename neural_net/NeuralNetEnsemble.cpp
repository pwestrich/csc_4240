
#include "NeuralNetEnsemble.h"
#include "getRealTime.h"

using namespace std;

NeuralNetEnsemble::NeuralNetEnsemble(double _errorTolerance, double _learningRate, double _multiplier,
			  							int _numInputs, int _numOutputs, int _numHiddenLayers, 
			  							int *_nodesPerLayer){

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

}

NeuralNetEnsemble::~NeuralNetEnsemble(){}

void NeuralNetEnsemble::train(const vector<double*> &data){

	vector<double> hypothesisWeights;
	vector<double> pointWeights(data.size(), 1.0 / data.size());

	cout << "Training first net...";

	//make the first net and train it
	NeuralNet *firstNet = new NeuralNet(errorTolerance, learningRate, multiplier, 
										numInputs, numOutputs, numHiddenLayers, nodesPerLayer);

	vector<double*> trainingPoints = pickPointsToTrain(data, pointWeights);

	trainNeuralNet(trainingPoints, firstNet);
	nets.push_back(firstNet);

	cout << "done!" << endl;

	int runCount = 1;


	do {

		cout << "Run #" << runCount++ << endl;
		
		hypothesisWeights.resize(nets.size());
		vector<int*> hypotheses(nets.size());

		//for each neural net
		for (int k = 0; k < nets.size(); ++k){

			int *discreteResults = new int[data.size()];

			//run every point through the net
			for (int i = 0; i < data.size(); ++i){

				double *temp = nets[k]->forwardPropogate(data[i]);
				discreteResults[i] = classify(temp[0]); //assume one output and three discrete results

			}

			//keep these discrete results
			hypotheses[k] = discreteResults;

			//calculate the errors for each point
			double error = 0.0;

			for (int i = 0; i < data.size(); ++i){

				if (hypotheses[k][i] != data[i][numInputs]){

					error += pointWeights[i];

				}

			}

			//update the weights of the correct points
			for (int i = 0; i < data.size(); ++i){

				if (hypotheses[k][i] == data[i][numInputs]){

					pointWeights[i] = pointWeights[i] * (error / (1.0 - error));

				}

			}

			//normalize the point weights
			normalize(pointWeights);

			//set the error for this net
			hypothesisWeights[k] = log((1.0 - error) / error);

		}

		//normalize the hypothesis weights
		normalize(hypothesisWeights);

		//get the weighted majority result for each net
		vector<double> results = weightedMajority(hypotheses, hypothesisWeights, data.size());

		//check if every one of them is correct
		bool quit = true;
		int count = 0;

		for (int i = 0; i < data.size(); ++i){

			if (data[i][numInputs] != double(results[i])){

				cout << "point " << i + 1 << " is " << results[i] << ", should be " << data[i][numInputs] << endl;
				quit = false;
				count++;

			}

		}

		cout << "error: " << (static_cast<double>(count) / data.size()) << endl; 
		cerr << "hypothesis weights: " << endl;

		for (int i = 0; i < hypothesisWeights.size(); ++i){

			cerr << "                 " << i + 1 << ": " << hypothesisWeights[i] << endl;

		}

		if (quit) break;

		//pick the points to train the new neural net with
		trainingPoints = pickPointsToTrain(data, pointWeights);

		//make the new net and train it
		NeuralNet *newNet = new NeuralNet(errorTolerance, learningRate, multiplier, 
										numInputs, numOutputs, numHiddenLayers, nodesPerLayer);

		trainNeuralNet(trainingPoints, newNet);
		nets.push_back(newNet);


	} while (true);

}

void NeuralNetEnsemble::trainNeuralNet(const vector<double*> &data, NeuralNet *net){

	double startTime = getRealTime();

	do { //train the net for the specified time

		for (int i = 0; i < data.size(); ++i){

			double *temp = net->test(data[i]);
			delete [] temp;

		}

	} while ((getRealTime() - startTime) < errorTolerance);

}

void NeuralNetEnsemble::normalize(vector<double> &input) const {

	double sum = 0.0;

	for (int i = 0; i < input.size(); ++i){

		sum += input[i];

	}

	for (int i = 0; i < input.size(); ++i){

		input[i] = (input[i] / sum);

	}

}

int NeuralNetEnsemble::classify(const double value) {

	double increment = 0.5;

	for (double i = 1; i <= multiplier; ++i){

		if (value < (i + increment)){

			return i;

		}

	}

	return multiplier;

}

vector<double*> NeuralNetEnsemble::pickPointsToTrain(const vector<double*> &points, const vector<double> &pointWeights) const{

	vector<double*> pickedPoints;
	double probabilities[points.size()];
	double sum = 0.0;

	for (int i = 0; i < points.size(); ++i){

		probabilities[i] = sum + pointWeights[i];
		sum += pointWeights[i];

	}

	while (pickedPoints.size() != points.size()){

		double prob = getRandomDouble(0.0, 1.0);

		for (int i = 0; i < points.size(); ++i){

			if (probabilities[i] > prob){

				pickedPoints.push_back(points[i]);
				break;

			}

		}

	}

	return pickedPoints;

}

vector<double> NeuralNetEnsemble::weightedMajority(const vector<int*> &hypotheses, const vector<double> &weights,
													const int numDataPoints) {

	vector<double> answers(numDataPoints);

	//for each data point
	for (int i = 0; i < numDataPoints; ++i){

		//each point's value is the weighted sum of each hypothesis
		double sum = 0.0;

		//for each hypothesis
		for (int j = 0; j < hypotheses.size(); ++j){

			sum += weights[j] * hypotheses[j][i];

		}

		//the answer is the weighted sum of all the hypotheses
		answers[i] = classify(sum);

	}

	return answers;

}
