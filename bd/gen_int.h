//
// Copyright 2018 StarPoint Inc.,Ltd
//

#ifndef GEN_INT_H
#define GEN_INT_H

#define MAX_DIGIT 255
#define MAX_BINARY_DIGIT 1024

#include "rd.h"
#include <stddef.h>

namespace sp_rd {

//! A general integer class,support 255 max width of the number part.
class RD_API gen_int
{
public:
    /*!
     * \brief Constructor.
     * \param in input characters."123456"=>123456
     */
    gen_int(const char *in = NULL);
    ~gen_int(void);

public:
    void print();
    short get_digit() const;
    gen_int abs(const gen_int src);
    gen_int operator + ();
    gen_int operator - ();
    gen_int operator + (const gen_int add_end);
    gen_int operator - (const gen_int minu_end);
    gen_int operator * (const gen_int multiplier);
    gen_int operator / (const signed long long divider);
    signed long long operator % (const signed long long divider);
    gen_int operator = (const gen_int src);
    gen_int operator += (const gen_int add_end);
    gen_int operator -= (const gen_int minu_end);
    gen_int operator *= (const gen_int multiplier);
    gen_int operator /= (const signed long long divider);
    gen_int operator ++ ();
    gen_int operator ++ (int);
    gen_int operator -- ();
    gen_int operator -- (int);
    bool operator == (const gen_int compare);
    bool operator >  (const gen_int compare);
    bool operator <  (const gen_int Compare);
    bool operator >= (const gen_int compare);
    bool operator <= (const gen_int compare);
    bool operator != (const gen_int compare);
    gen_int operator << (short digit);
    gen_int operator >> (short digit);
public:
    unsigned long long _2ull(short from,short to);	//directly to unsigned long long,regardless of where it is
    short odd_part();
    void _2bin_s(short *bin,short digit);
    void _2ui64(unsigned int &high,unsigned int &low);
    gen_int operator * (const short multiplier);
    short get_digit(const unsigned long long src);
private:
    short m_num[MAX_DIGIT + 1];
    char m_sign;
};

} //namespace sp_rd

#endif // GEN_INT_H
