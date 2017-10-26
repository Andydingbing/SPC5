#ifndef __TX_LOLEAKAGE_TABLE_H__
#define __TX_LOLEAKAGE_TABLE_H__
#include "CalTable.h"

//r1a/b
class TxLOLeakageTableR1A
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
		int16_t  m_iDCI;
		int16_t  m_iDCQ;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(DataM,2)
        int16_t m_iDCI : 16;
        int16_t m_iDCQ : 16;
    STRUCT_ALIGN_E(DataM,2)

	DECLARE_TX_CAL_ITEM_TABLE(TxLOLeakageTableR1A)
public:
	void Get(uint64_t uiFreq,int16_t &iDCI,int16_t &iDCQ);
};

//r1c
class TxLOLeakageTableR1C
{
public:
	typedef struct DataF : public TxLOLeakageTableR1A::DataF{
        bool   m_bUseSA;	//use sa or loopback
		double m_dPower;	//lo leakage power
		double m_dTemp[4];
        tm     m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_INHERIT_S(DataM,2,TxLOLeakageTableR1A::DataM)
        float m_fTemp5;	//hmc1197 temperature
    STRUCT_ALIGN_INHERIT_E(DataM,2)
public:
	TxLOLeakageTableR1C();
	virtual ~TxLOLeakageTableR1C();
public:
	void virtual Add(DataF *pData);
	void Map2Mem();
	void MapBuf(DataF *pBuf){m_pDataF = pBuf;}
	void SaveAs(char *pPath);
public:
	void Get(uint64_t uiFreq,int16_t &iDCI,int16_t &iDCQ,float &fTemp5);
	void Get(uint64_t uiFreq,DataM *pData);
	void Get(uint64_t uiFreq,DataF *pData);
private:
	DataF *m_pDataF;
	DataM *m_pDataM;
};

//x9119 r1a/b
class X9119Table
{
public:
	typedef struct Data {
		int16_t m_iValue;
	}Data;
public:
	void Add(Data *pData);
	void MapBuf(Data *pBuf){m_pData = pBuf;}
	void Map2Mem();
	void Get(int16_t &iValue);
	void SaveAs(char *pPath);
private:
	Data *m_pData;
	Data m_DataM[1];
};

#endif
