#include "CfvCtrl.h"
#include "Shlwapi.h"
#include "CalFileR1A.h"
#include "CalFileR1C.h"

typedef int32_t (CfvCtrl::*ForConv)();
typedef int32_t (CfvCtrl::*BckConv)();

void CfvCtrl::Target(ICalFile *pFile)
{
	m_pFile = pFile;
}

int32_t CfvCtrl::ForConv_0000_0001()
{
	struct CalItemInfoV0000 {
		uint32_t m_uiHead;
		CalFileR1A::CalItem m_Item;
		uint32_t m_uiSize;
		uint32_t m_uiPoint;
		uint32_t m_uiTail;
	};

	struct CalFileInfoV0000 {
		char m_szSN[32];
		uint32_t m_uiItems;
		CalItemInfoV0000 *m_pItemInfo;
	}; 

	struct CalFileInfoV0001 {
		uint32_t m_uiHead;
		char	 m_szSN[32];
		uint32_t m_uiItems;
		CalItemInfoV0000 *m_pItemInfo;
		uint32_t m_uiTail;
	};

	struct CalFileVerV0001 {
		uint32_t m_uiHead;
		uint32_t m_uiVer;
		uint32_t m_uiTail;
	};

	CalFileInfoV0000 FileInfoV0000;
	CalFileInfoV0001 FileInfoV0001; 
	CalFileVerV0001  FileVerV0001;

	char szPath[64] = {0};
	sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_pFile->m_uiRfuIdx,m_pFile->m_uiRfIdx);

	char szPathNew[64];
	strcpy(szPathNew,szPath);
	strcat(szPathNew,"_v0001_temp");

    FILE *fp_old;
    FILE *fp_new;

    fp_old = fopen(szPath,"r+");
    CFO_ASSERT(fp_old,NULL == fp_old);
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_old,fread(&FileInfoV0000,sizeof(CalFileInfoV0000),1,fp_old) == 0);
	FileInfoV0000.m_pItemInfo = new CalItemInfoV0000[(FileInfoV0000.m_uiItems)];
    for (uint32_t i = 0;i < FileInfoV0000.m_uiItems;i ++)
        CFO_ASSERT(fp_old,fread(&(FileInfoV0000.m_pItemInfo[i]),sizeof(CalItemInfoV0000),1,fp_old) == 0);

	FileInfoV0001.m_uiHead	  = 0x02ABCDEF;
	strcpy(FileInfoV0001.m_szSN,FileInfoV0000.m_szSN);
	FileInfoV0001.m_uiItems	  = FileInfoV0000.m_uiItems;
	FileInfoV0001.m_pItemInfo = FileInfoV0000.m_pItemInfo;
	FileInfoV0001.m_uiTail	  = 0x03ABCDEF;

	FileVerV0001.m_uiHead = 0x04ABCDEF;
	FileVerV0001.m_uiVer  = 0x0001;
	FileVerV0001.m_uiTail = 0x05ABCDEF;

    fp_new = fopen(szPathNew,"w+");
    CFO_ASSERT(fp_new,NULL == fp_new);
    CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
    CFO_ASSERT(fp_new,fwrite(&FileVerV0001,sizeof(CalFileVerV0001),1,fp_new) == 0);
    CFO_ASSERT(fp_new,fwrite(&FileInfoV0001,sizeof(CalFileInfoV0001),1,fp_new) == 0);
    for (uint32_t i = 0;i < FileInfoV0001.m_uiItems;i ++)
        CFO_ASSERT(fp_new,fwrite(&(FileInfoV0001.m_pItemInfo[i]),sizeof(CalItemInfoV0000),1,fp_new) == 0);

	for (uint32_t i = 0;i < FileInfoV0001.m_uiItems;i ++) {
		uint32_t uiSize = ((FileInfoV0001.m_pItemInfo[i]).m_uiSize) * ((FileInfoV0001.m_pItemInfo[i]).m_uiPoint); 
		unsigned char *pBuf = new unsigned char[uiSize];
		memset(pBuf,0,uiSize * sizeof(unsigned char));
        CFO_ASSERT(fp_old,fread(pBuf,sizeof(unsigned char),uiSize,fp_old) == 0);
        CFO_ASSERT(fp_new,fwrite(pBuf,sizeof(unsigned char),uiSize,fp_new) == 0);
		delete []pBuf;
	}
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
    fclose(fp_old);
    fclose(fp_new);

    remove(szPath);
    rename(szPathNew,szPath);
	delete []FileInfoV0000.m_pItemInfo;
    Log.stdprintf("file \"%s\" version 0x0000 ---> 0x0001\n",szPath);
    Log.AddMsgList(0,"file \"%s\" version 0x0000 ---> 0x0001",szPath);
	return 0;
}

int32_t CfvCtrl::ForConv_0001_0002()
{
	struct CalFileVerV0001 {
		uint32_t m_uiHead;
		uint32_t m_uiVer;
		uint32_t m_uiTail;
	};

	struct CalItemInfoV0000 {
		uint32_t m_uiHead;
		CalFileR1A::CalItem m_Item;
		uint32_t m_uiSize;
		uint32_t m_uiPoint;
		uint32_t m_uiTail;
	};

	struct CalFileInfoV0001 {
		uint32_t m_uiHead;
		char	 m_szSN[32];
		uint32_t m_uiItems;
		CalItemInfoV0000 *m_pItemInfo;
		uint32_t m_uiTail;
	};

	struct X9119Data {
		int16_t m_iValue;
	};

	CalFileInfoV0001 FileInfoV0001;
	CalFileVerV0001 FileVerV0001;
	X9119Data Data;
	Data.m_iValue = 0;

	char szPath[64] = {0};
	char szPathNew[64] = {0};
    sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_pFile->m_uiRfuIdx,m_pFile->m_uiRfIdx);
    strcpy(szPathNew,szPath);

    FILE *fp_old;
    FILE *fp_new;

    fp_old = fopen(szPath,"r+");
    CFO_ASSERT(fp_old,NULL == fp_old);
    CFO_ASSERT(fp_old,fseek(fp_old,sizeof(CalFileVerV0001),SEEK_SET));
    CFO_ASSERT(fp_old,fread(&FileInfoV0001,sizeof(CalFileInfoV0001),1,fp_old) == 0);
	FileInfoV0001.m_pItemInfo = new CalItemInfoV0000[(FileInfoV0001.m_uiItems)];
    for (uint32_t i = 0;i < FileInfoV0001.m_uiItems;i ++)
        CFO_ASSERT(fp_old,fread(&FileInfoV0001.m_pItemInfo[i],sizeof(CalItemInfoV0000),1,fp_old) == 0);

    for (int32_t i = 0;i < CalFileR1A::X9119;i ++) {//for the list of calibration item is only allowed to accumulate,so it is safe to use the enum member here
        CFO_ASSERT(fp_old,fseek(fp_old,FileInfoV0001.m_pItemInfo[i].m_uiSize * FileInfoV0001.m_pItemInfo[i].m_uiPoint,SEEK_CUR));
    }
    CFO_ASSERT(fp_old,fread(&Data,sizeof(X9119Data),1,fp_old) == 0);

    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_old,fread(&FileVerV0001,sizeof(CalFileVerV0001),1,fp_old) == 0);
	FileVerV0001.m_uiVer += 1;
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_old,fwrite(&FileVerV0001,sizeof(CalFileVerV0001),1,fp_old) == 0);

	FileInfoV0001.m_uiItems -= 1;
    CFO_ASSERT(fp_old,fwrite(&FileInfoV0001,sizeof(CalFileInfoV0001),1,fp_old) == 0);

	int32_t iDataSize = 0;
	for (uint32_t i = 0;i < FileInfoV0001.m_uiItems;i ++)
		iDataSize += FileInfoV0001.m_pItemInfo[i].m_uiSize * FileInfoV0001.m_pItemInfo[i].m_uiPoint;
	char *pBuf = new char[iDataSize / sizeof(char)];
    CFO_ASSERT(fp_old,fseek(fp_old,(FileInfoV0001.m_uiItems + 1) * sizeof(CalItemInfoV0000),SEEK_CUR));
    CFO_ASSERT(fp_old,fread(pBuf,1,iDataSize,fp_old) == 0);
    CFO_ASSERT(fp_old,fseek(fp_old,sizeof(CalFileVerV0001) + sizeof(CalFileInfoV0001) + FileInfoV0001.m_uiItems * sizeof(CalItemInfoV0000),SEEK_SET));
    CFO_ASSERT(fp_old,fwrite(pBuf,1,iDataSize,fp_old) == 0);
    delete []pBuf;

    long lNewFileSize = ftell(fp_old);
    pBuf = new char[lNewFileSize / sizeof(char)];
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_old,fread(pBuf,1,lNewFileSize,fp_old) == 0);
    fclose(fp_old);
    fp_old = fopen(szPath,"w");
    CFO_ASSERT(fp_old,NULL == fp_old);
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    CFO_ASSERT(fp_old,fwrite(pBuf,1,lNewFileSize,fp_old) == 0);
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    fclose(fp_old);
    delete []pBuf;
	
	*strrchr(szPathNew,'\\') = 0;
	strcat(szPathNew,"\\cxu.cal");

    fp_new = fopen(szPathNew,"r+");
    if (NULL == fp_new) {
        fp_new = fopen(szPathNew,"w");
        CFO_ASSERT(fp_new,NULL == fp_new);
		X9119Data Data;
		Data.m_iValue = 0;
        CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
        CFO_ASSERT(fp_new,fwrite(&Data,sizeof(X9119Table::Data),1,fp_new) == 0);
        CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
        fclose(fp_new);
	}
    if (Data.m_iValue) {
        fp_new = fopen(szPathNew,"w");
        CFO_ASSERT(fp_new,NULL == fp_new);
        CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
        CFO_ASSERT(fp_new,fwrite(&Data,sizeof(X9119Table::Data),1,fp_new) == 0);
        CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
        fclose(fp_new);
	}
	delete []FileInfoV0001.m_pItemInfo;
    Log.stdprintf("file \"%s\" version 0x0001 ---> 0x0002\n",szPath);
    Log.AddMsgList(0,"file \"%s\" version 0x0001 ---> 0x0002",szPath);
	return 0;
}

