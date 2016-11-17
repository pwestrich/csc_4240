//
//  Graph.h
//  Genetic Travelling Salesman
//
//  Created by Philip Westrich on 9/6/14.
//  Copyright (c) 2014 Philip Westrich. All rights reserved.
//

#ifndef GRAPH

#define GRAPH
#define MIN_GRAPH_SIZE  3
#define MAX_GRAPH_SIZE  0xFFFF
#define NULL_EDGE       -1.0
//#define MORE_COUT //remove this for less cout

#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

#include "GraphException.h"

using namespace std;

class Graph {
   
private:
   
   //verticies in the graph
   struct Vertex {
      
      string label;
      int index;
      
   };
   
   struct Chromosome {
      
      vector<Vertex*> genes;
      int fitness;
      
   };
   
   static bool chromosomeSortMethod(const Chromosome &left, const Chromosome &right){
      
      return (left.fitness > right.fitness);
      
   }
   
   //the adjacency matrix
   double *edgeMatrix;
   int size;
   
   vector<Vertex*> verticies;
   
   bool checkForDuplicateVertex(const string &searchLabel);
   
   int getVertexIndex(const string &searchLabel);
   
   int calculateFitness(Chromosome item);
   
   Chromosome crossover(const Chromosome &first, const Chromosome &second, const int &splitPoint);
   
   vector<string> getSolution(const Chromosome &solution);
   
public:
   
   //Constricts the graph. Must give it a size.
   Graph(const int &newSize);
   ~Graph();
   
   //Add a vertex given its name, which must be unique
   void addVertex(const string &newVertexLabel);
   
   //add an edge between two edges
   void addEdge(const string &startVertex, const string &endVertex, double weight);
   
   void printGraph(ostream &out);
   
   vector<string> geneticTravellingSalesman(const int &populationSize, const int &numGenerations);
   
};

#endif
