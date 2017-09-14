#ifndef __TX_FILTEROFF_TABLE_H__
#define __TX_FILTEROFF_TABLE_H__
#include "CalTable.h"

class TxFilterOffTableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        double   m_dOffset;
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
    }DataF;

    STRUCT_ALIGN_S(DataM,1)
        float m_fOffset;
        float m_fTemp5;
    STRUCT_ALIGN_E(DataM,1)

	DECLARE_TX_CAL_ITEM_TABLE(TxFilterOffTableR1C)
public:
	void Get(uint64_t uiFreq,DataM *pData);
	void Get(uint64_t uiFreq,DataF *pData);
};

#endif