int32_t CfvCtrl::ForConv_0002_0003()
{
	struct CalFileVerV0003 {
		uint32_t m_uiHead;
		uint32_t m_uiVersion;
		uint32_t m_uiTail;
	};

	struct CalItemInfoV0003 {
		uint32_t m_uiHead;
		CalFileR1A::CalItem m_Item;
		uint32_t m_uiSize;
		uint32_t m_uiPoint;
		uint32_t m_uiTail;
	};

	struct CalFileInfoV0003 {
		uint32_t m_uiHead;
		char	 m_szSN[32];
		uint32_t m_uiItem;
        CalItemInfoV0003 *m_pItemInfo;
		uint32_t m_uiTail;
	};

	struct RxState {
		int64_t  m_iADOffset;
		int32_t  m_iLNAAtt;
		double   m_dAtt1;
		int32_t  m_iAtt2;
	};

	struct RxRefDataV0002 {
		uint64_t m_uiFreq;
		RxState m_StateOP[SERIE_SIZE(30,-30,-1)];
		RxState m_StateIO[SERIE_SIZE(30,-20,-1)];
	};

	struct RxRefDataV0003 {
		uint64_t m_uiFreq;
		RxState m_StateOP[SERIE_SIZE(30,-40,-1)];
		RxState m_StateIO[SERIE_SIZE(30,-30,-1)];
	public:
		RxRefDataV0003()
		{
            for (uint32_t i = 0;i < ARRAY_SIZE(m_StateOP);i ++) {
				m_StateOP[i].m_iLNAAtt = 0;
				m_StateOP[i].m_dAtt1 = 0.0;
				m_StateOP[i].m_iAtt2 = 0;
				m_StateOP[i].m_iADOffset = 0;
			}
            for (uint32_t i = 0;i < ARRAY_SIZE(m_StateIO);i ++) {
				m_StateIO[i].m_iLNAAtt = 0;
				m_StateIO[i].m_dAtt1 = 0.0;
				m_StateIO[i].m_iAtt2 = 0;
				m_StateIO[i].m_iADOffset = 0;
			}
		}
	};

	char szPathNew[64] = {0};
	char szPath[64] = {0};
	sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_pFile->m_uiRfuIdx,m_pFile->m_uiRfIdx);
	strcpy(szPathNew,szPath);
	strcat(szPathNew,"_v0001_temp");

	uint32_t uiMaxSize = 0;
	char *pBuf = NULL;

    FILE *fp_old = fopen(szPath,"r+");
    CFO_ASSERT(fp_old,NULL == fp_old);
    CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
    FILE *fp_new = fopen(szPathNew,"w+");
    CFO_ASSERT(fp_new,NULL == fp_new);
    CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));

	CalFileVerV0003 FileVer;
    CFO_ASSERT(fp_old,fread(&FileVer,sizeof(CalFileVerV0003),1,fp_old) == 0);
	FileVer.m_uiVersion += 1;
    CFO_ASSERT(fp_new,fwrite(&FileVer,sizeof(CalFileVerV0003),1,fp_new) == 0);

	CalFileInfoV0003 FileInfo;
    CFO_ASSERT(fp_old,fread(&FileInfo,sizeof(CalFileInfoV0003),1,fp_old) == 0);
    CFO_ASSERT(fp_new,fwrite(&FileInfo,sizeof(CalFileInfoV0003),1,fp_new) == 0);

    FileInfo.m_pItemInfo = new CalItemInfoV0003[FileInfo.m_uiItem];
	for (uint32_t i = 0;i < FileInfo.m_uiItem;i ++) {
        CFO_ASSERT(fp_old,fread(&(FileInfo.m_pItemInfo[i]),sizeof(CalItemInfoV0003),1,fp_old) == 0);
		if (i == CalFileR1A::RxRef)
            FileInfo.m_pItemInfo[i].m_uiSize = sizeof(RxRefDataV0003);
        if (FileInfo.m_pItemInfo[i].m_uiSize * FileInfo.m_pItemInfo[i].m_uiPoint > uiMaxSize)
            uiMaxSize = FileInfo.m_pItemInfo[i].m_uiSize * FileInfo.m_pItemInfo[i].m_uiPoint;
        CFO_ASSERT(fp_new,fwrite(&(FileInfo.m_pItemInfo[i]),sizeof(CalItemInfoV0003),1,fp_new) == 0);
	}
	pBuf = new char[uiMaxSize / sizeof(char)];
	for (uint32_t i = 0;i < CalFileR1A::RxRef;i ++) {
		memset(pBuf,0,uiMaxSize / sizeof(char));
        CFO_ASSERT(fp_old,fread(pBuf,1,FileInfo.m_pItemInfo[i].m_uiSize * FileInfo.m_pItemInfo[i].m_uiPoint,fp_old) == 0);
        CFO_ASSERT(fp_new,fwrite(pBuf,1,FileInfo.m_pItemInfo[i].m_uiSize * FileInfo.m_pItemInfo[i].m_uiPoint,fp_new) == 0);
	}
	RxRefDataV0002 DataV0002;
	RxRefDataV0003 DataV0003;
	for (int32_t i = 0;i < SERIE_SIZE(100000000,6000000000,5000000);i ++) {
        CFO_ASSERT(fp_old,fread(&DataV0002,sizeof(RxRefDataV0002),1,fp_old) == 0);
		DataV0003.m_uiFreq = DataV0002.m_uiFreq;
        for (uint32_t j = 0;j < ARRAY_SIZE(DataV0002.m_StateOP);j ++) {
			DataV0003.m_StateOP[j].m_iLNAAtt = DataV0002.m_StateOP[j].m_iLNAAtt;
			DataV0003.m_StateOP[j].m_dAtt1 = DataV0002.m_StateOP[j].m_dAtt1;
			DataV0003.m_StateOP[j].m_iAtt2 = DataV0002.m_StateOP[j].m_iAtt2;
			DataV0003.m_StateOP[j].m_iADOffset = DataV0002.m_StateOP[j].m_iADOffset;
		}
        for (uint32_t j = 0;j < ARRAY_SIZE(DataV0002.m_StateIO);j ++) {
			DataV0003.m_StateIO[j].m_iLNAAtt = DataV0002.m_StateIO[j].m_iLNAAtt;
			DataV0003.m_StateIO[j].m_dAtt1 = DataV0002.m_StateIO[j].m_dAtt1;
			DataV0003.m_StateIO[j].m_iAtt2 = DataV0002.m_StateIO[j].m_iAtt2;
			DataV0003.m_StateIO[j].m_iADOffset = DataV0002.m_StateIO[j].m_iADOffset;
		}
        CFO_ASSERT(fp_new,fwrite(&DataV0003,sizeof(RxRefDataV0003),1,fp_new) == 0);
	}
    fclose(fp_new);
    fclose(fp_old);

    remove(szPath);
    rename(szPathNew,szPath);

	delete []pBuf;
    delete []FileInfo.m_pItemInfo;
    Log.stdprintf("file \"%s\" version 0x0002 ---> 0x0003\n",szPath);
    Log.AddMsgList(0,"file \"%s\" version 0x0002 ---> 0x0003",szPath);
	return 0;
}

