#ifndef __RX_ATT_TABLE_H__
#define __RX_ATT_TABLE_H__
#include "CalTable.h"

class RxAttOPTableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        int64_t  m_iOffset[R1C_RX_REF_OP_PTS - 3 + 1];
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(DataM,4)
        int32_t m_iOffset[R1C_RX_REF_OP_PTS - 3 + 1];
        float   m_fTemp;
    STRUCT_ALIGN_E(DataM,4)

    DECLARE_R1C_RX_CAL_ITEM_TABLE(RxAttOPTableR1C)
public:
	int32_t Get(uint64_t uiFreq,double dRef);
	void Get(uint64_t uiFreq,DataM *pData);
	void Get(uint64_t uiFreq,DataF *pData);
};

class RxAttIOTableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        int64_t  m_iOffset[R1C_RX_REF_IO_PTS - 3 + 1];
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(DataM,4)
        int32_t m_iOffset[R1C_RX_REF_IO_PTS - 3 + 1];
        float   m_fTemp;
    STRUCT_ALIGN_E(DataM,4)

    DECLARE_R1C_RX_CAL_ITEM_TABLE(RxAttIOTableR1C)
public:
	int32_t Get(uint64_t uiFreq,double dRef);
	void Get(uint64_t uiFreq,DataM *pData);
	void Get(uint64_t uiFreq,DataF *pData);
};

#endif
