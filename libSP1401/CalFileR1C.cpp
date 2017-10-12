#include "CalFileR1C.h"
#include "CalFileR1A.h"
#include <io.h>
#include <direct.h>

#define R1A_FILE_SIZE 5541296	//file size before adding r1c data,make it easy to arrive to r1c data head directly

CalFileR1C::CalFileR1C(uint32_t uiRfIdx, uint32_t uiRfuIdx) : ICalFile(uiRfIdx,uiRfuIdx)
{
	m_pX9119 = NULL;
	m_pTxLOLeak = NULL;
	m_pTxSideband = NULL;
	m_pTxPowerOP = NULL;
	m_pTxPowerIO = NULL;
	m_pTxAttOP = NULL;
	m_pTxAttIO = NULL;
	m_pRxRefOP = NULL;
	m_pRxRefIO = NULL;
	m_pRxAttOP = NULL;
	m_pRxAttIO = NULL;
	m_pTxFilter_80M = NULL;
	m_pTxFilter_160M = NULL;
	m_pRxFilter_80M = NULL;
	m_pRxFilter_160M = NULL;
	m_pTxFilOffOP_80M = NULL;
	m_pTxFilOffIO_80M = NULL;
	m_pRxFilOffOP_80M = NULL;
	m_pRxFilOffIO_80M = NULL;

	m_pTxRFFr0 = NULL;
	m_pTxRFFr1 = NULL;
	m_pTxIFFr = NULL;
	m_pRxRFFr = NULL;
	m_pRxIFFr = NULL;
}

CalFileR1C::~CalFileR1C()
{
	SAFE_DELETE(m_pX9119);
	SAFE_DELETE(m_pTxLOLeak);
	SAFE_DELETE(m_pTxSideband);
	SAFE_DELETE(m_pTxPowerOP);
	SAFE_DELETE(m_pTxPowerIO);
	SAFE_DELETE(m_pTxAttOP);
	SAFE_DELETE(m_pTxAttIO);
	SAFE_DELETE(m_pRxRefOP);
	SAFE_DELETE(m_pRxRefIO);
	SAFE_DELETE(m_pRxAttOP);
	SAFE_DELETE(m_pRxAttIO);
	SAFE_DELETE(m_pTxFilter_80M);
	SAFE_DELETE(m_pTxFilter_160M);
	SAFE_DELETE(m_pRxFilter_80M);
	SAFE_DELETE(m_pRxFilter_160M);
	SAFE_DELETE(m_pTxFilOffOP_80M);
	SAFE_DELETE(m_pTxFilOffIO_80M);
	SAFE_DELETE(m_pRxFilOffOP_80M);
	SAFE_DELETE(m_pRxFilOffIO_80M);

	SAFE_DELETE(m_pTxRFFr0);
	SAFE_DELETE(m_pTxRFFr1);
	SAFE_DELETE(m_pTxIFFr);
	SAFE_DELETE(m_pRxRFFr);
	SAFE_DELETE(m_pRxIFFr);
}

int32_t CalFileR1C::Open()
{
	if (!m_pX9119)		m_pX9119 = new X9119TableR1C;
	if (!m_pTxLOLeak)	m_pTxLOLeak = new TxLOLeakageTableR1C;
	if (!m_pTxSideband)	m_pTxSideband = new TxSidebandTableR1C;
	if (!m_pTxPowerOP)	m_pTxPowerOP = new TxPowerOPTableR1C;
	if (!m_pTxPowerIO)	m_pTxPowerIO = new TxPowerIOTableR1C;
	if (!m_pTxAttOP)	m_pTxAttOP = new TxAttOPTableR1C;
	if (!m_pTxAttIO)	m_pTxAttIO = new TxAttIOTableR1C;
	if (!m_pRxRefOP)	m_pRxRefOP = new RxRefOPTableR1C;
	if (!m_pRxRefIO)	m_pRxRefIO = new RxRefIOTableR1C;
	if (!m_pRxAttOP)	m_pRxAttOP = new RxAttOPTableR1C;
	if (!m_pRxAttIO)	m_pRxAttIO = new RxAttIOTableR1C;

	if (_80M == m_BW) {
		if (!m_pTxFilter_80M)	m_pTxFilter_80M = new TxFilter_80M_TableR1C;
		if (!m_pRxFilter_80M)	m_pRxFilter_80M = new RxFilter_80M_TableR1C;
		if (!m_pTxFilOffOP_80M)	m_pTxFilOffOP_80M = new TxFilterOffTableR1C;
		if (!m_pTxFilOffIO_80M)	m_pTxFilOffIO_80M = new TxFilterOffTableR1C;
		if (!m_pRxFilOffOP_80M) m_pRxFilOffOP_80M = new RxFilterOffTableR1C;
		if (!m_pRxFilOffIO_80M) m_pRxFilOffIO_80M = new RxFilterOffTableR1C;
	}
	else if (_160M == m_BW) {
		if (!m_pTxFilter_160M)	m_pTxFilter_160M = new TxFilter_160M_TableR1C;
		if (!m_pRxFilter_160M)	m_pRxFilter_160M = new RxFilter_160M_TableR1C;
	}
	return ICalFile::Open();
}

