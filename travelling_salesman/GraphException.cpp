//
//  GraphException.cpp
//  Genetic Travelling Salesman
//
//  Created by Philip Westrich on 9/6/14.
//  Copyright (c) 2014 Philip Westrich. All rights reserved.
//

#include "GraphException.h"

using namespace std;

GraphException::GraphException(const string &message){
   
   this->message = message;
   
}

string GraphException::getMessage(){
   
   return message;
   
}

const char *GraphException::what(){
   
   return message.c_str();
   
}
