/******************************************************************************

    File:   RegisterMap.h
    Desc:   Contains declarations for most common registers of any Kionix ASIC.
            These registers are then defined inside the unique ASIC class for
            each specific ASIC.  New or special registers may also be declared
            and/or defined in the class for the unique ASIC.  The static
            registers are those that are initially defined in SEregisters.h
            because they are used by a Sense Element class (Accel, Gyro, etc.).

******************************************************************************/
#ifndef _REGISTER_MAP_H_
#define _REGISTER_MAP_H_

protected:
    //-------------------------------------------------------------------------
    // volatile memory
    ASICregister RawVOLATILE;       /* contains first Volatile register address
                                       and number of Volatile registers      */
#if NUM_VOL_PAGES > 1
    ASICregister RawVOLATILE_2;     /* extra page of Volatile registers      */
#endif
    
#ifdef _HAS_PAGES_
    static ASICregister REG_MEMPAGE;    /* sets page of memory to access     */
#endif
    
    ASICregister REG_TEMP_OUT;      /* temperature outputs                   */
    ASICregister REG_RESP;          /* read from this register for the 
                                       response test (COTR/STR)              */
    
    // common settings registers
    ASICregister REG_CNTL1;         /* control settings 1                    */
    ASICregister REG_CNTL2;         /* control settings 2                    */
    ASICregister REG_CNTL3;         /* control settings 3                    */
    ASICregister REG_CNTL4;         /* control settings 4                    */
    ASICregister REG_CNTL5;         /* control settings 5                    */
    ASICregister REG_PC1;           /* controls the operating mode:
                                       0 = standby, 1 = operating            */
    ASICregister REG_RES;           /* enabels full power mode               */
    ASICregister REG_COTC;          /* setting this bit to 1 sets 0xAA to
                                       REG_RESP until read (reverts to 0x55
                                       after read)                           */
    ASICregister REG_SRST;          /* soft reset performs POR routine:
                                       0 = no action, 1 = start POR routine
                                       NOTE: mask is used to do sw reset     */
    ASICregister REG_INC1;          /* interrupt control 1                   */
    ASICregister REG_INC2;          /* interrupt control 2                   */
    ASICregister REG_INC3;          /* interrupt control 3                   */
    ASICregister REG_INC4;          /* interrupt control 4                   */
    ASICregister REG_SELFTEST;      /* selftest initiation (requires key)    */
    byte KEY_SELFTEST;              /* key to turn on selftest               */
    ASICregister REG_OSC_OUT;       /* oscillator output on int pin (key)    */
    byte KEY_OSC_OUT;               /* key to turn on oscillator out         */
    ASICregister REG_WDT_OUT;       /* watchdog timer output on int pin (key)*/
    byte KEY_WDT_OUT;               /* key to turn on watchdog timer out     */
    ASICregister REG_MR;            /* operating and test mode control:
                                       0x11 for differential redundant read
                                       mode, 0x00 to write OTP               */
    ASICregister REG_OTPAC;         /* OTP access control                    */
    ASICregister REG_MUXC;          /* MUX control                           */
    
    // other settings registers
    ASICregister REG_TSC;           /* tilt position state timer             */
    ASICregister REG_EXTRA_MIRROR;  /* extra register mirror                 */
    ASICregister REG_TM_DTB;        /* test mode digital test bus            */
    ASICregister REG_STDBY;         /* standby mode                          */
    ASICregister REG_STNULL;        /* selftest null mode
                                       enable = 1, disable = 0               */
    ASICregister REG_STPOL;         /* selftest polarity
                                       positive = 1, negative = 0            */
    
#ifdef _OTP_
    ASICregister REG_AMEM2;         /* access memory 2 (requires key)        */
    ASICregister REG_AMEM1;         /* access memory 1 (requires key)        */
    byte KEY_AMEM2;                 /* key to access memory 2                */
    byte KEY_AMEM1;                 /* key to access memory 1                */
    byte SINGLE_ENDED;              /* value to change the OTP into to
                                       single-ended read mode                */
    byte REDUNDANT;                 /* value to change the OTP into to
                                       redundant read mode                   */
#elif defined(_EEPROM_)
    ASICregister REG_PRG_EN;        /* enable writing to non-volatile memory */
    ASICregister REG_NVM_EN;        /* non-volatile memory burn controls     */