int32_t CalFileR1C::SetBW(BW bw)
{
	if (m_BW == bw)
		return 0;
	if (_80M == bw) {
		SAFE_DELETE(m_pTxFilter_160M);
		SAFE_DELETE(m_pRxFilter_160M);

		if (!m_pTxFilter_80M)
			m_pTxFilter_80M = new TxFilter_80M_TableR1C;
        Map2Buf(ICalFile::TxFil_80);
		m_pTxFilter_80M->Map2Mem();

		if (!m_pRxFilter_80M)
			m_pRxFilter_80M = new RxFilter_80M_TableR1C;
        Map2Buf(ICalFile::RxFil_80);
		m_pRxFilter_80M->Map2Mem();

		if (!m_pTxFilOffOP_80M)
			m_pTxFilOffOP_80M = new TxFilterOffTableR1C;
        Map2Buf(ICalFile::TxFilOffOP_80);
		m_pTxFilOffOP_80M->Map2Mem();

		if (!m_pTxFilOffIO_80M)
			m_pTxFilOffIO_80M = new TxFilterOffTableR1C;
        Map2Buf(ICalFile::TxFilOffIO_80);
		m_pTxFilOffIO_80M->Map2Mem();

		if (!m_pRxFilOffOP_80M)
			m_pRxFilOffOP_80M = new RxFilterOffTableR1C;
        Map2Buf(ICalFile::RxFilOffOP_80);
		m_pRxFilOffOP_80M->Map2Mem();

		if (!m_pRxFilOffIO_80M)
			m_pRxFilOffIO_80M = new RxFilterOffTableR1C;
        Map2Buf(ICalFile::RxFilOffIO_80);
		m_pRxFilOffIO_80M->Map2Mem();
	}
	else if (_160M == bw) {
		SAFE_DELETE(m_pTxFilOffOP_80M);
		SAFE_DELETE(m_pTxFilOffIO_80M);
		SAFE_DELETE(m_pRxFilOffOP_80M);
		SAFE_DELETE(m_pRxFilOffIO_80M);

		SAFE_DELETE(m_pTxFilter_80M);
		SAFE_DELETE(m_pRxFilter_80M);

		if (!m_pTxFilter_160M)
			m_pTxFilter_160M = new TxFilter_160M_TableR1C;
        Map2Buf(ICalFile::TxFil_160);
		m_pTxFilter_160M->Map2Mem();

		if (!m_pRxFilter_160M)
			m_pRxFilter_160M = new RxFilter_160M_TableR1C;
        Map2Buf(ICalFile::RxFil_160);
		m_pRxFilter_160M->Map2Mem();
	}
	return ICalFile::SetBW(bw);
}

