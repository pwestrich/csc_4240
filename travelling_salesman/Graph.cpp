/*
 * Graph.cpp
 * by Philip Westrich
 * 09/06/2014
 *
 * This is the implementation of the class described in Graph.h
 */

#include "Graph.h"

using namespace std;

Graph::Graph(const int &newSize){
   
   //size cannot be too large or small
   if (newSize <= MIN_GRAPH_SIZE){
      
      throw new GraphException("Error: attempt to create graph below minimum size");
      
   } else if (newSize > MAX_GRAPH_SIZE){
      
      throw new GraphException("Error: attempt to create graph too large");
      
   }
   
   //do this multiplication once
   int matrixSize = (newSize * newSize);
   
   edgeMatrix = new double[matrixSize];
   size = newSize;
   
   //initialize the matrix
   for (int i = 0; i < matrixSize; ++i){
      
      edgeMatrix[i] = NULL_EDGE;
      
   }
   
   //seed the random number generator
   srand(static_cast<unsigned>(time(NULL))); //SHUT UP ABOUT YOUR INTEGER PRECISION, DAMN YOU!
   
}
   
Graph::~Graph(){
      
   delete [] edgeMatrix;
      
}
   
void Graph::addVertex(const string &newVertexLabel){
      
   if (checkForDuplicateVertex(newVertexLabel)){
      
      throw new GraphException("Error: Duplicate vertex");
      
   }
   
   Vertex *newVertex = new Vertex;
   
   newVertex->label = newVertexLabel;
   newVertex->index = static_cast<int>(verticies.size());
   
   verticies.push_back(newVertex);
      
}
   
void Graph::addEdge(const string &startVertex, const string &endVertex, double weight){
   
   int startIndex = getVertexIndex(startVertex);
   int endIndex = getVertexIndex(endVertex);
   
   if (startVertex == endVertex){
      
      throw new GraphException("Error: Cennot create edge");
      
   }
   
   edgeMatrix[(startIndex * size) + endIndex] = weight;
   
}

void Graph::printGraph(ostream &out){
   
   for (size_t i = 0; i < verticies.size(); ++i){
      
      Vertex *item = verticies[i];
      
      out << "Vertex label: " << item->label << endl;
      out << "Edges:" << endl;
      
      for (size_t j = 0; j < size; ++j){
         
         double currentWeight = edgeMatrix[(i * size) + j];
         
         if (currentWeight != NULL_EDGE){
            
            out << verticies[i]->label << " " << verticies[j]->label << " " << currentWeight << endl;
            
         }
         
      }
      
      out << endl;
      
   }
   
}

vector<string> Graph::geneticTravellingSalesman(const int &populationSize, const int &numGenerations){
   
   //create generation
   vector<Chromosome> *currentGeneration = new vector<Chromosome>(populationSize);
   vector<Chromosome> *nextGeneration = NULL;
   
   for (size_t i = 0; i < populationSize; ++i){
      
      //copy the verticies, but not first
      vector<Vertex*> temp(verticies);
      temp.erase(temp.begin()); //erase the first city
      
      //shuffle them and keep it
      random_shuffle(temp.begin(), temp.end());
      (*currentGeneration)[i].genes = temp;
      (*currentGeneration)[i].fitness = calculateFitness((*currentGeneration)[i]);
      
   }
   
   //sort generation in decending order
   sort(currentGeneration->begin(), currentGeneration->end(), chromosomeSortMethod);
   
   for (size_t i = 1; i < numGenerations; ++i){
      
      #if defined MORE_COUT
      cout << "Generation " << i + 1 << " of " << numGenerations << "..." << endl;
      #endif
      
      //start with 10 best
      nextGeneration = new vector<Chromosome>(currentGeneration->begin(), currentGeneration->begin() + 10);
      
      //fill it up
      while (nextGeneration->size() < populationSize){
         
         vector<Chromosome> *temp = new vector<Chromosome>;
         
         for (size_t j = 0; j < 10; ++j){ //pick 10 randomly
            
            temp->push_back((*currentGeneration)[rand() % currentGeneration->size()]);
            
         }
         
         //pick the 4 best of that 10
         sort(temp->begin(), temp->end(), chromosomeSortMethod);
         vector<Chromosome> *temp2 = new vector<Chromosome>(temp->begin(), temp->begin() + 4);
         delete temp;
         
         int rand1 = rand() % 4;
         Chromosome crossover1 = (*temp2)[rand1];
         temp2->erase(temp2->begin() + rand1); //don't use the same one twice
         
         Chromosome crossover2 = (*temp2)[rand() % 3];
         
         rand1 = rand() % crossover1.genes.size(); //split point
         
         //have two children
         nextGeneration->push_back(crossover(crossover1, crossover2, rand1));
         nextGeneration->push_back(crossover(crossover2, crossover1, rand1));
         
         delete temp2;
         
      }
      
      //advance the generation
      vector<Chromosome> *deleteMe = currentGeneration;
      currentGeneration = nextGeneration;
      delete deleteMe;
      
      sort(currentGeneration->begin(), currentGeneration->end(), chromosomeSortMethod);
      
   }
   
   return getSolution((*currentGeneration)[0]);
   
}