#endif
    
#ifdef _HAS_BUFFER_
    ASICregister REG_BUF_CNTL1;     /* buffer control register 1             */
    ASICregister REG_BUF_CNTL2;     /* buffer control register 2             */
    ASICregister REG_BUF_STATUS1;   /* buffer status 1                       */
    ASICregister REG_BIST_CNTL;     /* RAM BIST control                      */
#endif
    
#ifdef _TEST_MODE_
    ASICregister REG_TM_ATB;        /* analog test bus                       */
    ASICregister REG_MISC1;         /* misc test mode register 1             */
    ASICregister REG_MISC2;         /* misc test mode register 2             */
    ASICregister REG_TM_OSC_LOW;    /* enable low frequency osc trim         */
    ASICregister REG_TM_BIAS;       /* enable bias current (and avdd_bias)   */
#endif
    
    //-------------------------------------------------------------------------
    // RAM registers
    ASICregister RawRAM;            /* contains first RAM register address
                                       and number of RAM registers           */
    ASICregister REG_WAI;           /* who am I register                     */
    ASICregister REG_EXTRA;         /* extra register (EXB)                  */
    ASICregister REG_PART_EN;       /* part enabled in full-power mode if 1  */
    ASICregister REG_OSC;           /* oscillator trim code (high frequency) */
    ASICregister REG_OSC_LF;        /* oscillator trim code (low frequency)  */
    ASICregister REG_WTSEL;         /* sensor warmup time                    */
    ASICregister REG_WDT;           /* watchdog timer trim code              */
    ASICregister REG_BGT;           /* bandgap trim code (regulator)         */
    ASICregister REG_BGCH;          /* bandgap chopping for lower noise:
                                       enable = 1, disable = 0               */
    ASICregister REG_AFE_RES;       /* analog front end resistor control     */
    ASICregister REG_SN;            /* serial number registers               */
    ASICregister REG_LL;            /* low limit threshold for tilt postion
                                       detection                             */
    ASICregister REG_HL;            /* high limit threshold for tilt postion
                                       detection                             */
    ASICregister REG_TPGC;          /* tilt position gain control            */
    ASICregister REG_LP_CNTL;       /* low power control                     */
    ASICregister REG_XCAL;          /* x offset calibration                  */
    ASICregister REG_YCAL;          /* y offset calibration                  */
    ASICregister REG_ZCAL;          /* z offset calibration                  */
    ASICregister REG_REV;           /* reserved register for any purpose     */
    ASICregister REG_REV_1;         /* spare register 1 for analog           */
    ASICregister REG_REV_2;         /* spare register 2 for analog           */
    ASICregister REG_WUFC;          /* wake-up-function (WUF) timer          */
    ASICregister REG_WUFTH;         /* WUF threshold                         */
    ASICregister REG_BTSC;          /* back-to-sleep (BTS) timer             */
    ASICregister REG_BTSTH;         /* BTS threshold                         */
    ASICregister REG_BTSTH_WUFTH;   /* extra BTS and WUF threshold bits      */
    
#ifdef _I2C_SLAVE_CONFIG_
    ASICregister REG_SAD1_6;        /* I2C slave address bits 1-6; mask =0x7E*/
#elif !defined(_NO_SAD1_BIT_)
    ASICregister REG_SAD1;          /* I2C slave address bit 1 (of 0-6)      */
#endif
    
#ifdef _HAS_SPARE_
    ASICregister REG_SPARE_1;        /* usually contains part-specific info  */
    ASICregister REG_SPARE_2;        /* spare register                       */
    ASICregister REG_SPARE_3;        /* spare register                       */
    ASICregister REG_SPARE_4;        /* spare register                       */
    ASICregister REG_SPARE_5;        /* spare register                       */
    ASICregister REG_SPARE_6;        /* spare register                       */
    ASICregister REG_SPARE_7;        /* spare register                       */
    ASICregister REG_SPARE_8;        /* spare register                       */
    ASICregister REG_SPARE_9;        /* spare register                       */
    ASICregister REG_SPARE_10;       /* spare register                       */
#endif
    
    //-------------------------------------------------------------------------
    // Sense Element registers (some RAM and some Volatile)
    // NOTE: Registers that will be used by Sense Elements need to be declared
    //       here and defined with default constructor in SEregisters.h
