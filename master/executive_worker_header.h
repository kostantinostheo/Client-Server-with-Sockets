#ifndef HEADER_FILE
#define HEADER_FILE

#include <iostream>
#include <dirent.h>
#include <string>
#include <fstream>
#include <sstream>  
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <cstring>



using namespace std;

//the entire header file file

void directoryOpen(string directory_name);
void load_dir(string directory_name, int workers ,int *fdr , int *fdw);
pid_t * worker_creation(int nWorkers);
void worker_working(int wId);
void send_path_to_worker(string path, int pipe_descriptor);
void shutting_down(pid_t * p_id, int num_workers, int * fdr, int * fdw);
void write_text_to_log_file();
void sig_handler(int signo);
int dateCompare(string date1 , string date2);
bool dateBounds(string date_bot , string date_top , string search_date);
void listCountries(int * readers , int * writers , int workers);
void s_patient_record(int * readers , int * writers , int workers , string rID);
void a_admissions(int * readers , int * writers , int workers , string d, string d1, string d2, string c = "");
void d_discharges(int * readers , int * writers , int workers , string d, string d1, string d2, string c = "");
void f_frequency(int * readers , int * writers , int workers , string d, string d1, string d2, string c = "");
void k_top(int * readers , int * writers , int workers , int k, string c, string d, string d1, string d2);
void sendIpAddressToWorker(string ipAddress , int * writers , int workers);
void sendPortToWorker(string port, int * writers , int workers);
void sendNumWorkers(int * writers , int workers);


#endif