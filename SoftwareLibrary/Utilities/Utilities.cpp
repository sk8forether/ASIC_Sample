/******************************************************************************

    File:   Utilities.cpp
    Desc:   Utilities is a component in Level 1 of the design that provides a
            useful suite of utilities for use within any other module.

******************************************************************************/
#include "Utilities.h"

/******************************************************************************
    Name:   CUtilities
    Desc:   Default constructor
******************************************************************************/
CUtilities::CUtilities(void)
{
}

/******************************************************************************
    Name:   ~CUtilities
    Desc:   Default destructor
******************************************************************************/
CUtilities::~CUtilities(void)
{
}

/******************************************************************************
    Name:   TimeStr
    Desc:   Returns the current time in a string format
******************************************************************************/
void CUtilities::GetTime(char* msg)
{
    DBGTrace("---> CUtilities::GetTime");
    
    time_t rawtime;
    struct tm timeinfo;
    
    time (&rawtime);
    localtime_s (&timeinfo, &rawtime);
    
    strftime (msg, APP_MAX_CHAR,"%c",&timeinfo);
}

/******************************************************************************
    Name:   ByteToString
    Desc:   Convert from a byte to a c-string
******************************************************************************/
String CUtilities::ByteToString(byte toConvert)
{
    DBGTrace("---> CUtilities::ByteToString");
    
    String bytestring;
    ToString(&toConvert, byte_type, "", bytestring);
    return bytestring;
}

/******************************************************************************
    Name:   ByteToString
    Desc:   Convert from a 1D byte array to a c-string
******************************************************************************/
String CUtilities::ByteToString(byte* toConvert, int length)
{
    DBGTrace("---> CUtilities::ByteToString (with length)");
    
    String temp, bytestring;
    for (int i = 0; i < length; i++)
    {
        temp = B2S(toConvert[i]);
        strcat(bytestring, temp);
    }
    
    return bytestring;
}

/******************************************************************************
    Name:   ByteToStringArray
    Desc:   Convert from a 2D byte array by listDut to a c-string array
******************************************************************************/
void CUtilities::ByteToStringArray(String* output, byte* toConvert, int length, word* listDut)
{
    int dut, index;
    String temp, bytestring;
    for(int d = 0; listDut[d] != 0; d++)
    {
        dut = listDut[d] - 1;
        for (int i = 0; i < length; i++)
        {
            index = (i * TOOL_MAX_DUT) + dut;
            temp = B2S(toConvert[index]);
            strcat(bytestring, temp);
        }
        
        output[dut] = bytestring;
        memset(bytestring, NULL, sizeof(bytestring));
    }
}

/******************************************************************************
    Name:   ByteToStringArray
    Desc:   Convert from a 2D byte array by listDut to a C++ string array
******************************************************************************/
void CUtilities::ByteToStringArray(string* output, byte* toConvert, int length, word* listDut)
{
    int dut, index;
    string temp, bytestring;
    for(int d = 0; listDut[d] != 0; d++)
    {
        dut = listDut[d] - 1;
        for (int i = 0; i < length; i++)
        {
            index = (i * TOOL_MAX_DUT) + dut;
            //temp = B2S(toConvert[index], NULL);
            temp = B2S(toConvert[index]);
            bytestring += temp;
        }
        
        output[dut] = bytestring;
        bytestring = "";
    }
}

/******************************************************************************
    Name:   StringToByte
    Desc:   Convert from a string to a 1D byte array
******************************************************************************/
void CUtilities::StringToByte(char* toConvert, byte* result, int max_size)
{
    DBGTrace("---> CUtilities::StringToByte");
    
    int value, position, temp;
    int hex_shift = 16;
    
    for (int i = 0; i < max_size; i++)
    {
        value = 0;
        
        for (int nybble = 0; nybble < 2; nybble++)
        {
            temp = 0;
            position = (i * 2) + nybble;
            
            switch(toConvert[position])
            {
            case '1': temp = 1; break;
            case '2': temp = 2; break;
            case '3': temp = 3; break;
            case '4': temp = 4; break;
            case '5': temp = 5; break;
            case '6': temp = 6; break;
            case '7': temp = 7; break;
            case '8': temp = 8; break;
            case '9': temp = 9; break;
            case 'A': temp = 10; break;
            case 'B': temp = 11; break;
            case 'C': temp = 12; break;
            case 'D': temp = 13; break;
            case 'E': temp = 14; break;
            case 'F': temp = 15; break;
            }
            
            // "shift" if it's the upper nybble
            value += ((nybble == 0) ? temp*hex_shift : temp);
        }
        
        result[i] = (byte)value;
    }
}

