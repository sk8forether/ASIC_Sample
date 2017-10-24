/******************************************************************************

    File:   PowerSupply.h
    Desc:   PowerSupply is a subclass in Level 2 that contains functionality
            for electrically connecting and disconnecting data acquisition
            lines via tool hardware.

******************************************************************************/
#ifndef _POWER_SUPPLY_H_
#define _POWER_SUPPLY_H_

#include "Defines.h"

//-----------------------------------------------------------------------------
//  PowerSupply class definition
class CPowerSupply
{
private:
    static CPowerSupply* Instance;

    int uses_VPP;
    int burn_with_vpp;
    DCPowerCard* PSSessions;
    int ConvertSiteToSessionIndex(int site, String label);
    int Configure(String location, dword *SlotID);
    int SetVDD(DCPowerCard* card, double voltage);
    int SetInterfaceVoltage(DCPowerCard* card, double voltage);
    int SetBurnVoltage(DCPowerCard* card, double voltage);

public:
    CPowerSupply(void) {}
    ~CPowerSupply(void) {}
    static CPowerSupply* GetInstance(void) { return NULL; }
    
    void Init(DCPowerCard *PSSessions, int uses_VPP);
    int Connect(void) { return NULL; }
    void Connect(word* listRly, word* listDut) {}
    int Disconnect(void) { return NULL; }
    void Disconnect(word* listRly, word* listDut) {}

    int PowerOnPart(double vdd, word* listDut) { return NULL; }
    int PowerOffPart(word* listDut) { return NULL; }
    int SetBurnVoltage(double vdd) { return NULL; }

    void SetVoltage(word *listPin, double voltage, word *listDut) {}
    int GetVoltageVDD(double* voltage, word* listDut) { return NULL; }
    int GetVoltageIO_VDD(double* voltage, word* listDut) { return NULL; }
    
    void SetCurrent(word* listPin, double current, word* listDut) {}
    int GetCurrentVDD(double* current, word* listDut) { return NULL; }
    int GetCurrentIO_VDD(double* current, word* listDut) { return NULL; }

    void AssertInternal(int status, String label, int sessionID = NULL) {}

private:
    int GetCurrent(String channel, double* current, word* listDut) { return NULL; }
    int GetVoltage(String channel, double *voltage, word *listDut) { return NULL; }

};

#endif