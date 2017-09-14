#ifndef __TX_FILTER_TABLE_H__
#define __TX_FILTER_TABLE_H__
#include "CalTable.h"

class TxRFFrTable
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        double   m_dPower;
	public:
		DataF();
	}DataF;
public:
    TxRFFrTable(){}
    virtual ~TxRFFrTable(){}
protected:
	DataF *m_pDataF;
};

class TxRFFr_0_TableR1C : public TxRFFrTable
{
public:
	void virtual Add(TxRFFrTable::DataF *pData);							
	void MapBuf(TxRFFrTable::DataF *pBuf){m_pDataF = pBuf;}
	void Get(uint64_t uiFreq,TxRFFrTable::DataF *pData);
	void SaveAs(char *pPath);
};

class TxRFFr_1_TableR1C : public TxRFFrTable
{
public:
	void virtual Add(TxRFFrTable::DataF *pData);
	void MapBuf(TxRFFrTable::DataF *pBuf){m_pDataF = pBuf;}
	void Get(uint64_t uiFreq,TxRFFrTable::DataF *pData);
	void SaveAs(char *pPath);
};

class TxIFFrTableR1C
{
public:
	typedef struct DataF {
		double m_dFreq;
		double m_dPower;
	public:
		DataF();
	}DataF;
public:
    TxIFFrTableR1C(){}
    virtual ~TxIFFrTableR1C(){}
public:
	void virtual Add(DataF *pData);
	void MapBuf(DataF *pBuf){m_pDataF = pBuf;}
	void Get(double dFreq,DataF *pData);
	void SaveAs(char *pPath);
private:
	DataF *m_pDataF;
};

class TxFilter_80M_TableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        int16_t  m_iCoefReal[TX_FILTER_ORDER_2I];
        int16_t  m_iCoefImag[TX_FILTER_ORDER_2I];
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(DataM,4)
        CoefComplex m_Coef[TX_FILTER_ORDER_2I];
    public:
        void _2Double(double *pReal,double *pImag);
    STRUCT_ALIGN_E(DataM,4)

	DECLARE_TX_CAL_ITEM_TABLE(TxFilter_80M_TableR1C)
public:
	void Get(uint64_t uiFreq,DataM *pData);
	void Get(uint64_t uiFreq,DataF *pData);
    static void Coef2Str(int16_t *pCoef,int32_t iOrder,char *pBuf);
};

class TxFilter_160M_TableR1C
{
public:
	typedef struct DataF {
		uint64_t m_uiFreq;
        int16_t  m_iCoefReal[TX_FILTER_ORDER_2I];
        int16_t  m_iCoefImag[TX_FILTER_ORDER_2I];
        double   m_dTemp[4];
        tm       m_EndTime;
	public:
		DataF();
	}DataF;

    STRUCT_ALIGN_S(DataM,4)
        CoefComplex m_Coef[TX_FILTER_ORDER_2I];
    public:
        void _2Double(double *pReal,double *pImag);
    STRUCT_ALIGN_E(DataM,4)

	DECLARE_TX_CAL_ITEM_TABLE(TxFilter_160M_TableR1C)
public:
	void Get(uint64_t uiFreq,DataM *pData);
	void Get(uint64_t uiFreq,DataF *pData);
};

#endif
