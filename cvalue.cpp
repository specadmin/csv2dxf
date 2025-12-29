#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "cvalue.h"
//-------------------------------------------------------------------------------------------------
CValue::CValue(const char *str)
{
    _buf = strdup(str);
}
//-------------------------------------------------------------------------------------------------
CValue::CValue(char *str, size_t len)
{
    _buf = (char*) malloc(len + 1);
    memcpy(_buf, str, len);
    _buf[len] = '\0';
}
//-------------------------------------------------------------------------------------------------
CValue::~CValue()
{
    if(_buf)        { free(_buf); _buf = NULL; }
    if(_form_buf)   { free(_form_buf); _form_buf = NULL; }
}
//-------------------------------------------------------------------------------------------------
CValue::CValue(const CValue& other)
{
    _buf = strdup(other._buf);
}
//-------------------------------------------------------------------------------------------------
CValue& CValue::operator=(const CValue& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
//-------------------------------------------------------------------------------------------------
char* CValue::text()
{
    return _buf;
}
//-------------------------------------------------------------------------------------------------
const char* CValue::format(const char *fmt, int roundto)
{
    if(_form_buf)
    {
        free(_form_buf);
        _form_buf = NULL;
    }

    if(strchr(fmt, 'd'))
    {
        // integer output
        int vali = strtol(_buf, NULL, 10);
        int len = snprintf(NULL, 0, fmt, vali);
        if(len > 0)
        {
            _form_buf = (char*) malloc(len + 1);
            sprintf(_form_buf, fmt, vali);
        }
    }

    if(strchr(fmt, 'f'))
    {
        // real output
        double valf = to_double(roundto);
        int len = snprintf(NULL, 0, fmt, valf);
        if(len > 0)
        {
            _form_buf = (char*) malloc(len + 1);
            sprintf(_form_buf, fmt, valf);
        }
    }

    return _form_buf;
}
//-------------------------------------------------------------------------------------------------
double CValue::to_double()
{
   return strtod(_buf, NULL);
}
//-------------------------------------------------------------------------------------------------
double CValue::to_double(int round_digits)
{
    double p = pow(10.0, round_digits);
    return round(to_double() * p) / p;
}
//-------------------------------------------------------------------------------------------------
size_t CValue::get_length()
{
    return strlen(_buf);
}
//-------------------------------------------------------------------------------------------------
