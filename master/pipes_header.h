#ifndef PIPES_HEADER_FILE
#define PIPES_HEADER_FILE

#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>
#include <sstream>    
#include <string>
#include <iostream>

using namespace std;

void pipes_creation(int numWorkers);
void pipes_removal(int numWorkers);
int * pipes_open_readers(int numWorkers);
int * pipes_open_writers(int numWorkers);





#endif