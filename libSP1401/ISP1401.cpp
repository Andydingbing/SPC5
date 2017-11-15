#include "ISP1401.h"
#include "RegDef.h"
#include <string.h>
#ifdef _WIN64
#include <windows.h>
#elif defined(__unix__) || defined(__linux__)
#include <unistd.h>
#endif

ISP1401::ISP1401()
{
    m_pK7 = NULL;
    m_pArbReader = new ArbReader;
}

ISP1401::~ISP1401()
{
    SAFE_DELETE(m_pCalFile);
    SAFE_DELETE(m_pArbReader);
}

BW ISP1401::GetBw()
{
    return m_pCalFile->GetBw();
}

void ISP1401::SetEnableExpt(bool bEnable)
{
    m_bEnExpt = bEnable;
}

uint32_t ISP1401::GetRfIdx()
{
    return m_pCalFile->GetRfIdx();
}

uint32_t ISP1401::GetRfuIdx()
{
    return m_pCalFile->GetRfuIdx();
}

ICalFile* ISP1401::GetCalFile()
{
    return m_pCalFile;
}

viPCIDev* ISP1401::GetK7()
{
    return m_pK7;
}

int32_t ISP1401::Connect(viPCIDev *pK7)
{
    m_pK7 = pK7;
    return 0;
}

int32_t ISP1401::GetHwVer(const char *pSN,HwVer &Ver)
{
    if (pSN[6] == 'R' && pSN[7] == '1') {
        if (pSN[8] == 'A')
            Ver = R1A;
        else if (pSN[8] == 'B')
            Ver = R1B;
        else if (pSN[8] == 'C')
            Ver = R1C;
        else {
            Ver = HwError;
            return -1;
        }
    }
    else {
        Ver = HwError;
        return -1;
    }
    return 0;
}

int32_t ISP1401::GetK7Ver(uint32_t &uiK7Ver)
{
    RFU_K7_REG_DECLARE(0x0000);
    RFU_K7_R(0x0000);
    uiK7Ver = RFU_K7_REG_U32(0x0000);
    return 0;
}

int32_t ISP1401::SetSN(const char *pRfVer,int32_t iOrdinal)
{
    char szSN[32];
    memset(szSN,0,32);
    uint32_t uiRfIdx = m_pCalFile->GetRfIdx();
    uint32_t uiRfuIdx = m_pCalFile->GetRfuIdx();
    sprintf(szSN,"%9s%08dRFU%dRF%d",pRfVer,iOrdinal,uiRfuIdx,uiRfIdx);
    INT_CHECK(WriteEEPROM(0x0000,24,szSN));
    return 0;
}

int32_t ISP1401::SetSN(char *pSN)
{
    INT_CHECK(WriteEEPROM(0x0000,24,pSN));
    return 0;
}

int32_t ISP1401::GetSN(char *pSN)
{
    INT_CHECK(ReadEEPROM(0x0000,24,pSN));
    return 0;
}

ArbReader *ISP1401::GetArbReader()
{
    return m_pArbReader;
}

int32_t ISP1401::IsValidSN(const char *pSN)
{
    int32_t i = 0;
    if (!strcmp(pSN,""))
        return SN_NULL;
    for (i = 0;i < RF_SN_LENGTH;i ++) {
        if (pSN[i] != '0')
            break;
    }
    if (RF_SN_LENGTH - 1 == i)
        return SN_NULL;

    bool bValid = false;
    char szSNHwVer[8],szSNSlot[8],szSNSlotTemp[8];

    for (uint32_t i = 0;i < sizeof(szSNHwVer);i ++)
        szSNHwVer[i] = pSN[i];
    for (uint32_t i = 0;i < sizeof(szSNSlot);i ++)
        szSNSlot[i] = pSN[17 + i];
    memset(szSNSlotTemp,0,sizeof(szSNSlotTemp));

    if (!strstr(szSNHwVer,"SP1401R1")) {
        Log.SetLastError("sn err:%24s",pSN);
        return SN_WRONG;
    }
    for (int i = 0;i < MAX_RFU;i ++) {
        for (int j = 0;j < MAX_RF;j ++) {
            sprintf(szSNSlotTemp,"RFU%dRF%d",i,j);
            if (strstr(szSNSlot,szSNSlotTemp)) {
                bValid = true;
                break;
            }
        }
        if (bValid)
            break;
    }
    if (bValid == false) {
        Log.SetLastError("sn err:%24s",pSN);
        return SN_WRONG;
    }
    return SN_RIGHT;
}

