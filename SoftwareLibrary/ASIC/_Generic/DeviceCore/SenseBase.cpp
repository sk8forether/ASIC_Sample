/******************************************************************************

    File:   SenseBase.cpp
    Desc:   SenseBase is a subcomponent in Level 1 of the design that
            contains some commonly used sense element functionality

******************************************************************************/
#include "SenseBase.h"

/******************************************************************************
    Name:   CSenseBase
    Desc:   Default constructor
******************************************************************************/
CSenseBase::CSenseBase(void)
{
    this->CurrODR = 0.0;
}

/******************************************************************************
    Name:   ~CSenseBase
    Desc:   Default destructor
******************************************************************************/
CSenseBase::~CSenseBase(void)
{
}

/******************************************************************************
    Name:   SampleOutputs
    Desc:   Read Sense Element outputs multiple times, collecting average and
            standard deviation for each DUT
            
            Currently in development
            
******************************************************************************/
void CSenseBase::SampleOutputs(int count, int timeout, double* avg, double* st_dev, int output_type, word* listDut)
{
    DBGTrace("--> CSenseBase::SampleOutputs");
    
    int dut;
    byte fromDut[MAX_NUM_AXES][APP_MAX_ADDR][TOOL_MAX_DUT];
    int int_output[APP_MAX_SAMPLES][MAX_NUM_AXES][TOOL_MAX_DUT];
    double average[MAX_NUM_AXES][TOOL_MAX_DUT];
    double sigma[MAX_NUM_AXES][TOOL_MAX_DUT];
    //vector<int> SampleList[MAX_NUM_AXES][TOOL_MAX_DUT];
    //vector<vector<int>> SampleList;
    
    memset(fromDut, 0x00, sizeof(fromDut));
    memset(int_output, 0, sizeof(int_output));
    memset(average, 0, sizeof(average));
    memset(sigma, 0, sizeof(sigma));
    
    // TODO: read multiple output types with one function call?
    switch(output_type)
    {
    case SENSE_ACCEL:
    {
        ASICregister reg[MAX_NUM_AXES] = {this->REG_ACCEL_OUT_X, 
                                      this->REG_ACCEL_OUT_Y,
                                      this->REG_ACCEL_OUT_Z};
        
        // The action happens here
        for (int sample = 0; sample < count; sample++)
        {
            for (int dim = X; dim < MAX_NUM_AXES; dim++)
            {
                // get a sample
                this->GetRegister(reg[dim], &fromDut[dim][0][0], listDut);
                
                // convert
                reg[dim].ConvertToInt(&fromDut[dim][0][0], &int_output[sample][dim][0], listDut);
                
                // sum for average
                for (int d = 0; listDut[d] != 0; d++)
                {
                    dut = listDut[d] - 1;
                    average[dim][dut] += (double)(int_output[sample][dim][dut]);
                }
            }
        }
        
        // average
        
        
        // standard deviation
        
        
        // median
        
        
        // store results
        // TODO: do this properly
        //int dataSize2D[APP_MAX_ARRAY_DIM + 1] = {3, TOOL_MAX_DUT, 0, 0};
        //ARR2D(&int_output[0][0], int_type, dataSize2D, avg, listDut);
        break;
    }
    case SENSE_GYRO:
        // TODO: implement
        
        // throw non-critical error
        ERRChk(ERROR_UNIMPLEMENTED, "Gyro output_type", "CSenseBase::Sample", false);
        break;

    case SENSE_MAG:
        // TODO: implement
        
        // throw non-critical error
        ERRChk(ERROR_UNIMPLEMENTED, "Mag output_type", "CSenseBase::Sample", false);
        break;

    default:
        // throw non-critical error
        ERRChk(ERROR_ASIC_SPECIFIC, "Invalid output_type chosen when attempting to Sample", "CSenseBase::Sample", false);
        break;
    }
}

/******************************************************************************
    Name:   ReadOutputs
    Desc:   Read & convert device outputs using Tool
******************************************************************************/
/* this will likely get moved to Comm
void CSenseBase::ReadOutputs(ASICregister reg, int output_type)
{
    DBGTrace("--> CSenseBase::ReadOutputs");
    
    // TODO: Decide how we will write this
    
    byte* raw = NULL;
    double* out = NULL;
    word* listDut = NULL;
    this->Decode(raw, out, listDut);
}
*/
/******************************************************************************
    Name:   Decode
    Desc:   Converts output register values to counts
******************************************************************************/
/* this will likely get moved to Comm
void CSenseBase::Decode(byte* raw, double* out, word* listDut)
{
    DBGTrace("--> CSenseBase::Decode");
    
    // TODO: Decide how we will write this
}
*/