#ifndef CLIENT_FILE
#define CLIENT_FILE

#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <iostream>
#include <sys/socket.h>	//socket


using namespace std;

extern pthread_mutex_t locker;
extern pthread_cond_t standby;
extern pthread_mutex_t prutex;
extern bool canStart;
extern int serverPort;
extern string serverIP;

class whoClientThread
{
private:
    pthread_t c_threadID;
    string query;
public:

    whoClientThread()
    {
        
    }
    ~whoClientThread()
    {
        pthread_join(c_threadID , nullptr);
    }
    void threadCreator(string query)
    {
        this->query = query;
        pthread_create(&c_threadID, nullptr, clientPrintResult, (void*)this->query.c_str());

    }

    static void * clientPrintResult(void *s)
    {
        string q((char*)s);
        pthread_mutex_lock(&locker);
        
        while (!canStart)
        {
            pthread_cond_wait(&standby, &locker);
        }
        pthread_mutex_unlock(&locker);
        
        int sock;
        struct sockaddr_in server;
        char message[1000] , server_reply[2000];
        
        //Create socket
        sock = socket(AF_INET , SOCK_STREAM , 0);
        if (sock == -1)
        {
            perror("Could not create socket");
        }
        
        
        server.sin_addr.s_addr = inet_addr(serverIP.c_str());
        server.sin_family = AF_INET;
        server.sin_port = htons(serverPort);

        if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
        {
            perror("connect failed. Error");
            pthread_exit(nullptr);
        }

        //Stelnw ta queries
        if( send(sock , q.c_str() , q.size()+1 , 0) < 0)
        {
            puts("Send failed");
            pthread_exit(nullptr);
        }
        

        //perimenw apantisis
        pthread_mutex_lock(&prutex);
        char buff[512];
        cout << "Result:" << endl;
        while( recv(sock , buff , 512 , 0) > 0)
        {   
            cout << buff << endl; 
        }

        pthread_mutex_unlock(&prutex);   
        
        close(sock);
    }
};

class whoClientPool
{
private:
    whoClientThread * qThreads;
    int thesi = 0;
    
public:
    whoClientPool(int numThreads)
    {
        qThreads = new whoClientThread[numThreads];
    }
    ~whoClientPool()
    {
        delete[] qThreads;
        pthread_mutex_destroy(&locker);
        pthread_cond_destroy(&standby);
        pthread_mutex_destroy(&prutex);

    }
    void settingThreadThesi(string q)
    {
        qThreads[thesi].threadCreator(q);
        thesi++;
    }
    void nowCanStart()
    {
        canStart = true;
        pthread_cond_broadcast(&standby);
    }
};

int queryFileRead(string queryFileName);
int sendQuery(string queryFileName, whoClientPool &pool);



#endif