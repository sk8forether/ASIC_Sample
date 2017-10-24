/******************************************************************************

    File:   RegisterTypeDefs.h
    Desc:   Defines the structs used for each register in the ASIC, and the RAM
            struct used in GetRAM.

******************************************************************************/
#ifndef _REGISTER_TYPE_DEFS_H_
#define _REGISTER_TYPE_DEFS_H_

#include "Defines.h"

//-----------------------------------------------------------------------------
// conversion types
typedef struct convert_type
{
    contype type;
    string name;
    
    convert_type(void) {}
    
    convert_type(contype type_in) : type(type_in)
    {
        string names[MAX_CONTYPE];
        SetNames(names);
        name = names[type_in];
    }
    
    void SetNames(string *names)
    {
        names[convert_bit] = STR(bit);
        names[convert_byte] = STR(byte);
        names[convert_uint] = STR(unsigned int);
        names[convert_SN] = STR(serial number);
        names[twos_comp] = "two's compliment";
        names[twos_comp_invert_neg] = "two's compliment with negatives inverted";
        names[twos_comp_invert_pos] = "two's compliment with positives inverted";
        names[sense_output] = STR(sense element output);
    }
    
    bool operator==(const convert_type& c) const
    {
        return (type == c.type);
    }
} convert_type;

//-----------------------------------------------------------------------------
//  memory types
typedef struct memory_type
{
    mtype type;
    string name;
    
    memory_type(void) {}
    
    memory_type(mtype type_in) : type(type_in)
    {
        string names[ROM + 1];
        SetNames(names);
        name = names[type_in];
    }
    
    void SetNames(string *names)
    { 
        names[Volatile] = STR(Volatile);
        names[RAM] = STR(RAM);
        names[ROM] = STR(ROM);
    }
    
    bool operator!=(const memory_type& m) const
    {
        return type != m.type;
    }
    
    bool operator==(const memory_type& m) const
    {
        return type == m.type;
    }
} memory_type;

