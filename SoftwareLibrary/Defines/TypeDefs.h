/******************************************************************************

    File:   TypeDefs.h
    Desc:   Defines some basic defines for C++ types and unions

******************************************************************************/
#ifndef _TYPE_DEFS_H_
#define _TYPE_DEFS_H_

//-----------------------------------------------------------------------------
// type definitons
typedef unsigned char       byte;
typedef unsigned short      word;
typedef unsigned int        uint;
typedef unsigned long       dword;
typedef unsigned long long  qword;

#include "MyString.h"

//-----------------------------------------------------------------------------
// range info
struct RangeInfo
{
    word Id;
    
    char Start[APP_MAX_CHAR];
    char Stop[APP_MAX_CHAR];
};

//-----------------------------------------------------------------------------
// pattern info
struct PatternInfo
{
    char Name[APP_MAX_CHAR];
    
    RangeInfo ListGet[64];
    word SizeGet;
    word PinGet;
    
    RangeInfo ListSet[64];
    word SizeSet;
    word PinSet;
    
    //bool sadModified;           // ran an experiment, not using (yet)
};

//-----------------------------------------------------------------------------
// variable types
enum variable_type {
    bool_type,
    byte_type,
    int_type,
    double_type,
    uint_type,
    word_type,
    dword_type,
    qword_type,
    scope_type      // WARNING: This is really a byte type with a special purpose. Make sure you only pass in bytes
};

//-----------------------------------------------------------------------------
// memory types
enum mtype {
    Volatile,
    RAM,
    ROM
};

//-----------------------------------------------------------------------------
// conversion types
enum contype {
    convert_bit,
    convert_byte,
    convert_uint,
    convert_SN,
    twos_comp,
    twos_comp_invert_neg,
    twos_comp_invert_pos,
    sense_output,
    MAX_CONTYPE                         // add new entries above this
};

//-----------------------------------------------------------------------------
// tool types
enum tool_type {
    ismeca_type,
    spea_type,
    tesec_type,
    ni_type
};

//-----------------------------------------------------------------------------
// Comparison type specifcation for binning
// inclusive means that passing parts may have a value of min or max
// exclusive means that passing parts may not have a value of min or max
enum comparison_type {
    bin_disabled,
    two_sided_inclusive_limit,
    two_sided_exclusive_limit,
    min_equals_limit,
    one_sided_min_inclusive_limit,
    one_sided_min_exclusive_limit,
    one_sided_max_inclusive_limit,
    one_sided_max_exclusive_limit
};

//-----------------------------------------------------------------------------
// unions
typedef union
{
    byte    byte[4];
    word    word[2];
    dword   dword;

} union32;

typedef union
{
    byte    byte[8];
    word    word[4];
    dword   dword[2];
    qword   qword;

} union64;

//-----------------------------------------------------------------------------
// storage of string of memory for each DUT
typedef struct MEMstring
{
    string Raw[TOOL_MAX_DUT];
    
    // default constructor
    MEMstring(void) : Raw() {}
    
    // construct with only a single RAM image (special case?)
    MEMstring(string MEMstr, int dut) { Raw[dut] = MEMstr; }
    
    // construct with multiple RAM images
    MEMstring(string* MEMstr, word* listDut)
    {
        int dut;
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            Raw[dut] = MEMstr[dut];
        }
    }
    
    // copy constructor
    MEMstring(MEMstring const& copy) { *this = copy; }
    
    // assignment operator
    MEMstring& operator=(MEMstring const& copy)
    {
        for (int d = 0; d < TOOL_MAX_DUT; d++)
        {
            Raw[d] = copy.Raw[d];
        }
        return *this;
    }
    
    // Add a string for a single DUT
    void Add(string MEMstr, int dut) { Raw[dut] = MEMstr; }
    
    // Empty the Raw array
    void Clear(void) { *this = MEMstring(); }
} MEMstring;

