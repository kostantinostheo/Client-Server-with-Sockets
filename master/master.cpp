#include <iostream>
#include "executive_worker_header.h"
#include "pipes_header.h"
#include "structures_header.h"


using namespace std;



int numWorkers;
int bufferSize;
string input_dir;
string serverIP;
string serverPort;

bool break_flag = true;




void sig_h(int signo)
{

    cout << " ctrl+c" << endl;
    if(signo == SIGINT)
      cout << signo << endl;
       // break_flag = false;
}

int main(int argc, char *argv[]) 
{
    

    for(int i=0; i<argc; i++)
    {
        if(argc>2 && string(argv[i]) == "-w")
          numWorkers = atoi(argv[i+1]);
        
        else if(argc>2 && string(argv[i]) == "-i")
          input_dir = argv[i+1];

        else if(argc>2 && string(argv[i]) == "-b")
          bufferSize = atoi(argv[i+1]);
        
        else if(argc>2 && string(argv[i]) == "-s")
          serverIP = argv[i+1];

        else if(argc>2 && string(argv[i]) == "-p")
          serverPort = argv[i+1];

        //input error checking.
        if (argc < 11)
        {
            perror("Error: Too few arguments");
            exit(1);
        }
        if (argc > 11)
        {
            perror("Error: Too many arguments");
            exit(1);
        }
        //SAFE MODE ON
        if (numWorkers <=0 )
          numWorkers = 2;
        
        if (bufferSize <=0)
          bufferSize = 5;
        
    }
    if(numWorkers == 1)
    {
      cout << "Are you sure you wanna do that? Only one worker..?" << endl;
      sleep(1);
    }
    puts("");
    cout << "...master..." << endl;
    cout << "workers: " << numWorkers << endl << "buffer size: " << bufferSize << endl;
    cout << "------------" << endl;
  
  
  pipes_creation(numWorkers);

  
  pid_t * proc_id;
  proc_id = worker_creation(numWorkers); //forks
  
  int * writersArr = pipes_open_writers(numWorkers);
  int * readerArr = pipes_open_readers(numWorkers);
  
  
  load_dir("dir", numWorkers, readerArr, writersArr);

  sendIpAddressToWorker(serverIP , writersArr , numWorkers);
  sendPortToWorker(serverPort, writersArr, numWorkers);
  sendNumWorkers(writersArr , numWorkers);
  
  stringstream cmd; 
  string command;
  while (break_flag)
  {
    //signal(SIGINT , sig_h);
    if(command == "/exit")
      break;
    
      
    sleep(1);
    //cout << "command: ";
    cin >> command;
    cmd.clear();
  }


  cout << "terminating...please wait" << endl;
  shutting_down(proc_id, numWorkers, readerArr , writersArr);

  return EXIT_SUCCESS;
}