//-----------------------------------------------------------------------------
//  register struct
typedef struct ASICregister
{
    string          name;                   // for use in debug
    byte            page;                   // all addr in reg are on same page
    byte            addr[APP_MAX_ADDR];     // can have multiple addresses
    byte            mask[APP_MAX_ADDR];     // each addr has a mask
    byte            shift[APP_MAX_ADDR];    // # of 0 bits below data in mask
    byte            bitcount[APP_MAX_ADDR]; // bits of usable info in each byte
    convert_type    conversion;             // how to read back reg as int
    memory_type     memory;                 // Volatile, RAM, or ROM
    byte            num_registers;          // total bytes in reg
    byte            lsb;                    // least significant bit addr index
    byte            msb;                    // same style as lsb
    
    // default contructor (empty)
    ASICregister(void)
    {
        memset(addr, ADDR_INVALID, sizeof(addr));
        memset(mask, NULL, sizeof(mask));
        memset(shift, NULL, sizeof(shift));
        memset(bitcount, NULL, sizeof(bitcount));
        lsb = ADDR_INVALID;
        msb = ADDR_INVALID;
    }
    
    // construct with single addr and mask
    // NOTE: for a single byte OR multiple consecutive registers
    // for multiple, set num_in to total, addr_in to starting address, and mask to 0xFF
    ASICregister(String name_in, byte page_in, byte addr_in, byte mask_in,
        convert_type conv_in, memory_type mem_in, int num_in)
        : name(name_in), page(page_in), conversion(conv_in), memory(mem_in),
        num_registers(num_in)
    {
        memset(addr, ADDR_INVALID, sizeof(addr));
        memset(mask, NULL, sizeof(mask));
        memset(shift, NULL, sizeof(shift));
        memset(bitcount, NULL, sizeof(bitcount));
        lsb = ADDR_INVALID;
        msb = ADDR_INVALID;
        
        addr[0] = addr_in;
        mask[0] = mask_in;
        
        if ((num_registers == 1) && (mask[0] == 0xFF))
            bitcount[0] = 8;
        else if (mask[0] != NULL)
        {
            int b;
            
            // calculate shift
            b = mask[0];
            for (shift[0] = 0; (b & 1) == 0; b >>= 1)
            {
                shift[0]++;
                
                if (shift[0] >= 8)
                    break;
            }
            
            // calculate bitcount
            // http://graphics.stanford.edu/~seander/bithacks.html#CountBitsSetNaive
            if (conversion == convert_bit)
                bitcount[0] = 1;
            else
            {
                b = mask[0];
                for (bitcount[0] = 0; b; b >>= 1)
                    bitcount[0] += b & 1;
                
                // not supported for large #s of consecutive registers
                if (num_registers <= APP_MAX_ADDR)
                {
                    // same bitcount for every byte
                    for (int a = 1; a < num_registers; a++)
                        bitcount[a] += bitcount[0];
                }
            }
        }
        else
        {
            String msg;
            sprintf(msg, "ASICregister %s setup with NULL mask", name);
            ERRChk(ERROR_ASIC_SPECIFIC, msg);
        }
    }
    
    // construct with multiple addrs and masks
    // NOTE: num_in is *intended* to be the total bytes or conversion will not work
    ASICregister(String name_in, byte page_in, byte* addr_in, byte* mask_in,
        convert_type conv_in, memory_type mem_in, int num_in)
        : name(name_in), page(page_in), conversion(conv_in), memory(mem_in),
        num_registers(num_in)
    {
        memset(addr, ADDR_INVALID, sizeof(addr));
        memset(mask, NULL, sizeof(mask));
        memset(shift, NULL, sizeof(shift));
        memset(bitcount, NULL, sizeof(bitcount));
        lsb = ADDR_INVALID;
        msb = ADDR_INVALID;
        
        int b;
        
        for (int a = 0; a < num_registers; a++)
        {
            addr[a] = addr_in[a];
            mask[a] = mask_in[a];
            
            if (mask[a] == 0xFF)
                bitcount[a] = 8;
            else if (mask[a] != NULL)
            {
                // calculate shift
                b = mask[a];
                for (shift[a] = 0; (b & 1) == 0; b >>= 1)
                {
                    shift[a]++;
                    
                    if (shift[a] >= 8)
                        break;
                }
                
                // calculate bitcount
                b = mask[a];
                for (bitcount[a] = 0; b; b >>= 1)
                    bitcount[a] += b & 1;
            }
        }
    }
    
    // construct with multiple addrs and masks, including a lsb and msb
    // NOTE: This is *intended* to be used for cases with > 1 byte for a single
    // value, but currently only supports 2 bytes total (see Convert functions)
    ASICregister(String name_in, byte page_in, byte* addr_in, byte* mask_in,
        convert_type conv_in, memory_type mem_in, int lsb_in, int msb_in)
        : name(name_in), page(page_in), conversion(conv_in), memory(mem_in),
        lsb(lsb_in), msb(msb_in)
    {
        memset(addr, ADDR_INVALID, sizeof(addr));
        memset(mask, NULL, sizeof(mask));
        memset(shift, NULL, sizeof(shift));
        memset(bitcount, NULL, sizeof(bitcount));
        
        // only 2 registers are supported
        num_registers = 2;
        
        // if 2 consecutive bytes, explicitly define addr[1] as next byte
        addr[0] = addr_in[0];
        addr[1] = ((addr_in[1] != ADDR_INVALID) ? addr_in[1] : addr_in[0] + 1);
        
        // if 2 consecutive bytes, explicitly define mask[1] = mask[0]
        mask[0] = mask_in[0];
        mask[1] = ((mask_in[1] != NULL) ? mask_in[1] : mask[0]);
        
        int b;
        
        for (int a = 0; a < num_registers; a++)
        {
            if (mask[a] == 0xFF)
                bitcount[a] = 8;
            else if (mask[a] != NULL)
            {
                // calculate shift
                b = mask[a];
                for (shift[a] = 0; (b & 1) == 0; b >>= 1)
                {
                    shift[a]++;
                    
                    if (shift[a] >= 8)
                        break;
                }
                
                // calculate bitcount
                b = mask[a];
                for (bitcount[a] = 0; b; b >>= 1)
                    bitcount[a] += b & 1;
            }
            else
            {
                String msg;
                sprintf(msg, "ASICregister %s setup with NULL mask", name);
                ERRChk(ERROR_ASIC_SPECIFIC, msg);
            }
        }
    }
    
    // Obtain the actual value of the register by masking out unwanted bits and
    // shifting the value to the bottom of the byte(s)
    void MaskAndShift(byte* output, word* listDut)
    {
        if (DBGVerify >= ENG_LEVEL_3)
            DBGPrint("MaskAndShift");
        
        String msg;
        int index, dut;
        
        for (int a = 0; a < num_registers; a++)
        {
            if ((addr[a] != ADDR_INVALID) && (mask[a] != 0xFF))
            {
                for (int d = 0; listDut[d] != 0; d++)
                {
                    dut = listDut[d] - 1;
                    index = (a * TOOL_MAX_DUT) + dut;
                    
                    if (DBGVerify >= ENG_LEVEL_3)
                    {
                        DBGPrint("");
                        sprintf(msg,"        byte: %s", (char *) CUtilities::DisplayBinary(output[index],8));
                        DBGPrint(msg);
                        sprintf(msg,"        mask: %s", (char *) CUtilities::DisplayBinary(mask[a],8));
                        DBGPrint(msg);
                    }
                    
                    // mask
                    output[index] &= mask[a];
                    
                    if (DBGVerify >= ENG_LEVEL_3)
                    {
                        sprintf(msg,"masked byte : %s", (char *) CUtilities::DisplayBinary(output[index],8));
                        DBGPrint(msg);
                    }
                    
                    // shift
                    output[index] >>= shift[a];
                    
                    if (DBGVerify >= ENG_LEVEL_3)
                    {
                        sprintf(msg,"  byte >> %i : %s", shift[a], (char *) CUtilities::DisplayBinary(output[index],8));
                        DBGPrint(msg);
                    }
                    
                }
            }
        }
    }
    
    // Restore the raw value of the register by masking out changes to bits that are
    // not supposed to change and shifting the value back to it's original byte(s)
    void ShiftAndMask(byte* original, byte* input, word* listDut)
    {
        if (DBGVerify >= ENG_LEVEL_3)
            DBGPrint("ShiftAndMask");
        
        String msg;
        int index, dut;
        byte notmask = NULL;
        
        for (int a = 0; a < num_registers; a++)
        {
            if ((addr[a] != ADDR_INVALID) && (mask[a] != 0xFF))
            {
                for (int d = 0; listDut[d] != 0; d++)
                {
                    dut = listDut[d] - 1;
                    index = (a * TOOL_MAX_DUT) + dut;
                    
                    if (DBGVerify >= ENG_LEVEL_3)
                    {
                        DBGPrint("");
                        sprintf(msg,"         byte: %s", (char *) CUtilities::DisplayBinary(input[index],8));
                        DBGPrint(msg);
                        sprintf(msg,"         mask: %s", (char *) CUtilities::DisplayBinary(mask[a],8));
                        DBGPrint(msg);
                    }
                    
                    // shift
                    input[index] <<= shift[a];
                    
                    if (DBGVerify >= ENG_LEVEL_3)
                    {
                        sprintf(msg,"   byte << %i : %s", shift[a], (char *) CUtilities::DisplayBinary(input[index],8));
                        DBGPrint(msg);
                    }
                    
                    // check for erroneous input
                    if ((input[index] & mask[a]) != input[index])
                    {
                        String msg;
                        sprintf(msg, "ASICregister %s received incorrect input %i", name, input[index]);
                        ERRChk(ERROR_ASIC_SPECIFIC, msg);
                    }
                    
                    // mask
                    notmask = ~mask[a];
                    input[index] = ( (input[index] & mask[a]) | (original[index] & notmask));
                    
                    if (DBGVerify >= ENG_LEVEL_3)
                    {
                        sprintf(msg,"  masked     : %s", (char *) CUtilities::DisplayBinary((input[index] & mask[a]),8));
                        DBGPrint(msg);
                        
                        sprintf(msg,"  masked orig: %s", (char *) CUtilities::DisplayBinary((original[index] & notmask),8));
                        DBGPrint(msg);
                    }
                    
                    // combine original and new information
                    input[index] = ((input[index] & mask[a]) | (original[index] & notmask));
                    
                    if (DBGVerify >= ENG_LEVEL_3)
                    {
                        //sprintf(msg,"combined byte: %s \t\t\t\t\t %02X", (char *) CUtilities::DisplayBinary(input[index],8), input[index]);
                        sprintf(msg,"combined byte: %s\n", (char *) CUtilities::DisplayBinary(input[index],8));
                        DBGPrint(msg);
                    }
                }
            }
        }
    }
    
    // convert byte values from register into ints
    void ConvertToInt(byte* values, int* output, word* listDut)
    {
        int dut, index;
        int temp = 0;
        int midpoint = 0;
        int max = 0;
        word total;
        String msg;
        bool wrong_setup = false;
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            index = ((0 * TOOL_MAX_DUT) + dut);
            
            switch(conversion.type)
            {
            case convert_bit:
            case convert_byte:
                temp = (int)(values[index]);
                
                if (num_registers > 1)
                    wrong_setup = true;
                
                break;
                
            case convert_uint:
                if (num_registers == 1)
                    temp = (int)(values[index]);
                else if (num_registers == 2)
                {
                    // combine 2 bytes
                    total = CombineBytes(values, dut);
                    
                    // cast as int
                    temp = (int)total;
                }
                else
                    wrong_setup = true;
                
                break;
                
            case convert_SN:
                sprintf(msg, "ASICregister currently doesn't support %s", conversion.name.c_str());
                ERRChk(ERROR_UNIMPLEMENTED, msg);
                
                // TODO: decide on implementation of this
                
                break;
                
            case twos_comp:
                BitCalcs(&midpoint, &max);
                
                if (num_registers == 1)
                    total = (word)values[index];
                else if (num_registers == 2)
                    total = CombineBytes(values, dut);
                else
                {
                    wrong_setup = true;
                    break;
                }
                
                // two's compliment and cast as int
                temp = (int)( (total <= midpoint) ? total : (total - max) );
                
                break;
                
            case twos_comp_invert_neg:
                BitCalcs(&midpoint, &max);
                
                if (num_registers == 1)
                    total = (word)values[index];
                else if (num_registers == 2)
                    total = CombineBytes(values, dut);
                else
                {
                    wrong_setup = true;
                    break;
                }
                
                // Nathan's weird shift
                total = ( (total == midpoint) ? (total - 1) : total );
                
                // Nathan's two's compliment and cast as int
                temp = (int)( (total < midpoint) ? (total + 1) : ((midpoint + 1) - total) );
                
                break;
                
            case twos_comp_invert_pos:
                sprintf(msg, "ASICregister currently doesn't support %s", conversion.name.c_str());
                ERRChk(ERROR_UNIMPLEMENTED, msg);
                break;
                
            case sense_output:
                if (num_registers == 2)
                    total = CombineBytes(values, dut);
                else
                {
                    wrong_setup = true;
                    break;
                }
                
                BitCalcs(&midpoint, &max);
                
                // two's comp
                temp = (int)( (total <= midpoint) ? total : (total - max) );
                
#ifdef _UNSIGNED_OUTPUTS_
                // subtract the mid for unsigned outputs
                temp -= midpoint;
#endif
                break;
                
            default:
                ERRChk(ERROR_UNDEFINED, "No such conversion-type exists.", "ASICregister.ConvertToInt", YES);
                break;
            }
            
            if (wrong_setup)
            {
                sprintf(msg, "ASICregister %s is incorrectly setup with %i bytes and %s", name, num_registers, conversion.name.c_str());
                ERRChk(ERROR_ASIC_SPECIFIC, msg, "ASICregister.ConvertToInt", YES);
            }
            
            output[index] = temp;
        }
    }
    
    // convert int values to set to a register into bytes
    void ConvertFromInt(int* values, byte* output, word* listDut)
    {
        int dut, index, midpoint, max, toconvert;
        byte temp[APP_MAX_ADDR];
        String msg;
        bool wrong_setup = false;
        
        memset(temp, 0x00, sizeof(temp));
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            index = ((0 * TOOL_MAX_DUT) + dut);
            
            switch(conversion.type)
            {
            case convert_bit:
            case convert_byte:
                temp[0] = (byte)(values[dut]);
                
                if (num_registers > 1)
                    wrong_setup = true;
                
                break;
                
            case convert_uint:
                if (num_registers == 1)
                    temp[0] = (byte)(values[index]);
                else if (num_registers == 2)
                {
                    // separate into 2 bytes (cast values as word)
                    SeparateBytes((word)values[dut], &temp[0]);
                }
                else
                    wrong_setup = true;
                
                break;
                
            case convert_SN:
                sprintf(msg, "ASICregister currently doesn't support %s", conversion.name.c_str());
                ERRChk(ERROR_UNIMPLEMENTED, msg);
                
                // TODO: decide on implementation of this
                
                break;
                
            case twos_comp:
                BitCalcs(&midpoint, &max);
                
                toconvert = ((values[index] >= 0) ? values[index] : (values[index] + max));
                
                if (num_registers == 1)
                    temp[0] = (byte)(toconvert);
                else if (num_registers == 2)
                {
                    // separate into 2 bytes (cast values as word)
                    SeparateBytes((word)toconvert, &temp[0]);
                }
                else
                    wrong_setup = true;
                
                break;
                
            /* don't need to undo Nathan's weird shift since midpoint is mapped
               to midpoint - 1 and there's no going back...*/
            case twos_comp_invert_neg:
                BitCalcs(&midpoint, &max);
                
                toconvert = ((values[index] > 0) ? (values[index] - 1) : ((midpoint + 1) - values[index]));
                
                if (num_registers == 1)
                    temp[0] = (byte)(toconvert);
                else if (num_registers == 2)
                {
                    // separate into 2 bytes (cast values as word)
                    SeparateBytes((word)toconvert, &temp[0]);
                }
                else
                    wrong_setup = true;
                
                break;
                
            case twos_comp_invert_pos:
                sprintf(msg, "ASICregister currently doesn't support %s", conversion.name.c_str());
                ERRChk(ERROR_UNIMPLEMENTED, msg);
                break;
                
            case sense_output:
                sprintf(msg, "You should never try to set a register of %s", conversion.name.c_str());
                ERRChk(ERROR_UNDEFINED, msg, "ASICregister.ConvertFromInt", YES);
                break;
                
            default:
                ERRChk(ERROR_UNDEFINED, "No such conversion-type exists.");
                break;
            }
            
            if (wrong_setup)
            {
                sprintf(msg, "ASICregister %s is incorrectly setup with %i bytes and %s", name, num_registers, conversion.name.c_str());
                ERRChk(ERROR_ASIC_SPECIFIC, msg, "ASICregister.ConvertFromInt", YES);
            }
            
            for (int i = 0; i < num_registers; i++)
            {
                index = (i * TOOL_MAX_DUT) + dut;
                output[index] = temp[i];
            }
        }
    }
    
    // shifts highbyte up and combines 2 bytes (assume MaskAndShift already called)
    word CombineBytes(byte* values, int dut)
    {
        String msg;
        byte lowbyte, highbyte;
        word combined;
        
        // both the msb and the lsb have valid numbers
        if( (lsb != ADDR_INVALID) && (msb != ADDR_INVALID) )
        {
            lowbyte = values[((lsb * TOOL_MAX_DUT) + dut)];
            highbyte = values[((msb * TOOL_MAX_DUT) + dut)];
            
            // shift and combine
            combined = ((highbyte << bitcount[lsb]) | lowbyte );
        }
        // assume 1st byte is the lowbyte and the 2nd byte is the highbyte
        else
        {
            lowbyte = values[((0 * TOOL_MAX_DUT) + dut)];
            highbyte = values[((1 * TOOL_MAX_DUT) + dut)];
            
            // shift and combine
            combined = ((highbyte << bitcount[0]) | lowbyte );
        }
        
        if (DBGVerify >= ENG_LEVEL_3)
        {
            sprintf(msg, "low  : 0x%02X", lowbyte);
            for (int i = 0; i < (28); i++)
                strcat(msg," ");
            sprintf(msg, "%s%s", (char *) msg, (char *) CUtilities::LongToBinaryString(lowbyte, 8));
            DBGPrint(msg);
            sprintf(msg, "high : 0x%02X", highbyte);
            for (int i = 0; i < (28-bitcount[lsb]); i++)
                strcat(msg," ");
            sprintf(msg, "%s%s", (char *) msg, (char *) CUtilities::LongToBinaryString(highbyte, 8));
            DBGPrint(msg);
                    
            sprintf(msg,"combined number:        0x%04X(%s), decimal: %i", 
                combined, (char *) CUtilities::LongToBinaryString(combined, 16), (int)combined);
            DBGPrint(msg);
        }
        
        return combined;
    }
    
    // separates 2 bytes and shifts highbyte back down (assume ShiftAndMask already called)
    void SeparateBytes(word input, byte* valuesOut)
    {
        String msg;
        byte lowbyte, highbyte;
        int lowmask;  // max_value - 1, from BitCalcs
        
        // both the msb and the lsb have valid numbers
        if( (lsb != ADDR_INVALID) && (msb != ADDR_INVALID) )
        {
            lowmask = ((1 << bitcount[lsb]) - 1);
            lowbyte = input & lowmask;
            highbyte = input >> bitcount[lsb];
            
            valuesOut[lsb] = lowbyte; 
            valuesOut[msb] = highbyte;
        }
        // assume 1st byte is the lowbyte and the 2nd byte is the highbyte
        else
        {
            lowmask = ((1 << bitcount[0]) - 1);
            lowbyte = input & lowmask;
            highbyte = input >> bitcount[0];
            
            valuesOut[0] = lowbyte;
            valuesOut[1] = highbyte;
        }
        
        if (DBGVerify >= ENG_LEVEL_3)
        {
            sprintf(msg,"         number: %s, decimal: %i", 
                (char *) CUtilities::DisplayBinary(input,16, (char *) "0x%04X(%s)"), input);
            DBGPrint(msg);
            
            sprintf(msg, "Separate using mask:      %s", (char *) CUtilities::DisplayBinary(lowmask,8));
            DBGPrint(msg);
            
            sprintf(msg, "low  : 0x%02X", lowbyte);
            strcat(msg, (char *) CUtilities::AddChar(' ',21));
            sprintf(msg, "%s%s", (char *) msg, (char *) CUtilities::LongToBinaryString(lowbyte, 8));
            DBGPrint(msg);
            sprintf(msg, "high : 0x%02X", highbyte);
            strcat(msg, (char *) CUtilities::AddChar(' ',21-bitcount[lsb]));
            sprintf(msg, "%s%s", (char *) msg, (char *) CUtilities::LongToBinaryString(highbyte, 8));
            DBGPrint(msg);
            DBGPrint("");
        }
    }
    
    // Calculate the minimum and maximum values that can be set to a register
    void Range(int* pmin, int* pmax)
    {
        String msg;
        int min = 0;
        int max = 0;
        int midpoint = 0;
        int calc_max = 0;
        
        switch(conversion.type)
        {
        case convert_bit:
            if (num_registers == 1)
            {
                min = LOW;
                max = HIGH;
            }
            break;
            
        case convert_byte:
            if (num_registers == 1)
            {
                min = 0;
                max = XtoY(2,TotalBits()) - 1;
            }
            break;
            
        case convert_uint:
            if ((num_registers == 1) || (num_registers == 2))
            {
                min = 0;
                max = XtoY(2,TotalBits()) - 1;
            }
            break;
            
        case twos_comp:
            if ((num_registers == 1) || (num_registers == 2))
            {
                BitCalcs(&midpoint, &calc_max);
                min = (-1)*(midpoint + 1);
                max = midpoint;
            }
            break;
            
        case twos_comp_invert_neg:
            if ((num_registers == 1) || (num_registers == 2))
            {
                BitCalcs(&midpoint, &calc_max);
                min = (-1)*(midpoint);
                max = midpoint;
            }
            break;
            
        case sense_output:
            if (num_registers == 2)
            {
                BitCalcs(&midpoint, &calc_max);
                min = (-1)*(midpoint + 1);
                max = midpoint;
            }
#ifdef _UNSIGNED_OUTPUTS_
            // subtract the mid for unsigned outputs
            min -= midpoint;
            max -= midpoint;
#endif
            break;
            
        case convert_SN:
        case twos_comp_invert_pos:
        default:
            sprintf(msg, "Cannot provide a range for %s due to conversion-type.", name);
            ERRChk(ERROR_UNDEFINED, msg);
            
            break;
        }
        
        // catch when min and max did not get set properly
        if (min >= max)
        {
            sprintf(msg, "Incorrect range of (%i, %i) for %s since it is setup with %i byte(s) and %s.", min, max, name.c_str(), num_registers, conversion.name.c_str());
            ERRChk(ERROR_ASIC_SPECIFIC, msg, "ASICregister.Range", YES);
        }
        
        *pmin = min;
        *pmax = max;
    }
    
    // total bitcount of register
    int TotalBits(void)
    {
        int total_bitcount = 0;
        
        for (int a = 0; a < num_registers; a++)
            total_bitcount += bitcount[a];
        
        return total_bitcount;
    }
    
    // calculate middle and max values based on ASICregister size (# of bytes)
    void BitCalcs(int* midpoint, int* max_value)
    {
        int total_bitcount = TotalBits();
        
        // place where two's comp splits into +/- (ex: 1 byte midpoint = 127)
        // de-reference pointer
        *midpoint = (1 << (total_bitcount - 1)) - 1;
        
        *max_value = (1 << total_bitcount);
    }
    
    // compare registers based on address
    bool operator<(const ASICregister& a) const
    {
        if (page == a.page)
            return addr[0] < a.addr[0];
        else
            return page < a.page;
    }
    bool operator>(const ASICregister& a) const
    {
        if (page == a.page)
            return addr[0] > a.addr[0];
        else
            return page > a.page;
    }
    bool operator==(const ASICregister& a) const
    {
        if (page != a.page)
            return false;
        if (memory != a.memory)
            return false;
        
        for (int i = 0; i < APP_MAX_ADDR; i++)
        {
            if ( (addr[i] != a.addr[i]) || (mask[i] != a.mask[i]) )
                return false;
        }
        
        // if everything up to this point matches, they are the same
        return true;
    }
    
    // print all information on a register
    void Print(void)
    {
        char msg[APP_MAX_CHAR_LONGER];
        String temp;
        
        sprintf(msg, "Register: %s\n", name);
        
        #ifdef _HAS_PAGES_
            sprintf(temp, "Page%02X\n", page);
        #else
            sprintf(temp, "");
        #endif
        strcat(msg, temp);
        
        for (int i = 0; addr[i] != ADDR_INVALID; i++)
        {
            sprintf(temp, "Addr%i: %02X  Mask: %02X\n", i, addr[i], mask[i]);
            strcat_s(msg, APP_MAX_CHAR_LONGER, temp);
        }
        
        sprintf(temp, "Read as: %s\n", conversion.name.c_str() );
        strcat(msg, temp);
        
        sprintf(temp, "Memory Type: %s\n", memory.name.c_str());
        strcat(msg, temp);
        
        sprintf(temp, "Number of bytes: %i\n", num_registers);
        strcat(msg, temp);
        
        // always display
        bool was_on = DBGVerboseEnabled;
        DBGVerboseEnabled = YES;
        DBGVerbose(msg);
        DBGVerboseEnabled = was_on;
    }
} ASICregister;