int32_t ISP1401::SetArbSegment(const ArbSeg_t &Seg)
{
    RFU_K7_REG_DECLARE_2(0x0503,0x0603);
    RFU_K7_REG_DECLARE_2(0x0504,0x0604);

    union as_reg_t as_reg[16] = { 0 };

    AS_REG(0).sample       = Seg.samples;
    AS_REG(1).seg_adr      = Seg.seg_adr;
    AS_REG(2).inter_fltr   = Seg.inter_fltr;

    AS_REG(3).mkr1_start   = Seg.markers[0].marker.period.start;
    AS_REG(4).mkr1_keep    = Seg.markers[0].marker.period.keep;
    AS_REG(5).mkr1_period  = Seg.markers[0].marker.period.period;

    AS_REG(6).mkr2_start   = Seg.markers[1].marker.period.start;
    AS_REG(7).mkr2_keep    = Seg.markers[1].marker.period.keep;
    AS_REG(8).mkr2_period  = Seg.markers[1].marker.period.period;

    AS_REG(9).mkr3_start   = Seg.markers[2].marker.period.start;
    AS_REG(10).mkr3_keep   = Seg.markers[2].marker.period.keep;
    AS_REG(11).mkr3_period = Seg.markers[2].marker.period.period;

    AS_REG(12).mkr4_start  = Seg.markers[3].marker.period.start;
    AS_REG(13).mkr4_keep   = Seg.markers[3].marker.period.keep;
    AS_REG(14).mkr4_period = Seg.markers[3].marker.period.period;

    AS_REG(16).sr = Seg.sr;

    if (Seg.markers[0].type >= 0) {
        AS_REG(17).mkr1_onoff = 1;
        AS_REG(17).mkr1_type = Seg.markers[0].type;
    }
    if (Seg.markers[1].type >= 0) {
        AS_REG(17).mkr2_onoff = 1;
        AS_REG(17).mkr2_type = Seg.markers[1].type;
    }
    if (Seg.markers[2].type >= 0) {
        AS_REG(17).mkr3_onoff = 1;
        AS_REG(17).mkr3_type = Seg.markers[2].type;
    }
    if (Seg.markers[3].type >= 0) {
        AS_REG(17).mkr4_onoff = 1;
        AS_REG(17).mkr4_type = Seg.markers[3].type;
    }

    for (int i = 0;i < 18;i ++) {
        RFU_K7_REG_2(0x0503,0x0603).adr = i;
        RFU_K7_REG_2(0x0503,0x0603).seg = Seg.seg_num;
        RFU_K7_REG_2(0x0504,0x0604).data = as_reg[i].data;
        RFU_K7_W_2(0x0504,0x0604);
        RFU_K7_OP_2(0x0503,0x0603);
    }

    for (int no = 0;no < 4;no ++) {
        if (1 == Seg.markers[no].type) {
            for (int i = 0;i < Seg.markers[no].marker.unperiod.count;i ++) {
                SetArbUnperiodAddMarker(no,
                    Seg.markers[no].marker.unperiod.start[i],
                    Seg.markers[no].marker.unperiod.keep[i]);
            }
        }
    }
    return 0;
}

int32_t ISP1401::SetArbSegments(uint16_t uiSeg)
{
    RFU_K7_REG_DECLARE_2(0x0505,0x0605);
    RFU_K7_REG_2(0x0505,0x0605).seg = uiSeg;
    RFU_K7_W_2(0x0505,0x0605);
    return 0;
}

int32_t ISP1401::SetArbParam(uint32_t uiAddSamp,uint32_t uiCycl,uint32_t uiRepMo)
{
    RFU_K7_REG_DECLARE_2(0x0502,0x0602);
    RFU_K7_REG_2(0x0502,0x0602).addition   = uiAddSamp;
    RFU_K7_REG_2(0x0502,0x0602).cycles	    = uiCycl;
    RFU_K7_REG_2(0x0502,0x0602).repetition = uiRepMo;
    RFU_K7_W_2(0x0502,0x0602);
    return 0;
}

int32_t ISP1401::SetArbTrigger(bool bReTrigger,bool bAutoStart,uint32_t uiSrc,uint32_t uiTrigDelay)
{
    RFU_K7_REG_DECLARE_2(0x050a,0x060a);
    RFU_K7_REG_2(0x050a,0x060a).retrig		= bReTrigger;
    RFU_K7_REG_2(0x050a,0x060a).delay		= uiTrigDelay;
    RFU_K7_REG_2(0x050a,0x060a).autostart	= bAutoStart;
    RFU_K7_REG_2(0x050a,0x060a).src		= uiSrc;
    RFU_K7_W_2(0x050a,0x060a);
    return 0;
}

int32_t ISP1401::SetArbManualTrigger()
{
    RFU_K7_REG_DECLARE_2(0x050b,0x060b);
    RFU_K7_OP_2(0x050b,0x060b);
    return 0;
}

