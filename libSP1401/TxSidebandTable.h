#ifndef __TX_SIDEBAND_TABLE_H__
#define __TX_SIDEBAND_TABLE_H__
#include "CalTable.h"

//r1a/b
class TxSidebandTableR1A
{
public:
    typedef struct DataF {
        uint64_t m_uiFreq;
        double   m_dTh;
        uint16_t m_uiAmI;
        uint16_t m_uiAmQ;
    public:
        DataF();
    }DataF;

    STRUCT_ALIGN_S(DataM,2)
        float    m_fTh;
        uint16_t m_uiAmI : 16;
        uint16_t m_uiAmQ : 16;
    STRUCT_ALIGN_E(DataM,2)

    DECLARE_TX_CAL_ITEM_TABLE(TxSidebandTableR1A)
public:
	void Get(uint64_t uiFreq,double &dTh,uint16_t &uiAmI,uint16_t &uiAmQ);
};

//r1c
class TxSidebandTableR1C {
public:
    typedef struct DataF : public TxSidebandTableR1A::DataF {
        bool   m_bUseSA;	//use sa or loopback
		double m_dPower;	//sideband power
		double m_dTemp[4];
        tm     m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_INHERIT_S(DataM,2,TxSidebandTableR1A::DataM)
        float m_fTemp5;	//hmc1197 temperature
    STRUCT_ALIGN_INHERIT_E(DataM,2)
public:
	TxSidebandTableR1C();
	virtual ~TxSidebandTableR1C();
public:
	void virtual Add(DataF *pData);
	void Map2Mem();
	void MapBuf(DataF *pBuf){m_pDataF = pBuf;}
	void SaveAs(char *pPath);
public:
	void Get(uint64_t uiFreq,double &dTh,uint16_t &uiAmI,uint16_t &uiAmQ,float &fTemp5);
	void Get(uint64_t uiFreq,DataM *pData);
	void Get(uint64_t uiFreq,DataF *pData);
private:
	DataF *m_pDataF;
	DataM *m_pDataM;
};

#endif
