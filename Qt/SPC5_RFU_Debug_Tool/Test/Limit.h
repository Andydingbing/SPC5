#ifndef LIMITS_H
#define LIMITS_H

#include <string>
#include <map>
#include <QObject>

using namespace std;

#define LIMIT Limits::instance()

class Limit {
public:
	Limit();
	Limit(double lower, double upper);
	double upper();
	double lower();
	bool pass(double value);
private:
	double m_lower;
	double m_upper;
};

class Limits
{
public:
	~Limits();
	static Limits &instance();
public:
	bool pass(string key, double value);
	bool fail(string key, double value);
	Limit limit(string key);
private:
	Limits();
	Limits(Limits &);
	map<string, Limit> m_limits;
};

#endif // LIMITS_H
