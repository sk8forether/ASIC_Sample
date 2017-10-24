/******************************************************************************

    File:   Pin.h
    Desc:   Pin is a subclass in Level 2 that contains functionality for
            electrically connecting, disconnecting, and measuring pins on a
            part via tool hardware.  Pin is declared as a subclass in the Tool
            module, and then expanded upon in the tool-type classes.

******************************************************************************/
#ifndef _PIN_H_
#define _PIN_H_

#include "Defines.h"
#ifdef _ISMECA_
    #include "PowerSupply.h"
#endif

//-----------------------------------------------------------------------------
//  Pin class definition
class CPin
{
private:
    static CPin* Instance;
    DAQmxCard* DutCntl;
    DAQmxCard* AnalogIn;
    byte relay_state;

    PinOutStruct PinOut;
#ifdef _ISMECA_
    CPowerSupply* PowerSupply;
#endif
    
    int GetPinType(word* listPin_in, word* listPin) {}
    
public:
    CPin(void) {}
    ~CPin(void) {}
    static CPin* GetInstance(void) { return NULL; }
    
    void Init(PinOutStruct PinOut_in, DAQmxCard* DutCntl_in, DAQmxCard* AnalogIn_in) {}
    void Connect(word* listPin, word* listDut, int gnd = false) {}
#ifdef _ISMECA_
    void SetPowerSupply(CPowerSupply* PowerSupply) { this->PowerSupply = PowerSupply; }
    int Disconnect(void) { return NULL; }
    
    void SetIntMux(byte state, word* listDut) {}
    void SetSBControl(byte state, word* listDut) {}
    void SetAllRelays(byte state, word* listDut) {}
#endif
    
    int GetVoltage(word* listPin, double* val, word* listDut, bool loopback = false) {}
    int GetLoopback(double* val, word* listDut) {}
    
    void SetVoltage(word* listPin, double voltage, double clamp, word* listDut) {}
    int GetCurrent(word* listPin, double* val, word* listDut) {}
    void SetCurrent(word* listPin, double current, double clamp, word* listDut) {}
    void GetFrequency(word* listPort, word* listPin, double* val, word* listDut) {}
    void SetFrequency(word* listPort, word* listPin, double vmin, double vmax, word* listDut) {}
    
    void SetAddrPin(byte state, word* listDut) {}
    
    // removed
    int SetRelays(word* listDut) { return NULL;}
    byte GetRelayState(void) { return NULL; }
    
    void AssertInternal(int status, String label, void* sessionID = NULL) {}
};

#endif