#ifndef __CFVCTRL_H__
#define __CFVCTRL_H__
#include "stdint_common.h"
/*all function names consist of "ForConv..." means converting the calibration file to the next version
  while all names consist of "BckConv..." means converting the file to the previous version*/
/*all backward converting functions just return zero at the version right now
  so note that only support convert to the next version now!!!,remember to backup the file!!!*/

class ICalFile;
class CalFileR1A;
class CalFileR1C;
class CfvCtrl
{
public:
	CfvCtrl(){}
	virtual ~CfvCtrl(){}
public:
	void Target(ICalFile *pFile);
public:
	int32_t ForConv_0000_0001();
	int32_t ForConv_0001_0002();
	int32_t ForConv_0002_0003();
	int32_t ForConv_0003_0004();
	int32_t ForConv_0004_0005();
	int32_t ForConv_0005_0006();
	int32_t ForwardConvert(int32_t iCurVer);
private:
	ICalFile *m_pFile;
};
#endif
