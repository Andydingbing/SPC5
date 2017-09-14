#ifndef __CAL_TABLE_H__
#define __CAL_TABLE_H__

#include <time.h>
#include "libLog.h"
#include "libBusDriver.h"

#define MAX_RFU 5
#define MAX_RF	4
#define MAX_CEU 2

#define ADF5355_FREQ_SPACE	6250
#define RF_FREQ_SPACE		100000

#define RF_SN_LENGTH 24
#define PM_IL 6.7   //Z28 Insertion Loss
#define _0dBFS  80000000LL

#define TX_FILTER_ORDER_2I          41
#define RX_FILTER_ORDER             96
#define RX_FILTER_ORDER_2I3D        144

#define RF_TX_FREQ_STAR             50000000LL		//r1a/b/c/d
#define RF_TX_FREQ_STOP             6000000000LL	//r1a/b/c/d
#define RF_TX_FREQ_STEP_CALLED      10000000LL		//r1a/b/c/d
#define RF_TX_FREQ_STEP_INFILE      5000000LL		//r1a/b/c/d
#define RF_TX_FREQ_PTS_CALLED       (SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_CALLED))
#define RF_TX_FREQ_PTS_INFILE       (SERIE_SIZE(RF_TX_FREQ_STAR,RF_TX_FREQ_STOP,RF_TX_FREQ_STEP_INFILE))

#define RF_TX_FILTER_FREQ_STAR_0	17000000LL		//(100  - 83) * 1000000
#define RF_TX_FILTER_FREQ_STOP_0	3183000000LL	//(3100 + 83) * 1000000
#define RF_TX_FILTER_FREQ_STAR_1	3017000000LL	//(3100 - 83) * 1000000
#define RF_TX_FILTER_FREQ_STOP_1	6083000000LL	//(6000 + 83) * 1000000
#define RF_TX_FILTER_FREQ_STEP		2000000LL
#define RF_TX_FILTER_PTS_0			(SERIE_SIZE(RF_TX_FILTER_FREQ_STAR_0,RF_TX_FILTER_FREQ_STOP_0,RF_TX_FILTER_FREQ_STEP))
#define RF_TX_FILTER_PTS_1			(SERIE_SIZE(RF_TX_FILTER_FREQ_STAR_1,RF_TX_FILTER_FREQ_STOP_1,RF_TX_FILTER_FREQ_STEP))
#define RF_TX_FILTER_PTS			(RF_TX_FILTER_PTS_0 + RF_TX_FILTER_PTS_1)

#define R1A_TX_ATT_STEP_MIN         10
#define R1A_TX_ATT_STEP_MAX         10
#define R1A_TX_ATT_DR               90
#define R1A_DGAIN_MAX               8
#define R1A_DGAIN_MIN               -4

#define R1A_TX_POWER_STAR           10
#define R1A_TX_POWER_STOP           -20
#define R1A_TX_POWER_STEP           -10

#define R1C_TX_ATT_STEP             -1
#define R1C_TX_ATT_OP_POWER_STAR    10
#define R1C_TX_ATT_OP_POWER_STOP    -90
#define R1C_TX_ATT_OP_POWER_PTS     (SERIE_SIZE(R1C_TX_ATT_OP_POWER_STAR,R1C_TX_ATT_OP_POWER_STOP,R1C_TX_ATT_STEP) - 1)
#define R1C_TX_ATT_IO_POWER_STAR    0
#define R1C_TX_ATT_IO_POWER_STOP    -97
#define R1C_TX_ATT_IO_POWER_PTS     (SERIE_SIZE(R1C_TX_ATT_IO_POWER_STAR,R1C_TX_ATT_IO_POWER_STOP,R1C_TX_ATT_STEP) - 1)

#define R1C_TX_BASE_POWER_OP        -5.0
#define R1C_TX_BASE_POWER_IO        -12.0
#define R1C_TX_ATT_STEP_MIN         0.5
#define R1C_DGAIN_MAX               2
#define R1C_DGAIN_MIN               -8

