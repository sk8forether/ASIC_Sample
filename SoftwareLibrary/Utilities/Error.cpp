/******************************************************************************

    File:   Error.cpp
    Desc:   Error is a subclass in Level 1 that contains functionality for 
            keeping track of errors, including an Error log, Critical Error
            message boxes, and error checking.

******************************************************************************/
#include "Testbench.h"
#include "Error.h"

bool CError::EnableWarnings = NO;

/******************************************************************************
    Name:   CError
    Desc:   Default constructor
******************************************************************************/
CError::CError(void)
{
}

/******************************************************************************
    Name:   ~CError
    Desc:   Default destructor
******************************************************************************/
CError::~CError(void)
{
}

/******************************************************************************
    Name:   Display
    Desc:   Displays the various error messages in the console, output window
            and generated log file.
******************************************************************************/
void CError::Display(char* label, char* msg)
{
    DBGTrace("---> CError::Display");
    
    cout << label << msg << endl;
    CTestbench::DisplayToOutputWindow(label, msg);
    CTestbench::WriteToLogFile(label, msg);
}

/******************************************************************************
    Name:   DisplayCode
    Desc:   Displays error code in human-readable format (TODO)
******************************************************************************/
void CError::DisplayCode(int status)
{
    DBGTrace("---> CError::DisplayCode");
    
    String label;
    sprintf(label, "ERROR CODE: %i - ", status);
    String errorcode = CError::CodeToString(status);
    
    CError::Display(label, errorcode);
}

/******************************************************************************
    Name:   Warn
    Desc:   For adding less important problems to the error log
******************************************************************************/
void CError::Warn(char* msg)
{
    DBGTrace("---> CError::Warn");
    
    if(EnableWarnings)
    {
        String label = "WARNING: ";
        CError::Display(label, msg);
    }
}

/******************************************************************************
    Name:   Add
    Desc:   Adds a string to an error log containing a message potentially with
            more details on what caused the problem.
******************************************************************************/
void CError::Add(char* msg)
{
    String label = "ERROR: ";
    
    CError::Display(label, msg);
}

/******************************************************************************
    Name:   Add
    Desc:   Adds a string to an error log containing a message potentially with
            more details on what caused the problem.  Then adds a string to the
            log with the error code that was received and what it stands for.
******************************************************************************/
void CError::Add(int status, char* msg)
{
    String label = "ERROR: ";
    
    CError::Display(label, msg);
    CError::DisplayCode(status);
}

/******************************************************************************
    Name:   Critical
    Desc:   
******************************************************************************/
void CError::Critical(int status, char* msg)
{
    DBGTrace("---> CError::Critical");
    
    String crit = "!!! CRITICAL ERROR !!! ";
    
    CError::Display(crit, msg);
    CError::DisplayCode(status);
    
    // Only allow user to continue when Testbench in enabled (Engineering)
    if (CTestbench::enabled == YES)
    {
        String input;
        cout << "Continue? (y/n) ";
        cin.get( input, APP_MAX_CHAR );
        cin.clear();
        cin.ignore(10000, '\n');
        CUtilities::UpperCase(input);
        
        // if input is y, yes, Y, or YES then execution will continue, otherwise exception will be thrown
        if (input[0] == 'Y')
            return;
    }
    
    // thinkfast (catch this)
    throw exception(msg, status);
}

/******************************************************************************
    Name:   Check
    Desc:   
******************************************************************************/
void CError::Check(int status)
{
    //DBGTrace("---> CError::Check (status)");
    
    CError::Check(status, "", "");
}

/******************************************************************************
    Name:   Check
    Desc:   
******************************************************************************/
void CError::Check(int status, char* msg)
{
    //DBGTrace("---> CError::Check (status, msg)");
    
    CError::Check(status, msg, "");
}

/******************************************************************************
    Name:   Check
    Desc:   
******************************************************************************/
void CError::Check(int status, char* msg, char* label_in, bool critical)
{
    DBGTrace("---> CError::Check");
    
    if (status != SUCCESS)
    {
        String label;
        
        if (label_in != "")
            sprintf(label,"ERROR occured at %s: \n", label_in);
        else
            sprintf(label,"ERROR: ");
        
        if ((status == ERROR_HW_CRITICAL) || (critical == YES))
        {
            strcat(label, msg);
            CError::Critical(status, label);
            return;
        }
        
        CError::Display(label,msg);
        CError::DisplayCode(status);
    }
    else
        return;
}

/******************************************************************************
    Name:   CodeToString
    Desc:   Convert to string using ErrorCodes.h
******************************************************************************/
String CError::CodeToString(int code)
{
    DBGTrace("---> CError::CodeToString");
    
    String msg;
    
    switch(code)
    {
        case SUCCESS:
            msg = "Success!!";
            break;
        case ERROR_SPEC:
            msg = "Something humorous about a Spec. Contact Product Engineering.";
            break;
        case ERROR_INIT:
            msg = "Aw, snap!  Something did not get initialized correctly.";
            break;
        case ERROR_RUN:
            msg = "Aw, shucks!  You're trying to ride a dead horse (again).";
            break;
        case ERROR_UNIMPLEMENTED:
            msg = "Oops!  We forgot to implement that...";
            break;
        case ERROR_UNDEFINED:
            msg = "Some part of the code here is undefined, abandoned, and feeling alone.";
            break;
        case ERROR_COMMUNICATION:
            msg = "Communication is overrated.";
            break;
        case WARN_HARDWARE:
            msg = "I might not be qualified to handle hardware debug.";
            break;
        case ERROR_HARDWARE:
            msg = "I am definitely not qualified to handle hardware debug.";
            break;
        case ERROR_HW_CRITICAL:
            msg = "Critical Hardware Error.  He's dead, Jim.";
            break;
        case SIMULATE_HARDWARE:
            msg = "You know you don't have hardware...  Moving along then.";
            break;
        case ERROR_ASIC_SPECIFIC:
            msg = "This software has a problem in ASIC.  Someone will know what that means.";
            break;
        default:
            msg = "I don't even know what you want right now... but I'm not going to do it.";
             break;
    }
    
    strcat(msg, "\nPlease contact Software Engineering.\n");
    return msg;
}