int32_t CfvCtrl::ForConv_0003_0004()
{
	typedef struct CalFileVerV0004 {
		uint32_t m_uiHead;
		uint32_t m_uiVersion;
		uint32_t m_uiTail;
	}CalFileVerV0004;

	typedef struct CalItemInfoV0004 {
		uint32_t m_uiHead;
		CalFileR1A::CalItem m_Item;
		uint32_t m_uiSize;
		uint32_t m_uiPoint;
		uint32_t m_uiTail;
	}CalItemInfoV0004;

	typedef struct CalFileInfoV0004 {
		uint32_t m_uiHead;
		char	 m_szSN[32];
		uint32_t m_uiItems;
		CalItemInfoV0004 *m_pItemInfo;
		uint32_t m_uiTail;
	public:
		CalFileInfoV0004() {
			m_pItemInfo = new CalItemInfoV0004[CAL_TOTAL_ITEMS];
		}
		~CalFileInfoV0004() {
			SAFE_DELETE(m_pItemInfo);
		}
	}CalFileInfoV0004;

	typedef struct AttIOData {
		uint64_t m_uiFreq;
		float	 m_fAtt1_10;
		float	 m_fAtt1_20;
		float	 m_fAtt1_30;
		float	 m_fAtt2_10;
		float	 m_fAtt2_20;
		float	 m_fAtt2_30;
		float	 m_fAtt3_10;
		float	 m_fAtt3_20;
		float	 m_fAtt3_30;
	}AttIOData;

	CalFileVerV0004 FileVer;
	uint32_t uiAttOutPos= 0,uiAttOutSize = 0;
    int32_t iOldDataSize = 0;
	CalFileInfoV0004 FileInfo;
	CalItemInfoV0004 ItemAttIOInfo;

	char szPath[64] = {0};
	sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_pFile->m_uiRfuIdx,m_pFile->m_uiRfIdx);

    FILE *fp = fopen(szPath,"r+");
    CFO_ASSERT(fp,NULL == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fread(&FileVer,sizeof(CalFileVerV0004),1,fp) == 0);
	FileVer.m_uiVersion ++;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fwrite(&FileVer,sizeof(CalFileVerV0004),1,fp) == 0);

	CalItemInfoV0004 *pItemInfo = FileInfo.m_pItemInfo;
    CFO_ASSERT(fp,fread(&FileInfo,sizeof(CalFileInfoV0004),1,fp) == 0);
	FileInfo.m_pItemInfo = pItemInfo;

    for (uint32_t i = 0;i < FileInfo.m_uiItems;i ++)
        CFO_ASSERT(fp,fread(&(FileInfo.m_pItemInfo[i]),sizeof(CalItemInfoV0004),1,fp) == 0);
	FileInfo.m_uiItems ++;
    CFO_ASSERT(fp,fseek(fp,sizeof(CalFileVerV0004),SEEK_SET));
    CFO_ASSERT(fp,fwrite(&FileInfo,sizeof(CalFileInfoV0004),1,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,(FileInfo.m_uiItems - 1) * sizeof(CalItemInfoV0004),SEEK_CUR));

    int32_t iPosTableStar = ftell(fp);
    int32_t iPosTableStop = 0;
    CFO_ASSERT(fp,-1 == iPosTableStar);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_END));
    iPosTableStop = ftell(fp);
    CFO_ASSERT(fp,-1 == iPosTableStop);
    iOldDataSize = iPosTableStop - iPosTableStar;
    CFO_ASSERT(fp,fseek(fp,iPosTableStar,SEEK_SET));
    char *pBuf = new char[iOldDataSize / sizeof(char)];
    CFO_ASSERT(fp,fread(pBuf,1,iOldDataSize,fp) == 0);

    CFO_ASSERT(fp,fseek(fp,sizeof(CalItemInfoV0004),SEEK_END));
    CFO_ASSERT(fp,fseek(fp,sizeof(CalFileVerV0004) + sizeof(CalFileInfoV0004),SEEK_SET));
    CFO_ASSERT(fp,fseek(fp,(FileInfo.m_uiItems - 1) * sizeof(CalItemInfoV0004),SEEK_CUR));

	ItemAttIOInfo.m_uiHead  = 0xaa;
	ItemAttIOInfo.m_Item    = CalFileR1A::TxAttIO;
	ItemAttIOInfo.m_uiSize  = sizeof(AttIOData);
	ItemAttIOInfo.m_uiPoint = (uint32_t)SERIE_SIZE(50000000,6000000000,5000000);
	ItemAttIOInfo.m_uiTail  = 0xff;

    CFO_ASSERT(fp,fwrite(&ItemAttIOInfo,sizeof(CalItemInfoV0004),1,fp) == 0);
    CFO_ASSERT(fp,fwrite(pBuf,1,iOldDataSize,fp) == 0);
	delete []pBuf;
	
	uiAttOutPos += sizeof(CalFileVerV0004);
	uiAttOutPos += sizeof(CalFileInfoV0004);
	for (uint32_t i = 0;i < FileInfo.m_uiItems;i ++)
		uiAttOutPos += sizeof(CalItemInfoV0004);
	for (int32_t i = 0;i < (int32_t)(CalFileR1A::TxAttOP);i ++)
		uiAttOutPos += ((FileInfo.m_pItemInfo[i]).m_uiSize) * ((FileInfo.m_pItemInfo[i]).m_uiPoint);

	uiAttOutSize = FileInfo.m_pItemInfo[(int32_t)CalFileR1A::TxAttOP].m_uiSize * FileInfo.m_pItemInfo[(int32_t)CalFileR1A::TxAttOP].m_uiPoint;	//it's safe here

    CFO_ASSERT(fp,fseek(fp,uiAttOutSize,SEEK_END));
    CFO_ASSERT(fp,fseek(fp,uiAttOutPos,SEEK_SET));

	pBuf = new char[uiAttOutSize / sizeof(char)];
    CFO_ASSERT(fp,fread(pBuf,1,uiAttOutSize,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,iPosTableStop + sizeof(CalItemInfoV0004),SEEK_SET));
    CFO_ASSERT(fp,fwrite(pBuf,1,uiAttOutSize,fp) == 0);
	delete []pBuf;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);

    Log.stdprintf("file \"%s\" version 0x0003 ---> 0x0004\n",szPath);
    Log.AddMsgList(0,"file \"%s\" version 0x0003 ---> 0x0004",szPath);
	return 0;
}

