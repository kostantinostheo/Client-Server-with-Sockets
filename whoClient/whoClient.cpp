#include <iostream>
#include <stdio.h>	//printf
#include <string.h>	//strlen
#include <sys/socket.h>	//socket
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>
#include "whoClient.h"


using namespace std; 

pthread_mutex_t locker = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t standby = PTHREAD_COND_INITIALIZER;

pthread_mutex_t prutex = PTHREAD_MUTEX_INITIALIZER;
bool canStart = false;
int serverPort;
string serverIP;


//Global vars.
int numThreads;
string queryFile;

int queryThreads;



int main(int argc, char *argv[])
{

    for(int i=0; i<argc; i++)
    {
        if(argc>2 && string(argv[i]) == "-w")
          numThreads = atoi(argv[i+1]);
        
        else if(argc>2 && string(argv[i]) == "-sp")
          serverPort = atoi(argv[i+1]);

        else if(argc>2 && string(argv[i]) == "-sip")
          serverIP = argv[i+1];
        
        else if(argc>2 && string(argv[i]) == "-q")
          queryFile = argv[i+1];


        //input error checking.
        if (argc < 9)
        {
            perror("Error: Too few arguments");
            exit(1);
        }
        if (argc > 9)
        {
            perror("Error: Too many arguments");
            exit(1);
        }
        //SAFE MODE ON
        if (numThreads <=0 )
          numThreads = 8;
        
        
    }
    puts("");
    cout << "--------------------------" << endl;
    cout << "Connecting to server with:" << endl << "IP address: " << serverIP << endl << "port number: " << serverPort << endl;
    cout << "--------------------------" << endl;
    puts("");

    queryThreads = queryFileRead(queryFile);
    queryThreads = queryThreads + 1; // oses oi entoles + mia krifi gia ta statistika.

    if(numThreads != queryThreads)
        numThreads = queryThreads;
	  
	
    whoClientPool wcp(numThreads);
    sendQuery(queryFile, wcp);
    wcp.nowCanStart();

    
	return 0;

}    
    
    