int32_t CalFileR1C::Add(CalItem Item,void *pData)
{
    Map2Buf(Item);
	switch (Item) {
		case ICalFile::X9119		: {m_pX9119->Add((X9119TableR1C::DataF *)pData);return 0;}
		case ICalFile::TxLoLeakage	: {m_pTxLOLeak->Add((TxLOLeakageTableR1C::DataF *)pData);return 0;}
		case ICalFile::TxSideband	: {m_pTxSideband->Add((TxSidebandTableR1C::DataF *)pData);return 0;}
		case ICalFile::TxPowerOP	: {m_pTxPowerOP->Add((TxPowerOPTableR1C::DataF *)pData);return 0;}
		case ICalFile::TxPowerIO	: {m_pTxPowerIO->Add((TxPowerIOTableR1C::DataF *)pData);return 0;}
		case ICalFile::TxAttOP		: {m_pTxAttOP->Add((TxAttOPTableR1C::DataF *)pData);return 0;}
		case ICalFile::TxAttIO		: {m_pTxAttIO->Add((TxAttIOTableR1C::DataF *)pData);return 0;}
		case ICalFile::RxRefOP		: {m_pRxRefOP->Add((RxRefOPTableR1C::DataF *)pData);return 0;}
		case ICalFile::RxRefIO		: {m_pRxRefIO->Add((RxRefIOTableR1C::DataF *)pData);return 0;}
		case ICalFile::RxAttOP		: {m_pRxAttOP->Add((RxAttOPTableR1C::DataF *)pData);return 0;}
		case ICalFile::RxAttIO		: {m_pRxAttIO->Add((RxAttIOTableR1C::DataF *)pData);return 0;}
		case ICalFile::TxFil_80		: {m_pTxFilter_80M->Add((TxFilter_80M_TableR1C::DataF *)pData);return 0;}
		case ICalFile::TxFil_160	: {m_pTxFilter_160M->Add((TxFilter_160M_TableR1C::DataF *)pData);return 0;}
		case ICalFile::RxFil_80		: {m_pRxFilter_80M->Add((RxFilter_80M_TableR1C::DataF *)pData);return 0;}
		case ICalFile::RxFil_160	: {m_pRxFilter_160M->Add((RxFilter_160M_TableR1C::DataF *)pData);return 0;}
		case ICalFile::TxFilOffOP_80: {m_pTxFilOffOP_80M->Add((TxFilterOffTableR1C::DataF *)pData);return 0;}
		case ICalFile::TxFilOffIO_80: {m_pTxFilOffIO_80M->Add((TxFilterOffTableR1C::DataF *)pData);return 0;}
		case ICalFile::RxFilOffOP_80: {m_pRxFilOffOP_80M->Add((RxFilterOffTableR1C::DataF *)pData);return 0;}
		case ICalFile::RxFilOffIO_80: {m_pRxFilOffIO_80M->Add((RxFilterOffTableR1C::DataF *)pData);return 0;}

		case ICalFile::TxRFFr_0	: {m_pTxRFFr0->Add((TxRFFr_0_TableR1C::DataF *)pData);return 0;}
		case ICalFile::TxRFFr_1	: {m_pTxRFFr1->Add((TxRFFr_1_TableR1C::DataF *)pData);return 0;}
		case ICalFile::TxIFFr	: {m_pTxIFFr->Add((TxIFFrTableR1C::DataF *)pData);return 0;}
		case ICalFile::RxRFFr	: {m_pRxRFFr->Add((RxRFFrTableR1C::DataF *)pData);return 0;}
		case ICalFile::RxIFFr	: {m_pRxIFFr->Add((RxIFFrTableR1C::DataF *)pData);return 0;}
		default:return 0;
	}
	return 0;
}

