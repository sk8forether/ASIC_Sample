/******************************************************************************

    File:   MyString.h
    Desc:   Contains a Kionix specific implementation of string concept

******************************************************************************/
#ifndef _MY_STRING_H_
#define _MY_STRING_H_

#include <stdio.h>

#define APP_MAX_CHAR            256     // character count
#define APP_MAX_CHAR_LONGER     2560    // character count longer

//------------------------------------------------------------------------------
// string class
typedef class myString
{
private:
    char data[APP_MAX_CHAR];

public:
    myString(void)  { memset(data, 0, APP_MAX_CHAR); }
    ~myString(void) {}
    
    // construction
    myString(const char* rhs) { strcpy_s(data, APP_MAX_CHAR, rhs); }
    myString(char*       rhs) { strcpy_s(data, APP_MAX_CHAR, rhs); }
    
    myString(int         rhs) { sprintf_s(data, APP_MAX_CHAR, "%d",   rhs); }
    myString(byte        rhs) { sprintf_s(data, APP_MAX_CHAR, "%02X", rhs); }
    myString(word        rhs) { sprintf_s(data, APP_MAX_CHAR, "%hu",  rhs); }
    myString(dword       rhs) { sprintf_s(data, APP_MAX_CHAR, "%lu",  rhs); }
    myString(double      rhs) { sprintf_s(data, APP_MAX_CHAR, "%.4f", rhs); }
    
    // assignment
    myString operator =(myString    rhs) { strcpy_s(data, APP_MAX_CHAR, rhs.data); return *this; }
    myString operator =(const char* rhs) { strcpy_s(data, APP_MAX_CHAR, rhs);      return *this; }
    myString operator =(char*       rhs) { strcpy_s(data, APP_MAX_CHAR, rhs);      return *this; }
    
    myString operator =(int         rhs) { sprintf_s(data, APP_MAX_CHAR, "%d",   rhs); return *this; }
    myString operator =(byte        rhs) { sprintf_s(data, APP_MAX_CHAR, "%02X", rhs); return *this; }
    myString operator =(word        rhs) { sprintf_s(data, APP_MAX_CHAR, "%hu",  rhs); return *this; }
    myString operator =(dword       rhs) { sprintf_s(data, APP_MAX_CHAR, "%lu",  rhs); return *this; }
    myString operator =(double      rhs) { sprintf_s(data, APP_MAX_CHAR, "%.4f", rhs); return *this; }
    
    // concatenation
    myString operator +=(myString    rhs) { strcat_s(data, APP_MAX_CHAR, rhs.data); return *this; }
    myString operator +=(const char* rhs) { strcat_s(data, APP_MAX_CHAR, rhs);      return *this; }
    myString operator +=(char*       rhs) { strcat_s(data, APP_MAX_CHAR, rhs);      return *this; }
    
    myString operator +=(int         rhs) { myString tmp; sprintf_s(tmp, APP_MAX_CHAR, "%d",   rhs); strcat_s(data, APP_MAX_CHAR, tmp); return *this; }
    myString operator +=(byte        rhs) { myString tmp; sprintf_s(tmp, APP_MAX_CHAR, "%02X", rhs); strcat_s(data, APP_MAX_CHAR, tmp); return *this; }
    myString operator +=(word        rhs) { myString tmp; sprintf_s(tmp, APP_MAX_CHAR, "%hu",  rhs); strcat_s(data, APP_MAX_CHAR, tmp); return *this; }
    myString operator +=(dword       rhs) { myString tmp; sprintf_s(tmp, APP_MAX_CHAR, "%lu",  rhs); strcat_s(data, APP_MAX_CHAR, tmp); return *this; }
    myString operator +=(double      rhs) { myString tmp; sprintf_s(tmp, APP_MAX_CHAR, "%.4f", rhs); strcat_s(data, APP_MAX_CHAR, tmp); return *this; }
    
    // subscription
    char operator [](int i) { return data[i]; }
    
    // comparison
    bool operator ==(myString    rhs) { return (strcmp(data, rhs.data) == 0 ? true : false); }
    bool operator !=(myString    rhs) { return (strcmp(data, rhs.data) != 0 ? true : false); }
    bool operator < (myString    rhs) { return (strcmp(data, rhs.data) <  0 ? true : false); }
    bool operator > (myString    rhs) { return (strcmp(data, rhs.data) >  0 ? true : false); }
    bool operator <=(myString    rhs) { return (strcmp(data, rhs.data) <= 0 ? true : false); }
    bool operator >=(myString    rhs) { return (strcmp(data, rhs.data) >= 0 ? true : false); }
    
    bool operator ==(const char* rhs) { return (strcmp(data, rhs) == 0 ? true : false); }
    bool operator !=(const char* rhs) { return (strcmp(data, rhs) != 0 ? true : false); }
    bool operator < (const char* rhs) { return (strcmp(data, rhs) <  0 ? true : false); }
    bool operator > (const char* rhs) { return (strcmp(data, rhs) >  0 ? true : false); }
    bool operator <=(const char* rhs) { return (strcmp(data, rhs) <= 0 ? true : false); }
    bool operator >=(const char* rhs) { return (strcmp(data, rhs) >= 0 ? true : false); }
    
    bool operator ==(char*       rhs) { return (strcmp(data, rhs) == 0 ? true : false); }
    bool operator !=(char*       rhs) { return (strcmp(data, rhs) != 0 ? true : false); }
    bool operator < (char*       rhs) { return (strcmp(data, rhs) <  0 ? true : false); }
    bool operator > (char*       rhs) { return (strcmp(data, rhs) >  0 ? true : false); }
    bool operator <=(char*       rhs) { return (strcmp(data, rhs) <= 0 ? true : false); }
    bool operator >=(char*       rhs) { return (strcmp(data, rhs) >= 0 ? true : false); }
    
    // conversion
    operator const char*(void) { return data; }
    operator char*(void)       { return data; }
    
    operator int(void)         { return    (int)atoi(data); }
    operator byte(void)        { return   (byte)atoi(data); }
    operator word(void)        { return   (word)atoi(data); }
    operator dword(void)       { return  (dword)atoi(data); }
    operator double(void)      { return (double)atof(data); }
    
    // function
    void init(void) { memset(data, 0, sizeof(data)); }
    int  size(void) { return (int)strlen(data); }
    
    void trim(char c)
    {
        char temp[APP_MAX_CHAR];
        int i = 0;
        int o = 0;
        
        while (i < APP_MAX_CHAR)
        {
            if (data[i] != c)
            {
                temp[o] = data[i];
                o++;
            }
            
            i++;
        }
        
        strcpy_s(data, APP_MAX_CHAR, temp);
    }
    
    void TrimToLength(int len)
    {
        int i = len;
        while (i < APP_MAX_CHAR)
        {
            data[i++] = 0;
        }
    }
} MyString;

inline MyString operator +(MyString lhs, MyString      rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(MyString lhs, const char* rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(MyString lhs, char*       rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }

inline MyString operator +(MyString lhs, int         rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(MyString lhs, byte        rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(MyString lhs, word        rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(MyString lhs, dword       rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(MyString lhs, double      rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }

inline MyString operator +(const char* lhs, MyString rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(char*       lhs, MyString rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }

inline MyString operator +(int         lhs, MyString rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(byte        lhs, MyString rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(word        lhs, MyString rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(dword       lhs, MyString rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }
inline MyString operator +(double      lhs, MyString rhs) { MyString tmp; tmp = lhs; tmp += rhs; return tmp; }

#endif