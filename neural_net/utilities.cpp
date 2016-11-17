
//A file of utilities

#include "utilities.h"

using std::string;
using std::vector;

int index(const int row, const int col, const int numCols){
	
	return ((row * numCols) + col);

}

double getRandomDouble(const double &lowBound, const double &highBound){

   unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
	default_random_engine generator(seed);
   
	uniform_real_distribution<double> distribution(lowBound, highBound);

	return distribution(generator);

}

vector<string> explode(const string &delimiter, const string &str){
   
   vector<string> arr;
   
   int strleng = static_cast<int>(str.length());
   int delleng = static_cast<int>(delimiter.length());
   
   if (delleng == 0) {
      
      return arr; //no change
      
   }
   
   int i = 0;
   int k = 0;
   
   while(i < strleng) {
      
      int j=0;
      while (i+j<strleng && j<delleng && str[i+j]==delimiter[j]) {
         
         j++;
         
      }
      
      //found delimiter
      if (j == delleng) {
         
         arr.push_back(str.substr(k, i - k));
         i += delleng;
         k = i;
      }
      
      else {
         
         i++;
         
      }
      
   }
   
   arr.push_back(str.substr(k, i - k));
   return arr;
   
}
