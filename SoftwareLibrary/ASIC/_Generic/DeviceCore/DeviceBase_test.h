/******************************************************************************

    File:   DeviceBase_test.h
    Desc:   DeviceBase_test is a header file to contain functionality of a
            FakeMemory so we can pretend to talk to a part with no hardware

******************************************************************************/
#ifndef _DEVICE_BASE_TEST_H_
#define _DEVICE_BASE_TEST_H_

#include "DeviceBase.h"

#ifdef _USE_FAKE_MEMORY_
    byte CDeviceBase::FakeMemory[NUM_PAGES][MAX_PAGE_SIZE][TOOL_MAX_DUT];
    
    /******************************************************************************
        Name:   GetByte
        Desc:   Read from device registers in FakeMemory
    ******************************************************************************/
    void CDeviceBase::FakeGetByte(byte page, byte reg_loc, int count, byte* values, word* listDut, string label)
    {
        int dut;
        if (page == 255)
            page = 0;
        for (int a = 0; a < count; a++)
        {
            for (int d = 0; listDut[d] != 0; d++)
            {
                dut = listDut[d] - 1;
                values[(a * TOOL_MAX_DUT) + dut] = FakeMemory[page][reg_loc + a][dut];
            }
        }
    }
    
    /******************************************************************************
        Name:   FakeSetByte
        Desc:   Write to device registers in FakeMemory
    ******************************************************************************/
    void CDeviceBase::FakeSetByte(byte page, byte reg_loc, int count, byte* values, word* listDut, string label)
    {
        int dut;
        if (page == 255)
            page = 0;
        for (int a = 0; a < count; a++)
        {
            for (int d = 0; listDut[d] != 0; d++)
            {
                dut = listDut[d] - 1;
                FakeMemory[page][reg_loc + a][dut] = values[(a * TOOL_MAX_DUT) + dut];
            }
        }
    }
#endif


#endif