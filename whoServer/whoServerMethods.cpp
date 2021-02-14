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


 int * cons_prod_buff;
 int buffSize;
 pthread_mutex_t buffex = PTHREAD_MUTEX_INITIALIZER; //tonizete sto 'u' :p 
 pthread_cond_t isEmpty = PTHREAD_COND_INITIALIZER;
 pthread_cond_t isFull = PTHREAD_COND_INITIALIZER;

 pthread_mutex_t scrutex = PTHREAD_MUTEX_INITIALIZER;
 string stats_from_worker;

 int * workerPorts = nullptr;
 int numWorkers = -1;
 int counter = 0;
 pthread_mutex_t wrutex = PTHREAD_MUTEX_INITIALIZER;
 string statistics;

void connectionWithWorker(int port)
{
    int socket_desc , client_sock , c , read_size;
	struct sockaddr_in server , client;
	char client_message[2000];

	
	//Create socket
	socket_desc = socket(AF_INET , SOCK_STREAM , 0);
	if (socket_desc == -1)
	{
		printf("Could not create socket");
	}
	//puts("Socket created");
	
	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	
	//Bind
	if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
	{
		//print the error message
		perror("bind failed. Error");
		return;
	}
	puts("bind done");
	
	//Listen
	listen(socket_desc , SOMAXCONN);
	
	//Accept and incoming connection
	puts("Waiting for incoming connections...");
	c = sizeof(struct sockaddr_in);
	
   
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
      perror("accept failed");
      return;
    }
}


int connect(int port)
{
    int sock;
	struct sockaddr_in server;
	char message[1000] , server_reply[2000];
	
	//Create socket
	sock = socket(AF_INET , SOCK_STREAM , 0);
	if (sock == -1)
	{
		printf("Could not create socket");
	}
	//puts("Socket created");
	
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_family = AF_INET;
	server.sin_port = htons( port );

	//Connect to remote server
	if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
	{
		perror("connect failed 6. Error");
		return 1;
	}
    return sock;
}

int countArguments(string input)
{
  int counter = 0;
  string word;
  stringstream ss_tmp(input);

  while(ss_tmp >> word)
    counter++;

  return counter;

}


void recieve_stats_from_worker(int fd, int workers)
{
    char buff[512];
    int m_size;
    int new_fd;

    for (int i = 0; i < workers; i++)
    {
        new_fd = connect(workerPorts[i]);

        read(fd , buff, 1);
        int N;
        int m_size;

        
        read(new_fd, &m_size, sizeof(int));
        if (m_size > 0)
        {
            read(new_fd, buff, m_size); 
            cout << "buff: " << buff << endl;
            write(fd, buff, m_size);
        }
        close(new_fd);
    }
}

void s_patient_record(int fd , int workers , string rID)
{
    char buff[512];
    int new_fd;
    int id_length = rID.length()+1;
    for (int i = 0; i < workers; i++)
    {
        new_fd = connect(workerPorts[i]);
        write(new_fd , "s" , 1);
        write(new_fd, &id_length, sizeof(int));
        write(new_fd, rID.c_str() , id_length );
        //cout << "EDW" << endl;
        
        int N;
        int m_size;

        
            read(new_fd, &m_size, sizeof(int));
            //cout << "m_size: " << m_size << endl;
            if (m_size > 0)
            {
               read(new_fd, buff, m_size); 
               //cout << "buff: " << buff << endl;
               write(fd, buff, m_size);
            }
            close(new_fd);
    }

    
}

void a_admissions(int fd , int workers , string d, string d1, string d2, string c)
{
    char buff[512];
    stringstream ss; 
    string tmp;
    int args;

    if(c!="")
    {
        ss << d << " " <<  d1 << " " << d2 << " " << c;
        args = 4;
    }   
    else
    {
        ss << d << " " <<  d1 << " " << d2;
        args = 3;
    }
    tmp = ss.str();
    int length = tmp.length()+1;

    for (int i = 0; i < workers; i++)
    {
        int new_fd = connect(workerPorts[i]);
        write(new_fd , "a" , 1);
        write(new_fd, &args, sizeof(int));
        write(new_fd, &length, sizeof(int));
        write(new_fd, tmp.c_str() , length);
        
        int N;
        int m_size;

            read(new_fd, &N, sizeof(int)); 
            string answer;
            for (int j = 0; j < N; j++)
            {
                read( new_fd, &m_size, sizeof(int));
                read(new_fd, buff, m_size); 
                answer += buff;
                //answer += "\n";
                
                //cout  << buff <<endl;
            }
            write(fd , answer.c_str() , answer.size()+1);
            close(new_fd);
    }
    
}

