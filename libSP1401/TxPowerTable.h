#ifndef __TX_POWER_TABLE_H__
#define __TX_POWER_TABLE_H__
#include "CalTable.h"

//r1a/b
class TxPowerTableR1A
{
public:
	typedef struct TxStateF {
		uint64_t m_uiFreq;
		double	 m_dDGain;
		int32_t  m_iAtt1;
		int32_t  m_iAtt2;
		int32_t  m_iPAAtt;
		int32_t  m_iAtt3;
	}TxStateF;

	typedef struct DataF {
		TxStateF m_State[5];
	}DataF;

    STRUCT_ALIGN_S(TxStateM,1)
        float m_fDGain;
        short m_iAtt1   : 8;
        short m_iAtt2   : 8;
        short m_iPAAtt  : 8;
        short m_iAtt3   : 8;
    STRUCT_ALIGN_E(TxStateM,1)

    STRUCT_ALIGN_S(DataM,8)
        TxStateM m_State[5];
    STRUCT_ALIGN_E(DataM,8)

	DECLARE_TX_CAL_ITEM_TABLE(TxPowerTableR1A)
public:
	uint32_t GetPowerIndex(double dPower);
	static void GuessBaseOP(int32_t &iAtt1,int32_t &iAtt2,int32_t &iAtt3);
	static void GuessBaseIO(int32_t &iAtt1,int32_t &iAtt2,int32_t &iAtt3);
	static void Split(double dAtt,double *pRf,double *pBb);
	static void Split(int32_t iAtt,int32_t &iAtt1,int32_t &iAtt2,int32_t &iAtt3);
	void GetOP(uint64_t uiFreq,double dPower,double &dBBGain,int32_t &iAtt1,int32_t &iAtt2,int32_t &iPAAtt,int32_t &iAtt3);
	void GetIO(uint64_t uiFreq,double dPower,double &dBBGain,int32_t &iAtt1,int32_t &iAtt2,int32_t &iPAAtt,int32_t &iAtt3);
};

//r1c
class TxPowerOPTableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        double   m_dDGain;
        double   m_dAtt0;
        double   m_dAtt1;
        double   m_dAtt2;
        double   m_dAtt3;
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
    }DataF;

    STRUCT_ALIGN_S(DataM,1)
        float m_fDGain;
        float m_fTemp5;
        short m_sAtt0 : 8;	//att = m_sAttx / 2.0,so this can be a short
        short m_sAtt1 : 8;
        short m_sAtt2 : 8;
        short m_sAtt3 : 8;
    STRUCT_ALIGN_E(DataM,1)

	DECLARE_TX_CAL_ITEM_TABLE(TxPowerOPTableR1C)
public:
	static void GetConfigTable(int32_t iPower,DataM DataBase,DataM *pData);
	static void GuessBase(double &dAtt0,double &dAtt1,double &dAtt2,double &dAtt3,double &dDGain);
	void GetBase(uint64_t uiFreq,DataM *pData);
	void GetBase(uint64_t uiFreq,DataF *pData);
	void Get(uint64_t uiFreq,double dPower,DataM *pData);
	void Get(uint64_t uiFreq,double dPower,DataF *pData);
};

class TxPowerIOTableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
		double m_dDGain;
		double m_dAtt0;
		double m_dAtt1;
		double m_dAtt2;
		double m_dAtt3;
		double m_dTemp[4];
        tm     m_EndTime;
	public:
		DataF();
    }DataF;

    STRUCT_ALIGN_S(DataM,1)
        float m_fDGain;
        float m_fTemp5;
        short m_sAtt0 : 8;	//att = m_sAttx / 2.0,so this can be a short
        short m_sAtt1 : 8;
        short m_sAtt2 : 8;
        short m_sAtt3 : 8;
    STRUCT_ALIGN_E(DataM,1)

	DECLARE_TX_CAL_ITEM_TABLE(TxPowerIOTableR1C)
public:
	static void GetConfigTable(int32_t iPower,DataM DataBase,DataM *pData);
	static void GuessBase(double &dAtt0,double &dAtt1,double &dAtt2,double &dAtt3,double &dDGain);
	void GetBase(uint64_t uiFreq,DataM *pData);
	void GetBase(uint64_t uiFreq,DataF *pData);
	void Get(uint64_t uiFreq,double dPower,DataM *pData);
	void Get(uint64_t uiFreq,double dPower,DataF *pData);
};

#endif