//-----------------------------------------------------------------------------
//  register comparison
struct find_address : unary_function<ASICregister, bool>
{
    byte addr;
    find_address(byte addr) : addr(addr) {}
    bool operator() (const ASICregister& reg) const
    {
        for (int a = 0; a < APP_MAX_ADDR; a++)
        {
            if (addr == reg.addr[a])
                return true;
        }
        return false;
    }
};

struct find_name : unary_function<ASICregister, bool>
{
    string name;
    find_name(string name) : name(name) {}
    bool operator() (const ASICregister& reg) const
    {
        if (name == reg.name.c_str())
            return true;
        else
            return false;
    }
};

//-----------------------------------------------------------------------------
//  RAM register struct
typedef struct RAMstruct
{
    vector<ASICregister> RAMvector;  // contains all RAM registers in ASIC
    int count;                       // number of bytes in RAM registers
    
    RAMstruct(void) { count = 0; }
    
    int Size(void) { return count; }
    
    void Add(ASICregister reg)
    {
        vector<ASICregister>::iterator it;
        
        // add register to vector if it's not already in it
        if (find(RAMvector.begin(), RAMvector.end(), reg) != RAMvector.end())
            ERRWarn("Tried to Add an ASICregister to RAMstruct more than once.");
        else
        {
            // keep track of the number of bytes in RAM
            count += reg.num_registers;
            for (int i = 0; i < APP_MAX_ADDR; i++)
            {
                if (reg.addr[i] != ADDR_INVALID)
                {
                    it = find_if(RAMvector.begin(), RAMvector.end(), find_address(reg.addr[i]));
                    if (it != RAMvector.end())
                        count--;
                }
            }
            
            RAMvector.push_back(reg);
        }
        
        // keep vector sorted by address (low to high)
        sort(RAMvector.begin(), RAMvector.end());
    }
    
    // ensure it contains all RAM registers and values
    void Check(void)
    {
        if (Size() != NUM_RAM_REG)
        {
            String msg;
            Print();
            sprintf(msg, "RAMstruct is not the proper size! (%i != %i)", Size(), NUM_RAM_REG);
            
            if (DBGTestModule)
                ERRChk(ERROR_ASIC_SPECIFIC, msg);
            else
                ERRCrit(ERROR_ASIC_SPECIFIC, msg);
        }
        
        if (RAMvector.size() != NUM_RAM_VALUES)
        {
            String msg;
            Print();
            sprintf(msg, "RAMvector is not the proper size! (%i != %i)", RAMvector.size(), NUM_RAM_VALUES);
            
            if (DBGTestModule)
                ERRChk(ERROR_ASIC_SPECIFIC, msg);
            else
                ERRCrit(ERROR_ASIC_SPECIFIC, msg);
        }
    }
    
    // prints all RAM register names, page, and addresses in vector for debug
    void Print(void)
    {
        char msg[APP_MAX_CHAR_LONGER];
        String temp;
        vector<ASICregister>::iterator it;
        
        sprintf(msg, "\nRAM Registers:");
        
        #ifdef _HAS_PAGES_
            sprintf(temp, " Page%02X\n", RAMvector.begin()->page);
        #else
            sprintf(temp, "\n");
        #endif
        strcat(msg, temp);
        
        for (it = RAMvector.begin(); it != RAMvector.end(); ++it)
        {
            sprintf(temp, "Register %02X: %s\n", it->addr[0], it->name.c_str());
            strcat_s(msg, APP_MAX_CHAR_LONGER, temp);
        }
        
        // always display
        bool was_on = DBGVerboseEnabled;
        DBGVerboseEnabled = YES;
        DBGVerbose(msg);
        DBGVerboseEnabled = was_on;
    }
    
    String ToString(int range_low, int range_high)
    {
        if (range_high - range_low > APP_MAX_REG_PRINT)
        {
            ERRChk(ERROR_ASIC_SPECIFIC, "Cannot print that many registers with this ToString function.");
            return "";
        }
        if ( range_low >= (int)RAMvector.size() )
        {
            ERRChk(ERROR_ASIC_SPECIFIC, "low input is passed end of range. Remember, not every ASICregister is only 1 byte.");
            return "";
        }
        if ( range_high > (int)RAMvector.size() )
        {
            ERRChk(ERROR_ASIC_SPECIFIC, "high input is passed end of range. Remember not every ASICregister is only 1 byte.");
            return "";
        }
        
        String temp, msg;
        vector<ASICregister>::iterator it = RAMvector.begin() + range_low;
        vector<ASICregister>::iterator it2 = RAMvector.begin() + range_high;
        
        sprintf(msg, "\nRAM (%i,%i):", range_low, range_high);
        
        #ifdef _HAS_PAGES_
            sprintf(temp, " Page%02X\n", RAMvector.begin()->page);
        #else
            sprintf(temp, "\n");
        #endif
        strcat(msg, temp);
        
        for (; it != it2; it++)
        {
            if (it > RAMvector.end())
            {
                sprintf(temp, "ERROR: surpassed end of range.");
                strcat(msg, temp);
                return msg;
            }
            sprintf(temp, "%s\n", it->name);
            msg += temp;
        }
        
        return msg;
    }
    
    // prints all information for each RAM register in vector for debug
    void PrintAll(void)
    {
        vector<ASICregister>::iterator it;
        for (it = RAMvector.begin(); it != RAMvector.end(); ++it)
        {
            it->Print();
        }
    }
} RAMstruct;