int32_t CfvCtrl::ForConv_0004_0005()
{
	typedef struct CalFileVerV0005 {
		uint32_t m_uiHead;
		uint32_t m_uiVersion;
		uint32_t m_uiTail;
	}CalFileVerV0005;

#ifdef _MSC_VER
#pragma pack(4)
#endif
	STRUCT_ALIGN_S(CalItemInfoV0005,4)
		uint32_t m_uiHead : 32;
		CalFileR1A::CalItem m_Item : 32;
		uint32_t m_uiSize : 32;
		uint32_t m_uiPoint : 32;
		uint32_t m_uiTail : 32;
	STRUCT_ALIGN_E(CalItemInfoV0005,4)
#ifdef _MSC_VER
#pragma pack()
#endif

	typedef struct CalFileInfoV0005 {
		uint32_t m_uiHead;
		char	 m_szSN[32];
		uint32_t m_uiItems;
		CalItemInfoV0005 *m_pItemInfo;
		uint32_t m_uiTail;
	public:
		CalFileInfoV0005() {
			m_pItemInfo = new CalItemInfoV0005[CAL_TOTAL_ITEMS];
		}
		~CalFileInfoV0005() {
			SAFE_DELETE(m_pItemInfo);
		}
	}CalFileInfoV0005;

	CalFileVerV0005 FileVer;
	uint32_t dwBytesWritten = 0;
	uint32_t dwBytesRead = 0;
	uint32_t dwPos = 0;
	uint32_t uiMaxByte = 0;
	CalFileInfoV0005 FileInfo;

	char szPath[64] = {0};
	sprintf(szPath,"C:\\CSECal\\rfu%drf%d.cal",m_pFile->m_uiRfuIdx,m_pFile->m_uiRfIdx);

    FILE *fp = fopen(szPath,"r+");
    CFO_ASSERT(fp,NULL == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fread(&FileVer,sizeof(CalFileVerV0005),1,fp) == 0);
	FileVer.m_uiHead = 0x44A4AE51;
	FileVer.m_uiVersion ++;
	FileVer.m_uiTail = 0x64704C83;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fwrite(&FileVer,sizeof(CalFileVerV0005),1,fp) == 0);

	CalItemInfoV0005 *pItemInfo = FileInfo.m_pItemInfo;
    CFO_ASSERT(fp,fread(&FileInfo,sizeof(CalFileInfoV0005),1,fp) == 0);
	FileInfo.m_pItemInfo = pItemInfo;
	FileInfo.m_uiHead = 0xA56834ED;
	FileInfo.m_uiTail = 0xDA974411;
    CFO_ASSERT(fp,fseek(fp,sizeof(CalFileVerV0005),SEEK_SET));
    CFO_ASSERT(fp,fwrite(&FileInfo,sizeof(CalFileInfoV0005),1,fp) == 0);

	for (uint32_t i = 0;i < FileInfo.m_uiItems;i ++) {
        CFO_ASSERT(fp,fread(&(FileInfo.m_pItemInfo[i]),sizeof(CalItemInfoV0005),1,fp) == 0);
		FileInfo.m_pItemInfo[i].m_uiHead = 0xaa;
		FileInfo.m_pItemInfo[i].m_uiTail = 0xff;
	}
    CFO_ASSERT(fp,fseek(fp,sizeof(CalFileVerV0005) + sizeof(CalFileInfoV0005),SEEK_SET));
    for (uint32_t i = 0;i < FileInfo.m_uiItems;i ++)
        CFO_ASSERT(fp,fwrite(&(FileInfo.m_pItemInfo[i]),sizeof(CalItemInfoV0005),1,fp) == 0);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_END));

	CalItemInfoV0005 ItemInfoR1C[23] = {
// 		{0xaa,ICalFile::TxLoLeakage,	104,	1,	 0xff},
// 		{0xaa,ICalFile::TxSideband,		112,	1,	 0xff},
// 		{0xaa,ICalFile::TxPowerOP,		120,	1191,0xff},
// 		{0xaa,ICalFile::TxPowerIO,		120,	1191,0xff},
// 		{0xaa,ICalFile::TxAttOP,		480,	1191,0xff},
// 		{0xaa,ICalFile::TxAttIO,		472,	1191,0xff},
// 		{0xaa,ICalFile::RxRefOP,		216,	1181,0xff},
// 		{0xaa,ICalFile::RxRefIO,		216,	1181,0xff},
// 		{0xaa,ICalFile::RxAttOP,		632,	1181,0xff},
// 		{0xaa,ICalFile::RxAttIO,		552,	1181,0xff},
// 		{0xaa,ICalFile::TxFil_80,		248,	1191,0xff},
// 		{0xaa,ICalFile::TxFil_160,		248,	1191,0xff},
// 		{0xaa,ICalFile::TxRFFr_0,		16,		1584,0xff},
// 		{0xaa,ICalFile::TxRFFr_1,		16,		1534,0xff},
// 		{0xaa,ICalFile::TxIFFr,			16,		84,	 0xff},
// 		{0xaa,ICalFile::RxFil_80,		464,	1181,0xff},
// 		{0xaa,ICalFile::RxFil_160,		656,	1181,0xff},
// 		{0xaa,ICalFile::RxRFFr,			16,		3034,0xff},
// 		{0xaa,ICalFile::RxIFFr,			16,		84,	 0xff},
// 		{0xaa,ICalFile::TxFilOffOP_80,	88,		1191,0xff},
// 		{0xaa,ICalFile::TxFilOffIO_80,	88,		1191,0xff},
// 		{0xaa,ICalFile::RxFilOffOP_80,	104,	1181,0xff},
// 		{0xaa,ICalFile::RxFilOffIO_80,	104,	1181,0xff}
		{0xaa,ICalFile::TxLoLeakage,	80,	1,	 0xff},
		{0xaa,ICalFile::TxSideband,		88,	1,	 0xff},
		{0xaa,ICalFile::TxPowerOP,		96,	1191,0xff},
		{0xaa,ICalFile::TxPowerIO,		96,	1191,0xff},
		{0xaa,ICalFile::TxAttOP,		456,1191,0xff},
		{0xaa,ICalFile::TxAttIO,		448,1191,0xff},
		{0xaa,ICalFile::RxRefOP,		192,1181,0xff},
		{0xaa,ICalFile::RxRefIO,		192,1181,0xff},
		{0xaa,ICalFile::RxAttOP,		608,1181,0xff},
		{0xaa,ICalFile::RxAttIO,		528,1181,0xff},
		{0xaa,ICalFile::TxFil_80,		224,1191,0xff},
		{0xaa,ICalFile::TxFil_160,		224,1191,0xff},
		{0xaa,ICalFile::TxRFFr_0,		16,	1584,0xff},
		{0xaa,ICalFile::TxRFFr_1,		16,	1534,0xff},
		{0xaa,ICalFile::TxIFFr,			16,	84,	 0xff},
		{0xaa,ICalFile::RxFil_80,		440,1181,0xff},
		{0xaa,ICalFile::RxFil_160,		632,1181,0xff},
		{0xaa,ICalFile::RxRFFr,			16,	3034,0xff},
		{0xaa,ICalFile::RxIFFr,			16,	84,	 0xff},
		{0xaa,ICalFile::TxFilOffOP_80,	64,	1191,0xff},
		{0xaa,ICalFile::TxFilOffIO_80,	64,	1191,0xff},
		{0xaa,ICalFile::RxFilOffOP_80,	80,	1181,0xff},
		{0xaa,ICalFile::RxFilOffIO_80,	80,	1181,0xff}
	};
	FileInfo.m_uiItems = 23;
	FileInfo.m_szSN[8] = 'C';
	for (uint32_t i = 0;i < FileInfo.m_uiItems;i ++)
		FileInfo.m_pItemInfo[i] = ItemInfoR1C[i];

    CFO_ASSERT(fp,fwrite(&FileInfo,sizeof(CalFileInfoV0005),1,fp) == 0);
    for (uint32_t i = 0;i < FileInfo.m_uiItems;i ++)
        CFO_ASSERT(fp,fwrite(&(FileInfo.m_pItemInfo[i]),sizeof(CalItemInfoV0005),1,fp) == 0);

	uiMaxByte = ICalFile::GetMaxItemByte();
	unsigned char *pBuf = new unsigned char[uiMaxByte];
	for (uint32_t i = 0;i < FileInfo.m_uiItems;i ++) {
		uint32_t uiSize = ((FileInfo.m_pItemInfo[i]).m_uiSize) * ((FileInfo.m_pItemInfo[i]).m_uiPoint);
		memset(pBuf,0,uiMaxByte * sizeof(unsigned char));
        CFO_ASSERT(fp,fwrite(pBuf,1,uiSize,fp) == 0);
	}
	delete []pBuf;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);

    Log.stdprintf("file \"%s\" version 0x0004 ---> 0x0005\n",szPath);
    Log.AddMsgList(0,"file \"%s\" version 0x0004 ---> 0x0005",szPath);
	return 0;
}

