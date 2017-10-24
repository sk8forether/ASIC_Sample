/******************************************************************************

    File:   PatternHS.h
    Desc:   (high-speed)

******************************************************************************/
#ifndef _PATTERN_HS_H_
#define _PATTERN_HS_H_

#include "Defines.h"

#define TIME_OUT 5000

//------------------------------------------------------------------------------
//  pattern class
class CPatternHS
{
private:
    
    HSDIOCard* CommGroup;
    word CurrCom;
    
    int Connected;
    
    char PattList[PATT_MAX_NUM][APP_MAX_CHAR];
    long PattSize[PATT_MAX_NUM];
    
    PatternInfo ListInfo[2][PATT_MAX_NUM];
    word SizeInfo[2];
    
    //dword RxBuff[TOOL_MAX_COM_GROUP][PATT_MAX_STEP];
    //long RxSize[TOOL_MAX_COM_GROUP];
    //byte RxBuffDecoded[PATT_MAX_STEP][TOOL_NUM_PATT_HS_LINES];
   
public:
    CPatternHS(void);
    ~CPatternHS(void);
    
    static CPatternHS* GetInstance(void);
    
    int Init(void);
    int LoadAll(byte slave_addr, word* listDut);
    void CommIn(HSDIOCard* CommGroup_in);
    
    int Connect(word com, word* listDut);
    int Disconnect(word* listDut);
    
    int ModifySad(char *name, byte slave_addr, word* listDut, int level = LOW);
    
    long Modify(PatternInfo pattInfo, byte* toSet, word* listDut, int level = LOW);
    int VerifyAcknowledgement(int Step, dword *data, dword mask, word site, int line, String pattName, bool showOutput = true);
    int Send(PatternInfo pattInfo, byte slave_addr, byte* toSet, word* listDut, int level = LOW);
    int Receive(PatternInfo pattInfo, byte slave_addr, byte* toGet, int itr, word* listDut, PatternInfo pattInfoSad, bool* acks = NULL);
    int Measure(PatternInfo pattInfo, byte slave_addr, double *toGet, int itr, word* listDut);
    int Scan(PatternInfo pattInfo, long *toGet, word* listDut);//
    
    int SendSAD(PatternInfo pattInfo, word* listDut, int level = LOW);
    int ReceiveSAD(PatternInfo pattInfoSad, word* listDut, bool* acks = NULL);
    
    int GetInfo(char *name, PatternInfo* info, bool slave_addr = false);
    int GetInfo(word index, PatternInfo* info, bool slave_addr = false);
    
    word GetCom(void) { return this->CurrCom; }//

private:
    long Load(PatternInfo pattInfo, word* listDut, String name = "");
    
    void Encode(byte* src, byte *dst);
    void Decode(byte* src, byte *dst);
    
    inline void AssertInternal(dword sid, long ret);
    
    static CPatternHS* Instance;
    
    void AssertInternal(int status, String label, int sessionID = NULL);
};

#endif