//-----------------------------------------------------------------------------
//  Default struct
//  Used for storing, comparing, and displaying the Default RAM image and mask
typedef struct Default
{
    byte            SpecImage[NUM_RAM_REG];
    byte            Mask[NUM_RAM_REG];
    
    // default contructor
    Default(void)
    {
        memset(SpecImage, 0x00, sizeof(SpecImage));
        memset(Mask, 0x00, sizeof(Mask));
    }
    
    // construct with strings from Spec
    Default(char* default_in, char* mask_in)
    {
        S2B(default_in, SpecImage, sizeof(SpecImage));
        S2B(mask_in, Mask, sizeof(Mask));
    }
    
    void Compare(byte* Raw, byte* difference, bool* result, word* listDut)
    {
        int dut, index;
        byte diff[NUM_RAM_REG][TOOL_MAX_DUT];
        
        memset(diff, 0x00, sizeof(diff));
        
        // Note: memset will not set a boolean array to true correctly
        // If the Spec image doesn't match for at least one byte, result will be false for that dut
        for (int i = 0; i < TOOL_MAX_DUT; i++)
            result[i] = true;
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            for (int i = 0; i < NUM_RAM_REG; i++)
            {
                index = (i * TOOL_MAX_DUT) + dut;
                if ( Raw[index] != SpecImage[i] )
                {
                    result[dut] = false;
                    diff[i][dut] = (Raw[index] ^ SpecImage[i]);
                }
            }
        }
        memcpy(difference, diff, sizeof(diff));
    }
    
    void CompareMasked(byte* Raw, byte* difference, bool* result, word* listDut)
    {
        int dut, index;
        byte diff[NUM_RAM_REG][TOOL_MAX_DUT];
        
        memset(diff, 0x00, sizeof(diff));
        
        for (int i = 0; i < TOOL_MAX_DUT; i++)
            result[i] = true;
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            for (int i = 0; i < NUM_RAM_REG; i++)
            {
                index = (i * TOOL_MAX_DUT) + dut;
                if ( (Raw[index] & Mask[i]) != (SpecImage[i]& Mask[i]) )
                {
                    result[dut] = false;
                    diff[i][dut] = (Raw[index] ^ SpecImage[i]);
                }
            }
        }
        memcpy(difference, diff, sizeof(diff));
    }
    
    void Print(void)
    {
        // always display
        bool was_on = DBGVerboseEnabled;
        DBGVerboseEnabled = YES;
        
        // print SpecImage and Mask
        String msg;
        sprintf(msg, "\nDefaultImage: %s", (char*)B2S(SpecImage, NUM_RAM_REG));
        DBGVerbose(msg);
        sprintf(msg, "DefaultMask:  %s\n", (char*)B2S(Mask, NUM_RAM_REG));
        DBGVerbose(msg);
        
        // restore setting
        DBGVerboseEnabled = was_on;
    }
} Default;