#define IF_TX_FILTER_160M_FREQ_STAR	-83000000.0     //(160 / 2) + 1.5 * 2
#define IF_TX_FILTER_160M_FREQ_STOP	83000000.0
#define IF_TX_FILTER_80M_FREQ_STAR  -43000000.0     //(80  / 2) + 1.5 * 2
#define IF_TX_FILTER_80M_FREQ_STOP	43000000.0
#define IF_TX_FILTER_FREQ_STEP      2000000.0
#define IF_TX_FILTER_160M_PTS       (SERIE_SIZE(IF_TX_FILTER_160M_FREQ_STAR,IF_TX_FILTER_160M_FREQ_STOP,IF_TX_FILTER_FREQ_STEP))
#define IF_TX_FILTER_80M_PTS        (SERIE_SIZE(IF_TX_FILTER_80M_FREQ_STAR,IF_TX_FILTER_80M_FREQ_STOP,IF_TX_FILTER_FREQ_STEP))

#define RF_RX_FREQ_STAR             100000000LL		//r1a/b/c/d
#define RF_RX_FREQ_STOP             6000000000LL	//r1a/b/c/d
#define RF_RX_FREQ_STEP_CALLED_R1A  5000000LL		//r1a/b fixed
#define RF_RX_FREQ_STEP_CALLED_R1C  10000000LL		//r1c/d
#define RF_RX_FREQ_STEP_INFILE      5000000LL		//r1c/d
#define RF_RX_FREQ_PTS_CALLED_R1A   (SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1A))
#define RF_RX_FREQ_PTS_CALLED_R1C   (SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_CALLED_R1C))
#define RF_RX_FREQ_PTS_INFILE       (SERIE_SIZE(RF_RX_FREQ_STAR,RF_RX_FREQ_STOP,RF_RX_FREQ_STEP_INFILE))

#define RF_RX_FILTER_FREQ_STAR      17000000LL		//(100  - 83) * 1000000
#define RF_RX_FILTER_FREQ_STOP      6083000000LL	//(6000 + 83) * 1000000
#define RF_RX_FILTER_FREQ_STEP      2000000LL
#define RF_RX_FILTER_PTS            (SERIE_SIZE(RF_RX_FILTER_FREQ_STAR,RF_RX_FILTER_FREQ_STOP,RF_RX_FILTER_FREQ_STEP))

#define R1A_RX_REF_STEP             -1
#define R1A_RX_REF_OP_STAR          30
#define R1A_RX_REF_OP_STOP          -40
#define R1A_RX_REF_OP_PTS           (SERIE_SIZE(R1A_RX_REF_OP_STAR,R1A_RX_REF_OP_STOP,R1A_RX_REF_STEP))
#define R1A_RX_REF_IO_STAR          30
#define R1A_RX_REF_IO_STOP          -30
#define R1A_RX_REF_IO_PTS           (SERIE_SIZE(R1A_RX_REF_IO_STAR,R1A_RX_REF_IO_STOP,R1A_RX_REF_STEP))

#define R1C_RX_REF_STEP             -1
#define R1C_RX_REF_OP_STAR          30
#define R1C_RX_REF_OP_STOP          -40
#define R1C_RX_REF_OP_PTS           (SERIE_SIZE(R1C_RX_REF_OP_STAR,R1C_RX_REF_OP_STOP,R1C_RX_REF_STEP))
#define R1C_RX_REF_IO_STAR          30
#define R1C_RX_REF_IO_STOP          -30
#define R1C_RX_REF_IO_PTS           (SERIE_SIZE(R1C_RX_REF_IO_STAR,R1C_RX_REF_IO_STOP,R1C_RX_REF_STEP))

#define IF_RX_FILTER_160M_FREQ_STAR -83000000.0     //(160 / 2) + 1.5 * 2
#define IF_RX_FILTER_160M_FREQ_STOP	83000000.0
#define IF_RX_FILTER_80M_FREQ_STAR	-43000000.0     //(80  / 2) + 1.5 * 2
#define IF_RX_FILTER_80M_FREQ_STOP	43000000.0
#define IF_RX_FILTER_FREQ_STEP      2000000.0
#define IF_RX_FILTER_160M_PTS       (SERIE_SIZE(IF_RX_FILTER_160M_FREQ_STAR,IF_RX_FILTER_160M_FREQ_STOP,IF_RX_FILTER_FREQ_STEP))
#define IF_RX_FILTER_80M_PTS        (SERIE_SIZE(IF_RX_FILTER_80M_FREQ_STAR,IF_RX_FILTER_80M_FREQ_STOP,IF_RX_FILTER_FREQ_STEP))