int32_t ISP1401::SetArbMultiSegMode(uint32_t uiMSTS, uint32_t uiMSRM)
{
    RFU_K7_REG_DECLARE_2(0x050c,0x060c);
    RFU_K7_REG_2(0x050c,0x060c).ms_trig = uiMSTS;
    RFU_K7_REG_2(0x050c,0x060c).ms_rm	 = uiMSRM;
    RFU_K7_W_2(0x050c,0x060c);
    return 0;
}

int32_t ISP1401::SetArbMultiSegNext(uint32_t uiNextSeg)
{
    RFU_K7_REG_DECLARE_2(0x050d,0x060d);
    RFU_K7_REG_2(0x050d,0x060d).next_seg = uiNextSeg;
    RFU_K7_OP_2(0x050d,0x060d);
    return 0;
}

uint32_t ISP1401::GetArbCurrentSeg()
{
    RFU_K7_REG_DECLARE_2(0x050e,0x060e);
    RFU_K7_R_2(0x050e,0x060e);
    return RFU_K7_REG_2(0x050e,0x060e).seg;
}

uint32_t ISP1401::GetArbCurrentSample()
{
    RFU_K7_REG_DECLARE_2(0x0540,0x0640);
    RFU_K7_R_2(0x0540,0x0640);
    return RFU_K7_REG_2(0x0540,0x0640).sample;
}

uint32_t ISP1401::GetArbCurrentCycle()
{
    RFU_K7_REG_DECLARE_2(0x0541,0x0641);
    RFU_K7_R_2(0x0541,0x0641);
    return RFU_K7_REG_2(0x0541,0x0641).cycle;
}

double ISP1401::GetArbCurrentTimer()
{
    RFU_K7_REG_DECLARE_2(0x0542,0x0642);
    RFU_K7_REG_DECLARE_2(0x0543,0x0643);
    RFU_K7_R_2(0x0542,0x0642);
    RFU_K7_R_2(0x0543,0x0643);
    return (RFU_K7_REG_2(0x0543,0x0643).time_l * 1.0 / 250e6 + RFU_K7_REG_2(0x0542,0x0642).time_h);
}

int32_t ISP1401::SetArbFreqOffset(uint32_t uiFreqMHz)
{
    RFU_K7_REG_DECLARE_2(0x0510,0x0610);
    RFU_K7_REG_DECLARE_2(0x0511,0x0611);
    RFU_K7_REG_2(0x0511,0x0611).offset = uiFreqMHz;
    RFU_K7_W_2(0x0511,0x0611);
    RFU_K7_OP_2(0x0510,0x0610);
    return 0;
}

int32_t ISP1401::SetArbMkrDelay(uint16_t *pDelays)
{
    RFU_K7_REG_DECLARE_2(0x053d,0x063d);
    RFU_K7_REG_DECLARE_2(0x053e,0x063e);
    RFU_K7_REG_2(0x053d,0x063d).delayr = pDelays[0];
    RFU_K7_REG_2(0x053d,0x063d).delay2 = pDelays[1];
    RFU_K7_W_2(0x053d,0x063d);
    RFU_K7_REG_2(0x053e,0x063e).delay3 = pDelays[2];
    RFU_K7_REG_2(0x053e,0x063e).delay4 = pDelays[3];
    RFU_K7_W_2(0x053e,0x063e);
    return 0;
}

int32_t ISP1401::SetArbUnperiodMarkerStart()
{
    static const int MARKER_DATA_OFFSET = 64;
    m_UnperiodMarkerCounter = MARKER_DATA_OFFSET;
    return 0;
}