/******************************************************************************
    Name:   LongToBinaryString
    Desc:   Convert from a long to a string
******************************************************************************/
String CUtilities::LongToBinaryString(long toConvert, int length, bool useSpacing)
{
    DBGTrace("---> CUtilities::IntToBinaryString");
    
    String output = "";
    for (int b = length-1; b >= 0; b--)
    {
        sprintf(output, "%s%s", (char *) output, ((toConvert & (1<<b)) > 0) ? "1" : "0");
        if (b > 0)
        {
            if (useSpacing)
            {
                // insert a space every nibble
                if ((b % 4) == 0)
                    strcat(output, " ");
                
                // insert another space every byte
                if ((b % 8) == 0)
                    strcat(output, " ");
            }
        }
    }
    
    return output;
}

/******************************************************************************
    Name:   BinaryStringToLong
    Desc:   Convert from a string to a long
******************************************************************************/
long CUtilities::BinaryStringToLong(String toConvert, int length)
{
    DBGTrace("---> CUtilities::LongToBinaryString");
    
    long output = 0;
    char *str = (char *) toConvert;
    int index = 0;
    for (int b = 0; b < length; b++)
    {
        while (str[index] == ' ')
            index++;
        
        // ignore spaces in number, inserted to make it more human readable
        if (str[index] == '1')
            output += (1 << ((length-1)-b));
        
        index++;
    }
    
    return output;
}

/******************************************************************************
    Name:   ToString
    Desc:   Convert from a specified type to a string
******************************************************************************/
void CUtilities::ToString(void* value, variable_type type, char* formatIn, char* output, int convertFromDouble)
{
    DBGTrace("---> CUtilities::ToString");
    
    String format = "";
    
    sprintf(output, "");
    strcpy(format, formatIn);
    
    switch(type)
    {
        case bool_type:
            {
                if (strcmp(formatIn,"") == 0)
                    strcpy(format, "%6s%s");
                bool val = *((bool*) value);
                if (val == true)
                    sprintf(output, format, "", "T");
                else
                    sprintf(output, format, "", "F");
                break;
            }
        case byte_type:
            {
                if (strcmp(formatIn,"") == 0)
                    strcpy(format, "%02X");
                sprintf(output, (char *)format, *((byte*) value));
                
                if (convertFromDouble)
                {
                    double valueD = *((double*) value);
                    byte valueB = (byte) valueD;
                    sprintf(output, (char *)format, valueB);
                }
                break;
            }
        case int_type:
            {
                if (strcmp(formatIn,"") == 0)
                    strcpy(format, "%3i");
                sprintf(output, (char *)format, *((int*) value));
                break;
            }
        case uint_type:
            {
                if (strcmp(formatIn,"") == 0)
                    strcpy(format, "%3u");
                sprintf(output, (char *)format, *((uint*) value));
                break;
            }
        case double_type:
            {
                if (strcmp(formatIn,"") == 0)
                    strcpy(format, "%7.3f");
                sprintf(output, (char *)format, *((double*) value));
                break;
            }
        case word_type:
            {
                if (strcmp(formatIn,"") == 0)
                    strcpy(format, "%i");
                sprintf(output, (char *)format, *((word*) value));
                break;
            }
        case dword_type:
            {
                if (strcmp(formatIn,"") == 0)
                    strcpy(format, "%i");
                sprintf(output, (char *)format, *((dword*) value));
                break;
            }
        case qword_type:
            {
                if (strcmp(formatIn,"") == 0)
                    strcpy(format, "%i");
                sprintf(output, (char *)format, *((qword*) value));
                break;
            }
        case scope_type:
            {
                switch (  *((byte*) value)  )
                {
                    case 0:
                        {
                            sprintf(output, "|-  ");
                            break;
                        }
                    case 1:
                        {
                            sprintf(output, "  -|");
                            break;
                        }
                    default:
                        {
                            ERRChk(ERROR_UNDEFINED, "unknown symbol");
                            break;
                        }
                }
                break;
            }
        default:
            {
                sprintf(output, "Error: Unknown type: %i", type);
                break;
            }
    }

}

