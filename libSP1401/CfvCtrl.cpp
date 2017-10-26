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

	typedef struct CalItemInfoV0005 {
		uint32_t m_uiHead;
		CalFileR1A::CalItem m_Item;
		uint32_t m_uiSize;
		uint32_t m_uiPoint;
		uint32_t m_uiTail;
	}CalItemInfoV0005;

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

int32_t CfvCtrl::ForwardConvert(int32_t iCurVer)
{
	ForConv t_ForwardConvert[] = {&CfvCtrl::ForConv_0000_0001,
								  &CfvCtrl::ForConv_0001_0002,
								  &CfvCtrl::ForConv_0002_0003,
								  &CfvCtrl::ForConv_0003_0004,
								  &CfvCtrl::ForConv_0004_0005};

	for (int32_t i = iCurVer;i < CAL_FILE_VER;i ++) {
		(this->*t_ForwardConvert[i])();
	}
	return 0;
}
