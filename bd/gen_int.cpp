#include "gen_int.h"
#include <stdio.h>
#include <string.h>
#include <math.h>

using namespace sp_rd;

gen_int::gen_int(const char *in)
{
    memset(m_num,0,sizeof(m_num));
    m_sign = '+';
    if (NULL == in)
		return;

    m_sign = (!(in[0] - '-') || !(in - '+') ? in[0] : '+');
    int start_pos = (!(in[0] - '-') || !(in - '+') ? 1 : 0);
    int num_idx = 0;
    for (int i = (int)(strlen(in) - 1);i >= start_pos;i --) {
        if (in[i] >= '0' && in[i] <= '9') {
            m_num[num_idx] = in[i] - '0';
            num_idx ++;
		}
	}
}

gen_int::~gen_int(void)
{
}

void gen_int::print()
{
    printf("%c",m_sign);
    for (short j = get_digit() - 1;j >= 0;j --) {
        printf("%d",m_num[j]);
	}
	printf("\n");
	return;
}

short gen_int::get_digit() const
{
	short i = MAX_DIGIT;
	for (i = MAX_DIGIT;i >= 0;i --) {
        if (m_num[i])
			break;
	}
	i = i < 0 ? 0 : i;
	return i + 1;
}

gen_int gen_int::abs(const gen_int src)
{
    gen_int res;
	for (short i = 0;i < MAX_DIGIT + 1;i ++)
        res.m_num[i] = src.m_num[i];
    res.m_sign = '+';
    return res;
}

gen_int gen_int::operator + ()
{
	return *this;
}

gen_int gen_int::operator - ()
{
    gen_int res;
	for (short i = 0;i < MAX_DIGIT + 1;i ++)
        res.m_num[i] = m_num[i];
    res.m_sign = ((0 == m_sign - '+') ? '-' : '+');
    return res;
}

gen_int gen_int::operator + (const gen_int add_end)
{
    gen_int res;
    short carry = 0;
    short this_digit = this->get_digit();
    short add_end_digit = add_end.get_digit();
    short smaxDigit = this_digit > add_end_digit ? this_digit : add_end_digit;
	int i = 0;

    if (0 == (m_sign - add_end.m_sign)) {
        for (i = 0;i < smaxDigit;i ++) {
            res.m_num[i] = m_num[i] + add_end.m_num[i] + carry;
            if (res.m_num[i] >= 10) {
                carry = 1;
                res.m_num[i] -= 10;
			}
			else
                carry = 0;
		}
        res.m_num[i] = carry;
	}
	else {
        gen_int *max = (abs(*this) >= abs(add_end) ? this : const_cast<gen_int *>(&add_end));
        gen_int *min = (max == this ? const_cast<gen_int *>(&add_end) : this);

        for (i = 0;i < smaxDigit;i ++) {
            max->m_num[i] -= carry;
            if (max->m_num[i] < 0) {
                max->m_num[i] = 9;
                carry = 1;
                res.m_num[i] = max->m_num[i] - min->m_num[i];
				continue;
			}
            if (max->m_num[i] < min->m_num[i]) {
                max->m_num[i] += 10;
                carry = 1;
			}
			else
                carry = 0;
            res.m_num[i] = max->m_num[i] - min->m_num[i];
		}
        res.m_sign = ((0 == max->m_sign - '+') ? '+' : '-');
	}
    return res;
}

gen_int gen_int::operator - (const gen_int minu_end)
{
    gen_int res = minu_end;
    res.m_sign = ((0 == res.m_sign - '+') ? '-' : '+');
    return (*this + res);
}

gen_int gen_int::operator * (const gen_int multiplier)
{
    gen_int res;
    short multi_digit = multiplier.get_digit();
    gen_int this_positive = abs(*this);
    gen_int multi_positive = abs(multiplier);
    for (short i = 0;i < multi_digit;i ++)
        res += (this_positive * multi_positive.m_num[i]) << i;
    res.m_sign = (0 == m_sign - multiplier.m_sign ? '+' : '-');
    return res;
}

