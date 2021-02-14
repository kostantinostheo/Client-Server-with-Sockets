#ifndef WHO_SERVER_H
#define WHO_SERVER_H
#include <pthread.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <iostream>
#include <sstream>

using namespace std;

int countArguments(string input);
int dateCompare(string date1 , string date2);
bool dateBounds(string date_bot , string date_top , string search_date);
void listCountries(int * readers , int * writers , int workers);
void s_patient_record(int fd , int workers , string rID);
void a_admissions(int fd , int workers , string d, string d1, string d2, string c = "");
void d_discharges(int fd, int workers , string d, string d1, string d2, string c = "");
void f_frequency(int fd, int workers , string d, string d1, string d2, string c = "");
void k_top(int fd, int workers , int k, string c, string d, string d1, string d2);
void recieve_stats_from_worker(int fd, int workers);

extern int * cons_prod_buff;
extern int buffSize;
extern pthread_mutex_t buffex; //tonizete sto 'u' :p 
extern pthread_cond_t isEmpty;
extern pthread_cond_t isFull;
extern string statistics;
extern pthread_mutex_t scrutex ;

extern int * workerPorts;
extern int numWorkers;
extern int counter;
extern pthread_mutex_t wrutex;

class whoThread
{
    private:
        pthread_t threadID;
    public:
        whoThread()
        {
            pthread_create(&threadID, nullptr, consumer, nullptr);

        }
        ~whoThread()
        {
            pthread_join(threadID , nullptr);
        }
        
    static void *consumer(void *s)
    {
        char buffAnswer [512];
        int fd;
            
        while(1)
        {
            pthread_mutex_lock(&buffex);
            while (buffSize == 0)
            {
                pthread_cond_wait(&isFull , &buffex);
            }
            fd = cons_prod_buff[buffSize - 1];
            buffSize--;

            pthread_cond_signal(&isEmpty);
            pthread_mutex_unlock(&buffex);


            //How thread works...
            callProtocol(fd);
            close(fd);
        }

        
        pthread_exit(nullptr);
    }

    static void callProtocol(int fd)
    {
        char buff[512];
        string cmd; 
        string commandFirst;
        int howMuchToCut;
        
        read(fd, buff, 512);
        
        cmd = buff;

        stringstream(cmd)>>commandFirst;
        if(commandFirst == "n")
        {
            pthread_mutex_lock(&wrutex);
            memcpy(&numWorkers, buff+2, sizeof(int));

            if(workerPorts==nullptr)
                workerPorts = new int[numWorkers];

            memcpy(&workerPorts[counter++], buff+6, sizeof(int));

            //cout << workerPorts[counter-1] << endl;
            statistics += buff+10;
            //cout << statistics << endl;
            pthread_mutex_unlock(&wrutex);
            return;
        }
        else if(commandFirst == "stats")
        {
            write(fd , statistics.c_str() , statistics.size()+1);
        }
        cmd = cmd.substr(commandFirst.size()+1);
        
        pthread_mutex_lock(&scrutex);
        //cout << commandFirst << endl;
        //cout << cmd << endl;
        pthread_mutex_unlock(&scrutex);

        

        if(commandFirst == "/numPatientAdmissions")
        {
        
            int n = countArguments(cmd);
            
            //count kena. ara kai lekseis sto stream.
            string disease;
            string date_bot;
            string date_top;
            string country;
            if(n == 3)
            {
                stringstream(cmd) >> disease >> date_bot >> date_top;
                a_admissions(fd, numWorkers, disease, date_bot, date_top);
            }
            else if(n == 4)
            {
                stringstream(cmd) >> disease >> date_bot >> date_top >> country;
                a_admissions(fd, numWorkers, disease, date_bot, date_top, country);
            }
                
            else
                cout << "error" << endl;
            }
            else if(commandFirst == "/numPatientDischarges")
            {
            
            int n = countArguments(cmd);
            
            //count kena. ara kai lekseis sto stream.
            string disease;
            string date_bot;
            string date_top;
            string country;
            if(n == 3)
            {
                stringstream(cmd) >> disease >> date_bot >> date_top;
                d_discharges(fd, numWorkers, disease, date_bot, date_top);
            }
            else if(n == 4)
            {
                stringstream(cmd) >> disease >> date_bot >> date_top >> country;
                d_discharges(fd, numWorkers, disease, date_bot, date_top, country);
            }
                
            else
                cout << "error" << endl;
            }

            else if(commandFirst == "/diseaseFrequency")
            {
            
            int n = countArguments(cmd);
            
            //count kena. ara kai lekseis sto stream.
            string disease;
            string date_bot;
            string date_top;
            string country;
            if(n == 3)
            {
                stringstream(cmd) >> disease >> date_bot >> date_top;
                f_frequency(fd, numWorkers, disease, date_bot, date_top);
            }
            else if(n == 4)
            {
                stringstream(cmd) >> disease >> date_bot >> date_top >> country;
                f_frequency(fd, numWorkers, disease, date_bot, date_top, country);
            }
                
            else
                cout << "error" << endl;
            }

            if(commandFirst == "/searchPatientRecord")
            {
                           
                int n = countArguments(cmd);

                string recordID_tmp;

                if(n == 1)
                {
                    stringstream(cmd) >> recordID_tmp;
                    //cout << "Inside WhoServer.h: " << recordID_tmp << endl;
                    s_patient_record(fd , numWorkers, recordID_tmp);
                } 
                    
                else if(n > 1)
                    cout << "Too many arguments" << endl;
                else
                    cout << "Too few arguments" << endl;  

            }


            else if(commandFirst == "/topk-AgeRanges")
            {
            
                int n = countArguments(cmd);
                
                if(n<5)
                    cout << "error" << endl;
                else if(n > 5)
                    cout << "error" << endl;

                //count kena. ara kai lekseis sto stream.
                int k;
                string country;
                string disease;
                string date_bot;
                string date_top;
                
                
                stringstream(cmd) >> k >> country >> disease >> date_bot >> date_top;
                k_top(fd, numWorkers, k, country, disease, date_bot, date_top);
            

            }
            else if(commandFirst == "/exit")
            {
                recieve_stats_from_worker(fd , numWorkers);
            }
    }
};


class whoPool
{
    private:
        whoThread * wThreads;
        
    public:
        whoPool(int numthreads , int whoServerBuffSize)
        {
            buffSize = 0;
            cons_prod_buff = new int[whoServerBuffSize];
            wThreads = new whoThread[numthreads];

        }
        ~whoPool()
        {
            delete[] wThreads;
            pthread_mutex_destroy(&buffex);
            pthread_cond_destroy(&isEmpty);
            pthread_cond_destroy(&isFull);
            pthread_mutex_destroy(&scrutex);
            pthread_mutex_destroy(&wrutex);
            delete[] workerPorts;
        }
};






#endif