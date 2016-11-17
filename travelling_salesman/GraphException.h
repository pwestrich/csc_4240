/*
 * GraphException.h
 * by Philip Westrich
 *
 * Simple exception class for the graph I've written.
 */

#ifndef GRAPH_EXCEPTION
#define GRAPH_EXCEPTION

#include <exception>
#include <string>

using namespace std;

class GraphException : public exception {
   
private:
   
   string message;
   
public:
   
   GraphException(const string &message);
   
   string getMessage();
   const char *what();
   
};

#endif
