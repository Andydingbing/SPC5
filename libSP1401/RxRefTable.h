#ifndef __RX_REF_TABLE_H__
#define __RX_REF_TABLE_H__
#include "CalTable.h"

#define AVERAGE_TIMES 1

//r1a/b
class RxRefTableR1A
{
public:
	typedef struct RxStateF {
		int64_t m_iADOffset;
		int32_t m_iLNAAtt;
		double  m_dAtt1;
		int32_t m_iAtt2;
	}RxStateF;

	typedef struct DataF {
		uint64_t m_uiFreq;
        RxStateF m_StateOP[R1A_RX_REF_OP_PTS];
        RxStateF m_StateIO[R1A_RX_REF_IO_PTS];
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(RxStateM,1)
        float   m_fAtt1;
        int32_t m_iADOffset;
        short   m_iAtt2		: 8;
        short   m_iLNAAtt	: 8;
    STRUCT_ALIGN_E(RxStateM,1)

    STRUCT_ALIGN_S(DataM,2)
        RxStateM m_StateOP[R1A_RX_REF_OP_PTS];
        RxStateM m_StateIO[R1A_RX_REF_IO_PTS];
    STRUCT_ALIGN_E(DataM,2)

    DECLARE_R1A_RX_CAL_ITEM_TABLE(RxRefTableR1A)
public:
	static int32_t GetSGPower(int32_t iRef);
	static void GuessBaseOP(int32_t &iLNAAtt,double &dAtt1,int32_t &iAtt2);
	static void GuessBaseIO(int32_t &iLNAAtt,double &dAtt1,int32_t &iAtt2);
    void Get(uint64_t uiFreq,double dRef,IOMode Mode,int64_t &iAD_0dBFS,SP1401::RxLNAAtt &LNAAtt,double &dAtt1,int32_t &iAtt2);
	void GetOP(uint64_t uiFreq,double dRef,int64_t &iAD_0dBFS,int32_t &iLNAAtt,double &dAtt1,int32_t &iAtt2);
	void GetIO(uint64_t uiFreq,double dRef,int64_t &iAD_0dBFS,int32_t &iLNAAtt,double &dAtt1,int32_t &iAtt2);
};

//r1c
class RxRefOPTableR1C
{
public:
	typedef struct RxStateF {
		int16_t m_iLNAAtt;
		int16_t m_iAtt019;
		float	m_fAtt1;
		float	m_fAtt2;
		float	m_fAtt3;
		double	m_dTemp[4];
		int64_t m_iADOffset;
	}RxStateF;

	typedef struct DataF {
		uint64_t m_uiFreq;
		RxStateF m_State[3];
        tm       m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(RxStateM,1)
        float	m_fAtt1;
        float	m_fAtt2;
        float	m_fAtt3;
        float	m_fTemp;
        int32_t m_iADOffset;
        short	m_sLNAAtt : 8;
        short	m_sAtt019 : 8;
    STRUCT_ALIGN_E(RxStateM,1)

    STRUCT_ALIGN_S(DataM,2)
        RxStateM m_State[3];
    STRUCT_ALIGN_E(DataM,2)

    DECLARE_R1C_RX_CAL_ITEM_TABLE(RxRefOPTableR1C)
public:
	static void GetConfigTable(int32_t iRef,DataM DataRef,RxStateM *pState);
	static void GuessBase(int32_t iIdx,int32_t &iRef,int32_t &iLNAAtt,int32_t &iAtt019,double &dAtt1,double &dAtt2,double &dAtt3);	//iIdx means m_State[0]/[1]/[2]
	static int32_t GetRefSegIdx(double dRef);
	void GetBase(uint64_t uiFreq,DataM *pData);
	void GetBase(uint64_t uiFreq,DataF *pData);
	void Get(uint64_t uiFreq,double dRef,RxStateM *pState);
    void Get(uint64_t uiFreq,double dRef,RxStateF *pState,tm *pTime = NULL);
};

class RxRefIOTableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
		RxRefOPTableR1C::RxStateF m_State[3];
        tm       m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(DataM,2)
        RxRefOPTableR1C::RxStateM m_State[3];
    STRUCT_ALIGN_E(DataM,2)

    DECLARE_R1C_RX_CAL_ITEM_TABLE(RxRefIOTableR1C)
public:
	static void GetConfigTable(int32_t iRef,DataM DataRef,RxRefOPTableR1C::RxStateM *pState);
	static void GuessBase(int32_t iIdx,int32_t &iRef,int32_t &iLNAAtt,int32_t &iAtt019,double &dAtt1,double &dAtt2,double &dAtt3);
	static int32_t GetRefSegIdx(double dRef);
	void GetBase(uint64_t uiFreq,DataM *pData);
	void GetBase(uint64_t uiFreq,DataF *pData);
	void Get(uint64_t uiFreq,double dRef,RxRefOPTableR1C::RxStateM *pState);
    void Get(uint64_t uiFreq,double dRef,RxRefOPTableR1C::RxStateF *pState,tm *pTime = NULL);
};

#endif
