/******************************************************************************

    File:   Message.cpp
    Desc:   Message is a subclass in Level 1 that contains functionality for 
    a Message log

******************************************************************************/
#include "Message.h"
#include "Testbench.h"

/******************************************************************************
    Name:   CMessage
    Desc:   Default constructor
******************************************************************************/
CMessage::CMessage(void)
{
}

/******************************************************************************
    Name:   ~CMessage
    Desc:   Default destructor
******************************************************************************/
CMessage::~CMessage(void)
{
}

/******************************************************************************
    Name:   AddMessage
    Desc:   Adds a string to a message log
******************************************************************************/
void CMessage::Add(char* msg)
{
    DBGTrace("---> CMessage::Add");
    
    cout << "" << msg << endl;
    CTestbench::DisplayToOutputWindow("", msg);
    CTestbench::WriteToLogFile("",msg);
}