int32_t ISP1401::SetArbUnperiodAddMarker(int iNO,uint32_t uiStart,uint32_t uiKeep)
{
    switch (iNO) {
        case 1 : {
            RFU_K7_REG_DECLARE_2(0x0533,0x0633);
            RFU_K7_REG_DECLARE_2(0x0534,0x0634);

            RFU_K7_REG_2(0x0534,0x0634).data = uiStart;
            RFU_K7_W_2(0x0534,0x0634);
            RFU_K7_REG_2(0x0533,0x0633).adr = m_UnperiodMarkerCounter;
            RFU_K7_OP_2(0x0533,0x0633);

            RFU_K7_REG_2(0x0534,0x0634).data = uiKeep;
            RFU_K7_W_2(0x0534,0x0634);
            RFU_K7_REG_2(0x0533,0x0633).adr = m_UnperiodMarkerCounter + 1;
            RFU_K7_OP_2(0x0533,0x0633);
            break;
                 }
        case 2 : {
            RFU_K7_REG_DECLARE_2(0x0535,0x0635);
            RFU_K7_REG_DECLARE_2(0x0536,0x0636);

            RFU_K7_REG_2(0x0536,0x0636).data = uiStart;
            RFU_K7_W_2(0x0536,0x0636);
            RFU_K7_REG_2(0x0535,0x0635).adr = m_UnperiodMarkerCounter;
            RFU_K7_OP_2(0x0535,0x0635);

            RFU_K7_REG_2(0x0536,0x0636).data = uiKeep;
            RFU_K7_W_2(0x0536,0x0636);
            RFU_K7_REG_2(0x0535,0x0635).adr = m_UnperiodMarkerCounter + 1;
            RFU_K7_OP_2(0x0535,0x0635);
            break;
                 }
        case 3 : {
            RFU_K7_REG_DECLARE_2(0x0537,0x0637);
            RFU_K7_REG_DECLARE_2(0x0538,0x0638);

            RFU_K7_REG_2(0x0538,0x0638).data = uiStart;
            RFU_K7_W_2(0x0538,0x0638);
            RFU_K7_REG_2(0x0537,0x0637).adr = m_UnperiodMarkerCounter;
            RFU_K7_OP_2(0x0537,0x0637);

            RFU_K7_REG_2(0x0538,0x0638).data = uiKeep;
            RFU_K7_W_2(0x0538,0x0638);
            RFU_K7_REG_2(0x0537,0x0637).adr = m_UnperiodMarkerCounter + 1;
            RFU_K7_OP_2(0x0537,0x0637);
            break;
                 }
        case 4 : {
            RFU_K7_REG_DECLARE_2(0x0539,0x0639);
            RFU_K7_REG_DECLARE_2(0x053a,0x063a);

            RFU_K7_REG_2(0x053a,0x063a).data = uiStart;
            RFU_K7_W_2(0x053a,0x063a);
            RFU_K7_REG_2(0x0539,0x0639).adr = m_UnperiodMarkerCounter;
            RFU_K7_OP_2(0x0539,0x0639);

            RFU_K7_REG_2(0x053a,0x063a).data = uiKeep;
            RFU_K7_W_2(0x053a,0x063a);
            RFU_K7_REG_2(0x0539,0x0639).adr = m_UnperiodMarkerCounter + 1;
            RFU_K7_OP_2(0x0539,0x0639);
            break;
                 }
    }
    m_UnperiodMarkerCounter += 2;								//µØÖ·ËæÊ±¼äµÝÔö
    return 0;
}

int32_t ISP1401::SetArbUnperiodMarkerEnd(int iNO,int iSeg)
{
    switch (iNO) {
        case 1 : {
            RFU_K7_REG_DECLARE_2(0x0533,0x0633);
            RFU_K7_REG_DECLARE_2(0x0534,0x0634);

            RFU_K7_REG_2(0x0534,0x0634).data = m_UnperiodMarkerCounter;
            RFU_K7_W_2(0x0534,0x0634);
            RFU_K7_REG_2(0x0533,0x0633).adr = iSeg;
            RFU_K7_OP_2(0x0533,0x0633);
            break;
                 }
        case 2 : {
            RFU_K7_REG_DECLARE_2(0x0535,0x0635);
            RFU_K7_REG_DECLARE_2(0x0536,0x0636);

            RFU_K7_REG_2(0x0536,0x0636).data = m_UnperiodMarkerCounter;
            RFU_K7_W_2(0x0536,0x0636);
            RFU_K7_REG_2(0x0535,0x0635).adr = iSeg;
            RFU_K7_OP_2(0x0535,0x0635);
            break;
                 }
        case 3 : {
            RFU_K7_REG_DECLARE_2(0x0537,0x0637);
            RFU_K7_REG_DECLARE_2(0x0538,0x0638);

            RFU_K7_REG_2(0x0538,0x0638).data = m_UnperiodMarkerCounter;
            RFU_K7_W_2(0x0538,0x0638);
            RFU_K7_REG_2(0x0537,0x0637).adr = iSeg;
            RFU_K7_OP_2(0x0537,0x0637);
            break;
                 }
        case 4 : {
            RFU_K7_REG_DECLARE_2(0x0539,0x0639);
            RFU_K7_REG_DECLARE_2(0x053a,0x063a);

            RFU_K7_REG_2(0x053a,0x063a).data = m_UnperiodMarkerCounter;
            RFU_K7_W_2(0x053a,0x063a);
            RFU_K7_REG_2(0x0539,0x0639).adr = iSeg;
            RFU_K7_OP_2(0x0539,0x0639);
            break;
                 }
    }
    return 0;
}

