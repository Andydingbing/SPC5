#ifndef __GENERAL_INTEGER_H__
#define __GENERAL_INTEGER_H__

#define MAX_DIGIT 255
#define MAX_BINARY_DIGIT 1024

#include <stddef.h>

class CGeneralInteger
{
public:
    CGeneralInteger(const char *pInt = NULL);
	~CGeneralInteger(void);
public:
	void Print();
	short GetDigit() const;
	CGeneralInteger abs(const CGeneralInteger Source);
	CGeneralInteger operator + ();
	CGeneralInteger operator - ();
	CGeneralInteger operator + (const CGeneralInteger AddEnd);
	CGeneralInteger operator - (const CGeneralInteger MinuEnd);
	CGeneralInteger operator * (const CGeneralInteger Multiplier);
	CGeneralInteger operator / (const signed long long Divider);
	signed long long operator % (const signed long long Divider);
	CGeneralInteger operator = (const CGeneralInteger Source);
	CGeneralInteger operator += (const CGeneralInteger AddEnd);
	CGeneralInteger operator -= (const CGeneralInteger MinuEnd);
	CGeneralInteger operator *= (const CGeneralInteger Multiplier);
	CGeneralInteger operator /= (const signed long long Divider);
	CGeneralInteger operator ++ ();
	CGeneralInteger operator ++ (int);
	CGeneralInteger operator -- ();
	CGeneralInteger operator -- (int);
	bool operator == (const CGeneralInteger Compare);
	bool operator >  (const CGeneralInteger Compare);
	bool operator <  (const CGeneralInteger Compare);
	bool operator >= (const CGeneralInteger Compare);
	bool operator <= (const CGeneralInteger Compare);
	bool operator != (const CGeneralInteger Compare);
	CGeneralInteger operator << (short Digit);
	CGeneralInteger operator >> (short Digit);
public:
	unsigned long long dtoull(short sFrom,short sTo);	//directly to unsigned long long,regardless of where it is
	short oddpart();
	void tobin_s(short *pBin,short sDigit);	
	void toui64(unsigned int &iHigh,unsigned int &iLow);
	CGeneralInteger operator * (const short Multiplier);
	short GetDigit(const unsigned long long ullSource);
private:
	short m_sNum[MAX_DIGIT + 1];
	char m_szSign;
};

#endif