//private member functions below here ------------------------------------------

bool Graph::checkForDuplicateVertex(const string &searchLabel){
   
   for (size_t i = 0; i < verticies.size(); ++i){
      
      if (verticies[i]->label == searchLabel) return true;
      
   }
   
   return false;
   
}

int Graph::getVertexIndex(const string &searchLabel){
   
   for (size_t i = 0; i < verticies.size(); ++i){
      
      if (verticies[i]->label == searchLabel) return static_cast<int>(i);
      
   }
   
   return -1;
   
}

int Graph::calculateFitness(Chromosome item){
   /*
   //get distance from start to first in the item
   int fitness = edgeMatrix[item.genes[0]->index];
   
   for (size_t i = 0; i < (item.genes.size() - 1); ++i){
      
      //get the distance between each node
      fitness += edgeMatrix[(item.genes[i]->index * size) + item.genes[i + 1]->index];
      
   }
   
   //distance from last in the item to the start
   fitness += edgeMatrix[item.genes.size()];
   
   return (fitness * -1); //miultiply by -1 because maximization
    */
   
   int fitness = 0;
   
   for (size_t i = 0; i < (item.genes.size() - 1); ++i){
      
      fitness += edgeMatrix[(item.genes[i]->index * size) + item.genes[i + 1]->index];
      
   }
   
   //add the start and end back
   fitness += edgeMatrix[item.genes[0]->index];
   fitness += edgeMatrix[(item.genes[item.genes.size() - 1]->index * size)];
   
   return (-1 * fitness);
   
}

Graph::Chromosome Graph::crossover(const Chromosome &first, const Chromosome &second, const int &splitPoint){
   
   Chromosome newItem;
   
   //build the new item
   for (size_t i = 0; i < splitPoint; ++i){
      
      newItem.genes.push_back(first.genes[i]);
      
   }
   
   for (size_t i = splitPoint; i < second.genes.size(); ++i){
      
      newItem.genes.push_back(second.genes[i]);
      
   }
   
   vector<Vertex*> used;
   
   //check for duplicate cities
   for (size_t i = 0; i < newItem.genes.size(); ++i){
      
      //mark it as used
      used.push_back(newItem.genes[i]);
      
      for (size_t j = (i + 1); j < newItem.genes.size(); ++j){
         
         //make sure a duplicate doesn't exist
         if (newItem.genes[i]->index == newItem.genes[j]->index){
            
            newItem.genes.erase(newItem.genes.begin() + j);
            
         }
         
      }
      
   }
   
   //add the not-used cities back to the end
   while (newItem.genes.size() < (verticies.size() - 1)){
      
      //find an item in the verticies that is not in the used set
      for (size_t i = 1; i < verticies.size(); ++i){
         
         if (find(used.begin(), used.end(), verticies[i]) == used.end()){
            
            newItem.genes.push_back(verticies[i]);
            
         }
         
      }
      
   }
   
   newItem.fitness = calculateFitness(newItem);
   
   return newItem;
   
}

vector<string> Graph::getSolution(const Chromosome &solution){
   
   vector<string> answer;
   ostringstream oss;
   
   oss << "1:\t\t" << verticies[0]->label;
   answer.push_back(oss.str());
   oss.str("");
   oss.clear();
   
   for (size_t i = 0; i < solution.genes.size(); ++i){
      
      
      oss << (solution.genes[i]->index + 1) << ":\t\t" << solution.genes[i]->label;
      answer.push_back(oss.str());
      oss.str("");
      oss.clear();
      
      
   }
   
   oss << "1:\t\t" << verticies[0]->label;
   answer.push_back(oss.str());
   oss.str("");
   oss.clear();
   
   oss << "Cost:\t";
   oss << (-1 * solution.fitness);
   
   answer.push_back(oss.str());
   
   return answer;
   
}
