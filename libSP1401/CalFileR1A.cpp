#include "CalFileR1A.h"
#include <io.h>

typedef int32_t (CalFileR1A::*GetItemSizeVxxxx)(ICalFile::CalItem,uint32_t &,uint32_t &);

CalFileR1A::CalFileR1A(uint32_t uiRfIdx,uint32_t uiRfuIdx) : ICalFile(uiRfIdx,uiRfuIdx)
{
	m_pTxSideband = NULL;
	m_pTxLOLeak = NULL;
	m_pTxAttOP = NULL;
	m_pTxPowerOP = NULL;
	m_pTxPowerIO = NULL;
	m_pRxRef = NULL;
	m_pX9119 = NULL;
	m_pTxAttIO = NULL;
}

CalFileR1A::~CalFileR1A()
{
	SAFE_DELETE(m_pTxSideband);
	SAFE_DELETE(m_pTxLOLeak);
	SAFE_DELETE(m_pTxAttOP);
	SAFE_DELETE(m_pTxPowerOP);
	SAFE_DELETE(m_pTxPowerIO);
	SAFE_DELETE(m_pRxRef);
	SAFE_DELETE(m_pX9119);
	SAFE_DELETE(m_pTxAttIO);
}

int32_t CalFileR1A::Open()
{
    if (!m_pTxSideband)	m_pTxSideband = new TxSidebandTableR1A;;
	if (!m_pTxLOLeak)	m_pTxLOLeak = new TxLOLeakageTableR1A;
	if (!m_pTxAttOP)	m_pTxAttOP = new TxAttTableR1A;
	if (!m_pTxPowerOP)	m_pTxPowerOP = new TxPowerTableR1A;
	if (!m_pTxPowerIO)	m_pTxPowerIO = new TxPowerTableR1A;
	if (!m_pRxRef)		m_pRxRef = new RxRefTableR1A;
	if (!m_pX9119)		m_pX9119 = new X9119TableR1A;
	if (!m_pTxAttIO)	m_pTxAttIO = new TxAttTableR1A;
	return ICalFile::Open();
}

