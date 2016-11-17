//
//  main.cpp
//  Genetic Travelling Salesman
//
//  Created by Philip Westrich on 9/6/14.
//  Copyright (c) 2014 Philip Westrich. All rights reserved.
//

#include <cstdlib>
#include <iostream>
#include <fstream>

#include "Graph.h"
#include "getRealTime.c" //because I can

using namespace std;

int stoi(const char *item){
 
 return static_cast<int>(atof(item));   
    
}

Graph *readVerticies(const char *filename);
void readEdges(Graph *graph, const char *filename);

int main(int argc, const char *argv[]){
   
   if (argc < 4){
      
      cerr << "Error: Too few arguments" << endl;
      exit(EXIT_FAILURE);
      
   }
   
   cout << "Reading verticies..." << endl;
   Graph *graph = readVerticies(argv[1]);
   
   cout << "Reading edges..." << endl;
   readEdges(graph, argv[2]);
   
   cout << "Finding a solition..." << endl;
   
   double startTime = getRealTime();
   
   vector<string> solution = graph->geneticTravellingSalesman(stoi(argv[3]), stoi(argv[4]));
   
   double endTime = getRealTime();
   
   cout << "Done! Took " << (endTime - startTime) << " seconds." <<  endl;
   
   for (size_t i = 0; i < solution.size(); ++i){
      
      cout << solution[i] << endl;
      
   }
   
   delete graph;

   return EXIT_SUCCESS;

}

Graph *readVerticies(const char *filename){
   
   fstream inFile(filename);
   string buffer;
   vector<string> cities;
   
   getline(inFile, buffer);
   
   while (!inFile.eof()){
      
      //Check for a \r at the end of the string
      if (buffer.back() == '\r'){
         
         //Get rid of it
         buffer.erase(buffer.length() - 1);
         
      }
      
      cities.push_back(buffer);
      getline(inFile, buffer);
      
   }
   
   inFile.close();
   
   //make graph now that we know how many cities we have
   Graph *graph = new Graph(static_cast<int>(cities.size()));
   
   for (int i = 0; i < cities.size(); ++i){
      
      try {
      
         graph->addVertex(cities[i]);
         
      } catch (GraphException *it){
         
         cerr << it->getMessage() << endl;
         cerr << "Vertex ignored." << endl;
         
         delete it;
         
      }
      
   }
   
   return graph;
   
}

void readEdges(Graph *graph, const char *filename){
   
   fstream inFile(filename);
   string buffer;
   
   getline(inFile, buffer, ' '); //seperated by spaces
   
   while (!inFile.eof()){
      
      string startVertex = buffer;
      
      getline(inFile, buffer, ' ');
      string endVertex = buffer;
      
      getline(inFile, buffer); //at end of line
      
      //Check for a \r at the end of the string
      if (buffer.back() == '\r'){
         
         //Get rid of it
         buffer.erase(buffer.length() - 1);
         
      }
      
      double weight = atof(buffer.c_str());
      
      try {
         
         graph->addEdge(startVertex, endVertex, weight);
         
      } catch (GraphException *it){
         
         cerr << it->getMessage() << endl;
         cerr << "Edge ignored." << endl;
         
         delete it;
         
      }
      
      getline(inFile, buffer, ' '); //prime eof
      
   }
   
}
