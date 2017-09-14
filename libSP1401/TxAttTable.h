#ifndef __TX_ATT_TABLE_H__
#define __TX_ATT_TABLE_H__
#include "CalTable.h"

class TxAttTableR1A
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
		float	 m_fAtt10;
		float	 m_fAtt20;
		float	 m_fAtt30;
		float	 m_fAtt40;
		float	 m_fAtt50;
		float	 m_fAtt60;
		float	 m_fAtt70;
		float	 m_fAtt80;
		float	 m_fAtt90;
	}DataF;

    STRUCT_ALIGN_S(DataM,4)
        float m_fAtt[9];
    STRUCT_ALIGN_E(DataM,4)

	DECLARE_TX_CAL_ITEM_TABLE(TxAttTableR1A)
public:
	void Get(uint64_t uiFreq, float *pAtt);
	void Get(uint64_t uiFreq, int32_t iAtt,double &dOffset);
};

//r1c
class TxAttOPTableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        float    m_fOffset[R1C_TX_ATT_OP_POWER_PTS];
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(DataM,4)
        float m_fOffset[R1C_TX_ATT_OP_POWER_PTS];
        float m_fTemp5;
    STRUCT_ALIGN_E(DataM,4)

	DECLARE_TX_CAL_ITEM_TABLE(TxAttOPTableR1C)
public:
	float Get(uint64_t uiFreq,double dPower);
	void Get(uint64_t uiFreq,DataM *pData);
	void Get(uint64_t uiFreq,DataF *pData);
};

class TxAttIOTableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        float    m_fOffset[R1C_TX_ATT_IO_POWER_PTS];
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(DataM,4)
        float m_fOffset[R1C_TX_ATT_IO_POWER_PTS];
        float m_fTemp5;
    STRUCT_ALIGN_E(DataM,4)

	DECLARE_TX_CAL_ITEM_TABLE(TxAttIOTableR1C)
public:
	float Get(uint64_t uiFreq,double dPower);
	void Get(uint64_t uiFreq,DataM *pData);
	void Get(uint64_t uiFreq,DataF *pData);
};

#endif
