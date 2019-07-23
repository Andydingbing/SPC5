#include "Limit.h"

Limit::Limit() {
}

Limit::Limit(double lower, double upper)
	: m_lower(lower)
	, m_upper(upper) {
}

double Limit::upper() {
	return m_upper;
}

double Limit::lower() {
	return m_lower;
}

bool Limit::pass(double value) {
	return (value >= m_lower && value <= m_upper);
}

Limits::Limits()
{
    m_limits["边带载漏测试(<=3.8G)."] = Limit(-100, -45);
    m_limits["边带载漏测试(>3.8G)."] = Limit(-100, -40);
    m_limits["发滤波器检测40."] = Limit(-0.2, 0.2);
    m_limits["发滤波器检测160."] = Limit(-1, 1);
    m_limits["发基准扫频."] = Limit(-1, 1);
    m_limits["发合路器校准."] = Limit(-3, 15);
    m_limits["发扫频测试."] = Limit(-1, 1);
    m_limits["发精度测试."] = Limit(-0.4, 0.4);
    m_limits["收滤波器检测."] = Limit(-1, 1);
    m_limits["收扫频."] = Limit(-0.5, 0.5);
    m_limits["收合路器校准."] = Limit(-4, 10);
    m_limits["收扫频."] = Limit(-0.5, 0.5);
    m_limits["收衰减器检测."] = Limit(-0.5, 0.5);
}

Limits::Limits(Limits &) {

}

Limits::~Limits()
{

}

Limits &Limits::instance() {
	static Limits limits;
	return limits;
}

bool Limits::pass(string key, double value) {
	Limit limit = this->limit(key);
	return limit.pass(value);
}

bool Limits::fail(string key, double value) {
	return !pass(key, value);
}

Limit Limits::limit(string key) {
	if (m_limits.find(key) == m_limits.end())
		throw "error";
	return m_limits[key];
}
