/******************************************************************************

    File:   PatternLS.h
    Desc:   (low-speed)

******************************************************************************/
#ifndef _PATTERN_LS_H_
#define _PATTERN_LS_H_

#include "Defines.h"

//-----------------------------------------------------------------------------
//  pattern class
class CPatternLS
{
private:
    word CurrCom;
    
    char TxName[PATT_MAX_NUM][APP_MAX_CHAR];
    byte TxBuff[PATT_MAX_NUM][PATT_MAX_STEP];
    long TxSize[PATT_MAX_NUM];
    long TxLast;
    void* TxTask;
    
    byte RxBuff[PATT_MAX_STEP];
    long RxSize;
    void* RxTask;
    
    char CoName[APP_MAX_CHAR];
    void* CoTask;
    
    PatternInfo ListInfo[2][PATT_MAX_NUM];
    word SizeInfo[2];
    
public:
    long Load(PatternInfo pattInfo, word* listDut);
    
    int ModifySad(char *name, byte slave_addr, word* listDut, int level = LOW);
    
    void Encode(byte *buff, long pattloc, long byteloc, long channel);
    void Decode(byte *buff, long pattloc, long byteloc, long channel);
    void Decode(double *buff, long pattloc, long steploc, long stepcnt, long channel);
    
    inline void AssertLS(long ret);

public:
    CPatternLS(void);
    ~CPatternLS(void);
    
    static CPatternLS* GetInstance(void);
    
    int Init(void);
    int LoadAll(word* listDut) { return ERROR_UNIMPLEMENTED; }
    
    int Connect(word com, word* listDut);
    int Disconnect(word* listDut);
    
    int Send(PatternInfo pattInfo, byte slave_addr, byte* toSet, word* listDut, int level = LOW);
    int Receive(PatternInfo pattInfo, byte slave_addr, byte* toGet, int itr, word* listDut);
    int Measure(PatternInfo pattInfo, byte slave_addr, double *toGet, int itr, word* listDut);
    
    int GetInfo(char *name, PatternInfo* info, bool slave_addr = false);
    int GetInfo(word index, PatternInfo* info, bool slave_addr = false);
    
    word GetCom(void) { return this->CurrCom; }

private:
    
    static CPatternLS* Instance;
};

#endif