int32_t ISP1401::ArbLoad(char *pPath)
{
    RFU_K7_REG_DECLARE_2(0x0506,0x0606);
    RFU_K7_REG_DECLARE_2(0x0507,0x0607);

    uint32_t uiSplsLeft = 0;
    uint32_t uiSplsTotalTrsed = 0;
    uint32_t uiSplsSegTrsed = 0;
    uint32_t uiSplsTrsing = 0;
    uint32_t uiDDRAddr = 0;
    uint32_t uiCnt = 0;

    FILE *fp = NULL;
    ArbReader::Param_t Param;
    list<ArbReader::SegHeader_t>::iterator iterSegHeader;
    list<ArbSeg_t>::iterator iterArbSeg;

    if (m_pArbReader->Load(pPath))
        return -1;
    if ((fp = m_pArbReader->Load(&Param)) == NULL)
        return -1;

    iterSegHeader = Param.SegHeader.begin();
    iterArbSeg = Param.ArbSeg.begin();

    INT_CHECK(ArbStop());
    INT_CHECK(SetArbSegments(Param.FileHeader.Segments));

    for (iterSegHeader = Param.SegHeader.begin();iterSegHeader != Param.SegHeader.end();iterSegHeader ++)
    {
        INT_CHECK(SetArbSegment(*iterArbSeg));

        uiSplsLeft = iterSegHeader->Samples;
        uiSplsSegTrsed = 0;
        while(uiSplsLeft > 0)
        {
            uiCnt ++;
            uiSplsTrsing = uiSplsLeft > T_BLOCK_SAMPLES ? T_BLOCK_SAMPLES : uiSplsLeft;

            uiDDRAddr = uiSplsTotalTrsed >> 1;
            RFU_K7_REG_2(0x0506,0x0606).samples = uiSplsTrsing;
            RFU_K7_W_2(0x0506,0x0606);
            RFU_K7_REG_2(0x0507,0x0607).adr = uiDDRAddr;
            RFU_K7_W_2(0x0507,0x0607);

            fread(DDR.GetUsrSpc(),sizeof(ArbReader::Data_t),uiSplsTrsing,fp);

            Log.stdprintf("dma%d ready:samples:%d\n",uiCnt,uiSplsTrsing);
            INT_CHECK(DDR.FpgaRead(m_pK7,DDR.GetUsrSpc(),uiSplsTrsing,NULL));
            Log.stdprintf("dma%d done\n",uiCnt);

            uiSplsTotalTrsed += uiSplsTrsing;
            uiSplsSegTrsed   += uiSplsTrsing;
            uiSplsLeft = iterSegHeader->Samples - uiSplsSegTrsed;
        }

        iterArbSeg ++;
    }
    Log.stdprintf("all done\n");
    return 0;
}

int32_t ISP1401::ArbStart()
{
    RFU_K7_REG_DECLARE_2(0x0509,0x0609);
    RFU_K7_REG_2(0x0509,0x0609).abort	= 0;
    RFU_K7_REG_2(0x0509,0x0609).en		= 1;
    RFU_K7_REG_2(0x0509,0x0609).apc	= 0;
    RFU_K7_W_2(0x0509,0x0609);
    return 0;
}

int32_t ISP1401::ArbStop()
{
    RFU_K7_REG_DECLARE_2(0x0509,0x0609);
    RFU_K7_REG_2(0x0509,0x0609).abort	= 1;
    RFU_K7_REG_2(0x0509,0x0609).en		= 0;
    RFU_K7_W_2(0x0509,0x0609);
    return 0;
}

int32_t ISP1401::WriteEEPROM(uint16_t uiAddr,uint32_t uiLength,char *pBuf)
{
    RFU_K7_REG_DECLARE_2(0x161d,0x169d);
    RFU_K7_REG_DECLARE_2(0x161e,0x169e);
    RFU_K7_REG_DECLARE_2(0x161f,0x169f);
    RFU_K7_REG_2(0x161d,0x169d).mode = 0;	//write mode
    RFU_K7_W_2(0x161d,0x169d);
#ifdef _WIN64
    Sleep(10);
#else
    usleep(10 * 1000);
#endif
    for (uint32_t i = 0;i < uiLength * sizeof(char);i ++) {
        RFU_K7_REG_2(0x161e,0x169e).addr = uiAddr + i;
        RFU_K7_W_2(0x161e,0x169e);

#ifdef _WIN64
        Sleep(10);
#else
        usleep(10 * 1000);
#endif

        RFU_K7_REG_2(0x161f,0x169f).wr_data = unsigned(*(pBuf + i));
        RFU_K7_W_2(0x161f,0x169f);

#ifdef _WIN64
        Sleep(10);
#else
        usleep(10 * 1000);
#endif

        RFU_K7_OP_2(0x161d,0x169d);

#ifdef _WIN64
        Sleep(10);
#else
        usleep(10 * 1000);
#endif
    }
    return 0;
}

int32_t ISP1401::ReadEEPROM(uint16_t uiAddr,uint32_t uiLength,char *pBuf)
{
    RFU_K7_REG_DECLARE_2(0x161d,0x169d);
    RFU_K7_REG_DECLARE_2(0x161e,0x169e);
    RFU_K7_REG_DECLARE_2(0x1620,0x16a0);
    RFU_K7_REG_2(0x161d,0x169d).mode = 1;	//read mode
    RFU_K7_W_2(0x161d,0x169d);
    for (uint32_t i = 0;i < uiLength * sizeof(char);i ++) {
        RFU_K7_REG_2(0x161e,0x169e).addr = uiAddr + i;
        RFU_K7_W_2(0x161e,0x169e);
        RFU_K7_OP_DELAY_2(0x161d,0x169d,10);
        RFU_K7_R_2(0x1620,0x16a0);
        pBuf[i] = RFU_K7_REG_2(0x1620,0x16a0).rd_data;
    }
    return 0;
}

