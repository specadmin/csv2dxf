#ifndef CVALUE_H
#define CVALUE_H
#include <stddef.h>
//-------------------------------------------------------------------------------------------------
class CValue
{
    public:
        CValue(const char *str);
        CValue(char *str, size_t len);
        virtual ~CValue();
        CValue(const CValue& other);
        CValue& operator=(const CValue& other);
        const char* text();
        const char* format(const char *fmt, int roundto = 0);
        size_t get_length();
        double to_double();
        double to_double(int round_digits);
    private:
        char *_buf = NULL;
        char *_form_buf = NULL;
};

#endif
