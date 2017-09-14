#include "ICalFile.h"
#include "CfvCtrl.h"

ICalFile::ItemBuf ICalFile::g_ItemBuf = ICalFile::ItemBuf();

ICalFile::FileInfo::FileInfo()
{
	m_pItemInfo = new ItemInfo[CAL_TOTAL_ITEMS];
}

ICalFile::FileInfo::~FileInfo()
{
	SAFE_DELETE(m_pItemInfo);
}

ICalFile::ItemBuf::ItemBuf()
{
	m_uiRfIdx = MAX_RF + 1;
	m_uiRfuIdx = MAX_RFU + 1;
	m_pBuf = NULL;
	m_Item = (CalItem)(-1);
}

ICalFile::ItemBuf::~ItemBuf()
{
    if (m_pBuf) {
        delete []((char *)m_pBuf);
        m_pBuf = NULL;
    }
}

ICalFile::ItemBuf ICalFile::ItemBuf::operator = (const ICalFile::ItemBuf &Buf)
{
	ItemBuf RetBuf;
	RetBuf.m_pBuf = Buf.m_pBuf;
	RetBuf.m_Item = Buf.m_Item;
	return RetBuf;
}

ICalFile::ICalFile(uint32_t uiRfIdx, uint32_t uiRfuIdx)
{
	m_uiRfIdx = uiRfIdx;
    m_uiRfuIdx = uiRfuIdx;
    m_uiOrdinal = 0;
	m_BW = _160M;
}

ICalFile::~ICalFile()
{
}

int32_t ICalFile::SetBW(BW bw)
{
	m_BW = bw;
	return 0;
}

BW ICalFile::GetBw()
{
	return m_BW;
}

uint32_t ICalFile::GetRfIdx()
{
	return m_uiRfIdx;
}

uint32_t ICalFile::GetRfuIdx()
{
    return m_uiRfuIdx;
}

uint32_t ICalFile::GetOrdinal()
{
    return m_uiOrdinal;
}

int32_t ICalFile::GetFileVer()
{
	FileVer Ver;

    char szPath[64] = {0};
    sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_uiRfuIdx,m_uiRfIdx);

    FILE *fp = fopen(szPath,"r");
    CFO_ASSERT(fp,NULL == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fread(&Ver,sizeof(FileVer),1,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));

	if ((0x04ABCDEF == Ver.m_uiHead && 0x05ABCDEF == Ver.m_uiTail) || (CAL_FILEVER_HEAD == Ver.m_uiHead && CAL_FILEVER_TAIL == Ver.m_uiTail))
		return Ver.m_uiVer;
	else
        return -1;
}

uint32_t ICalFile::GetFixItemTableR1A(ItemInfo *pInfo,uint32_t &uiItems)
{
	ItemInfo ItemInfoR1A[] = {
        {0xaa,ICalFile::TxSideband,	sizeof(TxSidebandTableR1A::DataF)	,uint32_t(RF_TX_FREQ_PTS_INFILE),0xff},
        {0xaa,ICalFile::TxLoLeakage,sizeof(TxLOLeakageTableR1A::DataF)	,uint32_t(RF_TX_FREQ_PTS_INFILE),0xff},
        {0xaa,ICalFile::TxAttOP,	sizeof(TxAttTableR1A::DataF)		,uint32_t(RF_TX_FREQ_PTS_INFILE),0xff},
        {0xaa,ICalFile::TxPowerOP,	sizeof(TxPowerTableR1A::DataF)		,uint32_t(RF_TX_FREQ_PTS_INFILE),0xff},
        {0xaa,ICalFile::TxPowerIO,	sizeof(TxPowerTableR1A::DataF)		,uint32_t(RF_TX_FREQ_PTS_INFILE),0xff},
        {0xaa,ICalFile::RxRef,		sizeof(RxRefTableR1A::DataF)		,uint32_t(RF_RX_FREQ_PTS_INFILE),0xff},
        {0xaa,ICalFile::X9119,	    sizeof(X9119TableR1A::Data)			,1                              ,0xff},
        {0xaa,ICalFile::TxAttIO,	sizeof(TxAttTableR1A::DataF)		,uint32_t(RF_TX_FREQ_PTS_INFILE),0xff}
	};
	uiItems = sizeof(ItemInfoR1A) / sizeof(ItemInfoR1A[0]);
	for (uint32_t i = 0;i < uiItems;i ++)
		pInfo[i] = ItemInfoR1A[i];

	return GetMaxItemByte(pInfo,uiItems);
}