#ifdef _ACCEL_ENABLED_
    static ASICregister REG_ACCEL_OUT_X;   /* accel output for x-axis        */
    static ASICregister REG_ACCEL_OUT_Y;   /* accel output for y-axis        */
    static ASICregister REG_ACCEL_OUT_Z;   /* accel output for z-axis        */
    static ASICregister REG_ACCEL_ODR;     /* accel output data rate (ODCNTL)*/
    static ASICregister REG_ACCEL_CGX;     /* accel coarse gain code for x   */
    static ASICregister REG_ACCEL_CGY;     /* accel coarse gain code for y   */
    static ASICregister REG_ACCEL_CGZ;     /* accel coarse gain code for z   */
    static ASICregister REG_ACCEL_FGX;     /* accel fine gain code for x     */
    static ASICregister REG_ACCEL_FGY;     /* accel fine gain code for y     */
    static ASICregister REG_ACCEL_FGZ;     /* accel fine gain code for z     */
    static ASICregister REG_ACCEL_FOX;     /* accel final offset code for x  */
    static ASICregister REG_ACCEL_FOY;     /* accel final offset code for y  */
    static ASICregister REG_ACCEL_FOZ;     /* accel final offset code for z  */
    static ASICregister REG_ACCEL_DOX;     /* accel dig offset code for x    */
    static ASICregister REG_ACCEL_DOY;     /* accel dig offset code for y    */
    static ASICregister REG_ACCEL_DOZ;     /* accel dig offset code for z    */
    static ASICregister REG_ACCEL_TCX;     /* accel temp comp code for x     */
    static ASICregister REG_ACCEL_TCY;     /* accel temp comp code for y     */
    static ASICregister REG_ACCEL_TCZ;     /* accel temp comp code for z     */
    static ASICregister REG_ACCEL_STX;     /* accel selftest codes for x     */
    static ASICregister REG_ACCEL_STY;     /* accel selftest codes for y     */
    static ASICregister REG_ACCEL_STZ;     /* accel selftest codes for z     */
    static ASICregister REG_ACCEL_STX_POS; /* + accel selftest codes for x   */
    static ASICregister REG_ACCEL_STY_POS; /* + accel selftest codes for y   */
    static ASICregister REG_ACCEL_STZ_POS; /* + accel selftest codes for z   */
    static ASICregister REG_ACCEL_STX_NEG; /* - accel selftest codes for x   */
    static ASICregister REG_ACCEL_STY_NEG; /* - accel selftest codes for y   */
    static ASICregister REG_ACCEL_STZ_NEG; /* - accel selftest codes for z   */
    #ifdef _ONLY_1_POL_BIT_
        static ASICregister REG_ACCEL_POL; /* accel polarity for x, y, and z */
    #else
        static ASICregister REG_ACCEL_POLX;    /* accel polarity for x       */
        static ASICregister REG_ACCEL_POLY;    /* accel polarity for y       */
        static ASICregister REG_ACCEL_POLZ;    /* accel polarity for z       */
    #endif
    
    // uncommon
    static ASICregister REG_ACCEL_CNTL;    /* accel control                  */
    static ASICregister REG_ACCEL_SWAP;    /* accel axis swap                */
    static ASICregister REG_ACCEL_DIS;     /* accel disable bit: accel not
                                              disabled = 0, disabled = 1     */
    static ASICregister REG_ACCEL_TC_EN;   /* accel temp comp enabled        */
    static ASICregister REG_SSB;           /* sensor substrate bias          */
    static ASICregister REG_STNF;          /* controls selftest for no-flip;
                                              ST enabled = 1, ST polarity 
                                              toggled & ST set normally = 0  */
    static ASICregister REG_I2C_MODE;      /* selects between I2C fast and high sp*/
    static ASICregister REG_TCEN;          /* temperature comp enable bit    */
    static ASICregister REG_S_MODE;        /* selects between DC and AC scan */
    static ASICregister REG_ATH;           /* active threshold               */
#endif
    
    //-------------------------------------------------------------------------
    // ROM registers
    ASICregister RawROM;            /* contains 1st ROM register address (OTP
                                       or EEPROM) and number of ROM registers*/
    ASICregister REG_SN_ROM;        /* serial number registers in ROM        */
    
    //-------------------------------------------------------------------------
    // TESTING
    static ASICregister REG_TEST;   /* special register for development      */

#endif