void d_discharges(int fd , int workers , string d, string d1, string d2, string c)
{
    char buff[512];
    stringstream ss; 
    string tmp;
    int args;

    if(c!="")
    {
        ss << d << " " <<  d1 << " " << d2 << " " << c;
        args = 4;
    }   
    else
    {
        ss << d << " " <<  d1 << " " << d2;
        args = 3;
    }
    tmp = ss.str();
    int length = tmp.length()+1;

    for (int i = 0; i < workers; i++)
    {
        int new_fd = connect(workerPorts[i]);
        write(new_fd , "d" , 1);
        write(new_fd, &args, sizeof(int));
        write(new_fd, &length, sizeof(int));
        write(new_fd, tmp.c_str() , length);
        
        int N;
        int m_size;

            read(new_fd, &N, sizeof(int));
            string answer; 
            
            for (int j = 0; j < N; j++)
            {
                read(new_fd, &m_size, sizeof(int));
                read(new_fd, buff, m_size);
                //answer = "discharges\n";
                answer += buff;
                //answer += "\n"; 
            }
            write(fd , answer.c_str() , answer.size()+1);
  
            close(new_fd);
    }
    
}
void f_frequency(int fd, int workers , string d, string d1, string d2, string c)
{
    char buff[512];
    stringstream ss; 
    string tmp;
    string answer;
    int result=0; 
    int args;

    if(c!="")
    {
        ss << d << " " <<  d1 << " " << d2 << " " << c;
        args = 4;
    }   
    else
    {
        ss << d << " " <<  d1 << " " << d2;
        args = 3;
    }
    tmp = ss.str();
    int length = tmp.length()+1;

    for (int i = 0; i < workers; i++)
    {
        int new_fd = connect(workerPorts[i]);
        write(new_fd , "f" , 1);
        write(new_fd, &args, sizeof(int));
        write(new_fd, &length, sizeof(int));
        write(new_fd, tmp.c_str() , length);
        
        int N;
        int m_size;

            read(new_fd, &N, sizeof(int)); 
            
            for (int j = 0; j < N; j++)
            {
                read(new_fd, &m_size, sizeof(int));
                read(new_fd, buff, m_size);
                result = result + atoi(buff); 
                 
            }
            answer = to_string(result);
            cout << "result: " << answer <<endl;
            
            
            close(new_fd);
    }
    write(fd, answer.c_str(), answer.size()+1);
    
    
}

void k_top(int fd, int workers , int k, string c, string d, string d1, string d2)
{
    char buff[512];
    stringstream ss; 
    string tmp;
    int result=0; 
    int k_;
    string answer;
    
    ss << c << " " << d << " " <<  d1 << " " << d2;


    tmp = ss.str();
    int length = tmp.length()+1;

    for (int i = 0; i < workers; i++)
    {
        int new_fd = connect(workerPorts[i]);
        write(new_fd , "k" , 1);
        write(new_fd, &k, sizeof(int));
        write(new_fd, &length, sizeof(int));
        write(new_fd, tmp.c_str() , length);
        
        int N;
        int m_size;

            read(new_fd, &N, sizeof(int)); 
            
            for (int j = 0; j < N; j++)
            {
                read(new_fd, &m_size, sizeof(int));
                read(new_fd, buff, m_size);
                answer += buff;
                answer += "\n";
                //cout << buff << endl;
                
            }
            //if(!answer.empty())
                write(fd , answer.c_str() , answer.size()+1);
            
            close(fd);
    }
    
    
}




