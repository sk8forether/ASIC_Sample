// SampleConsole.cpp : Defines the entry point for the console application.
//

#include <tchar.h>

#include "PTC.h"

using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
    OutputDebugStringA("\n");
    
    //-------------------------------------------------------------
    // PTC Main
    PTC::GetInstance()->Main();
    
    //-------------------------------------------------------------
    // End
    cout << endl << "Press enter to exit" << endl;
    cin.ignore();
    
    return 0;
}
