#ifndef __RX_FILTER_TABLE_H__
#define __RX_FILTER_TABLE_H__
#include "CalTable.h"

class RxRFFrTableR1C {
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        double   m_dPower;
	public:
		DataF();
    }DataF;
public:
    RxRFFrTableR1C(){}
    virtual ~RxRFFrTableR1C(){}
public:
	void virtual Add(DataF *pData);							
	void MapBuf(DataF *pBuf){m_pDataF = pBuf;}
	void Get(uint64_t uiFreq,DataF *pData);
	void SaveAs(char *pPath);
private:					
	DataF *m_pDataF;
};

class RxIFFrTableR1C
{
public:
	typedef struct DataF {
		double m_dFreq;
		double m_dPower;
	public:
		DataF();
	}DataF;
public:
    RxIFFrTableR1C(){}
    virtual ~RxIFFrTableR1C(){}
public:
	void virtual Add(DataF *pData);
	void MapBuf(DataF *pBuf){m_pDataF = pBuf;}
	void Get(double dFreq,DataF *pData);
	void SaveAs(char *pPath);
private:
	DataF *m_pDataF;
};

class RxFilter_80M_TableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        int16_t  m_iCoefReal[RX_FILTER_ORDER];
        int16_t  m_iCoefImag[RX_FILTER_ORDER];
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(DataM,4)
        CoefComplex m_Coef[RX_FILTER_ORDER];
    public:
        void _2Double(double *pReal,double *pImag);
    STRUCT_ALIGN_E(DataM,4)

    DECLARE_R1C_RX_CAL_ITEM_TABLE(RxFilter_80M_TableR1C)
public:
	void Get(uint64_t uiFreq,DataM *pData);
    void Get(uint64_t uiFreq,DataF *pData);
};

class RxFilter_160M_TableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        int16_t  m_iCoefReal[RX_FILTER_ORDER_2I3D];	//order 144 in file,maybe reserved for later change,but only 96 in memory
        int16_t  m_iCoefImag[RX_FILTER_ORDER_2I3D];
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(DataM,4)
        CoefComplex m_Coef[RX_FILTER_ORDER];		//order 96 in memory
    public:
        void _2Double(double *pReal,double *pImag);
    STRUCT_ALIGN_E(DataM,4)

    DECLARE_R1C_RX_CAL_ITEM_TABLE(RxFilter_160M_TableR1C)
public:
	void Get(uint64_t uiFreq,DataM *pData);
    void Get(uint64_t uiFreq,DataF *pData);
};

#endif
