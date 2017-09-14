#ifndef __RX_FILTEROFF_TABLE_H__
#define __RX_FILTEROFF_TABLE_H__
#include "CalTable.h"

class RxFilterOffTableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        int64_t  m_iOffset[3];
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
    }DataF;

    STRUCT_ALIGN_S(DataM,1)
        int32_t m_iOffset[3];
        float   m_fTemp5;
    STRUCT_ALIGN_E(DataM,1)

    DECLARE_R1C_RX_CAL_ITEM_TABLE(RxFilterOffTableR1C)
public:
	void Get(uint64_t uiFreq,DataM *pData);
	void Get(uint64_t uiFreq,DataF *pData);
};

#endif