gen_int gen_int::operator / (const signed long long divider)
{
    gen_int this_copy = *this;
    gen_int res;
    unsigned long long divider_abs = divider < 0 ? divider * -1 : divider;
    short div_digit = get_digit(divider_abs);
    short to = this_copy.get_digit() - 1,from = to - div_digit + 1;
    unsigned long long div_end = this_copy._2ull(from,to);
    unsigned long long remainder = 0;
    if (div_end < divider_abs) {
        from --;
        from = from < 0 ? 0 : from;
        div_end = this_copy._2ull(from,to);
	}
    for (short i = from;i >= 0;i --) {
        res.m_num[i] = short(div_end / divider_abs);
        remainder = div_end % divider_abs;
        for (short j = 0;j <= div_digit;j ++)
            this_copy.m_num[from + j] = short(remainder % (unsigned long long)(pow(10.0,j + 1)) / (unsigned long long)(pow(10.0,j)));
        from --;
        if (!this_copy.m_num[to])
            to --;
        div_end = this_copy._2ull(from,to);
	}
    remainder *= 10;
    if (divider_abs * 5 < remainder)
        res ++;
    if ((0 == this_copy.m_sign - '+' && divider < 0) || (0 == this_copy.m_sign - '-' && divider > 0))
        res.m_sign = '-';
    return res;
}

signed long long gen_int::operator % (const signed long long divider)
{
    gen_int this_copy = *this;
    gen_int Res;
    unsigned long long divider_abs = divider < 0 ? divider * -1 : divider;
    short sDivDigit = get_digit(divider_abs);
    short to = this_copy.get_digit() - 1,from = to - sDivDigit + 1;
    unsigned long long div_end = this_copy._2ull(from,to);
    unsigned long long remainder = 0;
    if (div_end < divider_abs) {
        from --;
        from = from < 0 ? 0 : from;
        div_end = this_copy._2ull(from,to);
	}
    for (short i = from;i >= 0;i --) {
        Res.m_num[i] = short(div_end / divider_abs);
        remainder = div_end % divider_abs;
		for (short j = 0;j <= sDivDigit;j ++)
            this_copy.m_num[from + j] = short(remainder % (unsigned long long)(pow(10.0,j + 1)) / (unsigned long long)(pow(10.0,j)));
        from --;
        if (!this_copy.m_num[to])
            to --;
        div_end = this_copy._2ull(from,to);
	}
    return remainder;
}

gen_int gen_int::operator = (const gen_int src)
{
	for (short i = 0;i < MAX_DIGIT + 1;i ++)
        m_num[i] = src.m_num[i];
    m_sign = src.m_sign;
	return *this;
}

gen_int gen_int::operator += (const gen_int add_end)
{
    *this = *this + add_end;
	return *this;
}

gen_int gen_int::operator -= (const gen_int minu_end)
{
    *this = *this - minu_end;
	return *this;
}

gen_int gen_int::operator *= (const gen_int multiplier)
{
    *this = *this * multiplier;
	return *this;
}

gen_int gen_int::operator /= (signed long long divider)
{
    *this = *this / divider;
	return *this;
}

gen_int gen_int::operator ++ ()
{
    *this = *this + gen_int("1");
	return *this;
}

gen_int gen_int::operator ++ (int)
{
    gen_int res = *this;
	++(*this);
    return res;
}

gen_int gen_int::operator -- ()
{
    *this = *this - gen_int("1");
	return *this;
}

gen_int gen_int::operator -- (int)
{
    gen_int res = *this;
	--(*this);
    return res;
}

bool gen_int::operator == (const gen_int compare)
{
    if (m_sign - compare.m_sign)
		return false;
	for (short i = 0;i < MAX_DIGIT + 1;i ++) {
        if (m_num[i] != compare.m_num[i])
			return false;
	}
	return true;
}

