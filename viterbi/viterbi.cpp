/*
 viterbi.cpp
 by Philip Westrich
 for CSC 4240 Assignemnt #2
 Monday, November 10, 2014
 
   This program computes a series of hidden states given a lot of probabilities 
 and a series of observed states.
 
   This program works with both the rain and umbrella example and the DNA example
 on your website, at least using the input files the way I formatted them. I did 
 change the prior probability files I used to match yours. The rain one is the same
 as it is on your website. The DNA ones should be the same as well.
 
   I had an interesting problem while testing the DNA one. Turns out that the number
 0.000030375 is one of those numbers that can't be reprisented in binary very well.
 So, at one point in the program, I had 0.000030375 >= 0.00003037499999 as one of 
 my comparisons for determining which path to keep. I wanted the second one, but it
 kept giving me the first.
 
   My solution to this was one of the suggestins from the DNA example slides: use a
 multipler to make all the numbers much, much larger, do the calculations, then 
 undo it all at the end when printing them back out. Turned out that was enough to 
 get rid of the imprecision in floating-point numbers.
 
   It works, so I'm happy. Hopefully it'll get me a decent grade. Somehow, the 
 simplest assignment was super hard...
*/

#include <cstdlib>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iomanip>

#define PRINT_PRECISION 16    //how many zeroes you want?
#define MULTIPLIER      10.0  //how big of a multiplier?

//rather than use namespace std, use specific ones
using std::cout;
using std::cerr;
using std::fstream;
using std::endl;
using std::fixed;
using std::setprecision;
using std::pow;

//function for computing the array indicies consistently
inline int index(const int x, const int y, const int numCols){
   
   return ((x * numCols) + y);
   
}

int main(int argc, const char *argv[]){
   
   //check argument count, should be 4
   if (argc < 4){
      
      cerr << "Error: Too few arguments." << endl;
      exit(EXIT_FAILURE);
      
   }
   
   //process transition matrix
   fstream transitionFile(argv[1]);
   int transitionNumRows, transitionNumCols;
   transitionFile >> transitionNumRows >> transitionNumCols;
   double *transitionArray = new double[transitionNumCols * transitionNumRows];
   
   for (int i = 0; i < transitionNumRows; ++i){
      
      for (int j = 0; j < transitionNumCols; ++j){
         
         double temp;
         transitionFile >> temp;
         transitionArray[index(i, j, transitionNumCols)] = temp * MULTIPLIER;
         
      }
      
   }
   
   transitionFile.close();
   
   //now the sensor matrix, same format
   fstream sensorFile(argv[2]);
   int sensorNumRows, sensorNumCols;
   sensorFile >> sensorNumRows >> sensorNumCols;
   double *sensorArray = new double[sensorNumCols * sensorNumRows];
   
   for (int i = 0; i < sensorNumRows; ++i){
      
      for (int j = 0; j < sensorNumCols; ++j){
         
         double temp;
         sensorFile >> temp;
         sensorArray[index(i, j, sensorNumCols)] = temp * MULTIPLIER;
         
      }
      
   }
   
   sensorFile.close();
   
   // next up prior probabilities
   fstream priorFile(argv[3]);
   int priorNumRows, priorNumCols;
   priorFile >> priorNumRows >> priorNumCols;
   double *priorProb = new double[priorNumCols * priorNumRows];
   
   for (int i = 0; i < priorNumRows; ++i){
      
      for (int j = 0; j < priorNumCols; ++j){
         
         double temp;
         priorFile >> temp;
         priorProb[index(i, j, priorNumCols)] = temp * MULTIPLIER;
         
      }
      
   }
   
   priorFile.close();
   
   // then the sequence
   fstream sequenceFile(argv[4]);
   int numSequences;
   sequenceFile >> numSequences;
   int observeSequence[numSequences];
   
   for (int i = 0; i < numSequences; ++i){
      
      sequenceFile >> observeSequence[i];
      observeSequence[i]--; //decrement because arrays are 0-based
      
   }
   
   int path[numSequences];
   double probs[numSequences];
   
   //initialize the path and probabilities
   for (int i = 0; i < numSequences; ++i){
      
      path[i] = -1;
      probs[i] = 0.0;
      
   }
   
   //fill in path for time = 0
   double highest = 0.0;
   
   for (int j = 0; j < transitionNumCols; ++j){
      
      double argMax = 0.0; //keep the argument that gives us the largest probability
      double v_prob = 0.0; //and the probability itself
      
      //for each sensor reading we can have
      for (int k = 0; k < sensorNumRows; ++k){
         
         double a = priorProb[index(k, 1, priorNumCols)]; //the prior probability matrix should always have 1 column
         double b = sensorArray[index(k, observeSequence[0], sensorNumCols)]; //what we sensed at this step
         double c = transitionArray[index(j, k, transitionNumCols)];          //the transition in this step
         v_prob = a * b * c;
         
         if (v_prob > highest){
            
            highest = v_prob;
            argMax = j;
            
         }
         
      }
      
      if (probs[0] < highest){
         
         probs[0] = highest;
         path[0] = argMax;
         
      }
      
   }
   
   //fill in path for t > 0
   for (int i = 1; i < numSequences; ++i){
      
      double highest = 0.0; //keep the highest probability
      double argMax = 0.0; //keep the argument that gives us the largest probability
         
      //for each transition we can make
      for (int k = 0; k < sensorNumRows; ++k){
            
         double a = probs[i - 1];                                              //probability from last step
         double b = sensorArray[index(k, observeSequence[i], sensorNumCols)];  //what we sensed at this step
         double c = transitionArray[index(path[i - 1], k, transitionNumCols)]; //the transition we are testing
         double v_prob = a * b * c;
            
         if (v_prob >= highest){
               
            highest = v_prob;
            argMax = k;
               
         }
         
      }
         
      if (highest >= probs[i]){
            
         probs[i] = highest;
         path[i] = argMax;
            
      }
      
   }
   
   cout << "State\tProbability" << endl;
   
   for (int i = 0; i < numSequences; ++i){
      
      cout << ++path[i] << "\t\t" << fixed << setprecision(PRINT_PRECISION) << (probs[i] / pow(MULTIPLIER, ((i + 1) * 2) + 1)) << endl;
      
   }
   
   delete [] sensorArray;
   delete [] transitionArray;
   delete [] priorProb;
   
   return EXIT_SUCCESS;

}