#define DECLARE_TX_CAL_ITEM_TABLE(class_name)                   \
    public:                                                     \
        class_name() {                                          \
            m_pDataM = new DataM[RF_TX_FREQ_PTS_CALLED];        \
        }                                                       \
        virtual ~class_name() {                                 \
            if (m_pDataM) delete []m_pDataM;                    \
        }                                                       \
    public:                                                     \
        void virtual Add(DataF *pData);                         \
        void Map2Mem();                                         \
        void MapBuf(DataF *pBuf){m_pDataF = pBuf;}              \
        void SaveAs(char *pPath);                               \
    private:                                                    \
        DataF *m_pDataF;                                        \
		DataM *m_pDataM;

#define IMPLEMENT_TX_CAL_ITEM_TABLE(class_name)																\
    void class_name::Add(DataF *pData)																		\
        {																									\
            uint32_t uiIdx = (uint32_t)SERIE_INDEX(pData->m_uiFreq,RF_TX_FREQ_STAR,RF_TX_FREQ_STEP_INFILE); \
            m_pDataF[uiIdx] = *pData;																		\
		}

//r1a/b
#define DECLARE_R1A_RX_CAL_ITEM_TABLE(class_name)               \
    public:                                                     \
        class_name() {                                          \
            m_pDataM = new DataM[RF_RX_FREQ_PTS_CALLED_R1A];    \
        }                                                       \
        virtual ~class_name() {                                 \
            if (m_pDataM) delete []m_pDataM;                    \
        }                                                       \
    public:                                                     \
        void virtual Add(DataF *pData);                         \
        void Map2Mem();                                         \
        void MapBuf(DataF *pBuf){m_pDataF = pBuf;}              \
        void SaveAs(char *pPath);                               \
    private:                                                    \
        DataF *m_pDataF;                                        \
		DataM *m_pDataM;

#define IMPLEMENT_R1A_RX_CAL_ITEM_TABLE(class_name)															\
    void class_name::Add(DataF *pData)																		\
        {																									\
            uint32_t uiIdx = (uint32_t)SERIE_INDEX(pData->m_uiFreq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE); \
            m_pDataF[uiIdx] = *pData;																		\
		}

//r1c/d
#define DECLARE_R1C_RX_CAL_ITEM_TABLE(class_name)				\
    public:														\
        class_name() {                                          \
            m_pDataM = new DataM[RF_RX_FREQ_PTS_CALLED_R1C];    \
        }														\
        virtual ~class_name() {									\
            if (m_pDataM) delete []m_pDataM;					\
        }														\
    public:                                                     \
        void virtual Add(DataF *pData);                         \
        void Map2Mem();                                         \
        void MapBuf(DataF *pBuf){m_pDataF = pBuf;}              \
        void SaveAs(char *pPath);                               \
    private:                                                    \
        DataF *m_pDataF;                                        \
        DataM *m_pDataM;

#define IMPLEMENT_R1C_RX_CAL_ITEM_TABLE(class_name)															\
    void class_name::Add(DataF *pData)																		\
        {																									\
            uint32_t uiIdx = (uint32_t)SERIE_INDEX(pData->m_uiFreq,RF_RX_FREQ_STAR,RF_RX_FREQ_STEP_INFILE); \
            m_pDataF[uiIdx] = *pData;																		\
        }

namespace SP1401
{
    enum TxPAAtt {  //r1a/b
        Tx_PA = 0,
        Tx_Att
    };
    enum RxLNAAtt { //r1a/b/c/d
        Rx_Att = 0,
        Rx_LNA
    };
    enum RxAtt019 {	//r1c/d
        Rx_Att_19 = 0,
        Rx_Att_0
    };
    enum IOMode {
		IO = 0,
		OutPut,
		Loop
	};
	enum BW {
		_80M,
		_160M
	};

    STRUCT_ALIGN_S(CoefComplex,2)
        int16_t m_iReal : 16;
        int16_t m_iImag : 16;
    STRUCT_ALIGN_E(CoefComplex,2)
}

using namespace std;
using namespace SP1401;

#endif