int32_t CfvCtrl::ForConv_0005_0006()
{
	struct FileVerV0005 {
		uint32_t m_uiHead;
		uint32_t m_uiVer;
		uint32_t m_uiTail;
	};

#ifdef _MSC_VER
#pragma pack(4)
#endif
    STRUCT_ALIGN_S(ItemInfoV0005,4)
        uint32_t m_uiHead : 32;
        ICalFile::CalItem  m_Item : 32;
        uint32_t m_uiSize : 32;
        uint32_t m_uiPoint : 32;
        uint32_t m_uiTail : 32;
    STRUCT_ALIGN_E(ItemInfoV0005,4)
#ifdef _MSC_VER
#pragma pack()
#endif

	typedef struct FileInfoV0005 {
		uint32_t  m_uiHead;
		char	  m_szSN[32];
		uint32_t  m_uiItems;
		ItemInfoV0005 *m_pItemInfo;
		uint32_t  m_uiTail;
	public:
		FileInfoV0005() {
			m_pItemInfo = new ItemInfoV0005[CAL_TOTAL_ITEMS];
		}
		~FileInfoV0005() {
			SAFE_DELETE(m_pItemInfo);
		}
	}FileInfoV0005; 

#ifdef _MSC_VER
#pragma pack(4)
#endif
	CLASS_ALIGN_S(UD_OleDateTime,4)		//from windows <ATLComTime.h>
	public:
		enum UD_DateTimeStatus	//from windows COleDateTime::DateTimeStatus
		{
			error = -1,
			valid = 0,
			invalid = 1,    // Invalid date (out of range, etc.)
			null = 2,       // Literally has no value
		};
	public:
		tm _2tm() {
			double dDiffDay = m_dT - 25569;
			time_t t_t = time_t((dDiffDay > 0.0 ? dDiffDay : 0.0) * 24 * 60 * 60);
			tm *t = localtime(&t_t);
			return *t;
		}
	public:
		double m_dT;
		UD_DateTimeStatus m_Status;
	CLASS_ALIGN_E(UD_OleDateTime,4)
#ifdef _MSC_VER
#pragma pack()
#endif
//tx lo leakage
	struct DataF_TxLOLeakage {
		uint64_t m_uiFreq;
		int16_t  m_iDCI;
		int16_t  m_iDCQ;
	};
	struct DataFV0005_TxLOLeakage : DataF_TxLOLeakage {
		bool   m_bUseSA;
		double m_dPower;
		double m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_TxLOLeakage : DataF_TxLOLeakage {
		bool   m_bUseSA;
		double m_dPower;
		double m_dTemp[4];
		tm	   m_EndTime;
	};
//tx sideband
	struct DataF_TxSideband {
		uint64_t m_uiFreq;
		double   m_dTh;
		uint16_t m_uiAmI;
		uint16_t m_uiAmQ;
	};
	struct DataFV0005_TxSideband : DataF_TxSideband {
		bool   m_bUseSA;
		double m_dPower;
		double m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_TxSideband : DataF_TxSideband {
		bool   m_bUseSA;
		double m_dPower;
		double m_dTemp[4];
		tm	   m_EndTime;
	};
//tx power
	struct DataFV0005_TxPower {
		uint64_t m_uiFreq;
		double   m_dDGain;
		double   m_dAtt0;
		double   m_dAtt1;
		double   m_dAtt2;
		double   m_dAtt3;
		double   m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_TxPower {
		uint64_t m_uiFreq;
		double   m_dDGain;
		double   m_dAtt0;
		double   m_dAtt1;
		double   m_dAtt2;
		double   m_dAtt3;
		double   m_dTemp[4];
		tm		 m_EndTime;
	};
//tx att
	struct DataFV0005_TxAttOP {
		uint64_t m_uiFreq;
		float    m_fOffset[R1C_TX_ATT_OP_POWER_PTS];
		double   m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_TxAttOP {
		uint64_t m_uiFreq;
		float    m_fOffset[R1C_TX_ATT_OP_POWER_PTS];
		double   m_dTemp[4];
		tm m_EndTime;
	};
	struct DataFV0005_TxAttIO {
		uint64_t m_uiFreq;
		float    m_fOffset[R1C_TX_ATT_IO_POWER_PTS];
		double   m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_TxAttIO {
		uint64_t m_uiFreq;
		float    m_fOffset[R1C_TX_ATT_IO_POWER_PTS];
		double   m_dTemp[4];
		tm		 m_EndTime;
	};
//rx ref
	struct RxStateF {
		int16_t m_iLNAAtt;
		int16_t m_iAtt019;
		float	m_fAtt1;
		float	m_fAtt2;
		float	m_fAtt3;
		double	m_dTemp[4];
		int64_t m_iADOffset;
	};
	struct DataFV0005_RxRef {
		uint64_t m_uiFreq;
		RxStateF m_State[3];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_RxRef {
		uint64_t m_uiFreq;
		RxStateF m_State[3];
		tm		 m_EndTime;
	};
//rx att
	struct DataFV0005_RxAttOP {
		uint64_t m_uiFreq;
		int64_t  m_iOffset[R1C_RX_REF_OP_PTS - 3 + 1];
		double   m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_RxAttOP {
		uint64_t m_uiFreq;
		int64_t  m_iOffset[R1C_RX_REF_OP_PTS - 3 + 1];
		double   m_dTemp[4];
		tm		 m_EndTime;
	};
	struct DataFV0005_RxAttIO {
		uint64_t m_uiFreq;
		int64_t  m_iOffset[R1C_RX_REF_IO_PTS - 3 + 1];
		double   m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_RxAttIO {
		uint64_t m_uiFreq;
		int64_t  m_iOffset[R1C_RX_REF_IO_PTS - 3 + 1];
		double   m_dTemp[4];
		tm		 m_EndTime;
	};
//tx filter
	struct DataFV0005_TxFil {
		uint64_t m_uiFreq;
		int16_t  m_iCoefReal[41];
		int16_t  m_iCoefImag[41];
		double   m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_TxFil {
		uint64_t m_uiFreq;
		int16_t  m_iCoefReal[41];
		int16_t  m_iCoefImag[41];
		double   m_dTemp[4];
		tm		 m_EndTime;
	};
//rx filter
	struct DataFV0005_RxFil_80 {
		uint64_t m_uiFreq;
		int16_t  m_iCoefReal[96];
		int16_t  m_iCoefImag[96];
		double   m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_RxFil_80 {
		uint64_t m_uiFreq;
		int16_t  m_iCoefReal[96];
		int16_t  m_iCoefImag[96];
		double   m_dTemp[4];
		tm		 m_EndTime;
	};
	struct DataFV0005_RxFil_160 {
		uint64_t m_uiFreq;
		int16_t  m_iCoefReal[144];
		int16_t  m_iCoefImag[144];
		double   m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_RxFil_160 {
		uint64_t m_uiFreq;
		int16_t  m_iCoefReal[144];
		int16_t  m_iCoefImag[144];
		double   m_dTemp[4];
		tm		 m_EndTime;
	};
//filter offset
	struct DataFV0005_TxFilOff {
		uint64_t m_uiFreq;
		double   m_dOffset;
		double   m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_TxFilOff {
		uint64_t m_uiFreq;
		double   m_dOffset;
		double   m_dTemp[4];
		tm		 m_EndTime;
	};
	struct DataFV0005_RxFilOff {
		uint64_t m_uiFreq;
		int64_t  m_iOffset[3];
		double   m_dTemp[4];
		UD_OleDateTime m_EndTime;
	};
	struct DataFV0006_RxFilOff {
		uint64_t m_uiFreq;
		int64_t  m_iOffset[3];
		double   m_dTemp[4];
		tm		 m_EndTime;
	};

	char szPathNew[64] = {0};
	char szPathOld[64] = {0};
	sprintf(szPathOld,"C:\\CSECal\\rfu%drf%d.cal",m_pFile->m_uiRfuIdx,m_pFile->m_uiRfIdx);
	strcpy(szPathNew,szPathOld);
	strcat(szPathNew,"_v0006_temp");

	FILE *fp_old = fopen(szPathOld,"r+");
	CFO_ASSERT(fp_old,NULL == fp_old);
	CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
	FILE *fp_new = fopen(szPathNew,"w+");
	CFO_ASSERT(fp_new,NULL == fp_new);
	CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));

	int32_t iSizeR1A = 5541296;
	char *pBufR1A = new char[iSizeR1A/sizeof(char)];
	CFO_ASSERT_S(fp_old,(fread(pBufR1A,iSizeR1A,1,fp_old) == 0),delete []pBufR1A;);
	CFO_ASSERT_S(fp_new,(fwrite(pBufR1A,iSizeR1A,1,fp_new) == 0),delete []pBufR1A;);
	CFO_ASSERT_S(fp_new,fflush(fp_new) == EOF,delete []pBufR1A;);
	CFO_ASSERT_S(fp_old,fseek(fp_old,0,SEEK_SET),delete []pBufR1A;);
	CFO_ASSERT_S(fp_new,fseek(fp_new,0,SEEK_SET),delete []pBufR1A;);
	delete []pBufR1A;

	FileVerV0005 Ver;
	CFO_ASSERT(fp_old,fread(&Ver,sizeof(Ver),1,fp_old) == 0);
	Ver.m_uiVer ++;
	CFO_ASSERT(fp_new,fwrite(&Ver,sizeof(Ver),1,fp_new) == 0);
	CFO_ASSERT(fp_new,fflush(fp_new) == EOF);

	CFO_ASSERT(fp_old,fseek(fp_old,iSizeR1A,SEEK_SET));
	CFO_ASSERT(fp_new,fseek(fp_new,iSizeR1A,SEEK_SET));

	ItemInfoV0005 ActualItemInfoR1C[23] = {
		{0xaa,ICalFile::TxLoLeakage,	104,	1,	 0xff},
		{0xaa,ICalFile::TxSideband,		112,	1,	 0xff},
		{0xaa,ICalFile::TxPowerOP,		120,	1191,0xff},
		{0xaa,ICalFile::TxPowerIO,		120,	1191,0xff},
		{0xaa,ICalFile::TxAttOP,		480,	1191,0xff},
		{0xaa,ICalFile::TxAttIO,		472,	1191,0xff},
		{0xaa,ICalFile::RxRefOP,		216,	1181,0xff},
		{0xaa,ICalFile::RxRefIO,		216,	1181,0xff},
		{0xaa,ICalFile::RxAttOP,		632,	1181,0xff},
		{0xaa,ICalFile::RxAttIO,		552,	1181,0xff},
		{0xaa,ICalFile::TxFil_80,		248,	1191,0xff},
		{0xaa,ICalFile::TxFil_160,		248,	1191,0xff},
		{0xaa,ICalFile::TxRFFr_0,		16,		1584,0xff},
		{0xaa,ICalFile::TxRFFr_1,		16,		1534,0xff},
		{0xaa,ICalFile::TxIFFr,			16,		84,	 0xff},
		{0xaa,ICalFile::RxFil_80,		464,	1181,0xff},
		{0xaa,ICalFile::RxFil_160,		656,	1181,0xff},
		{0xaa,ICalFile::RxRFFr,			16,		3034,0xff},
		{0xaa,ICalFile::RxIFFr,			16,		84,	 0xff},
		{0xaa,ICalFile::TxFilOffOP_80,	88,		1191,0xff},
		{0xaa,ICalFile::TxFilOffIO_80,	88,		1191,0xff},
		{0xaa,ICalFile::RxFilOffOP_80,	104,	1181,0xff},
		{0xaa,ICalFile::RxFilOffIO_80,	104,	1181,0xff}
	};
	FileInfoV0005 Info;
	ItemInfoV0005 *pItemInfo = Info.m_pItemInfo;
	CFO_ASSERT(fp_old,fread(&Info,sizeof(Info),1,fp_old) == 0);
	Info.m_pItemInfo = pItemInfo;
	CFO_ASSERT(fp_new,fwrite(&Info,sizeof(Info),1,fp_new) == 0);
	CFO_ASSERT(fp_new,fflush(fp_new) == EOF);

	for (uint32_t i = 0;i < Info.m_uiItems;i ++) {
		CFO_ASSERT(fp_old,fread(&(Info.m_pItemInfo[i]),sizeof(ItemInfoV0005),1,fp_old) == 0);
		CFO_ASSERT(fp_new,fwrite(&(ActualItemInfoR1C[i]),sizeof(ItemInfoV0005),1,fp_new) == 0);
		CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
	}

	uint32_t uiCurItemIdx = 0;
	uint32_t uiMaxByte = ICalFile::GetMaxItemByte();
	char *pBuf = new char[uiMaxByte / sizeof(char)];

	for (uint32_t uiCurItemIdx = 0;uiCurItemIdx < Info.m_uiItems;uiCurItemIdx ++) {
		CFO_ASSERT(fp_old,fread(pBuf,Info.m_pItemInfo[uiCurItemIdx].m_uiSize * Info.m_pItemInfo[uiCurItemIdx].m_uiPoint,1,fp_old) == 0);
		switch (Info.m_pItemInfo[uiCurItemIdx].m_Item) {
			case ICalFile::TxLoLeakage : {
				DataFV0005_TxLOLeakage *pDataLOLeakage = (DataFV0005_TxLOLeakage *)pBuf;
				DataFV0006_TxLOLeakage DataLOLeakage;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataLOLeakage.m_uiFreq	= pDataLOLeakage[i].m_uiFreq;
					DataLOLeakage.m_iDCI	= pDataLOLeakage[i].m_iDCI;
					DataLOLeakage.m_iDCQ	= pDataLOLeakage[i].m_iDCQ;
					DataLOLeakage.m_bUseSA	= pDataLOLeakage[i].m_bUseSA;
					DataLOLeakage.m_dPower	= pDataLOLeakage[i].m_dPower;
					DataLOLeakage.m_EndTime	= pDataLOLeakage[i].m_EndTime._2tm();
					for (int j = 0;j < 4;j ++)
						DataLOLeakage.m_dTemp[j] = pDataLOLeakage[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataLOLeakage,sizeof(DataLOLeakage),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
										 }
			case ICalFile::TxSideband : {
				DataFV0005_TxSideband *pDataSideband = (DataFV0005_TxSideband *)pBuf;
				DataFV0006_TxSideband DataSideband;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataSideband.m_uiFreq  = pDataSideband[i].m_uiFreq;
					DataSideband.m_dTh	   = pDataSideband[i].m_dTh;
					DataSideband.m_uiAmI   = pDataSideband[i].m_uiAmI;
					DataSideband.m_uiAmQ   = pDataSideband[i].m_uiAmQ;
					DataSideband.m_bUseSA  = pDataSideband[i].m_bUseSA;
					DataSideband.m_dPower  = pDataSideband[i].m_dPower;
					DataSideband.m_EndTime = pDataSideband[i].m_EndTime._2tm();
					for (int j = 0;j < 4;j ++)
						DataSideband.m_dTemp[j] = pDataSideband[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataSideband,sizeof(DataSideband),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
										}
			case ICalFile::TxPowerOP : 
			case ICalFile::TxPowerIO : {
				DataFV0005_TxPower *pDataTxPower = (DataFV0005_TxPower *)pBuf;
				DataFV0006_TxPower DataTxPower;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataTxPower.m_uiFreq  = pDataTxPower[i].m_uiFreq;
					DataTxPower.m_dDGain  = pDataTxPower[i].m_dDGain;
					DataTxPower.m_dAtt0   = pDataTxPower[i].m_dAtt0;
					DataTxPower.m_dAtt1   = pDataTxPower[i].m_dAtt1;
					DataTxPower.m_dAtt2   = pDataTxPower[i].m_dAtt2;
					DataTxPower.m_dAtt3	  = pDataTxPower[i].m_dAtt3;
					DataTxPower.m_EndTime = pDataTxPower[i].m_EndTime._2tm();
					for (int j = 0;j < 4;j ++)
						DataTxPower.m_dTemp[j] = pDataTxPower[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataTxPower,sizeof(DataTxPower),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
									   }
			case ICalFile::TxAttOP : {
				DataFV0005_TxAttOP *pDataTxAttOP = (DataFV0005_TxAttOP *)pBuf;
				DataFV0006_TxAttOP DataTxAttOP;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataTxAttOP.m_uiFreq  = pDataTxAttOP[i].m_uiFreq;
					DataTxAttOP.m_EndTime = pDataTxAttOP[i].m_EndTime._2tm();
					for (int j = 0;j < R1C_TX_ATT_OP_POWER_PTS;j ++)
						DataTxAttOP.m_fOffset[j] = pDataTxAttOP[i].m_fOffset[j];
					for (int j = 0;j < 4;j ++)
						DataTxAttOP.m_dTemp[j] = pDataTxAttOP[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataTxAttOP,sizeof(DataTxAttOP),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
									 }
			case ICalFile::TxAttIO : {
				DataFV0005_TxAttIO *pDataTxAttIO = (DataFV0005_TxAttIO *)pBuf;
				DataFV0006_TxAttIO DataTxAttIO;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataTxAttIO.m_uiFreq  = pDataTxAttIO[i].m_uiFreq;
					DataTxAttIO.m_EndTime = pDataTxAttIO[i].m_EndTime._2tm();
					for (int j = 0;j < R1C_TX_ATT_IO_POWER_PTS;j ++)
						DataTxAttIO.m_fOffset[j] = pDataTxAttIO[i].m_fOffset[j];
					for (int j = 0;j < 4;j ++)
						DataTxAttIO.m_dTemp[j] = pDataTxAttIO[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataTxAttIO,sizeof(DataTxAttIO),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
									 }
			case ICalFile::RxRefOP : 
			case ICalFile::RxRefIO : {
				DataFV0005_RxRef *pDataRxRef = (DataFV0005_RxRef *)pBuf;
				DataFV0006_RxRef DataRxRef;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataRxRef.m_uiFreq  = pDataRxRef[i].m_uiFreq;
					DataRxRef.m_EndTime = pDataRxRef[i].m_EndTime._2tm();
					for (int j = 0;j < 3;j ++) {
						DataRxRef.m_State[j].m_iLNAAtt	 = pDataRxRef[i].m_State[j].m_iLNAAtt;
						DataRxRef.m_State[j].m_iAtt019	 = pDataRxRef[i].m_State[j].m_iAtt019;
						DataRxRef.m_State[j].m_fAtt1	 = pDataRxRef[i].m_State[j].m_fAtt1;
						DataRxRef.m_State[j].m_fAtt2	 = pDataRxRef[i].m_State[j].m_fAtt2;
						DataRxRef.m_State[j].m_fAtt3	 = pDataRxRef[i].m_State[j].m_fAtt3;
						DataRxRef.m_State[j].m_iADOffset = pDataRxRef[i].m_State[j].m_iADOffset;
						DataRxRef.m_State[j].m_dTemp[0]  = pDataRxRef[i].m_State[j].m_dTemp[0];
						DataRxRef.m_State[j].m_dTemp[1]	 = pDataRxRef[i].m_State[j].m_dTemp[1];
						DataRxRef.m_State[j].m_dTemp[2]	 = pDataRxRef[i].m_State[j].m_dTemp[2];
						DataRxRef.m_State[j].m_dTemp[3]	 = pDataRxRef[i].m_State[j].m_dTemp[3];
					}
					CFO_ASSERT(fp_new,fwrite(&DataRxRef,sizeof(DataRxRef),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
									 }
			case ICalFile::RxAttOP : {
				DataFV0005_RxAttOP *pDataRxAttOP = (DataFV0005_RxAttOP *)pBuf;
				DataFV0006_RxAttOP DataRxAttOP;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataRxAttOP.m_uiFreq  = pDataRxAttOP[i].m_uiFreq;
					DataRxAttOP.m_EndTime = pDataRxAttOP[i].m_EndTime._2tm();
					for (int j = 0;j < R1C_RX_REF_OP_PTS - 3 + 1;j ++)
						DataRxAttOP.m_iOffset[j] = pDataRxAttOP[i].m_iOffset[j];
					for (int j = 0;j < 4;j ++)
						DataRxAttOP.m_dTemp[j] = pDataRxAttOP[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataRxAttOP,sizeof(DataRxAttOP),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
									 }
			case ICalFile::RxAttIO : {
				DataFV0005_RxAttIO *pDataRxAttIO = (DataFV0005_RxAttIO *)pBuf;
				DataFV0006_RxAttIO DataRxAttIO;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataRxAttIO.m_uiFreq  = pDataRxAttIO[i].m_uiFreq;
					DataRxAttIO.m_EndTime = pDataRxAttIO[i].m_EndTime._2tm();
					for (int j = 0;j < R1C_RX_REF_IO_PTS - 3 + 1;j ++)
						DataRxAttIO.m_iOffset[j] = pDataRxAttIO[i].m_iOffset[j];
					for (int j = 0;j < 4;j ++)
						DataRxAttIO.m_dTemp[j] = pDataRxAttIO[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataRxAttIO,sizeof(DataRxAttIO),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
									 }
			case ICalFile::TxFil_80 : 
			case ICalFile::TxFil_160 : {
				DataFV0005_TxFil *pDataTxFil = (DataFV0005_TxFil *)pBuf;
				DataFV0006_TxFil DataTxFil;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataTxFil.m_uiFreq  = pDataTxFil[i].m_uiFreq;
					DataTxFil.m_EndTime = pDataTxFil[i].m_EndTime._2tm();
					for (int j = 0;j < 41;j ++) {
						DataTxFil.m_iCoefReal[j] = pDataTxFil[i].m_iCoefReal[j];
						DataTxFil.m_iCoefImag[j] = pDataTxFil[i].m_iCoefImag[j];
					}
					for (int j = 0;j < 4;j ++)
						DataTxFil.m_dTemp[j] = pDataTxFil[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataTxFil,sizeof(DataTxFil),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
									   }
			case ICalFile::RxFil_80 : {
				DataFV0005_RxFil_80 *pDataRxFil_80 = (DataFV0005_RxFil_80 *)pBuf;
				DataFV0006_RxFil_80 DataRxFil_80;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataRxFil_80.m_uiFreq  = pDataRxFil_80[i].m_uiFreq;
					DataRxFil_80.m_EndTime = pDataRxFil_80[i].m_EndTime._2tm();
					for (int j = 0;j < 96;j ++) {
						DataRxFil_80.m_iCoefReal[j] = pDataRxFil_80[i].m_iCoefReal[j];
						DataRxFil_80.m_iCoefImag[j] = pDataRxFil_80[i].m_iCoefImag[j];
					}
					for (int j = 0;j < 4;j ++)
						DataRxFil_80.m_dTemp[j] = pDataRxFil_80[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataRxFil_80,sizeof(DataRxFil_80),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
									  }
			case ICalFile::RxFil_160 : {
				DataFV0005_RxFil_160 *pDataRxFil_160 = (DataFV0005_RxFil_160 *)pBuf;
				DataFV0006_RxFil_160 DataRxFil_160;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataRxFil_160.m_uiFreq  = pDataRxFil_160[i].m_uiFreq;
					DataRxFil_160.m_EndTime = pDataRxFil_160[i].m_EndTime._2tm();
					for (int j = 0;j < 144;j ++) {
						DataRxFil_160.m_iCoefReal[j] = pDataRxFil_160[i].m_iCoefReal[j];
						DataRxFil_160.m_iCoefImag[j] = pDataRxFil_160[i].m_iCoefImag[j];
					}
					for (int j = 0;j < 4;j ++)
						DataRxFil_160.m_dTemp[j] = pDataRxFil_160[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataRxFil_160,sizeof(DataRxFil_160),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
									   }
			case ICalFile::TxFilOffOP_80 : 
			case ICalFile::TxFilOffIO_80 : {
				DataFV0005_TxFilOff *pDataTxFilOff = (DataFV0005_TxFilOff *)pBuf;
				DataFV0006_TxFilOff DataTxFilOff;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataTxFilOff.m_uiFreq  = pDataTxFilOff[i].m_uiFreq;
					DataTxFilOff.m_dOffset = pDataTxFilOff[i].m_dOffset;
					DataTxFilOff.m_EndTime = pDataTxFilOff[i].m_EndTime._2tm();
					for (int j = 0;j < 4;j ++)
						DataTxFilOff.m_dTemp[j] = pDataTxFilOff[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataTxFilOff,sizeof(DataTxFilOff),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
										   }
			case ICalFile::RxFilOffOP_80 : 
			case ICalFile::RxFilOffIO_80 : {
				DataFV0005_RxFilOff *pDataRxFilOff = (DataFV0005_RxFilOff *)pBuf;
				DataFV0006_RxFilOff DataRxFilOff;
				for (uint32_t i = 0;i < Info.m_pItemInfo[uiCurItemIdx].m_uiPoint;i ++) {
					DataRxFilOff.m_uiFreq  = pDataRxFilOff[i].m_uiFreq;
					DataRxFilOff.m_EndTime = pDataRxFilOff[i].m_EndTime._2tm();
					for (int j = 0;j < 3;j ++)
						DataRxFilOff.m_iOffset[j] = pDataRxFilOff[i].m_iOffset[j];
					for (int j = 0;j < 4;j ++)
						DataRxFilOff.m_dTemp[j] = pDataRxFilOff[i].m_dTemp[j];
					CFO_ASSERT(fp_new,fwrite(&DataRxFilOff,sizeof(DataRxFilOff),1,fp_new) == 0);
					CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				}
				break;
										   }
			case ICalFile::TxRFFr_0 : 
			case ICalFile::TxRFFr_1 : 
			case ICalFile::TxIFFr	: 
			case ICalFile::RxRFFr	: 
			case ICalFile::RxIFFr	: {
				CFO_ASSERT(fp_new,fwrite(pBuf,Info.m_pItemInfo[uiCurItemIdx].m_uiSize * Info.m_pItemInfo[uiCurItemIdx].m_uiPoint,1,fp_new) == 0);
				CFO_ASSERT(fp_new,fflush(fp_new) == EOF);
				break;
									}
		}
	}
	CFO_ASSERT(fp_old,fseek(fp_old,0,SEEK_SET));
	fclose(fp_old);
	CFO_ASSERT(fp_new,fseek(fp_new,0,SEEK_SET));
	fclose(fp_new);

	remove(szPathOld);
	rename(szPathNew,szPathOld);

	delete []pBuf;
	Log.stdprintf("file \"%s\" version 0x0005 ---> 0x0006\n",szPathOld);
	Log.AddMsgList(0,"file \"%s\" version 0x0005 ---> 0x0006",szPathOld);
	return 0;
}

int32_t CfvCtrl::ForwardConvert(int32_t iCurVer)
{
	ForConv t_ForwardConvert[] = {&CfvCtrl::ForConv_0000_0001,
								  &CfvCtrl::ForConv_0001_0002,
								  &CfvCtrl::ForConv_0002_0003,
								  &CfvCtrl::ForConv_0003_0004,
								  &CfvCtrl::ForConv_0004_0005,
								  &CfvCtrl::ForConv_0005_0006};

	for (int32_t i = iCurVer;i < CAL_FILE_VER;i ++) {
		(this->*t_ForwardConvert[i])();
	}
	return 0;
}
