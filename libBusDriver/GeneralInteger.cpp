#include "GeneralInteger.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

CGeneralInteger::CGeneralInteger(const char *pInt)
{
	memset(m_sNum,0,sizeof(m_sNum));
	m_szSign = '+';
	if (NULL == pInt)
		return;

	m_szSign = (!(pInt[0] - '-') || !(pInt - '+') ? pInt[0] : '+');
	int iStartPos = (!(pInt[0] - '-') || !(pInt - '+') ? 1 : 0);
	int iNumIdx = 0;
	for (int i = (int)(strlen(pInt) - 1);i >= iStartPos;i --) {
		if (pInt[i] >= '0' && pInt[i] <= '9') {
			m_sNum[iNumIdx] = pInt[i] - '0';
			iNumIdx ++;
		}
	}
}

CGeneralInteger::~CGeneralInteger(void)
{
}

void CGeneralInteger::Print()
{
	printf("%c",m_szSign);
	for (short j = GetDigit() - 1;j >= 0;j --) {
		printf("%d",m_sNum[j]);
	}
	printf("\n");
	return;
}

short CGeneralInteger::GetDigit() const
{
	short i = MAX_DIGIT;
	for (i = MAX_DIGIT;i >= 0;i --) {
		if (m_sNum[i])
			break;
	}
	i = i < 0 ? 0 : i;
	return i + 1;
}

CGeneralInteger CGeneralInteger::abs(const CGeneralInteger Source)
{
	CGeneralInteger Res;
	for (short i = 0;i < MAX_DIGIT + 1;i ++)
		Res.m_sNum[i] = Source.m_sNum[i];
	Res.m_szSign = '+';
	return Res;
}

CGeneralInteger CGeneralInteger::operator + ()
{
	return *this;
}

CGeneralInteger CGeneralInteger::operator - ()
{
	CGeneralInteger Res;
	for (short i = 0;i < MAX_DIGIT + 1;i ++)
		Res.m_sNum[i] = m_sNum[i];
	Res.m_szSign = ((0 == m_szSign - '+') ? '-' : '+');
	return Res;
}

CGeneralInteger CGeneralInteger::operator + (const CGeneralInteger AddEnd)
{
	CGeneralInteger Res;
	short sCarry = 0;
	short sThisDigit = this->GetDigit();
	short sAddEndDigit = AddEnd.GetDigit();
	short sMaxDigit = sThisDigit > sAddEndDigit ? sThisDigit : sAddEndDigit;
	int i = 0;

	if (0 == (m_szSign - AddEnd.m_szSign)) {
		for (i = 0;i < sMaxDigit;i ++) {
			Res.m_sNum[i] = m_sNum[i] + AddEnd.m_sNum[i] + sCarry;
			if (Res.m_sNum[i] >= 10) {
				sCarry = 1;
				Res.m_sNum[i] -= 10;
			}
			else
				sCarry = 0;
		}
		Res.m_sNum[i] = sCarry;
	}
	else {
		CGeneralInteger *Max = (abs(*this) >= abs(AddEnd) ? this : const_cast<CGeneralInteger *>(&AddEnd));
		CGeneralInteger *Min = (Max == this ? const_cast<CGeneralInteger *>(&AddEnd) : this);

		for (i = 0;i < sMaxDigit;i ++) {
			Max->m_sNum[i] -= sCarry;
			if (Max->m_sNum[i] < 0) {
				Max->m_sNum[i] = 9;
				sCarry = 1;
				Res.m_sNum[i] = Max->m_sNum[i] - Min->m_sNum[i];
				continue;
			}
			if (Max->m_sNum[i] < Min->m_sNum[i]) {
				Max->m_sNum[i] += 10;
				sCarry = 1;
			}
			else
				sCarry = 0;
			Res.m_sNum[i] = Max->m_sNum[i] - Min->m_sNum[i];
		}
		Res.m_szSign = ((0 == Max->m_szSign - '+') ? '+' : '-');
	}
	return Res;
}

CGeneralInteger CGeneralInteger::operator - (const CGeneralInteger MinuEnd)
{
	CGeneralInteger Res = MinuEnd;
	Res.m_szSign = ((0 == Res.m_szSign - '+') ? '-' : '+');
	return (*this + Res);
}

CGeneralInteger CGeneralInteger::operator * (const CGeneralInteger Multiplier)
{
	CGeneralInteger Res;
	short sMultiDigit = Multiplier.GetDigit();
	CGeneralInteger thisPositive = abs(*this);
	CGeneralInteger MultiPositive = abs(Multiplier);
	for (short i = 0;i < sMultiDigit;i ++)
		Res += (thisPositive * MultiPositive.m_sNum[i]) << i;
	Res.m_szSign = (0 == m_szSign - Multiplier.m_szSign ? '+' : '-');
	return Res;
}