int32_t ISP1401::SetNListIQCapTrigOffset(uint32_t uiSamples)
{
    RFU_K7_REG_DECLARE_2(0x10c0,0x10f0);
    RFU_K7_REG_2(0x10c0,0x10f0).iqcap_trig_offset = uiSamples;
    RFU_K7_W_2(0x10c0,0x10f0);
    return 0;
}

int32_t ISP1401::SetNListIQCapCapOffset(uint32_t uiSamples)
{
    RFU_K7_REG_DECLARE_2(0x10c1,0x10f1);
    if (uiSamples % 16 != 0) {
        Log.SetLastError("cap offset invalid");
        return -1;
    }
    RFU_K7_REG_2(0x10c1,0x10f1).iqcap_cap_offset = uiSamples;
    RFU_K7_W_2(0x10c1,0x10f1);
    return 0;
}

int32_t ISP1401::IQCapStart()
{
    RFU_K7_REG_DECLARE(0x10c4);
    RFU_K7_REG_DECLARE(0x10c5);
    RFU_K7_REG_DECLARE(0x10cf);

    RFU_K7_REG(0x10cf).cap_tlp_count = 4096/*((368.64 * 1e3) /32 )/ 2*/;
    RFU_K7_W(0x10cf);

    RFU_K7_REG(0x10c5).abort = 0;
    RFU_K7_W(0x10c5);

    RFU_K7_OP(0x10c4);

    INT_CHECK(DDR.WStart(m_pK7));
    return 0;
}

int32_t ISP1401::IQCapAbort()
{
    RFU_K7_REG_DECLARE(0x10c5);
    RFU_K7_REG(0x10c5).abort = 1;
    RFU_K7_W(0x10c5);
    return 0;
}

int32_t ISP1401::IQDump2Buf(int16_t *pI,int16_t *pQ,uint32_t uiSamples)
{
    return 0;
}

int32_t ISP1401::IQDump2File(char *pPath,uint32_t uiSamples)
{
    return 0;
}

int32_t ISP1401::SetIQCapSamples(uint32_t uiSamples)
{
    RFU_K7_REG_DECLARE(0x10c6);
    RFU_K7_REG(0x10c6).samples = uiSamples * 2;
    RFU_K7_W(0x10c6);
    return 0;
}

int32_t ISP1401::GetIQCapSamples(uint32_t &uiSamples)
{
    RFU_K7_REG_DECLARE(0x10c6);
    RFU_K7_R(0x10c6);
    uiSamples = RFU_K7_REG_U32(0x10c6);
    uiSamples /= 2;
    return 0;
}

int32_t ISP1401::SetIQCapTrigGap(uint32_t us)
{
    RFU_K7_REG_DECLARE_2(0x10c7,0x10f7);
    RFU_K7_REG_2(0x10c7,0x10f7).gap = unsigned(us * 62.5 + 0.5);
    RFU_K7_W_2(0x10c7,0x10f7);
    return 0;
}

