/******************************************************************************

    File:   Error.h
    Desc:   Error is a subclass in Level 1 that contains functionality for 
            keeping track of errors, including an Error log, Critical Error
            message boxes, and error checking.

******************************************************************************/
#ifndef _ERROR_H_
#define _ERROR_H_

//-----------------------------------------------------------------------------
//  Error class definition
class CError
{
private:
    static bool EnableWarnings;
    
    static void Display(char* label, char* msg);
    static void DisplayCode(int status);
    static String CodeToString(int code);

public:
    CError(void);
    ~CError(void);
    
    static void SetWarnings(bool state) { CError::EnableWarnings = state; }
    
    static void Warn(char* msg);
    static void Add(char* msg);
    static void Add(int status, char* msg);
    static void Critical(int status, char* msg);
    static void Check(int status);
    static void Check(int status, char* msg);
    static void Check(int status, char* msg, char* label_in, bool critical = false);
};

#endif