//-----------------------------------------------------------------------------
// Container for the transportation of converted RAM values primarily between
// the ASIC and Test modules, and secondarily between PTCBase and ASIC module
typedef struct RAMrailway
{
    map<string, vector<int>> transit_map;
    MEMstring cargo;
    
    // default constructor
    RAMrailway(void) : transit_map(), cargo() {}
    
    // construct with only a single RAM string (special case?)
    RAMrailway(string RAMstr, int dut) : cargo(MEMstring(RAMstr, dut)) {}
    
    // construct with only an array of RAM strings
    RAMrailway(string* RAMstr, word* listDut)
    {
        cargo = MEMstring(RAMstr, listDut);
    }
    
    // construct with only a single key and no values
    RAMrailway(string key) { Add(key); }
    
    // construct with only multiple keys
    RAMrailway(string* keys, int num_keys) { Add(keys, num_keys); }
    
    // construct with a single key and values for each DUT
    RAMrailway(string key, int* values, word* listDut)
    {
        Add(key, values, listDut);
    }
    
    // construct with multiple keys with values for a single DUT (special case?)
    RAMrailway(string* keys, int* values, int num_keys, int dut)
    {
        Add(keys, values, num_keys, dut);
    }
    
    // construct with multiple keys with values for each DUT
    RAMrailway(string* keys, int* values, int num_keys, word* listDut)
    {
        Add(keys, values, num_keys, listDut);
    }
    
    // construct with array of RAM strings, multiple keys, and values for each DUT
    RAMrailway(string* RAMstr, string* keys, int* values, int num_keys, word* listDut)
    {
        int dut;
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            
            cargo.Add(RAMstr[dut], dut);
            Add(keys, values, num_keys, dut);
        }
    }
    
    // copy constructor
    RAMrailway(RAMrailway const& copy) { *this = copy; }
    
    // assignment operator
    RAMrailway& operator=(RAMrailway const& copy)
    {
        transit_map = copy.transit_map;
        cargo = copy.cargo;
        return *this;
    }
    
    // Add a single key with no values to map
    void Add(string key) { transit_map[key]; }
    
    // Add multiple keys with no values to map
    void Add(string* keys, int num_keys)
    {
        for (int i = 0; i < num_keys; i++)
            transit_map[ keys[i] ];
    }
    
    // Add/update a single key with a value for each DUT
    void Add(string key, int* values, word* listDut)
    {
        int dut;
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            transit_map[key].push_back(values[dut]);
        }
    }
    
    // Add/update multiple keys with values for a single DUT
    void Add(string* keys, int* values, int num_keys, int dut)
    {
        // TODO: Call Clear() here? Possibly throw a warning?
        
        int index;
        for (int i = 0; i < num_keys; i++)
        {
            index = (i * TOOL_MAX_DUT) + dut;
            transit_map[ keys[i] ].push_back(values[index]);
        }
    }
    
    // Add/update multiple keys with values for each DUT
    void Add(string* keys, int* values, int num_keys, word* listDut)
    {
        int dut, index;
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            for (int i = 0; i < num_keys; i++)
            {
                index = (i * TOOL_MAX_DUT) + dut;
                transit_map[ keys[i] ].push_back(values[index]);
            }
        }
    }
    
    // Add RAM string for a particular DUT
    void AddRAM(string RAMstr, int dut) { cargo.Add(RAMstr, dut); }
    
    // Add RAM string for all DUTs
    void AddRAM(string* RAMstr, word* listDut)
    {
        int dut;
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            cargo.Add(RAMstr[dut], dut);
        }
    }
    
    // Add RAM string, multiple keys, and values for each DUT
    void AddAll(string* RAMstr, string* keys, int* values, int num_keys, word* listDut)
    {
        int dut;
        for (int d = 0; listDut[d] != 0; d++)
        {
            AddRAM(RAMstr[dut], dut);
            Add(keys, values, num_keys, dut);
        }
    }
    
    // Store vector into output array for all keys
    void Get(int* output, word* listDut)
    {
        int dut;//, index;
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            
            // TODO: this
        }
    }
    
    // Clears out everything except the keys
    void Clear(void)
    {
        transit_map.clear();
        cargo.Clear();
    }
} RAMrailway;


#endif