int32_t CalFileR1C::Map2Buf(CalItem Item)
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
			case ICalFile::X9119		: {m_pX9119->MapBuf((X9119TableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxLoLeakage	: {m_pTxLOLeak->MapBuf((TxLOLeakageTableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxSideband	: {m_pTxSideband->MapBuf((TxSidebandTableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxPowerOP	: {m_pTxPowerOP->MapBuf((TxPowerOPTableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxPowerIO	: {m_pTxPowerIO->MapBuf((TxPowerIOTableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxAttOP		: {m_pTxAttOP->MapBuf((TxAttOPTableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxAttIO		: {m_pTxAttIO->MapBuf((TxAttIOTableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::RxRefOP		: {m_pRxRefOP->MapBuf((RxRefOPTableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::RxRefIO		: {m_pRxRefIO->MapBuf((RxRefIOTableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::RxAttOP		: {m_pRxAttOP->MapBuf((RxAttOPTableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::RxAttIO		: {m_pRxAttIO->MapBuf((RxAttIOTableR1C::DataF *)(g_ItemBuf.m_pBuf));return 0;}
			case ICalFile::TxFil_80		: {
				if (!m_pTxFilter_80M)
					m_pTxFilter_80M = new TxFilter_80M_TableR1C;
				m_pTxFilter_80M->MapBuf((TxFilter_80M_TableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
										  }
			case ICalFile::TxFil_160	: {
				if (!m_pTxFilter_160M)
					m_pTxFilter_160M = new TxFilter_160M_TableR1C;
				m_pTxFilter_160M->MapBuf((TxFilter_160M_TableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
										  }
			case ICalFile::RxFil_80		: {
				if (!m_pRxFilter_80M)
					m_pRxFilter_80M = new RxFilter_80M_TableR1C;
				m_pRxFilter_80M->MapBuf((RxFilter_80M_TableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
										  }
			case ICalFile::RxFil_160	: {
				if (!m_pRxFilter_160M)
					m_pRxFilter_160M = new RxFilter_160M_TableR1C;
				m_pRxFilter_160M->MapBuf((RxFilter_160M_TableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
										  }

			case ICalFile::TxFilOffOP_80 : {
				if (!m_pTxFilOffOP_80M)
					m_pTxFilOffOP_80M = new TxFilterOffTableR1C;
				m_pTxFilOffOP_80M->MapBuf((TxFilterOffTableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
										  }
			case ICalFile::TxFilOffIO_80 : {
				if (!m_pTxFilOffIO_80M)
					m_pTxFilOffIO_80M = new TxFilterOffTableR1C;
				m_pTxFilOffIO_80M->MapBuf((TxFilterOffTableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
										  }
			case ICalFile::RxFilOffOP_80 : {
				if (!m_pRxFilOffOP_80M)
					m_pRxFilOffOP_80M = new RxFilterOffTableR1C;
				m_pRxFilOffOP_80M->MapBuf((RxFilterOffTableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
										   }
			case ICalFile::RxFilOffIO_80 : {
				if (!m_pRxFilOffIO_80M)
					m_pRxFilOffIO_80M = new RxFilterOffTableR1C;
				m_pRxFilOffIO_80M->MapBuf((RxFilterOffTableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
										   }
			case ICalFile::TxRFFr_0 : {
				if (!m_pTxRFFr0)
					m_pTxRFFr0 = new TxRFFr_0_TableR1C;
				m_pTxRFFr0->MapBuf((TxRFFrTable::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
							   }
			case ICalFile::TxRFFr_1 : {
				if (!m_pTxRFFr1)
					m_pTxRFFr1 = new TxRFFr_1_TableR1C;
				m_pTxRFFr1->MapBuf((TxRFFrTable::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
									  }
			case ICalFile::TxIFFr : {
				if (!m_pTxIFFr)
					m_pTxIFFr = new TxIFFrTableR1C;
				m_pTxIFFr->MapBuf((TxIFFrTableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
							   } 
			case ICalFile::RxRFFr : {
				if (!m_pRxRFFr)
					m_pRxRFFr = new RxRFFrTableR1C;
				m_pRxRFFr->MapBuf((RxRFFrTableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
									}
			case ICalFile::RxIFFr : {
				if (!m_pRxIFFr)
					m_pRxIFFr = new RxIFFrTableR1C;
				m_pRxIFFr->MapBuf((RxIFFrTableR1C::DataF *)(g_ItemBuf.m_pBuf));
				return 0;
									}
			default:return 0;
		}
	}
	return 0;
}

int32_t CalFileR1C::Map2Mem()
{
    Map2Buf(ICalFile::X9119);		m_pX9119->Map2Mem();
    Map2Buf(ICalFile::TxLoLeakage);	m_pTxLOLeak->Map2Mem();
    Map2Buf(ICalFile::TxSideband);	m_pTxSideband->Map2Mem();
    Map2Buf(ICalFile::TxPowerOP);	m_pTxPowerOP->Map2Mem();
    Map2Buf(ICalFile::TxPowerIO);	m_pTxPowerIO->Map2Mem();
    Map2Buf(ICalFile::TxAttOP);		m_pTxAttOP->Map2Mem();
    Map2Buf(ICalFile::TxAttIO);		m_pTxAttIO->Map2Mem();
    Map2Buf(ICalFile::RxRefOP);		m_pRxRefOP->Map2Mem();
    Map2Buf(ICalFile::RxRefIO);		m_pRxRefIO->Map2Mem();
    Map2Buf(ICalFile::RxAttOP);		m_pRxAttOP->Map2Mem();
    Map2Buf(ICalFile::RxAttIO);		m_pRxAttIO->Map2Mem();
	if (_80M == m_BW) {
        Map2Buf(ICalFile::TxFil_80);		m_pTxFilter_80M->Map2Mem();
        Map2Buf(ICalFile::TxFilOffOP_80);	m_pTxFilOffOP_80M->Map2Mem();
        Map2Buf(ICalFile::TxFilOffIO_80);	m_pTxFilOffIO_80M->Map2Mem();
        Map2Buf(ICalFile::RxFil_80);		m_pRxFilter_80M->Map2Mem();
        Map2Buf(ICalFile::RxFilOffOP_80);	m_pRxFilOffOP_80M->Map2Mem();
        Map2Buf(ICalFile::RxFilOffIO_80);	m_pRxFilOffIO_80M->Map2Mem();
	}
	else if (_160M == m_BW) {
        Map2Buf(ICalFile::TxFil_160);		m_pTxFilter_160M->Map2Mem();
        Map2Buf(ICalFile::RxFil_160);		m_pRxFilter_160M->Map2Mem();
	}
    return 0;
}

int32_t CalFileR1C::Create()
{
	ICalFile::FileVer Ver;
	ICalFile::FileInfo R1AInfo;
    ICalFile::FileInfo R1CInfo;
	uint32_t uiItemIdxR1A = 0;
	uint32_t uiItemIdxR1C = 0;
	uint32_t uiMaxByteR1A = 0;
	uint32_t uiMaxByteR1C = 0;
	uint32_t uiMaxByte = 0;
	char szPath[64] = {0};
	sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_uiRfuIdx,m_uiRfIdx);

	Ver.m_uiHead = CAL_FILEVER_HEAD;
	Ver.m_uiVer  = CAL_FILE_VER;
	Ver.m_uiTail = CAL_FILEVER_TAIL;

	R1AInfo.m_uiHead = CAL_FILEINFO_HEAD;
	R1AInfo.m_uiTail = CAL_FILEINFO_TAIL;
	memset(R1AInfo.m_szSN,0,sizeof(R1AInfo.m_szSN));
    sprintf(R1AInfo.m_szSN,"SP1401R1B%08dRFU%uRF%u",m_uiOrdinal,m_uiRfuIdx,m_uiRfIdx);

	uiMaxByteR1A = CalFileR1A::GetFixItemTableR1A(R1AInfo.m_pItemInfo,R1AInfo.m_uiItems);
	uiMaxByteR1C = CalFileR1C::GetFixItemTableR1C(R1CInfo.m_pItemInfo,R1CInfo.m_uiItems);
	uiMaxByte = uiMaxByteR1A >= uiMaxByteR1C ? uiMaxByteR1A : uiMaxByteR1C;

	R1AInfo.m_uiItems -= 1;
	for (uint32_t i = 0;i < R1AInfo.m_uiItems;i ++) {
		if (X9119 == i)		//r1a's cal item table = enum CalItem,so it is safe here
			uiItemIdxR1A ++;
		R1AInfo.m_pItemInfo[i] = R1AInfo.m_pItemInfo[uiItemIdxR1A];
		uiItemIdxR1A ++;
	}

    FILE *fp;
    if (_access("C:\\CSECal",0) == -1) {
        if (_mkdir("C:\\CSECal") == -1) {
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
    //r1a/b
    fp = fopen(szPath,"w+");
    CFO_ASSERT(fp,NULL == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fwrite(&Ver,sizeof(ICalFile::FileVer),1,fp) == 0);
    CFO_ASSERT(fp,fwrite(&R1AInfo,sizeof(ICalFile::FileInfo),1,fp) == 0);
    for (uint32_t i = 0;i < R1AInfo.m_uiItems;i ++)
        CFO_ASSERT(fp,fwrite(&(R1AInfo.m_pItemInfo[i]),sizeof(ICalFile::ItemInfo),1,fp) == 0);

	unsigned char *pBuf = new unsigned char[uiMaxByte];
	for (uint32_t i = 0;i < R1AInfo.m_uiItems;i ++) {
		uint32_t uiSize = ((R1AInfo.m_pItemInfo[i]).m_uiSize) * ((R1AInfo.m_pItemInfo[i]).m_uiPoint); 
		memset(pBuf,0,uiMaxByte * sizeof(unsigned char));
        CFO_ASSERT(fp,fwrite(pBuf,1,uiSize,fp) == 0);
	}

    //r1c/d
	R1CInfo.m_uiHead = CAL_FILEINFO_HEAD;
	R1CInfo.m_uiTail = CAL_FILEINFO_TAIL;
	memset(R1CInfo.m_szSN,0,sizeof(R1CInfo.m_szSN));
    sprintf(R1CInfo.m_szSN,"SP1401R1C%08dRFU%uRF%u",m_uiOrdinal,m_uiRfuIdx,m_uiRfIdx);

	R1CInfo.m_uiItems -= 1;
	for (uint32_t i = 0;i < R1CInfo.m_uiItems;i ++) {
		if (X9119 == R1CInfo.m_pItemInfo[uiItemIdxR1C].m_Item)	//r1c may add or only use a part of cal item which defined before,so make some change here
			uiItemIdxR1C ++;
		R1CInfo.m_pItemInfo[i] = R1CInfo.m_pItemInfo[uiItemIdxR1C];
		uiItemIdxR1C ++;
	}
	
    CFO_ASSERT(fp,fwrite(&R1CInfo,sizeof(ICalFile::FileInfo),1,fp) == 0);
    for (uint32_t i = 0;i < R1CInfo.m_uiItems;i ++)
        CFO_ASSERT(fp,fwrite(&(R1CInfo.m_pItemInfo[i]),sizeof(ICalFile::ItemInfo),1,fp) == 0);

	for (uint32_t i = 0;i < R1CInfo.m_uiItems;i ++) {
		uint32_t uiSize = ((R1CInfo.m_pItemInfo[i]).m_uiSize) * ((R1CInfo.m_pItemInfo[i]).m_uiPoint);
        memset(pBuf,0,uiMaxByte * sizeof(unsigned char));
        CFO_ASSERT(fp,fwrite(pBuf,1,uiSize,fp) == 0);
	}
	delete []pBuf;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);
	return 0;
}

int32_t CalFileR1C::Load(FileInfo &Info)
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

    CFO_ASSERT(fp,fseek(fp,R1A_FILE_SIZE,SEEK_SET));

	ItemInfo *pItemInfo = Info.m_pItemInfo;
    CFO_ASSERT(fp,fread(&Info,sizeof(ICalFile::FileInfo),1,fp) == 0);
    Info.m_pItemInfo = pItemInfo;
	ShowFileInfo(&Info);

	for (uint32_t i = 0;i < Info.m_uiItems;i ++) {
        CFO_ASSERT(fp,fread(&(Info.m_pItemInfo[i]),sizeof(ICalFile::ItemInfo),1,fp) == 0);
		ShowItemInfo(&(Info.m_pItemInfo[i]));
	}
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);
	return 0;
}

int32_t CalFileR1C::GetItemSize(CalItem Item,uint32_t &dwPos,uint32_t &uiSize)
{
	ICalFile::FileInfo Info;
    INT_CHECK(Load(Info));

	uint32_t uiPoint = 0;
	for (uint32_t i = 0;i < Info.m_uiItems;i ++) {
		if (Info.m_pItemInfo[i].m_Item == Item) {
			uiSize = Info.m_pItemInfo[i].m_uiSize;
			uiPoint = Info.m_pItemInfo[i].m_uiPoint;
			break;
		}
	}
	uiSize = uiSize * uiPoint;

	if (ICalFile::X9119 == Item) {
		dwPos = 0;
        uiSize = sizeof(X9119TableR1C::DataF);
		return 0;
	}

	dwPos = R1A_FILE_SIZE;
	dwPos += sizeof(ICalFile::FileInfo);
	for (uint32_t i = 0;i < Info.m_uiItems;i ++)
		dwPos += sizeof(ICalFile::ItemInfo);
	for (uint32_t i = 0;i < Info.m_uiItems;i ++) {
		if (Item != Info.m_pItemInfo[i].m_Item)
			dwPos += ((Info.m_pItemInfo[i]).m_uiSize) * ((Info.m_pItemInfo[i]).m_uiPoint);
		else
			break;
	}
	return 0;
}
