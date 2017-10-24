/******************************************************************************

    File:   FileIO.cpp
    Desc:   FileIO is a subclass in Level 1 that contains functionality for 
    to write output to a file

******************************************************************************/
#include "FileIO.h"

//CFileIO *CFileIO::Instance = NULL;
CFileIO *CFileIO::Instance[3] = {NULL, NULL, NULL};


/******************************************************************************
    Name:   CFileIO
    Desc:   Default constructor
******************************************************************************/
CFileIO::CFileIO(void)
{
    dir = FILEIO_WRITE;
}

/******************************************************************************
    Name:   ~CFileIO
    Desc:   Default destructor
******************************************************************************/
CFileIO::~CFileIO(void)
{
}

/******************************************************************************
    Name:   Open
    Desc:   Open file
******************************************************************************/
void CFileIO::Open(int dirIn, char *fileName)
{
    if (DBGTraceEnabled)
        cout << "---> CFileIO::Open" << endl;
    
    dir = dirIn;
    
    if (dir == FILEIO_WRITE)
    {
        myoutfile.open (fileName);
    }
    else
    {
        myinfile.open (fileName);
    }
}

/******************************************************************************
    Name:   Close
    Desc:   Close file
******************************************************************************/
void CFileIO::Close(void)
{
    if (DBGTraceEnabled)
        cout << "---> CFileIO::Close" << endl;
    
    if (dir == FILEIO_WRITE)
    {
        myoutfile.close();
    }
    else
    {
        myinfile.close();
    }

}

/******************************************************************************
    Name:   WriteLine
    Desc:   Write a string followed by a new line
******************************************************************************/
void CFileIO::WriteLine(char* msg)
{
    if (DBGTraceEnabled)
        cout << "---> CFileIO::WriteLine" << endl;
    
    if (dir == FILEIO_READ)
    {
        CUtilities::Error.Add("CFileIO::WriteLine: Tried to write output from an input instance");
        return;
    }
    
    myoutfile << msg << endl;
}

/******************************************************************************
    Name:   Write
    Desc:   Write a string
******************************************************************************/
void CFileIO::Write(char* msg)
{
    if (DBGTraceEnabled)
        cout << "---> CFileIO::Write" << endl;
    
    if (dir == FILEIO_READ)
    {
        CUtilities::Error.Add("CFileIO::Write: Tried to write output from an input instance");
        return;
    }
    
    myoutfile << msg;
}


/******************************************************************************
    Name:   ReadLine
    Desc:   Read a string followed by a new line
******************************************************************************/
void CFileIO::ReadLine(char* msg)
{
    if (DBGTraceEnabled)
        cout << "---> CFileIO::ReadLine" << endl;
    
    if (dir == FILEIO_WRITE)
    {
        CUtilities::Error.Add("CFileIO::ReadLine: Tried to read input from an output instance");
        return;
    }

    string tmp;
    getline (myinfile,tmp);
    sprintf(msg, (char *) tmp.data());
}

/******************************************************************************
    Name:   Read
    Desc:   Read a string
******************************************************************************/
void CFileIO::Read(char* msg)
{
    if (DBGTraceEnabled)
        cout << "---> CFileIO::Read" << endl;
    
    if (dir == FILEIO_WRITE)
    {
        CUtilities::Error.Add("CFileIO::Read: Tried to read input from an output instance");
        return;
    }

    myinfile >> msg;
}

/******************************************************************************
    Name:   GetInstance
    Desc:   Returns/creates singleton instance for the specified id 
******************************************************************************/
CFileIO *CFileIO::GetInstance(int id)
{
    if (DBGTraceEnabled)
        cout << "---> CFileIO::GetInstance (id)" << endl;
    
    if (id > (FILEIO_MAX_FILES-1))
    {
        String msg;
        sprintf(msg, "CFileIO::GetSpecificInstance id:%i must be less than %i", id, FILEIO_MAX_FILES);
        CUtilities::Error.Add(msg);
    }
    
    if (CFileIO::Instance[id] == NULL)
        CFileIO::Instance[id] = new CFileIO();
    
    return CFileIO::Instance[id];
}