/******************************************************************************
    Name:   IndexIntoArray
    Desc:   Indexes into a void* array, containing data of a specifed type
******************************************************************************/
void CUtilities::IndexIntoArray(void* input, variable_type type, int index, int* output)
{
    DBGTrace("---> CUtilities::IndexIntoArray");
    
    switch(type)
    {
        case bool_type:
            {
                *output = (int) &((bool*) input)[index];
                
                /*int OutputVal;
                int* inputI = (int*) input;
                int* outputI = &inputI[index];
                outputVal = (int) outputI;
                *output = outputVal;*/
                break;
            }
        case int_type:
            {
                *output = (int) &((int*) input)[index];
                
                /*int OutputVal;
                int* inputI = (int*) input;
                int* outputI = &inputI[index];
                outputVal = (int) outputI;
                *output = outputVal;*/
                break;
            }
        case double_type:
            {
                *output = (int) &((double*) input)[index];
                
                /*int outputVal;
                double* inputD = (double*) input;
                double* outputD = &inputD[index];
                outputVal = (int) outputD;
                *output = outputVal;*/
                break;
            }
        case word_type:
            {
                *output = (int) &((word*) input)[index];
                
                /*int outputVal;
                word* inputD = (word*) input;
                word* outputD = &inputD[index];
                outputVal = (int) outputD;
                *output = outputVal;*/
                break;
            }
        case byte_type:
        case scope_type:
            {
                *output = (int) &((byte*) input)[index];
                
                /*double* inputD = (double*) input;
                double* outputD = &inputD[index];
                outputVal = (int) outputD;
                *output = outputVal;*/
                break;
            }
        default:
            {
                String msg;
                sprintf(msg, "Unimplemented type: %i", type);
                CUtilities::Error.Add(msg);
                break;
            }
    }
}

/******************************************************************************
    Name:   CopyArrayToDouble
    Desc:   Copies a void * array of the specified type into a double array
******************************************************************************/
void CUtilities::InitializeArray(word value, int* size, word* output)
{
    DBGTrace("---> CUtilities::InitializeArray");
    
    String msg;
    int numDim = 0;
    
    for (int d = 0; size[d] != 0; d++)
    {
        if (d > APP_MAX_ARRAY_DIM)
        {
            sprintf(msg, "Found %i dimensions. Only %i allowed.", d, APP_MAX_ARRAY_DIM);
            CUtilities::Error.Add(msg);
            break;
        }
        numDim++;
        //size[d] = sizeIn[d];
    }
    numDim--;
    
    if (numDim > 1)
        ERRChk(ERROR_HW_CRITICAL, "More than one dimension not implemented yet.", "CUtilities::InitializeArray");
    
    for (int i = 0; i < size[0]; i++)
    {
        output[i] = value;
    }
}

/******************************************************************************
    Name:   CopyArrayToDouble
    Desc:   Copies a void * array of the specified type into a double array
******************************************************************************/
void CUtilities::CopyArrayToDouble(void* input, variable_type type, int* sizeIn, double* output, word* listDut_in)
{
    DBGTrace("---> CUtilities::CopyArrayToDouble");
    
    // removed because I did not write this function
}

/******************************************************************************
    Name:   BreakOutPowersOf2
    Desc:   Take a double and break out all the powers of two and return a string
******************************************************************************/
void CUtilities::BreakOutPowersOf2(dword value, char* output, int dir)
{
    DBGTrace("---> CUtilities::BreakOutPowersOf2");
    
    String piece;
    sprintf(output, "");
    
    if (dir == 0)
    {
        for (int i = 0; i <= 32; i++)
        {
            if ((value & (1<<i)) != 0)
            {
                sprintf(piece, "%i ", 1<<i);
                strcat(output, (char *)piece);
            }
        
        }
    }
    else
    {
        for (int i = 31; i > 0; i--)
        {
            if ((value & (1<<i)) != 0)
            {
                sprintf(piece, "%i ", 1<<i);
                strcat(output, (char *)piece);
            }
        
        }
    }
}

/******************************************************************************
    Name:   ConcatenateArray
    Desc:   Concatenate 2 zeros terminated arrays and return result in variable array_in
******************************************************************************/
void CUtilities::ConcatenateArray(int* array_in, int* array_to_add)
{
    DBGTrace("---> CUtilities::ConcatenateArray");
    
    int index = 0;
    int index2 = 0;
    for (index = 0; array_in[index] != 0; index++) { ; }
    for (int index2 = 0; array_to_add[index2] != 0; index2++)
    {
        array_in[index++] = array_to_add[index2];
    }
}

/******************************************************************************
    Name:   FindInArray
    Desc:   Find value in zero terminated array
******************************************************************************/
int CUtilities::FindInArray(int value, int* array_in)
{
    DBGTrace("---> CUtilities::FindInArray");
    
    int index = 0;
    while (array_in[index] != 0)
    {
        if (array_in[index] == value)
            return index;
        index++;
    }
    
    return -1;
}