int32_t ISP1401::IQCap()
{
    RFU_K7_REG_DECLARE(0x0014);

    //Reserved
    RFU_K7_REG_DECLARE(0x0025);
    RFU_K7_REG_DECLARE(0x1670);

    RFU_K7_REG(0x0025).data = 0;
    RFU_K7_W(0x0025);
    RFU_K7_REG(0x0025).data = 1;
    RFU_K7_W(0x0025);

    RFU_K7_REG(0x1670).data = 0;
    RFU_K7_W(0x1670);
    RFU_K7_REG(0x1670).data = 1;
    RFU_K7_W(0x1670);
#ifdef _WIN64
    Sleep(100);
#else
    usleep(100 * 1000);
#endif
    RFU_K7_REG(0x1670).data = 0;
    RFU_K7_W(0x1670);

    RFU_K7_REG(0x0025).data = 2;
    RFU_K7_W(0x0025);
    RFU_K7_REG(0x0025).data = 0;
    RFU_K7_W(0x0025);
    RFU_K7_REG(0x0025).data = 32;
    RFU_K7_W(0x0025);
    //Reserved

    INT_CHECK(IQCapAbort());
    INT_CHECK(DDR.Reset(m_pK7));
    INT_CHECK(IQCapStart());

    double dTimeStar = Log.GetTimeStamp();
    double dTime = 0.0;
    bool bTimeout = false;

    uint32_t uiCurSamples[5] = {0};
    uint32_t uiSamples = 0;
    GetIQCapSamples(uiSamples);

    while (1) {
        dTime = Log.GetTimeStamp() - dTimeStar;
        bTimeout = dTime > 6 ? true : false;
//		STACHK(GetIQCapTimeout(timeout));

        for (int32_t i = 0;i < 5;i ++) {
            RFU_K7_R(0x0014);
            uiCurSamples[i] = RFU_K7_REG(0x0014).samples;
        }
        if (!bTimeout) {
            if (uiCurSamples[4] == (uiSamples * 2))	//success
                break;
            if (uiCurSamples[4] == uiCurSamples[3] && uiCurSamples[4] < (uiSamples * 2)) {
                Log.SetLastError("stop at samples:%d(need samples:%d)",uiCurSamples[4],uiSamples * 2);
                return -1;
            }
            if (uiCurSamples[4] == uiCurSamples[3] && uiCurSamples[4] > (uiSamples * 2)) {
                Log.SetLastError("exceed,samples:%d(need samples%d)",uiCurSamples[4],uiSamples * 2);
                return -1;
            }
        }
        else if (bTimeout) {
            if (uiCurSamples[0]  < uiCurSamples[1])
                continue;
            if (uiCurSamples[4] == (uiSamples * 2))	//also success
                break;
            if (uiCurSamples[4] == uiCurSamples[3] && uiCurSamples[4] < (uiSamples * 2)) {
                Log.SetLastError("stop at samples:%d(need samples:%d)",uiCurSamples[4],uiSamples * 2);
                return -1;
            }
            if (uiCurSamples[4] == uiCurSamples[3] && uiCurSamples[4] > (uiSamples * 2)) {
                Log.SetLastError("exceed,samples:%d(need samples%d)",uiCurSamples[4],uiSamples * 2);
                return -1;
            }
        }
    }
    return 0;
}

int32_t ISP1401::SetPowerMeasSrc(PowerMeasSrcs Src,bool bPosedge)
{
    RFU_K7_REG_DECLARE_2(0x1062,0x1092);
    RFU_K7_REG_2(0x1062,0x1092).iqcap_src_sel  = (unsigned)Src;
    RFU_K7_REG_2(0x1062,0x1092).iqcap_src_edge = 0/*bPosedge ? 1 : 0*/;
    RFU_K7_W_2(0x1062,0x1092);
    return 0;
}

int32_t ISP1401::SetPowerMeasTimeout(uint32_t us)
{
    double dCycle = 1.0 / 62.5;
    RFU_K7_REG_DECLARE_2(0x1063,0x1093);
    RFU_K7_REG_2(0x1063,0x1093).iqcap_timeout = (unsigned)(round(us / dCycle,0));
    RFU_K7_W_2(0x1063,0x1093);
    return 0;
}

int32_t ISP1401::GetPowerMeasTimeout(bool &bTimeout)
{
    RFU_K7_REG_DECLARE_2(0x1063,0x1093);
    RFU_K7_R_2(0x1063,0x1093);
    bTimeout = (1 == RFU_K7_REG_2(0x1063,0x1093).iqcap_tmo) ? true : false;
    return 0;
}

int32_t ISP1401::SetPowerMeasTrigThreshold(double dIFPower)
{
    RFU_K7_REG_DECLARE_2(0x1068,0x1098);
    RFU_K7_REG_2(0x1068,0x1098).threshold = (unsigned)(dBc2ad(_0dBFS,-1 * dIFPower));
    RFU_K7_W_2(0x1068,0x1098);
    return 0;
}

int32_t ISP1401::SetPowerMesaTrigGap(uint32_t us)
{
    RFU_K7_REG_DECLARE_2(0x1067,0x1097);
    RFU_K7_REG_2(0x1067,0x1097).gap = unsigned(us * 62.5 + 0.5);
    RFU_K7_W_2(0x1067,0x1097);
    return 0;
}

int32_t ISP1401::PowerMeasStart()
{
    RFU_K7_REG_DECLARE_2(0x1064,0x1094);
    RFU_K7_REG_DECLARE_2(0x1065,0x1095);
    RFU_K7_REG_DECLARE(0x10cf);

    RFU_K7OP_2(0x1065,0x1095,abort);
//	RFU_K7_OP_2(0x1064,0x1094);

    RFU_K7_REG_2(0x1064,0x1094).op = 0;
    RFU_K7_W_2(0x1064,0x1094);

    RFU_K7_REG_2(0x1064,0x1094).op = 1;
    RFU_K7_W_2(0x1064,0x1094);
    return 0;
}

int32_t ISP1401::PowerMeasAbort()
{
    RFU_K7_REG_DECLARE_2(0x1065,0x1095);
    RFU_K7_REG_2(0x1065,0x1095).abort = 1;
    RFU_K7_W_2(0x1065,0x1095);
    return 0;
}

