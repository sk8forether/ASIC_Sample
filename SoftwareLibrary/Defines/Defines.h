/******************************************************************************

    File:   Defines.h
    Desc:   Contains defines for a number of generic constants used throughout
            Kionix software

******************************************************************************/
#ifndef _DEFINES_H_
#define _DEFINES_H_

#include <windows.h>
#include <process.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <functional>
#include <map>
using namespace std; 

#include "ToolInfo.h"
#include "TypeDefs.h"
#include "GlobalFunctions.h"
#include "DefineForEveryASIC.h"
#include "ProductTypeDefs.h"
#include "ErrorCodes.h"
#include "States.h"

#include "Testbench.h"
#include "Utilities.h"

//-----------------------------------------------------------------------------
// application modes
#define DISABLED                        0
#define ENABLED                         1
//#define _LV_COMM_                                     // Enable LVInterpreter
//#define _USE_FAKE_MEMORY_                             // Enable fake memory
#define _EXTRA_CHECKS_ENABLED_                          // Enable extra checks

// application thresholds
#define APP_MAX_ADDR                    6               // currently arbitrary
#define APP_MAX_COL                     256             // used by Parser
#define APP_MAX_ARRAY_DIM               4               // for DBGArray
#define APP_MAX_REG_PRINT               15              // for ToString in RAM
#define APP_MAX_SAMPLES                 100             // for sense_output

// file i/o directory
#define FILEIO_READ                     0
#define FILEIO_WRITE                    1

//-----------------------------------------------------------------------------
// test flows
#define PROGRAMMING                     0
#define READONLY                        1
#define AUDIT                           2
#define TAPING                          3

// test algorithms
#define TEST_ALG_SWEEP                  0
#define TEST_ALG_BISECT                 1
#define TEST_ALG_INCREMENT              2

// test data array limits
#define MAX_PARAM_3D                    5
#define MAX_PARAM_2D                    6
#define MAX_PARAM_1D                    16

//-----------------------------------------------------------------------------
// communication protocols
#define COM_INVALID                     0x00
#define COM_I2C                         0x01            // bit 1
#define COM_I3C                         0x02            // bit 2
#define COM_SPI3                        0x04            // bit 3
#define COM_SPI4                        0x08            // bit 4

// communication methods
#define COM_METHOD_INVALID              0
#define COM_METHOD_PATT                 1
#define COM_METHOD_PLU                  2

// communication actions
#define READ                            0
#define WRITE                           1

// pattern speeds
#define PATT_LOW_SPEED                  0
#define PATT_HIGH_SPEED                 1
#define PATT_INVALID_SPEED              2

// pattern thresholds
#define PATT_MAX_NUM                    24
#define PATT_MAX_STEP                   400000

// pattern file
#define APP_PATH_IOHS                   ".\\PATT_HS"
//#define APP_PATH_IOHS                   "C:\\PATT_HS"

//-----------------------------------------------------------------------------
// pages and addressing
#define PAGE_00                         0x00
#define PAGE_01                         0x01
#define PAGE_02                         0x02
#define PAGE_INVALID                    0xFF
#define ADDR_INVALID                    0xFF

//-----------------------------------------------------------------------------
// sense element flags
#define SENSE_ACCEL                     0x01            // bit 1
#define SENSE_MAG                       0x02            // bit 2
#define SENSE_GYRO                      0x04            // bit 3
#define SENSE_PED                       0x08            // bit 4

//-----------------------------------------------------------------------------
// human-readable constants
#define X                               0
#define Y                               1
#define Z                               2

#define NEG                             0
#define POS                             1

#define NO                              0
#define YES                             1

#endif