/******************************************************************************

    File:   FileIO.h
    Desc:   FileIO is a subclass in Level 1 that contains functionality for 
    file input and output

******************************************************************************/
#ifndef _FILE_IO_H_
#define _FILE_IO_H_

#include "Defines.h"

#define FILEIO_MAX_FILES    5

//-----------------------------------------------------------------------------
//  FileIO class definition
class CFileIO
{
private:
    //static CFileIO* Instance;
    static CFileIO* Instance[3];
    ofstream myoutfile;
    ifstream myinfile;

    int dir;

public:
    CFileIO(void);
    ~CFileIO(void);

    //static CFileIO* GetInstance(int id = 0);
    static CFileIO* GetInstance(int id = 0);

    void Open(int dir, char* fileName);
    void Close(void);
    
    void Write(char* msg);
    void WriteLine(char* msg);
    void Read(char* msg);
    void ReadLine(char* msg);
};

#endif