int32_t CalFileR1A::Map2Buf(ICalFile::CalItem Item)
{
	if (NULL == g_ItemBuf.m_pBuf) {
		ICalFile::FileInfo Info;
		uint32_t uiMaxByte = ICalFile::GetMaxItemByte();

		g_ItemBuf.m_pBuf = new char[uiMaxByte / sizeof(char)];
		if (NULL == g_ItemBuf.m_pBuf) {
            Log->SetLastError("%s:%s:%d",__FILE__,__FUNCTION__,__LINE__);
			return -1;
		}
		memset(g_ItemBuf.m_pBuf,0,uiMaxByte);
	}

	if (Item != g_ItemBuf.m_Item || this->m_uiRfIdx != g_ItemBuf.m_uiRfIdx || this->m_uiRfuIdx != g_ItemBuf.m_uiRfuIdx) {
        INT_CHECK(Read(Item,g_ItemBuf.m_pBuf));
		g_ItemBuf.m_Item = Item;
		g_ItemBuf.m_uiRfIdx = this->m_uiRfIdx;
		g_ItemBuf.m_uiRfuIdx = this->m_uiRfuIdx;
		switch (Item) {
            case ICalFile::TxSideband  : {m_pTxSideband->MapBuf((TxSidebandTableR1A::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxLoLeakage : {m_pTxLOLeak->MapBuf((TxLOLeakageTableR1A::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxAttOP	   : {m_pTxAttOP->MapBuf((TxAttTableR1A::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxPowerOP   : {m_pTxPowerOP->MapBuf((TxPowerTableR1A::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxPowerIO   : {m_pTxPowerIO->MapBuf((TxPowerTableR1A::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::RxRef	   : {m_pRxRef->MapBuf((RxRefTableR1A::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::X9119	   : {m_pX9119->MapBuf((X9119TableR1A::Data *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxAttIO	   : {m_pTxAttIO->MapBuf((TxAttTableR1A::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			default:return 0;
		}
	}
	return 0;
}

int32_t CalFileR1A::Map2Mem()
{
    Map2Buf(ICalFile::TxSideband);	m_pTxSideband->Map2Mem();
    Map2Buf(ICalFile::TxLoLeakage);	m_pTxLOLeak->Map2Mem();
    Map2Buf(ICalFile::TxAttOP);		m_pTxAttOP->Map2Mem();
    Map2Buf(ICalFile::TxPowerOP);	m_pTxPowerOP->Map2Mem();
    Map2Buf(ICalFile::TxPowerIO);	m_pTxPowerIO->Map2Mem();
    Map2Buf(ICalFile::RxRef);		m_pRxRef->Map2Mem();
    Map2Buf(ICalFile::X9119);		m_pX9119->Map2Mem();
    Map2Buf(ICalFile::TxAttIO);		m_pTxAttIO->Map2Mem();
    return 0;
}

int32_t CalFileR1A::Add(ICalFile::CalItem Item,void *pData)
{
    Map2Buf(Item);
	switch (Item) {
        case ICalFile::TxSideband  : {m_pTxSideband->Add((TxSidebandTableR1A::DataF *)pData);return 0;}
		case ICalFile::TxLoLeakage : {m_pTxLOLeak->Add((TxLOLeakageTableR1A::DataF *)pData);return 0;}
		case ICalFile::TxAttOP	   : {m_pTxAttOP->Add((TxAttTableR1A::DataF *)pData);return 0;}
		case ICalFile::TxPowerOP   : {m_pTxPowerOP->Add((TxPowerTableR1A::DataF *)pData);return 0;}
		case ICalFile::TxPowerIO   : {m_pTxPowerIO->Add((TxPowerTableR1A::DataF *)pData);return 0;}
		case ICalFile::RxRef	   : {m_pRxRef->Add((RxRefTableR1A::DataF *)pData);return 0;}
		case ICalFile::X9119	   : {m_pX9119->Add((X9119TableR1A::Data *)pData);return 0;}
		case ICalFile::TxAttIO	   : {m_pTxAttIO->Add((TxAttTableR1A::DataF *)pData);return 0;}
		default:return 0;
	}
	return 0;
}

int32_t CalFileR1A::Create()
{
	ICalFile::FileVer Ver;
	ICalFile::FileInfo Info;
	uint32_t dwBytesWritten = 0;
	uint32_t uiItemIdx = 0;
	uint32_t uiMaxByte = 0;
	char szPath[64] = {0};
	sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_uiRfuIdx,m_uiRfIdx);

	Ver.m_uiHead = CAL_FILEVER_HEAD;
	Ver.m_uiVer  = CAL_FILE_VER > 4 ? 4 : CAL_FILE_VER;	//r1a:ver0---4,r1c:ver5---
	Ver.m_uiTail = CAL_FILEVER_TAIL;

	Info.m_uiHead = CAL_FILEINFO_HEAD;
	Info.m_uiTail = CAL_FILEINFO_TAIL;
	memset(Info.m_szSN,0,sizeof(Info.m_szSN));
    sprintf(Info.m_szSN,"SP1401R1B%08dRFU%uRF%u",m_uiOrdinal,m_uiRfuIdx,m_uiRfIdx);

	uiMaxByte = GetFixItemTableR1A(Info.m_pItemInfo,Info.m_uiItems);
	Info.m_uiItems -= 1;
	for (uint32_t i = 0;i < Info.m_uiItems;i ++) {
		if (X9119 == i)
			uiItemIdx ++;
		Info.m_pItemInfo[i] = Info.m_pItemInfo[uiItemIdx];
		uiItemIdx ++;
	}

    FILE *fp;
    if (_access("C:\\CSECal",0) == -1) {
        if (mkdir("C:\\CSECal") == -1) {
            Log->SetLastError("%s:%s:%d(%d)",__FILE__,__FUNCTION__,__LINE__,errno);
            return -1;
        }
    }
    if (_access("C:\\CSECal\\cxu.cal",0) == -1) {
		X9119TableR1A::Data Data;
		Data.m_iValue = 0;

        fp = fopen("C:\\CSECal\\cxu.cal","w+");
        CFO_ASSERT(fp,NULL == fp);
        CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
        CFO_ASSERT(fp,fwrite(&Data,sizeof(X9119TableR1A::Data),1,fp) == 0);
        CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
        fclose(fp);
	}
    fp = fopen(szPath,"w+");
    CFO_ASSERT(fp,NULL == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fwrite(&Ver,sizeof(ICalFile::FileVer),1,fp) == 0);
    CFO_ASSERT(fp,fwrite(&Info,sizeof(ICalFile::FileInfo),1,fp) == 0);
    for (uint32_t i = 0;i < Info.m_uiItems;i ++)
        CFO_ASSERT(fp,fwrite(&(Info.m_pItemInfo[i]),sizeof(ICalFile::ItemInfo),1,fp) == 0);

	unsigned char *pBuf = new unsigned char[uiMaxByte];
	for (uint32_t i = 0;i < Info.m_uiItems;i ++) {
		uint32_t uiSize = ((Info.m_pItemInfo[i]).m_uiSize) * ((Info.m_pItemInfo[i]).m_uiPoint); 
		memset(pBuf,0,uiMaxByte * sizeof(unsigned char));
        CFO_ASSERT(fp,fwrite(pBuf,1,uiSize,fp) == 0);
	}
	delete []pBuf;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);
	return 0;
}

int32_t CalFileR1A::Load(FileInfo &Info)
{
	uint32_t dwBytesRead = 0;
	uint32_t dwPos = 0;
	FileVer Ver;
	char szPath[64] = {0};
	sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_uiRfuIdx,m_uiRfIdx);

    FILE *fp = fopen(szPath,"r+");
    CFO_ASSERT(fp,NULL == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fread(&Ver,sizeof(FileVer),1,fp) == 0);
	ShowFileVer(&Ver);

	ItemInfo *pItemInfo = Info.m_pItemInfo;
    CFO_ASSERT(fp,fread(&Info,sizeof(FileInfo),1,fp) == 0);
	Info.m_pItemInfo = pItemInfo;
	ShowFileInfo(&Info);

	for (uint32_t i = 0;i < Info.m_uiItems;i ++) {
        CFO_ASSERT(fp,fread(&(Info.m_pItemInfo[i]),sizeof(ItemInfo),1,fp) == 0);
		ShowItemInfo(&(Info.m_pItemInfo[i]));
	}
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);
	return 0;
}

int32_t CalFileR1A::GetItemSize(ICalFile::CalItem Item,uint32_t &dwPos,uint32_t &uiSize)
{
	GetItemSizeVxxxx t_GetItemSize[] = {&CalFileR1A::GetItemSizeV0000,
										&CalFileR1A::GetItemSizeV0001,
										&CalFileR1A::GetItemSizeV0002,
										&CalFileR1A::GetItemSizeV0002,
										&CalFileR1A::GetItemSizeV0004,
										&CalFileR1A::GetItemSizeV0004};

	return (this->*t_GetItemSize[CAL_FILE_VER])(Item,dwPos,uiSize);
}

int32_t CalFileR1A::GetItemSizeV0000(ICalFile::CalItem Item,uint32_t &dwPos,uint32_t &uiSize)
{
	ICalFile::FileInfo Info;
    INT_CHECK(Load(Info));

	int32_t iItem = (int32_t)Item;

	uiSize = ((Info.m_pItemInfo[iItem]).m_uiSize) * ((Info.m_pItemInfo[iItem]).m_uiPoint);

	dwPos = 0;
	dwPos += sizeof(ICalFile::FileInfo);
	for (uint32_t i = 0;i < Info.m_uiItems;i ++)
		dwPos += sizeof(ICalFile::ItemInfo);
	for (int32_t i = 0;i < iItem;i ++)
		dwPos += ((Info.m_pItemInfo[i]).m_uiSize) * ((Info.m_pItemInfo[i]).m_uiPoint);
	return 0;
}

int32_t CalFileR1A::GetItemSizeV0001(ICalFile::CalItem Item,uint32_t &dwPos,uint32_t &uiSize)
{
	ICalFile::FileInfo Info;
    INT_CHECK(Load(Info));

	int32_t iItem = (int32_t)Item;
	uiSize = ((Info.m_pItemInfo[iItem]).m_uiSize) * ((Info.m_pItemInfo[iItem]).m_uiPoint);

	dwPos = 0;
	dwPos += sizeof(ICalFile::FileVer);
	dwPos += sizeof(ICalFile::FileInfo);
	for (uint32_t i = 0;i < Info.m_uiItems;i ++)
		dwPos += sizeof(ICalFile::ItemInfo);
	for (int32_t i = 0;i < iItem;i ++)
		dwPos += ((Info.m_pItemInfo[i]).m_uiSize) * ((Info.m_pItemInfo[i]).m_uiPoint);
	return 0;
}

int32_t CalFileR1A::GetItemSizeV0002(ICalFile::CalItem Item,uint32_t &dwPos,uint32_t &uiSize)
{
	ICalFile::FileInfo Info;
    INT_CHECK(Load(Info));

	int32_t iItem = (int32_t)Item;

	if (ICalFile::X9119 == Item) {
		uiSize = Info.m_pItemInfo[iItem].m_uiSize * Info.m_pItemInfo[iItem].m_uiPoint;
		dwPos = 0;
		return 0;
	}

	uiSize = ((Info.m_pItemInfo[iItem]).m_uiSize) * ((Info.m_pItemInfo[iItem]).m_uiPoint);

	dwPos = 0;
	dwPos += sizeof(ICalFile::FileVer);
	dwPos += sizeof(ICalFile::FileInfo);
	for (uint32_t i = 0;i < Info.m_uiItems;i ++)
		dwPos += sizeof(ICalFile::ItemInfo);
	for (int32_t i = 0;i < iItem;i ++)
		dwPos += ((Info.m_pItemInfo[i]).m_uiSize) * ((Info.m_pItemInfo[i]).m_uiPoint);
	return 0;
}

int32_t CalFileR1A::GetItemSizeV0004(ICalFile::CalItem Item,uint32_t &dwPos,uint32_t &uiSize)
{
	ICalFile::FileInfo Info;
    INT_CHECK(Load(Info));

	if (ICalFile::X9119 == Item) {
		uiSize = Info.m_pItemInfo[(int32_t)(ICalFile::X9119)].m_uiSize * Info.m_pItemInfo[(int32_t)(ICalFile::X9119)].m_uiPoint;
		dwPos = 0;
		return 0;
	}
	int32_t iItemIdx = (int32_t)(Item > ICalFile::X9119 ? (Item - 1) : Item);
	uiSize = ((Info.m_pItemInfo[iItemIdx]).m_uiSize) * ((Info.m_pItemInfo[iItemIdx]).m_uiPoint);
	dwPos = 0;
	dwPos += sizeof(ICalFile::FileVer);
	dwPos += sizeof(ICalFile::FileInfo);
	for (uint32_t i = 0;i < Info.m_uiItems;i ++)
		dwPos += sizeof(ICalFile::ItemInfo);
	for (int32_t i = 0;i < iItemIdx;i ++)
		dwPos += ((Info.m_pItemInfo[i]).m_uiSize) * ((Info.m_pItemInfo[i]).m_uiPoint);
	return 0;
}

void CalFileR1A::GetTxAtt(uint64_t uiFreq,IOMode Mode,float *pAtt)
{
	if (OutPut == Mode)
		m_pTxAttOP->Get(uiFreq,pAtt);
	if (IO == Mode)
		m_pTxAttIO->Get(uiFreq,pAtt);
}

void CalFileR1A::GetTxAtt(uint64_t uiFreq,IOMode Mode,int32_t iAtt,double &dOffset)
{
	if (OutPut == Mode)
		m_pTxAttOP->Get(uiFreq,iAtt,dOffset);
	if (IO == Mode)
		m_pTxAttIO->Get(uiFreq,iAtt,dOffset);
}

void CalFileR1A::GetTxPower(uint64_t uiFreq,double dPower,IOMode Mode,double &dBBGain,int32_t &iAtt1,int32_t &iAtt2,SP1401::TxPAAtt &PAAtt,int32_t &iAtt3)
{
	int32_t iPAAtt = 1;
	if (OutPut == Mode)
		m_pTxPowerOP->GetOP(uiFreq,dPower,dBBGain,iAtt1,iAtt2,iPAAtt,iAtt3);
	if (IO == Mode)
		m_pTxPowerIO->GetIO(uiFreq,dPower,dBBGain,iAtt1,iAtt2,iPAAtt,iAtt3);
    PAAtt = (0 == iPAAtt ? SP1401::Tx_PA : SP1401::Tx_Att);
}
