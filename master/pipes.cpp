#include "pipes_header.h"

int * pipes_open_readers(int numWorkers)
{   
    int * fd_r = new int[numWorkers];
    
    for (int i = 0; i < numWorkers; i++)
    {
        //cout << i <<endl;
        std::stringstream pipe_name;
        pipe_name << "pipes/worker_W_executive_R" << i;
        fd_r[i] = open(pipe_name.str().c_str() , O_RDONLY );
    }

    return fd_r;
    
} 

int * pipes_open_writers(int numWorkers)
{
    int * fd_w = new int[numWorkers];
    for (int i = 0; i < numWorkers; i++)
    {
        //cout << i <<endl;
        std::stringstream pipe_name;
        pipe_name << "pipes/executive_W_worker_R" << i;
        fd_w[i] = open(pipe_name.str().c_str() , O_WRONLY);
    }

    return fd_w;
    
} 


void pipes_creation(int numWorkers)
{
    for (int i = 0; i < numWorkers; i++)
    {
        std::stringstream pipe_name;

        pipe_name << "pipes/executive_W_worker_R" << i;
        mkfifo(pipe_name.str().c_str() , 0666); 

        //pipe_name.clear();
        pipe_name = stringstream();

        pipe_name << "pipes/worker_W_executive_R" << i;
        mkfifo(pipe_name.str().c_str() , 0666); 
    }
   
}


void pipes_removal(int numWorkers)
{
    for (int i = 0; i < numWorkers; i++)
    {
        std::stringstream pipe_name;

        pipe_name << "pipes/executive_W_worker_R" << i;
        remove(pipe_name.str().c_str()); 

        pipe_name  = stringstream();

        pipe_name << "pipes/worker_W_executive_R" << i;
        remove(pipe_name.str().c_str()); 
    }
    cout << "All pipes deleted" << endl;
}
