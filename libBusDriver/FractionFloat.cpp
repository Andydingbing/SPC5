#include "FractionFloat.h"
#include <stdio.h>
#include <math.h>

CFractionFloat::CFractionFloat(signed long long Numerator,signed long long Denominator)
{
	m_llNumerator = Numerator;
	m_llDenominator = Denominator;
	reduction();
	if (m_llDenominator < 0) {
		m_llDenominator *= -1;
		m_llNumerator *= -1;
	}
}

CFractionFloat::~CFractionFloat(void)
{
}

void CFractionFloat::Print()
{
    printf("~=% .15f	exact : %+I64i/%I64i\n",(double)m_llNumerator / (double)m_llDenominator,m_llNumerator,m_llDenominator);
}

CFractionFloat CFractionFloat::abs(const CFractionFloat Source)
{
	CFractionFloat Res;
	Res.m_llNumerator = (Source.m_llNumerator >= 0 ? Source.m_llNumerator : Source.m_llNumerator * -1);
	return Res;
}

signed long long CFractionFloat::abs(const signed long long Source)
{
	return Source >= 0 ? Source : Source * -1;
}

unsigned long long CFractionFloat::gcd(unsigned long long x,unsigned long long y)
{
	if (0 == x) return y;
	if (0 == y) return x;
	unsigned long long *Max = (x > y ? &x : &y);
	unsigned long long *Min = (Max == &x ? &y : &x);
	unsigned long long llRemainder = 1;
	while (llRemainder != 0) {
		llRemainder = *Max % *Min;
		if (llRemainder) {
			*Max = *Min;
			*Min = llRemainder;
		}
	}
	return *Min;
}

void CFractionFloat::reduction()
{
	unsigned long long GCD = gcd(abs(m_llNumerator),abs(m_llDenominator));
	m_llNumerator /= (signed long long)GCD;
	m_llDenominator /= (signed long long)GCD;
}

unsigned long long CFractionFloat::lcm(unsigned long long x,unsigned long long y,unsigned long long &xMulti,unsigned long long &yMulti)
{
	if (0 == x || 0 == y) return 0;
	unsigned long long *Max = (x > y ? &x : &y);
	unsigned long long *Min = (Max == &x ? &y : &x);
	unsigned long long j = 1;
	for (unsigned long long i = *Max;;i += *Max) {
		if (0 == i % x && 0 == i % y) {
			unsigned long long MaxMulti = j;
			unsigned long long MinMulti = i / *Min;
			xMulti = (Max == &x ? MaxMulti : MinMulti);
			yMulti = (Max == &y ? MaxMulti : MinMulti);
			return i;
		}
		j ++;
	}
}

CFractionFloat CFractionFloat::operator + ()
{
	return *this;
}

CFractionFloat CFractionFloat::operator - ()
{
	CFractionFloat Res;
	Res.m_llNumerator = -m_llNumerator;
	Res.m_llDenominator = m_llDenominator;
	return Res;
}

CFractionFloat CFractionFloat::operator + (const CFractionFloat AddEnd)
{
	CFractionFloat Res;
	unsigned long long thisMulti = 1,AddEndMulti = 1;
	unsigned long long LCM = lcm(m_llDenominator,AddEnd.m_llDenominator,thisMulti,AddEndMulti);

	Res.m_llNumerator = m_llNumerator * thisMulti + AddEnd.m_llNumerator * AddEndMulti;
	Res.m_llDenominator = LCM;

	Res.reduction();
	return Res;
}

CFractionFloat CFractionFloat::operator - (const CFractionFloat MinuEnd)
{
	CFractionFloat Res = MinuEnd;
	Res.m_llNumerator *= -1;
	return (*this + Res);
}

CFractionFloat CFractionFloat::operator * (const CFractionFloat Multiplier)
{
	CFractionFloat Res;
	Res.m_llNumerator = m_llNumerator * Multiplier.m_llNumerator;
	Res.m_llDenominator = m_llDenominator * Multiplier.m_llDenominator;
	Res.reduction();
	return Res;
}

CFractionFloat CFractionFloat::operator = (const CFractionFloat Source)
{
	m_llNumerator = Source.m_llNumerator;
	m_llDenominator = Source.m_llDenominator;
	return *this;
}

CFractionFloat CFractionFloat::operator += (const CFractionFloat AddEnd)
{
	*this = *this + AddEnd;
	return *this;
}

CFractionFloat CFractionFloat::operator -= (const CFractionFloat MinuEnd)
{
	*this = *this - MinuEnd;
	return *this;
}

CFractionFloat CFractionFloat::operator *= (const CFractionFloat Multiplier)
{
	*this = *this * Multiplier;
	return *this;
}

CFractionFloat CFractionFloat::operator ++ ()
{
	*this = *this + CFractionFloat(1,1);
	return *this;
}

CFractionFloat CFractionFloat::operator ++ (int)
{
	CFractionFloat Res = *this;
	++(*this);
	return Res;
}

CFractionFloat CFractionFloat::operator -- ()
{
	*this = *this - CFractionFloat(1,1);
	return *this;
}

CFractionFloat CFractionFloat::operator -- (int)
{
	CFractionFloat Res = *this;
	--(*this);
	return Res;
}

bool CFractionFloat::operator == (const CFractionFloat Compare)
{
	return (m_llNumerator == Compare.m_llNumerator && m_llDenominator == Compare.m_llDenominator);
}

bool CFractionFloat::operator >  (const CFractionFloat Compare)
{
	if (m_llNumerator >= 0 && Compare.m_llNumerator < 0) return true;
	if (m_llNumerator <0 && Compare.m_llNumerator >= 0) return false;

	unsigned long long thisNumerator = 0,CompareNumerator = 0;
	unsigned long long thisMulti = 1,CompareMulti = 1;
    lcm(m_llDenominator,Compare.m_llDenominator,thisMulti,CompareMulti);

	thisNumerator = m_llNumerator * thisMulti;
	CompareNumerator = Compare.m_llNumerator * CompareMulti;
	return (thisNumerator > CompareNumerator);
}

bool CFractionFloat::operator <  (const CFractionFloat Compare)
{
	return !(*this == Compare || *this > Compare);
}

bool CFractionFloat::operator >= (const CFractionFloat Compare)
{
	return *this == Compare || *this > Compare;
}

bool CFractionFloat::operator <= (const CFractionFloat Compare)
{
	return !(*this > Compare);
}

bool CFractionFloat::operator != (const CFractionFloat Compare)
{
	return !(*this == Compare);
}