/******************************************************************************
    Name:   FormatString
    Desc:   
******************************************************************************/
int CUtilities::FormatString(String* labels_in, String labelFormat, int count, String* output, String* indicies)
{
    DBGTrace("---> CUtilities::FormatString");
    
    String tmpStr;
    
    sprintf(*output,"");
    for (int i = 0; i < count; i++)
    {
        for (int j = 0; strcmp(labels_in[j],"") == 1; j++)
        {
            if (indicies != NULL)
            {
                sprintf(tmpStr,(char *) labelFormat, (char *) labels_in[j], (char *) indicies[i]);
            }
            else
            {
                sprintf_s(tmpStr, APP_MAX_CHAR, (char *) labelFormat, (char *)labels_in[j]);
            }
            strcat(*output,tmpStr);            
        }
    }
    
    return SUCCESS;
}

/******************************************************************************
    Name:   AddChar
    Desc:   Output a line of characters, most often it will be spaces, but can be any character
******************************************************************************/
String CUtilities::AddChar(char ch, int num)
{
    String tmp = "";
    for (int i = 0; i < num; i++)
        strcat(tmp,&ch);
    
    return tmp;
}

/******************************************************************************
    Name:   DisplayBinary
    Desc:   returns a string with this format (0xFE)11111110, in other words 2 hex digits and 8 binary ones,
    unless formatIn specifies something different
******************************************************************************/
String CUtilities::DisplayBinary(long val, int length, char* formatIn)
{
    String msg = "";
    String format = "";
    
    if (strcmp(formatIn,"") == 0)
        strcpy(format, "(0x%02X)%s");
    else
        strcpy(format, formatIn);
    
    // mask out possible extra bits in variable val (if it is negative, for instance)
    val &= (1 << length) - 1;
    sprintf(msg, format, val, (char *) CUtilities::LongToBinaryString(val, length));
    return msg;
}

/******************************************************************************
    Name:   ChangeCase
    Desc:   Change the case of each letter (either to upper case or to lower case)
******************************************************************************/
void CUtilities::ChangeCase(char* s)
{
    int i=0;
    char c;
    while (s[i])
    {
        c=s[i];
        if (islower(c))
        {
            c=toupper(c);
        }
        else if (isupper(c))
        {
            c=tolower(c);
        }
        s[i] = c;
        
        i++;
    }
}

/******************************************************************************
    Name:   UpperCase
    Desc:   Change each letter to upper case
******************************************************************************/
void CUtilities::UpperCase(char* s)
{
    int i=0;
    char c;
    while (s[i])
    {
        c=s[i];
        if (islower(c))
        {
            c=toupper(c);
        }
        s[i] = c;
        
        i++;
    }
}

/******************************************************************************
    Name:   XToTheY
    Desc:   returns x raised to the power of y (int)
******************************************************************************/
int CUtilities::XToTheY(int x, int y)
{
    if (x == 2)
        return 1 << y;
    
    return (int) pow((double)x,(double)y);
}

/******************************************************************************
    Name:   XToTheY
    Desc:   returns x raised to the power of y (double)
******************************************************************************/
double CUtilities::XToTheY(double x, double y)
{
    return pow(x,y);
}

/******************************************************************************
    Name:   ClipToRange
    Desc:   clip value to fit within range specified by min and max (int)
    
    Output using this function:
    
    value:0, Clip(1, 0, 5, true ): 1, Clip(1, 0, 5, false ): 2
    value:1, Clip(1, 1, 5, true ): 1, Clip(1, 1, 5, false ): 2
    value:2, Clip(1, 2, 5, true ): 2, Clip(1, 2, 5, false ): 2
    value:3, Clip(1, 3, 5, true ): 3, Clip(1, 3, 5, false ): 3
    value:4, Clip(1, 4, 5, true ): 4, Clip(1, 4, 5, false ): 4
    value:5, Clip(1, 5, 5, true ): 5, Clip(1, 5, 5, false ): 4
    value:6, Clip(1, 6, 5, true ): 5, Clip(1, 6, 5, false ): 4

******************************************************************************/
int CUtilities::ClipToRange(int min, int val, int max, bool inclusive)
{
    if (inclusive)
    {
        if (val < min)
            return min;
        else if (val > max)
            return max;
        else
            return val;
    }
    else
    {
        if (val <= min)
            return min+1;
        else if (val >= max)
            return max-1;
        else
            return val;
    }
}

/******************************************************************************
    Name:   ClipToRange
    Desc:   clip value to fit within range specified by min and max (double)
******************************************************************************/
double CUtilities::ClipToRange(double min, double val, double max)
{
    if (val < min)
        return min;
    else if (val > max)
        return max;
    else
        return val;
}