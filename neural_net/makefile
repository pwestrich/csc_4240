
.PHONY: NeuralNetDriver NeuralNetEnsembleDriver

NET_DATAFILE=radiation_test.txt
ENSEMBLE_DATAFILE=seed_test.txt

CC=g++
CCFLAGS= -g -O0 -std=c++11

NeuralNetEnsembleDriver: utilities.o NeuralNet.o NeuralNetEnsemble.o NeuralNetEnsembleDriver.o NeuralNetNode.o getRealTime.o

	$(CC) $(CCFLAGS) -o NeuralNetEnsembleDriver utilities.o NeuralNet.o NeuralNetEnsemble.o NeuralNetEnsembleDriver.o NeuralNetNode.o getRealTime.o
	./NeuralNetEnsembleDriver $(ENSEMBLE_DATAFILE)

NeuralNetDriver: utilities.o NeuralNet.o NeuralNetDriver.o NeuralNetNode.o getRealTime.o

	$(CC) $(CCFLAGS) -o NeuralNetDriver utilities.o NeuralNet.o NeuralNetDriver.o NeuralNetNode.o getRealTime.o
	./NeuralNetDriver $(NET_DATAFILE)

NeuralNetDriver.o: NeuralNetDriver.cpp NeuralNet.h

	$(CC) $(CCFLAGS) -c NeuralNetDriver.cpp

NeuralNetEnsemble.o: NeuralNetEnsemble.cpp NeuralNetEnsemble.h NeuralNet.h

	$(CC) $(CCFLAGS) -c NeuralNetEnsemble.cpp

NeuralNet.o: NeuralNet.cpp NeuralNet.h utilities.h NeuralNetNode.h

	$(CC) $(CCFLAGS) -c NeuralNet.cpp

utilities.o: utilities.cpp utilities.h

	$(CC) $(CCFLAGS) -c utilities.cpp

NeuralNetNode.o: NeuralNetNode.cpp NeuralNetNode.h

	$(CC) $(CCFLAGS) -c NeuralNetNode.cpp

getRealTime.o: getRealTime.cpp getRealTime.h

	$(CC) $(CCFLAGS) -c getRealTime.cpp