CGeneralInteger CGeneralInteger::operator / (const signed long long Divider)
{
	CGeneralInteger thisCopy = *this;
	CGeneralInteger Res;
	unsigned long long ullDivider = Divider < 0 ? Divider * -1 : Divider;
	short sDivDigit = GetDigit(ullDivider);
	short sTo = thisCopy.GetDigit() - 1,sFrom = sTo - sDivDigit + 1;
	unsigned long long ullDividend = thisCopy.dtoull(sFrom,sTo);
	unsigned long long ullRemainder = 0;
	if (ullDividend < ullDivider) {
		sFrom --;
		sFrom = sFrom < 0 ? 0 : sFrom;
		ullDividend = thisCopy.dtoull(sFrom,sTo);
	}
	for (short i = sFrom;i >= 0;i --) {
		Res.m_sNum[i] = short(ullDividend / ullDivider);
		ullRemainder = ullDividend % ullDivider;
		for (short j = 0;j <= sDivDigit;j ++)
            thisCopy.m_sNum[sFrom + j] = short(ullRemainder % (unsigned long long)(pow(10.0,j + 1)) / (unsigned long long)(pow(10.0,j)));
		sFrom --;
		if (!thisCopy.m_sNum[sTo])
			sTo --;	
		ullDividend = thisCopy.dtoull(sFrom,sTo);
	}
	ullRemainder *= 10;
	if (ullDivider * 5 < ullRemainder)
		Res ++;
	if ((0 == thisCopy.m_szSign - '+' && Divider < 0) || (0 == thisCopy.m_szSign - '-' && Divider > 0))
		Res.m_szSign = '-';
	return Res;
}

signed long long CGeneralInteger::operator % (const signed long long Divider)
{
	CGeneralInteger thisCopy = *this;
	CGeneralInteger Res;
	unsigned long long ullDivider = Divider < 0 ? Divider * -1 : Divider;
	short sDivDigit = GetDigit(ullDivider);
	short sTo = thisCopy.GetDigit() - 1,sFrom = sTo - sDivDigit + 1;
	unsigned long long ullDividend = thisCopy.dtoull(sFrom,sTo);
	unsigned long long ullRemainder = 0;
	if (ullDividend < ullDivider) {
		sFrom --;
		sFrom = sFrom < 0 ? 0 : sFrom;
		ullDividend = thisCopy.dtoull(sFrom,sTo);
	}
	for (short i = sFrom;i >= 0;i --) {
		Res.m_sNum[i] = short(ullDividend / ullDivider);
		ullRemainder = ullDividend % ullDivider;
		for (short j = 0;j <= sDivDigit;j ++)
            thisCopy.m_sNum[sFrom + j] = short(ullRemainder % (unsigned long long)(pow(10.0,j + 1)) / (unsigned long long)(pow(10.0,j)));
		sFrom --;
		if (!thisCopy.m_sNum[sTo])
			sTo --;	
		ullDividend = thisCopy.dtoull(sFrom,sTo);
	}
	return ullRemainder;
}

CGeneralInteger CGeneralInteger::operator = (const CGeneralInteger Source)
{
	for (short i = 0;i < MAX_DIGIT + 1;i ++)
		m_sNum[i] = Source.m_sNum[i];
	m_szSign = Source.m_szSign;
	return *this;
}

CGeneralInteger CGeneralInteger::operator += (const CGeneralInteger AddEnd)
{
	*this = *this + AddEnd;
	return *this;
}

CGeneralInteger CGeneralInteger::operator -= (const CGeneralInteger MinuEnd)
{
	*this = *this - MinuEnd;
	return *this;
}

CGeneralInteger CGeneralInteger::operator *= (const CGeneralInteger Multiplier)
{
	*this = *this * Multiplier;
	return *this;
}

CGeneralInteger CGeneralInteger::operator /= (signed long long Divider)
{
	*this = *this / Divider;
	return *this;
}

CGeneralInteger CGeneralInteger::operator ++ ()
{
    *this = *this + CGeneralInteger("1");
	return *this;
}

CGeneralInteger CGeneralInteger::operator ++ (int)
{
	CGeneralInteger Res = *this;
	++(*this);
	return Res;
}

CGeneralInteger CGeneralInteger::operator -- ()
{
    *this = *this - CGeneralInteger("1");
	return *this;
}

CGeneralInteger CGeneralInteger::operator -- (int)
{
	CGeneralInteger Res = *this;
	--(*this);
	return Res;
}

bool CGeneralInteger::operator == (const CGeneralInteger Compare)
{
	if (m_szSign - Compare.m_szSign)
		return false;
	for (short i = 0;i < MAX_DIGIT + 1;i ++) {
		if (m_sNum[i] != Compare.m_sNum[i])
			return false;
	}
	return true;
}

