#include <iostream>
#include <string>
#include <stdio.h>
#include <string.h>	//strlen
#include <sys/socket.h>
#include <arpa/inet.h>	//inet_addr
#include <unistd.h>	//write
#include <sys/types.h> 
#include "whoServerhead.h"
using namespace std;

int bufferSize;
int queryPortNum;
int statisticsPortNum;
int numThreads;

int main(int argc , char *argv[])
{


	for(int i=0; i<argc; i++)
    {
        if(argc>2 && string(argv[i]) == "-q")
          queryPortNum = atoi(argv[i+1]);
        
        else if(argc>2 && string(argv[i]) == "-s")
          statisticsPortNum = atoi(argv[i+1]);

        else if(argc>2 && string(argv[i]) == "-w")
          numThreads = atoi(argv[i+1]);
        
        else if(argc>2 && string(argv[i]) == "-b")
          bufferSize = atoi(argv[i+1]);


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
          numThreads = 5;
        
        if (bufferSize <=0)
          bufferSize = 5;
        
        
    }
  
  ////////////////// PHASE 1 ////////////////////////////
	int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char client_message[2000];
	whoPool wp(numThreads, bufferSize);
	

	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
		perror("Could not create socket");
	

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(statisticsPortNum);
	
	
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("bind failed. Error");
		return 1;
	}
	cout << "PHASE 1 bind done" << endl;
	

	listen(socket_desc , SOMAXCONN);
	
	
	cout << "Waiting for incoming connections..." << endl;
	c = sizeof(struct sockaddr_in);
	
  int i = 0;
  //producer
  while(1)
  {

    pthread_mutex_lock(&buffex);
    while (buffSize != 0)
    {
      pthread_cond_wait(&isEmpty , &buffex);
    }

    pthread_mutex_unlock(&buffex);

    pthread_mutex_lock(&wrutex);
    if (i >= numWorkers && numWorkers != -1)
    {
      pthread_mutex_unlock(&wrutex); 
      break;
    }
    else 
      pthread_mutex_unlock(&wrutex);
    


    
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    i++;
    if (client_sock < 0)
    {
      perror("accept failed");
      return 1;
    }

    pthread_mutex_lock(&buffex);
    while (buffSize == bufferSize)
    {
      pthread_cond_wait(&isEmpty, &buffex);
    }
    cons_prod_buff[buffSize] = client_sock;
    buffSize++;

    pthread_cond_signal(&isFull);
    pthread_mutex_unlock(&buffex);
    sleep(1);
    
  }
  close(socket_desc);

	
  ////////////////// PHASE 2 ////////////////////////////
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	

	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(queryPortNum);
	

	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("bind failed. Error");
		return 1;
	}
	cout << "PHASE 2 bind done" << endl;
	
	listen(socket_desc , SOMAXCONN);
	
	
	cout << "Waiting for incoming connections..." << endl;
	c = sizeof(struct sockaddr_in);
	
  //producer
  while(1)
  {
    /* 
      Diadikasia paragogou-katanaloti
      O paragogos stin siggekrineni periptosi dld o Server kanei accept tin sindesi apo ton Client.
    */
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
      perror("accept failed");
      return 1;
    }
    //puts("Connection accepted");
    pthread_mutex_lock(&buffex);
    while (buffSize == bufferSize)
    {
      pthread_cond_wait(&isEmpty, &buffex);
    }
    cons_prod_buff[buffSize] = client_sock;
    buffSize++;

    pthread_cond_signal(&isFull);
    pthread_mutex_unlock(&buffex);

    
  }

	close(socket_desc);

	


  

	return 0;
}