uint32_t ICalFile::GetFixItemTableR1C(ItemInfo *pInfo,uint32_t &uiItems)
{
	ItemInfo ItemInfoR1C[] = {
        {0xaa,ICalFile::X9119,			sizeof(X9119TableR1C::DataF),			1,									0xff},
        {0xaa,ICalFile::TxLoLeakage,	sizeof(TxLOLeakageTableR1C::DataF),		1,									0xff},
        {0xaa,ICalFile::TxSideband,		sizeof(TxSidebandTableR1C::DataF),		1,									0xff},
        {0xaa,ICalFile::TxPowerOP,		sizeof(TxPowerOPTableR1C::DataF),		uint32_t(RF_TX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::TxPowerIO,		sizeof(TxPowerIOTableR1C::DataF),		uint32_t(RF_TX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::TxAttOP,		sizeof(TxAttOPTableR1C::DataF),			uint32_t(RF_TX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::TxAttIO,		sizeof(TxAttIOTableR1C::DataF),			uint32_t(RF_TX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::RxRefOP,		sizeof(RxRefOPTableR1C::DataF),			uint32_t(RF_RX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::RxRefIO,		sizeof(RxRefIOTableR1C::DataF),			uint32_t(RF_RX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::RxAttOP,		sizeof(RxAttOPTableR1C::DataF),			uint32_t(RF_RX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::RxAttIO,		sizeof(RxAttIOTableR1C::DataF),			uint32_t(RF_RX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::TxFil_80,		sizeof(TxFilter_80M_TableR1C::DataF),	uint32_t(RF_TX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::TxFil_160,		sizeof(TxFilter_160M_TableR1C::DataF),	uint32_t(RF_TX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::TxRFFr_0,		sizeof(TxRFFrTable::DataF),				uint32_t(RF_TX_FILTER_PTS_0),		0xff},
        {0xaa,ICalFile::TxRFFr_1,		sizeof(TxRFFrTable::DataF),				uint32_t(RF_TX_FILTER_PTS_1),		0xff},
        {0xaa,ICalFile::TxIFFr,			sizeof(TxIFFrTableR1C::DataF),			uint32_t(IF_TX_FILTER_160M_PTS),	0xff},
        {0xaa,ICalFile::RxFil_80,		sizeof(RxFilter_80M_TableR1C::DataF),	uint32_t(RF_RX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::RxFil_160,		sizeof(RxFilter_160M_TableR1C::DataF),	uint32_t(RF_RX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::RxRFFr,			sizeof(RxRFFrTableR1C::DataF),			uint32_t(RF_RX_FILTER_PTS),			0xff},
        {0xaa,ICalFile::RxIFFr,			sizeof(RxIFFrTableR1C::DataF),			uint32_t(IF_RX_FILTER_160M_PTS),	0xff},
        {0xaa,ICalFile::TxFilOffOP_80,	sizeof(TxFilterOffTableR1C::DataF),		uint32_t(RF_TX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::TxFilOffIO_80,	sizeof(TxFilterOffTableR1C::DataF),		uint32_t(RF_TX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::RxFilOffOP_80,	sizeof(RxFilterOffTableR1C::DataF),		uint32_t(RF_RX_FREQ_PTS_INFILE),	0xff},
        {0xaa,ICalFile::RxFilOffIO_80,	sizeof(RxFilterOffTableR1C::DataF),		uint32_t(RF_RX_FREQ_PTS_INFILE),	0xff}
	};
	uiItems = sizeof(ItemInfoR1C) / sizeof(ItemInfoR1C[0]);
	for (uint32_t i = 0;i < uiItems;i ++)
		pInfo[i] = ItemInfoR1C[i];

	return GetMaxItemByte(pInfo,uiItems);
}

uint32_t ICalFile::GetMaxItemByte(ItemInfo *pInfo,uint32_t uiItems)
{
	uint32_t uiMaxByte = 0;
	for (uint32_t i = 0;i < uiItems;i ++) {
		if (uiMaxByte < pInfo[i].m_uiSize * pInfo[i].m_uiPoint)
			uiMaxByte = pInfo[i].m_uiSize * pInfo[i].m_uiPoint;
	}
	return uiMaxByte;
}

uint32_t ICalFile::GetMaxItemByte()
{
	FileInfo Info;
	uint32_t uiMaxByteR1A = ICalFile::GetFixItemTableR1A(Info.m_pItemInfo,Info.m_uiItems);
	uint32_t uiMaxByteR1C = ICalFile::GetFixItemTableR1C(Info.m_pItemInfo,Info.m_uiItems);
	return uiMaxByteR1A >= uiMaxByteR1C ? uiMaxByteR1A : uiMaxByteR1C;
}

int32_t ICalFile::Open()
{
	if (!IsFileValid()) {
		if (Create() < 0)
			return -1;
		if (!IsFileValid())
			return -1;
	}
	Map2Mem();
	return 0;
}

bool ICalFile::IsFileValid()
{
	ICalFile::FileInfo Info;
    int32_t iVer = GetFileVer();
    if (iVer < 0)
        return false;
    else if (iVer < CAL_FILE_VER) {
        CfvCtrl VerCtrl;
        VerCtrl.Target(this);
        VerCtrl.ForwardConvert(iVer);
	}
	if (Load(Info))
		return false;
	for (uint32_t i = 0;i < Info.m_uiItems;i ++) {
		if ((Info.m_pItemInfo[i]).m_uiHead != 0xaa || (Info.m_pItemInfo[i]).m_uiTail != 0xff) {
            Log->SetLastError("invalid data format");
			return false;
		}
	}
	return true;
}

int32_t ICalFile::WriteFromPos(char *pPath, uint32_t uiPos, uint32_t uiSize, void *pData)
{
    FILE *fp = fopen(pPath,"r+");
    CFO_ASSERT(fp,NULL == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fseek(fp,uiPos,SEEK_SET));
    CFO_ASSERT(fp,fwrite(pData,uiSize,1,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
	return 0;
}

int32_t ICalFile::ReadFromPos(char *pPath, uint32_t uiPos, uint32_t uiSize, void *pData)
{
    FILE *fp = fopen(pPath,"r");
    CFO_ASSERT(fp,NULL == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fseek(fp,uiPos,SEEK_SET));
    CFO_ASSERT(fp,fread(pData,uiSize,1,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    return 0;
}

int32_t ICalFile::Write(ICalFile::CalItem Item)
{
    uint32_t uiPos = 0,uiSize = 0;
	char szPath[64] = {0};
	sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_uiRfuIdx,m_uiRfIdx);

	if (ICalFile::X9119 == Item) {
		*strrchr(szPath,'\\') = 0;
		strcat(szPath,"\\cxu.cal");
	}
    INT_CHECK(GetItemSize(Item,uiPos,uiSize));
    INT_CHECK(WriteFromPos(szPath,uiPos,uiSize,g_ItemBuf.m_pBuf));
	Map2Mem();
    Map2Buf(Item);
	return 0;	
}

int32_t ICalFile::Read(ICalFile::CalItem Item,void *pData)
{
    uint32_t uiPos = 0,uiSize = 0;
	char szPath[64] = {0};
	sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_uiRfuIdx,m_uiRfIdx);

	if (ICalFile::X9119 == Item) {
		*strrchr(szPath,'\\') = 0;
		strcat(szPath,"\\cxu.cal");
	}
    INT_CHECK(GetItemSize(Item,uiPos,uiSize));
    INT_CHECK(ReadFromPos(szPath,uiPos,uiSize,pData));
	for (uint32_t i = 0;i < uiSize;i ++)
		((char *)(g_ItemBuf.m_pBuf))[i] = ((char *)pData)[i];
	return 0;
}

void ICalFile::ShowFileVer(FileVer *pVer)
{
//	Log.stdprintf("file ver : %#06x\n",pVer->m_uiVer);
}

void ICalFile::ShowFileInfo(FileInfo *pInfo)
{
// 	Log.stdprintf("sn : %s\n",pInfo->m_szSN);
// 	Log.stdprintf("num of item : %d\n",pInfo->m_uiItems);
// 	Log.stdprintf("first table addr : %#x\n",pInfo->m_pItemInfo);
}

void ICalFile::ShowItemInfo(ItemInfo *pInfo)
{
// 	char szItemName[][64] = {
// 		{"Tx SideBand"},
// 		{"Tx LoLeakage"},
// 		{"Tx Att Output"},
// 		{"Tx Power Output(160M)"},
// 		{"Tx Power IO(160M)"},
// 		{"Rx Reference"},
// 		{"X9119"},
// 		{"Tx Att IO"},
// 		{"Rx Reference Output"},
// 		{"Rx Reference IO"},
// 		{"Rx Att Output"},
// 		{"Rx Att IO"},
// 		{"Tx Filter 80M"},
// 		{"Tx Filter 160M"},
// 		{"Tx RF Freq Response(<=3100)"},
//		{"Tx RF Freq Response(>3100)"},
// 		{"Tx IF Freq Response"},
// 		{"Rx Filter 80M"},
// 		{"Rx Filter 160M"},
// 		{"Rx RF Freq Response"},
// 		{"Rx IF Freq Response"},
// 		{"Tx Filter Offset Output(80M)"},
// 		{"Tx Filter Offset IO(80M)"},
// 		{"Rx Filter Offset Output(80M)"},
// 		{"Rx Filter Offset IO(80M)"}
// 	};
// 	Log.stdprintf("item name : %s\n",szItemName[pInfo->m_Item]);
// 	Log.stdprintf("data byte : %d\n",pInfo->m_uiSize);
// 	Log.stdprintf("points    : %d\n",pInfo->m_uiPoint);
}