bool CGeneralInteger::operator > (const CGeneralInteger Compare)
{
	if (m_szSign < Compare.m_szSign) return true;
	if (m_szSign > Compare.m_szSign) return false;
	for (short i = MAX_DIGIT;i >= 0;i --) {
		if ((0 == m_sNum[i] && 0 == Compare.m_sNum[i]) || (m_sNum[i] == Compare.m_sNum[i]))
			continue;
		else if (m_sNum[i] > Compare.m_sNum[i])
			return (0 == m_szSign - '+') ?  true : false;
		else
			return (0 == m_szSign - '+') ? false : true;
	}
	return false;
}

bool CGeneralInteger::operator < (const CGeneralInteger Compare)
{
	return !(*this == Compare || *this > Compare);
}

bool CGeneralInteger::operator >= (const CGeneralInteger Compare)
{
	return *this == Compare || *this > Compare; 
}

bool CGeneralInteger::operator <= (const CGeneralInteger Compare)
{
	return !(*this > Compare);
}

bool CGeneralInteger::operator != (const CGeneralInteger Compare)
{
	return !(*this == Compare);
}

CGeneralInteger CGeneralInteger::operator << (short Digit)
{
	if (Digit < 0)
		return (*this >> (Digit * -1));
	if (Digit > 0) {
		for (short i = MAX_DIGIT;i >= Digit;i --)
			m_sNum[i] = m_sNum[i - Digit];
		for (short i = 0;i < Digit;i ++)
			m_sNum[i] = 0;
	}
	return *this;
}

CGeneralInteger CGeneralInteger::operator >> (short Digit)
{
	if (Digit < 0)
		return (*this << (Digit * -1));
	if (Digit > 0) {
		short i = 0;
		for (i = 0;i < MAX_DIGIT - Digit;i ++)
			m_sNum[i] = m_sNum[i + Digit];
		for (;i < MAX_DIGIT;i++)
			m_sNum[i] = 0;
	}
	return *this;
}

CGeneralInteger CGeneralInteger::operator * (const short Multiplier)
{
	CGeneralInteger Res;
	char szMultiSign = (Multiplier >= 0 ? '+' : '-');
	short sDigit = GetDigit();
	short sCarry = 0;
	short i = 0;
	for (i = 0;i < sDigit;i ++) {
		Res.m_sNum[i] = m_sNum[i] * Multiplier + sCarry;
		sCarry = Res.m_sNum[i] / 10;
		Res.m_sNum[i] -= sCarry * 10;
	}
	Res.m_sNum[i] = sCarry;
	Res.m_szSign = (0 == m_szSign - szMultiSign ? '+' : '-');
	return Res;
}

unsigned long long CGeneralInteger::dtoull(short sFrom,short sTo)
{
	short sDigit = sTo - sFrom + 1;
	unsigned long long ullRes = 0;
	for (short i = 0;i < sDigit;i ++)
		ullRes += (unsigned long long)m_sNum[sFrom + i] * (unsigned long long)pow(10.0,i);
	return ullRes;
}

short CGeneralInteger::oddpart()
{
	return short(m_sNum[0] % 2);
}

void CGeneralInteger::tobin_s(short *pBin,short sDigit)
{
	short sRes[MAX_BINARY_DIGIT];
	memset(sRes,0,sizeof(sRes));
	CGeneralInteger Divider = abs(*this);
    CGeneralInteger DivRes("1");
	short i = 0,j = 0;
    while (CGeneralInteger("0") != DivRes) {
		DivRes = Divider / 2;
		sRes[i] = Divider.oddpart();
		Divider = DivRes;
		i ++;
	}
    if ((0 == m_szSign - '-') && (CGeneralInteger("0") != *this)) {
		short sCarry = 0;
		for (short i = 0;i < MAX_BINARY_DIGIT;i ++)
			sRes[i] = (0 == sRes[i] ? 1 : 0);
		sCarry = (1 == sRes[0] ? 1 : 0);
		sRes[0] = (1 == sRes[0] ? 0 : 1);
		for (short i = 1;i < MAX_BINARY_DIGIT;i ++) {
			sRes[i] += sCarry;
			if (2 == sRes[i]) {
				sRes[i] = 0;
				sCarry = 1;
			}
			else
				sCarry = 0;
		}
	}
	for (j = 0;j < sDigit;j ++)
		pBin[j] = sRes[j];
}

void CGeneralInteger::toui64(unsigned int &iHigh,unsigned int &iLow)
{
	short sBin[64];
	iHigh = iLow = 0;
	tobin_s(sBin,64);
	for (short i = 0;i < 32;i ++) {
		iLow  += sBin[i] << i;
		iHigh += sBin[32 + i] << i;
	}
}

short CGeneralInteger::GetDigit(const unsigned long long ullSource)
{
	short j = 1;
	for (unsigned long long i = 10;;i *= 10) {
		if (0 == ullSource / i)
			break;
		j ++;
	}
	return j;
}
