#include "executive_worker_header.h"
#include "pipes_header.h"
#include <sys/wait.h>

void load_dir(string directory_name, int workers ,int *fdr , int *fdw) //Pernei svarna to directory kai vlepei ena ena ta onomata twn arxeiwn h allwn fakelwn.
{
        struct dirent *directoryEntry; //file_name
        DIR *directoryFolder;
        int i = 0;


        directoryFolder = opendir (directory_name.c_str());
        if (directoryFolder == NULL) 
        {
            cerr << "Error: No such  file or directory" << endl;
        }

        // Process each entry.

        while ((directoryEntry = readdir(directoryFolder)) != NULL)
        {
            if(string(directoryEntry->d_name) != "." && string(directoryEntry->d_name) != "..")
            {
                //cout << directoryEntry->d_name << endl;    //DEBUG 
                //Call function to open directory.
                //directoryOpen(directory_name + "/" + directoryEntry->d_name);
                send_path_to_worker(directory_name + "/" + directoryEntry->d_name, fdw[i%workers]);
                i++;
            }

        }

        // Close directory and exit.
        closedir (directoryFolder);
        
}


void sendNumWorkers(int * writers , int workers)
{
    for (int i = 0; i < workers; i++)
    {
        write(writers[i] , "n" , 1);
        write(writers[i], &workers, sizeof(int));

    }
}


void listCountries(int * readers , int * writers , int workers)
{
    char buff[512];
    for (int i = 0; i < workers; i++)
    {
        write(writers[i] , "l" , 1);

        //cout <<"readers here: "<< readers[i] << endl;
        
        int N;
        int m_size;

        read(readers[i], &N, sizeof(int));

        for (int j = 0; j < N; j++)
        {
            read(readers[i], &m_size, sizeof(int));
            read(readers[i], buff, m_size);
            
            //cout << buff << endl;
        }

    }
    
}

void s_patient_record(int * readers , int * writers , int workers , string rID)
{
    char buff[512];
    int id_length = rID.length()+1;
    for (int i = 0; i < workers; i++)
    {
        write(writers[i] , "s" , 1);
        write(writers[i], &id_length, sizeof(int));
        write(writers[i], rID.c_str() , id_length );

        //cout <<"readers here: "<< readers[i] << endl;
        
        int N;
        int m_size;

        
            read(readers[i], &m_size, sizeof(int));
            if (m_size > 0)
            {
               read(readers[i], buff, m_size); 
               //cout << buff << endl;
            }
    }
    
}

void a_admissions(int * readers , int * writers , int workers , string d, string d1, string d2, string c)
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
        write(writers[i] , "a" , 1);
        write(writers[i], &args, sizeof(int));
        write(writers[i], &length, sizeof(int));
        write(writers[i], tmp.c_str() , length);
        
        int N;
        int m_size;

            read(readers[i], &N, sizeof(int)); 
            
            for (int j = 0; j < N; j++)
            {
                read(readers[i], &m_size, sizeof(int));
                read(readers[i], buff, m_size); 
                //cout << buff << endl;
            }
    }
    
}

void d_discharges(int * readers , int * writers , int workers , string d, string d1, string d2, string c)
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
        write(writers[i] , "d" , 1);
        write(writers[i], &args, sizeof(int));
        write(writers[i], &length, sizeof(int));
        write(writers[i], tmp.c_str() , length);
        
        int N;
        int m_size;

            read(readers[i], &N, sizeof(int)); 
            
            for (int j = 0; j < N; j++)
            {
                read(readers[i], &m_size, sizeof(int));
                read(readers[i], buff, m_size); 
                //cout << buff << endl;
            }
    }
    
}
void f_frequency(int * readers , int * writers , int workers , string d, string d1, string d2, string c)
{
    char buff[512];
    stringstream ss; 
    string tmp;
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
        write(writers[i] , "f" , 1);
        write(writers[i], &args, sizeof(int));
        write(writers[i], &length, sizeof(int));
        write(writers[i], tmp.c_str() , length);
        
        int N;
        int m_size;

            read(readers[i], &N, sizeof(int)); 
            
            for (int j = 0; j < N; j++)
            {
                read(readers[i], &m_size, sizeof(int));
                read(readers[i], buff, m_size);
                result = result + atoi(buff); 
                
            }
    }
    //cout << result << endl;
    
}

void k_top(int * readers , int * writers , int workers , int k, string c, string d, string d1, string d2)
{
    char buff[512];
    stringstream ss; 
    string tmp;
    int result=0; 
    int k_;

    
    ss << c << " " << d << " " <<  d1 << " " << d2;


    tmp = ss.str();
    int length = tmp.length()+1;

    for (int i = 0; i < workers; i++)
    {
        write(writers[i] , "k" , 1);
        write(writers[i], &k, sizeof(int));
        write(writers[i], &length, sizeof(int));
        write(writers[i], tmp.c_str() , length);
        
        int N;
        int m_size;

            read(readers[i], &N, sizeof(int)); 
            
            for (int j = 0; j < N; j++)
            {
                read(readers[i], &m_size, sizeof(int));
                read(readers[i], buff, m_size);
                //cout << buff << endl;
                
            }
    }
    
    
}

int process_creation(int wId)
{
    int fid = fork();
    
    if(fid < 0)
        perror("Eroor: process failed");

    if(fid > 0)//parent process.
    {
        //cout << "Worker has been created" << endl;
        return fid;
    }
        
        
    if(fid == 0)
    {
        //cout << "Worker_" << wId <<"'I am working'" << endl;
        worker_working(wId);
        exit(0);
    }
}

pid_t * worker_creation(int numWorkers)
{
    pid_t * process_id = new pid_t[numWorkers];
    for (int i = 0; i < numWorkers; i++)
    {
        process_id[i] = process_creation(i);
    }
    return process_id;
}

void sendIpAddressToWorker(string ipAddress , int * writers , int workers)
{
    char buff[512];
    int ipLength = ipAddress.length()+1;

    for(int i = 0; i < workers; i++)
    {
        write(writers[i] , "i" , 1);
        write(writers[i], &ipLength, sizeof(int));
        write(writers[i], ipAddress.c_str(), ipLength);
    }


}
void sendPortToWorker(string port, int * writers , int workers)
{
    char buff[512];
    int portLength = port.length();
    int p = atoi(port.c_str());

    for(int i = 0; i < workers; i++)
    {
        write(writers[i] , "t" , 1);
        write(writers[i], &p, sizeof(int));
    }
    


}

void send_path_to_worker(string path, int pipe_descriptor)
{
    char buff[512];
    buff[0] = 'p'; //p for "path".
    int msg_size = path.length()+1;
    
    memcpy(buff+1, &msg_size, sizeof(int));
    write(pipe_descriptor , buff, 5);
    
    memcpy(buff, path.c_str(), msg_size);
    write(pipe_descriptor , buff, msg_size);
}

void shutting_down(pid_t * p_id, int num_workers, int * fdr, int * fdw)
{
    for (int i = 0; i < num_workers; i++)
    {
        kill(p_id[i], SIGINT);
        close(fdw[i]);        
        close(fdr[i]);
        
    }

    for (int i = 0; i < num_workers; i++)
    {
        int x;
        wait(&x);
    }
    
    cout << "Deleting pipes" << endl;
    sleep(1);
    delete[] p_id;
    delete[] fdr;
    delete[] fdw;
    pipes_removal(num_workers);
}