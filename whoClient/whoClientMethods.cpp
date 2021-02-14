#include <iostream>
#include <fstream>
#include "whoClient.h"

using namespace std;

int queryFileRead(string queryFileName)
{
    ifstream qFile (queryFileName);
    int countFileLines = 0;

    if (!qFile)
    {
        cout << "can't open file" << endl;
        return -1;
    }
    
    char strs[255];

    while(qFile)
    {
        qFile.getline(strs, 255);
        if (qFile)
            countFileLines++;
        
    }

    qFile.close();
    return countFileLines;
}

int sendQuery(string queryFileName, whoClientPool &pool)
{
    ifstream qFile (queryFileName);
    int countFileLines = 0;

    if (!qFile)
    {
        cout << "can't open file" << endl;
        return -1;
    }
    
    char strs[255];
    pool.settingThreadThesi("stats _");
    while(!qFile.eof())
    {
        qFile.getline(strs, 255);
        if (qFile)
            pool.settingThreadThesi(strs);
        
    }

    qFile.close();
    return countFileLines;
}