bool gen_int::operator > (const gen_int compare)
{
    if (m_sign < compare.m_sign) return true;
    if (m_sign > compare.m_sign) return false;
	for (short i = MAX_DIGIT;i >= 0;i --) {
        if ((0 == m_num[i] && 0 == compare.m_num[i]) || (m_num[i] == compare.m_num[i]))
			continue;
        else if (m_num[i] > compare.m_num[i])
            return (0 == m_sign - '+') ?  true : false;
		else
            return (0 == m_sign - '+') ? false : true;
	}
	return false;
}

bool gen_int::operator < (const gen_int compare)
{
    return !(*this == compare || *this > compare);
}

bool gen_int::operator >= (const gen_int compare)
{
    return *this == compare || *this > compare;
}

bool gen_int::operator <= (const gen_int compare)
{
    return !(*this > compare);
}

bool gen_int::operator != (const gen_int compare)
{
    return !(*this == compare);
}

gen_int gen_int::operator << (short digit)
{
    if (digit < 0)
        return (*this >> (digit * -1));
    if (digit > 0) {
        for (short i = MAX_DIGIT;i >= digit;i --)
            m_num[i] = m_num[i - digit];
        for (short i = 0;i < digit;i ++)
            m_num[i] = 0;
	}
	return *this;
}

gen_int gen_int::operator >> (short digit)
{
    if (digit < 0)
        return (*this << (digit * -1));
    if (digit > 0) {
		short i = 0;
        for (i = 0;i < MAX_DIGIT - digit;i ++)
            m_num[i] = m_num[i + digit];
		for (;i < MAX_DIGIT;i++)
            m_num[i] = 0;
	}
	return *this;
}

gen_int gen_int::operator * (const short multiplier)
{
    gen_int res;
    char multi_sign = (multiplier >= 0 ? '+' : '-');
    short digit = get_digit();
    short carry = 0;
	short i = 0;
    for (i = 0;i < digit;i ++) {
        res.m_num[i] = m_num[i] * multiplier + carry;
        carry = res.m_num[i] / 10;
        res.m_num[i] -= carry * 10;
	}
    res.m_num[i] = carry;
    res.m_sign = (0 == m_sign - multi_sign ? '+' : '-');
    return res;
}

unsigned long long gen_int::_2ull(short from, short to)
{
    short sDigit = to - from + 1;
    unsigned long long res = 0;
	for (short i = 0;i < sDigit;i ++)
        res += (unsigned long long)m_num[from + i] * (unsigned long long)pow(10.0,i);
    return res;
}

short gen_int::odd_part()
{
    return short(m_num[0] % 2);
}

void gen_int::_2bin_s(short *bin, short digit)
{
    short res[MAX_BINARY_DIGIT];
    memset(res,0,sizeof(res));
    gen_int divider = abs(*this);
    gen_int div_res("1");
	short i = 0,j = 0;
    while (gen_int("0") != div_res) {
        div_res = divider / 2;
        res[i] = divider.odd_part();
        divider = div_res;
		i ++;
	}
    if ((0 == m_sign - '-') && (gen_int("0") != *this)) {
        short carry = 0;
		for (short i = 0;i < MAX_BINARY_DIGIT;i ++)
            res[i] = (0 == res[i] ? 1 : 0);
        carry = (1 == res[0] ? 1 : 0);
        res[0] = (1 == res[0] ? 0 : 1);
		for (short i = 1;i < MAX_BINARY_DIGIT;i ++) {
            res[i] += carry;
            if (2 == res[i]) {
                res[i] = 0;
                carry = 1;
			}
			else
                carry = 0;
		}
	}
    for (j = 0;j < digit;j ++)
        bin[j] = res[j];
}

void gen_int::_2ui64(unsigned int &high, unsigned int &low)
{
    short bin[64] = {0};
    high = low = 0;
    _2bin_s(bin,64);
	for (short i = 0;i < 32;i ++) {
        low  += bin[i] << i;
        high += bin[32 + i] << i;
	}
}

short gen_int::get_digit(const unsigned long long src)
{
	short j = 1;
	for (unsigned long long i = 10;;i *= 10) {
        if (0 == src / i)
			break;
		j ++;
	}
	return j;
}
