/******************************************************************************

    File:   Message.h
    Desc:   Message is a subclass in Level 1 that contains functionality for 
    a Message log

******************************************************************************/
#ifndef _MESSAGE_H_
#define _MESSAGE_H_

//-----------------------------------------------------------------------------
//  Message class definition
class CMessage
{
private:

public:
    CMessage(void);
    ~CMessage(void);
    
    static void Add(char* msg);
};

#endif