//-----------------------------------------------------------------------------
//  Image struct
//  Used for storing, comparing, and displaying RAM and ROM images
typedef struct Image
{
    byte            Raw[NUM_RAM_REG][TOOL_MAX_DUT];
    int             Converted[NUM_RAM_VALUES][TOOL_MAX_DUT];
    memory_type     memory;
    byte            page;
    
    Image(void) {}
    
    Image(mtype mem_in, byte page_in) : memory(mem_in), page(page_in)
    {
        memset(Raw, 0x00, sizeof(Raw));
        memset(Converted, 0, sizeof(Converted));
    }
    
    Image(mtype mem_in, byte page_in, byte* raw_in) : memory(mem_in), page(page_in)
    {
        memcpy(Raw, raw_in, sizeof(Raw));
        memset(Converted, 0, sizeof(Converted));
    }
    
    void SetRaw(byte* raw_in)
    {
        memcpy(Raw, raw_in, sizeof(Raw));
    }
    
    // convert raw register values from byte to int using RAMstruct
    void Convert(int* output, RAMstruct RAMregisters, word* listDut)
    {
        // TODO: finish
        
        // for loop over RAMValues.strarray
        //for (int r = 0; r < RAMValues.count; r++)
        //{
            // use find_name to find a register based on name
            //if (find(RAMvector.begin(), RAMvector.end(), reg) != RAMvector.end())
            //RAMValues.intarray[r] = RAMregisters.RAMvector[r].ConvertToInt()
        //}
        
        // Need a version on B2S or B2SArray (using C++ string) that makes all RAM 1 string
        //RAMValues.raw_str = //B2S(&Raw[0][0], NUM_RAM_REG)
    }
    
    // compare array to array of bytes
    void Compare(byte* otherRaw, byte* difference, bool* result, word* listDut)
    {
        int dut, index;
        String temp[TOOL_MAX_DUT];
        char msg[APP_MAX_CHAR_LONGER];
        byte diff[NUM_RAM_REG][TOOL_MAX_DUT];
        
        memset(diff, 0x00, sizeof(diff));
        
        for (int i = 0; i < TOOL_MAX_DUT; i++)
            result[i] = true;
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            for (int i = 0; i < NUM_RAM_REG; i++)
            {
                index = (i * TOOL_MAX_DUT) + dut;
                if (Raw[i][dut] != otherRaw[index])
                {
                    result[dut] = false;
                    diff[i][dut] = (Raw[i][dut] ^ otherRaw[index]);
                }
            }
            
            B2SArray(&temp[0], &diff[0][0], NUM_RAM_REG, listDut);
            
            // print difference
            if (result[dut] == false)
            {
                sprintf_s(msg, APP_MAX_CHAR_LONGER, "\n%sImage diff array [%i]: ", memory.name.c_str(), dut);
                strcat_s(msg, APP_MAX_CHAR_LONGER, temp[dut]);
                DBGVerbose(msg);
            }
        }
        
        if (difference != NULL)
            memcpy(difference, diff, sizeof(diff));
    }
    
    // compare images
    void Compare(Image img, byte* difference, bool* result, word* listDut)
    {
        int dut;
        String temp[TOOL_MAX_DUT];
        char msg[APP_MAX_CHAR_LONGER];
        byte diff[NUM_RAM_REG][TOOL_MAX_DUT];
        
        memset(diff, 0x00, sizeof(diff));
        
        for (int i = 0; i < TOOL_MAX_DUT; i++)
            result[i] = true;
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            for (int i = 0; i < NUM_RAM_REG; i++)
            {
                if (Raw[i][dut] != img.Raw[i][dut])
                {
                    result[dut] = false;
                    diff[i][dut] = (Raw[i][dut] ^ img.Raw[i][dut]);
                }
            }
            
            B2SArray(&temp[0], &diff[0][0], NUM_RAM_REG, listDut);
            
            // print difference
            if (result[dut] == false)
            {
                sprintf_s(msg, APP_MAX_CHAR_LONGER, "\n%sImage diff %sImage [%i]: ", memory.name.c_str(), img.memory.name.c_str(), dut);
                strcat_s(msg, APP_MAX_CHAR_LONGER, temp[dut]);
                DBGVerbose(msg);
            }
        }
        
        if (difference != NULL)
            memcpy(difference, diff, sizeof(diff));
    }
    
    void CompareDefault(Default DefaultImg, byte* difference, bool* result, word* listDut)
    {
        byte diff[NUM_RAM_REG][TOOL_MAX_DUT];
        memset(diff, 0x00, sizeof(diff));
        
        DefaultImg.Compare(&Raw[0][0], &diff[0][0], result, listDut);
        
        // debug output of img difference from default img
        if (DBGVerboseEnabled)
        {
            int dut;
            String temp[TOOL_MAX_DUT];
            char msg[APP_MAX_CHAR_LONGER] = "";
            
            B2SArray(&temp[0], &diff[0][0], NUM_RAM_REG, listDut);
            
            for (int d = 0; listDut[d] != 0; d++)
            {
                dut = listDut[d] - 1;
                
                if (result[dut] == false)
                {
                    sprintf_s(msg, APP_MAX_CHAR_LONGER, "\n%sImage diff DefaultImage [%i]: ", memory.name.c_str(), dut);
                    strcat_s(msg, APP_MAX_CHAR_LONGER, temp[dut]);
                    DBGVerbose(msg);
                }
            }
        }
        
        if (difference != NULL)
            memcpy(difference, diff, sizeof(diff));
    }
    
    void CompareMaskedDefault(Default DefaultImg, byte* difference, bool* result, word* listDut)
    {
        int dut;
        String temp[TOOL_MAX_DUT];
        char msg[APP_MAX_CHAR_LONGER];
        
        byte diff[NUM_RAM_REG][TOOL_MAX_DUT];
        memset(diff, 0x00, sizeof(diff));
        
        DefaultImg.CompareMasked(&Raw[0][0], &diff[0][0], result, listDut);
        
        B2SArray(&temp[0], &diff[0][0], NUM_RAM_REG, listDut);
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            
            if (result[dut] == false)
            {
                sprintf_s(msg, APP_MAX_CHAR_LONGER, "\nMasked %sImage diff Masked DefaultImage [%i]: ", memory.name.c_str(), dut);
                strcat_s(msg, APP_MAX_CHAR_LONGER, temp[dut]);
                DBGVerbose(msg);
            }
        }
        
        if (difference != NULL)
            memcpy(difference, diff, sizeof(diff));
    }
    
    // print raw image for 1 DUT
    void Print(int dut)
    {
        String msg;
        String output = B2S(&Raw[0][dut], NUM_RAM_REG);
        sprintf(msg, "\n%sImage[%i]: %s", memory.name.c_str(), dut, (char*)output);
        
        // always display
        bool was_on = DBGVerboseEnabled;
        DBGVerboseEnabled = YES;
        DBGVerbose(msg);
        DBGVerboseEnabled = was_on;
    }
    
    // print raw image for all DUTs
    void Print(word* listDut)
    {
        int dut;
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            Print(dut);
        }
    }
    
    // print converted values of RAM registers for 1 DUT
    void PrintConverted(int dut)
    {
        String msg, temp;
        
        // always display
        bool was_on = DBGVerboseEnabled;
        DBGVerboseEnabled = YES;
        
        sprintf(msg, "\n%sImage[%i]: ", memory.name.c_str(), dut);
        
        for (int i; i < NUM_RAM_REG; i++)
        {
            sprintf(temp, "%i", Converted[i][dut]);
            strcat(msg, temp);
        }
        
        DBGVerbose(msg);
        
        DBGVerboseEnabled = was_on;
    }
    
    // print out converted values of RAM registers for all DUTs
    void PrintConverted(word* listDut)
    {
        int dut;
        String msg, temp;
        
        // always display
        bool was_on = DBGVerboseEnabled;
        DBGVerboseEnabled = YES;
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            
            sprintf(msg, "\n%sImage[%i]: ", memory.name.c_str(), dut);
            
            for (int i = 0; i < NUM_RAM_REG; i++)
            {
                sprintf(temp, "%i", Converted[i][dut]);
                strcat(msg, temp);
            }
            
            DBGVerbose(msg);
        }
        
        DBGVerboseEnabled = was_on;
    }
    
    void View(int dut)
    {
        CTestbench::LVImageView(B2S(&Raw[0][dut], NUM_RAM_REG),page);
    }
    
    void View(Image Img, int dut)
    {
        CTestbench::LVImageView(B2S(&Raw[0][dut], NUM_RAM_REG), B2S(&Img.Raw[0][dut], NUM_RAM_REG), page);
    }
} Image;