int32_t ISP1401::SetPowerMeasSamples(uint32_t uiSamples)
{
    RFU_K7_REG_DECLARE_2(0x1066,0x1096);
    RFU_K7_REG_2(0x1066,0x1096).samples = uiSamples;
    RFU_K7_W_2(0x1066,0x1096);
    return 0;
}

int32_t ISP1401::GetPowerMeasSamples(uint32_t &uiSamples)
{
    RFU_K7_REG_DECLARE_2(0x1066,0x1096);
    RFU_K7_R_2(0x1066,0x1096);
    uiSamples = RFU_K7_REG_U32_2(0x1066,0x1096);
    return 0;
}

int32_t ISP1401::GetPowerMeasState(PowerMeasState &State)
{
    RFU_K7_REG_DECLARE_2(0x1074,0x10a4);
    RFU_K7_R_2(0x1074,0x10a4);
    State = (PowerMeasState)(RFU_K7_REG_2(0x1074,0x10a4).state);
    return 0;
}

int32_t ISP1401::GetPowerMeasPeak(double &dPower)
{
    RFU_K7_REG_DECLARE_2(0x1075,0x10a5);
    RFU_K7_R_2(0x1075,0x10a5);
    dPower = 0.0 + ad2dBc((double)_0dBFS,(double)(RFU_K7_REG_2(0x1075,0x10a5).iq_sum));
    return 0;
}

int32_t ISP1401::GetPowerMeasPower(double &dPower)
{
    RFU_K7_REG_DECLARE_2(0x1076,0x10a6);
    RFU_K7_REG_DECLARE_2(0x1077,0x10a7);
    RFU_K7_R_2(0x1076,0x10a6);
    RFU_K7_R_2(0x1077,0x10a7);
    int64_t iIQSumL = (int64_t)RFU_K7_REG_2(0x1077,0x10a7).iq_sum_l;
    int64_t iIQSumH = (int64_t)RFU_K7_REG_2(0x1076,0x10a6).iq_sum_h;
    uint32_t uiSamples = 0;
    GetPowerMeasSamples(uiSamples);
    double iq_sum = ((double)(iIQSumL + (iIQSumH << 32)) / (double)uiSamples);
    dPower = 0.0 + ad2dBc((double)_0dBFS,iq_sum);
    return 0;
}

int32_t ISP1401::GetADS5474(int64_t &iAD)
{
    RFU_K7_REG_DECLARE_2(0x00b9,0x20b9);
    RFU_K7_REG_DECLARE_2(0x00e5,0x20e5);

    RFU_K7_OP_2(0x00e5,0x20e5);
    RFU_K7_WAIT_IDLE_2(0x00e5,0x20e5,1,10000);
    RFU_K7_R_2(0x00b9,0x20b9);

    RFU_K7_OP_2(0x00e5,0x20e5);
    RFU_K7_WAIT_IDLE_2(0x00e5,0x20e5,1,10000);
    RFU_K7_R_2(0x00b9,0x20b9);

    iAD = (RFU_K7_REG_2(0x00b9,0x20b9).iq_avg) << 2;
    return 0;
}

int32_t ISP1401::GetADS5474Manual(int64_t &iAD)
{
    iAD = 0;
    int16_t I[4096] = {0};
    int16_t Q[4096] = {0};
    double dSumI = 0.0;
    double dSumQ = 0.0;

    INT_CHECK(SetIQCapSamples(4096));
    INT_CHECK(IQCap());
    INT_CHECK(DDR.IQToBuf(m_pCalFile->GetRfIdx(),I,Q,4096));

    for (int32_t i = 0;i < 4096;i ++) {
        dSumI += pow((double)I[i],2);
        dSumQ += pow((double)Q[i],2);
    }
    iAD = (int64_t)((dSumI + dSumQ) / 4096);
    return 0;
}

int32_t ISP1401::SetRxIFFilterSw(RxIFFilter Filter)
{
    RFU_K7_REG_DECLARE_2(0x1668,0x16e8);
    RFU_K7_REG_2(0x1668,0x16e8).fil_sw = (_100MHz == Filter ? 1 : 0);
    RFU_K7_W_2(0x1668,0x16e8);
    return 0;
}

int32_t ISP1401::GetAD7680(uint32_t &uiDet)
{
    RFU_K7_REG_DECLARE_2(0x1611,0x1691);
    RFU_K7_REG_DECLARE_2(0x1612,0x1692);
    RFU_K7_WAIT_IDLE_2(0x1611,0x1691,0,1000);
    RFU_K7_OP_2(0x1611,0x1691);
    RFU_K7_WAIT_IDLE_2(0x1611,0x1691,0,1000);

    RFU_K7_R_2(0x1612,0x1692);
    uiDet = RFU_K7_REG_2(0x1612,0x1692).adc;
    return 0;
}
