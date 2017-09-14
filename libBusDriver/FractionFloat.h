#ifndef __FRACTION_FLOAT_H__
#define __FRACTION_FLOAT_H__

class CFractionFloat
{
public:
	CFractionFloat(signed long long Numerator = 0,signed long long Denominator = 1);
	~CFractionFloat(void);
public:
	void Print();
	CFractionFloat abs(const CFractionFloat Source);
	signed long long abs(const signed long long Source);
	CFractionFloat operator + ();
	CFractionFloat operator - ();
	CFractionFloat operator + (const CFractionFloat AddEnd);
	CFractionFloat operator - (const CFractionFloat MinuEnd);
	CFractionFloat operator * (const CFractionFloat Multiplier);
	CFractionFloat operator = (const CFractionFloat Source);
	CFractionFloat operator += (const CFractionFloat AddEnd);
	CFractionFloat operator -= (const CFractionFloat MinuEnd);
	CFractionFloat operator *= (const CFractionFloat Multiplier);
	CFractionFloat operator ++ ();
	CFractionFloat operator ++ (int);
	CFractionFloat operator -- ();
	CFractionFloat operator -- (int);
	bool operator == (const CFractionFloat Compare);
	bool operator >  (const CFractionFloat Compare);
	bool operator <  (const CFractionFloat Compare);
	bool operator >= (const CFractionFloat Compare);
	bool operator <= (const CFractionFloat Compare);
	bool operator != (const CFractionFloat Compare);
private:
	unsigned long long gcd(unsigned long long x,unsigned long long y);
	unsigned long long lcm(unsigned long long x,unsigned long long y,unsigned long long &xMulti,unsigned long long &yMulti);
	void reduction();
private:
	signed long long m_llNumerator;
	signed long long m_llDenominator;
};

#endif