//-----------------------------------------------------------------------------
//  Volatile Image struct
//  Used for storing, comparing, and displaying RAM, ROM, and Volatile images
typedef struct VolImage
{
    byte            Raw[MAX_PAGE_SIZE][TOOL_MAX_DUT];
    memory_type     memory;
    byte            page;              // needs to be defined for LV front panel
    int             count;             // number of registers in image
    
    VolImage(void)
    {
        page = PAGE_00;
        memory = Volatile;
        memset(Raw, 0x00, sizeof(Raw));
        count = 0;
    }
    
    VolImage(byte page_in) : page(page_in)
    {
        memory = Volatile;
        memset(Raw, 0x00, sizeof(Raw));
        count = 0;
    }
    
    VolImage(byte page_in, int size) : page(page_in), count(size)
    {
        memory = Volatile;
        memset(Raw, 0x00, sizeof(Raw));
    }
    
    VolImage(byte page_in, byte* array_in, int size) : page(page_in), count(size)
    {
        memory = Volatile;
        memcpy(Raw, array_in, sizeof(Raw));
    }
    
    void SetRaw(byte* raw_in, int size)
    {
        memcpy(Raw, raw_in, sizeof(Raw));
        count = size;
    }
    
    void Compare(VolImage img, byte* difference, bool* result, word* listDut)
    {
        int dut;
        String temp[TOOL_MAX_DUT];
        char msg[APP_MAX_CHAR_LONGER];
        byte diff[MAX_PAGE_SIZE][TOOL_MAX_DUT];
        
        memset(diff, 0x00, sizeof(diff));
        
        for (int i = 0; i < TOOL_MAX_DUT; i++)
            result[i] = true;
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            for (int i = 0; i < count; i++)
            {
                if (Raw[i][dut] != img.Raw[i][dut])
                {
                    result[dut] = false;
                    diff[i][dut] = (Raw[i][dut] ^ img.Raw[i][dut]);
                }
            }
            
            B2SArray(&temp[0], &diff[0][0], count, listDut);
            
            // print difference
            if (result[dut] == false)
            {
                sprintf_s(msg, APP_MAX_CHAR_LONGER, "\n%sImage diff %sImage [%i]: ", memory.name.c_str(), img.memory.name.c_str(), dut);
                strcat_s(msg, APP_MAX_CHAR_LONGER, temp[dut]);
                DBGVerbose(msg);
            }
        }
        
        if (difference != NULL)
            memcpy(difference, diff, sizeof(diff));
    }
    
    // print raw image for 1 DUT
    void Print(int dut)
    {
        String msg;
        String output = B2S(&Raw[0][dut], count);
        sprintf(msg, "\n%sVolatile Image[%i]: %s", memory.name.c_str(), dut, (char*)output);
        
        // always display
        bool was_on = DBGVerboseEnabled;
        DBGVerboseEnabled = YES;
        DBGVerbose(msg);
        DBGVerboseEnabled = was_on;
    }
    
    // print raw image for all DUTs
    void Print(word* listDut)
    {
        int dut;
        
        for (int d = 0; listDut[d] != 0; d++)
        {
            dut = listDut[d] - 1;
            Print(dut);
        }
    }
    
    void View(int dut)
    {
        CTestbench::LVImageView(B2S(&Raw[0][dut], count),page);
    }
    
    void View(Image Img, int dut)
    {
        CTestbench::LVImageView(B2S(&Raw[0][dut], count), B2S(&Img.Raw[0][dut], count), page);
    }
} VolImage;

#endif
