#include "ArbReader.h"
#include "libLog.h"
#include <string.h>

ArbReader::ArbReader()
{
	m_pFile = NULL;
}

ArbReader::~ArbReader()
{
	if (m_pFile)
		fclose(m_pFile);
}

int ArbReader::Load(const char *pPath)
{
	if (m_pFile) {
		fclose(m_pFile);
		m_pFile = NULL;
	}

	m_pFile = fopen(pPath,"rb");
	if (NULL == m_pFile) {
		fclose(m_pFile);
        Log.SetLastError("can't open %s",pPath);
		return -1;
	}
	return 0;
}

FILE *ArbReader::Load(Param_t *pParam,vector<DataVector> *pData)
{
	int iFileSize = 0;
	int iTotalSamples = 0;
	FileHeader_t FileHeader;
	list<SegHeader_t> SegHeader;
	list<SegHeader_t>::iterator iterSegHeader;
	list<ArbSeg_t>	ArbSeg;
	list<MarkerList> Marker;
	int iMkrIdx = 0;

	fseek(m_pFile,0L,SEEK_END);
	iFileSize = ftell(m_pFile);
	fseek(m_pFile,0L,SEEK_SET);

	fread(&FileHeader,sizeof(FileHeader_t),1,m_pFile);
	Show(&FileHeader);

	SegHeader.clear();
	ArbSeg.clear();
	Marker.clear();

	for (int i = 0;i < FileHeader.Segments;i ++) {
		SegHeader_t Segment;
		fread(&Segment,sizeof(SegHeader_t),1,m_pFile);
		SegHeader.push_back(Segment);
		Show(&Segment);

		iTotalSamples += Segment.Samples;

		int iMarkers = Segment.MarkerBytes / sizeof(Marker_t);
		Marker_t Mkr;
		MarkerList MkrList;
		ArbSeg_t Seg;

		Seg.inter_fltr = (uint32_t)(245.76e6 / Segment.SampleRate);
		Seg.seg_num = i;
		Seg.samples = Segment.Samples;
		Seg.seg_adr	= i;
		Seg.sr      = (uint32_t)(Segment.SampleRate);

		for (int j = 0;j < iMarkers;j ++) {
			fread(&Mkr,sizeof(Marker_t),1,m_pFile);
			MkrList.push_back(Mkr);

			iMkrIdx = Mkr.No - 1;
			if (ArbReader::Period == Mkr.Type) {
				Seg.markers[iMkrIdx].type = 0;
				Seg.markers[iMkrIdx].marker.period.start  = Mkr.Period.StartPos;
				Seg.markers[iMkrIdx].marker.period.period = Mkr.Period.Period;
				Seg.markers[iMkrIdx].marker.period.keep   = Mkr.Period.Duration;
			}
			else if (ArbReader::UnPeriod == Mkr.Type) {
				Seg.markers[iMkrIdx].type = 1;
				Seg.markers[iMkrIdx].marker.unperiod.count = Mkr.UnPeriod.Count;
				for (int i =0;i < Mkr.UnPeriod.Count;i ++){
					Seg.markers[iMkrIdx].marker.unperiod.start[i] = Mkr.UnPeriod.StartPos[i];
					Seg.markers[iMkrIdx].marker.unperiod.keep[i]  = Mkr.UnPeriod.Duration[i];
				}
			}
		}
		ArbSeg.push_back(Seg);
		Show(MkrList);
		Marker.push_back(MkrList);	
	}

	if (pParam) {
		pParam->FileSize = iFileSize;
		pParam->TotalSamples = iTotalSamples;
		pParam->FileHeader = FileHeader;
		pParam->SegHeader = SegHeader;
		pParam->ArbSeg = ArbSeg;
		pParam->Marker = Marker;
	}

	if (pData) {
		Data_t IQData;
		int i = 0;
		for (iterSegHeader = SegHeader.begin();iterSegHeader != SegHeader.end();iterSegHeader ++) {
			for (int j = 0;j < iterSegHeader->Samples;j ++) {
				fread(&IQData,sizeof(Data_t),1,m_pFile);
				pData->at(i).push_back(IQData);
			}
			i ++;
		}
	}
	return m_pFile;
}

bool ArbReader::IsLoaded()
{
	return (NULL == m_pFile ? false : true);
}

void ArbReader::Show(const FileHeader_t *p) {
//  	Log.stdprintf("**************************** File Header ****************************\n");
//  	Log.stdprintf("FileName = %s\n", p->FileName);
//  	Log.stdprintf("Date     = %s\n", p->Date);
//  	Log.stdprintf("Version  = %s\n", p->Version);
//  	Log.stdprintf("Segments = %d\n", p->Segments);
//  	Log.stdprintf("Checksum = 0x%x\n", p->Checksum);
//  	Log.stdprintf("*********************************************************************\n");
}

void ArbReader::Show(const SegHeader_t *p) {
//  	Log.stdprintf("************************** Segment Header ***************************\n");
//  	Log.stdprintf("Samples      = %d\n", p->Samples);
//  	Log.stdprintf("Duration     = %d\n", p->Duration);
//  	Log.stdprintf("LevelOffset  = %f\n", p->LevelOffset);
//  	Log.stdprintf("PeakOffset   = %d\n", p->PeakOffset);
//  	Log.stdprintf("AuthInfo     = %s\n", p->AuthInfo);
//  	Log.stdprintf("Name         = %s\n", p->Name);
//  	Log.stdprintf("MarkerBytes  = %d\n", p->MarkerBytes);
//  	Log.stdprintf("SampleRate   = %f\n", p->SampleRate);
//  	Log.stdprintf("*********************************************************************\n");
}

void ArbReader::Show(MarkerList &markerList) {
// 	Log.stdprintf("****************************** Marker *******************************\n");
// 	MarkerList::const_iterator iter;
// 
// 	for (iter = markerList.begin(); iter != markerList.end(); iter++) {
// 		Log.stdprintf("No = %d\n", iter->No);
// 		Log.stdprintf("Type = %s\n", iter->Type == Period ? "period" : "unperiod");
// 		if (iter->Type == Period) {
// 			Log.stdprintf("StartPos = %d\n", iter->Period.StartPos);
// 			Log.stdprintf("Duration = %d\n", iter->Period.Duration);
// 			Log.stdprintf("Period   = %d\n", iter->Period.Period);
// 		} else if (iter->Type == UnPeriod) {
// 			for (int i = 0; i < iter->UnPeriod.Count; i++) {
// 				Log.stdprintf("StartPos = %d\n", iter->UnPeriod.StartPos[i]);
// 				Log.stdprintf("Duration = %d\n", iter->UnPeriod.Duration[i]);
// 			}
// 		} else {
// 			Log.stdprintf("Unknown Marker Type");
// 		}
// 	}
// 	Log.stdprintf("*********************************************************************\n");
}

void ArbReader::Show(const Data_t *p)
{
    Log.stdprintf("data.i = %d, data.q = %d\n", p->i, p->q);
}

int ArbReader::GetFileInfo(char *pBuf)
{
#define ADD_INFO() strcat(pBuf,szInfoBuf);strcat(pBuf,"\r\n");

    char szInfoBuf[576] = {0};

	list<SegHeader_t>::iterator iterSegHeader;
	list<MarkerList>::iterator iterMkrList;
	Param_t Param;
	Load(&Param);
	iterSegHeader = Param.SegHeader.begin();
	iterMkrList = Param.Marker.begin();

	sprintf(szInfoBuf,"FileSize = %d",Param.FileSize);
	ADD_INFO();

	sprintf(szInfoBuf,"********** File Header **********");	//File Header
	ADD_INFO();	sprintf(szInfoBuf,"FileName = %s",Param.FileHeader.FileName);
	ADD_INFO(); sprintf(szInfoBuf,"Data = %s",Param.FileHeader.Date);
	ADD_INFO(); sprintf(szInfoBuf,"Version = %s",Param.FileHeader.Version);
	ADD_INFO(); sprintf(szInfoBuf,"Segments = %d",Param.FileHeader.Segments);
	ADD_INFO(); sprintf(szInfoBuf,"Checksum = %#x",Param.FileHeader.Checksum);
	ADD_INFO(); 
	sprintf(szInfoBuf,"*********************************");	//File Header
	ADD_INFO();

	int iSeg = 0;
	for (iterSegHeader = Param.SegHeader.begin();iterSegHeader != Param.SegHeader.end();iterSegHeader ++) {
		int iMkrs = iterSegHeader->MarkerBytes / sizeof(Marker_t);

		sprintf(szInfoBuf,"Segment:%d",iSeg);
		ADD_INFO();	sprintf(szInfoBuf," Samples = %d",iterSegHeader->Samples);
		ADD_INFO();	sprintf(szInfoBuf," Duration = %d",iterSegHeader->Duration);
		ADD_INFO(); sprintf(szInfoBuf," LevelOffset = %f",iterSegHeader->LevelOffset);
		ADD_INFO(); sprintf(szInfoBuf," PeakOffset = %d",iterSegHeader->PeakOffset);
		ADD_INFO(); sprintf(szInfoBuf," AuthInfo = %s",iterSegHeader->AuthInfo);
		ADD_INFO(); sprintf(szInfoBuf," Name = %s",iterSegHeader->Name);
		ADD_INFO(); sprintf(szInfoBuf," MarkerBytes = %d,Makers = %d",iterSegHeader->MarkerBytes,iMkrs);
		ADD_INFO(); sprintf(szInfoBuf," SampleRate = %f",iterSegHeader->SampleRate);
		ADD_INFO();

		MarkerList::const_iterator iter;

		int iMkr = 0;
		for (iter = iterMkrList->begin();iter != iterMkrList->end();iter ++) {
			sprintf(szInfoBuf,"   Marker %d:",iter->No);ADD_INFO(); 
			sprintf(szInfoBuf,"    Type = %s",ArbReader::Period == iter->Type ? "Period" : "Unperiod");ADD_INFO();
			if (ArbReader::Period == iter->Type) {
				sprintf(szInfoBuf,"    StarPos = %d",iter->Period.StartPos);ADD_INFO();
				sprintf(szInfoBuf,"    Duration = %d",iter->Period.Duration);ADD_INFO();
				sprintf(szInfoBuf,"    Period = %d",iter->Period.Period);ADD_INFO();
			}
			else if (ArbReader::UnPeriod == iter->Type) {
				for (int k = 0;k < iter->UnPeriod.Count;k ++) {
					sprintf(szInfoBuf,"    StarPos = %d",iter->UnPeriod.StartPos[k]);ADD_INFO();
					sprintf(szInfoBuf,"    Duration = %d",iter->UnPeriod.Duration[k]);ADD_INFO();
				}
			}
			else {
				sprintf(szInfoBuf,"    Unknown Marker Type");
				ADD_INFO();
			}
			iMkr ++;
		}

		iSeg ++;
		iterMkrList ++;
	}
	return 0;
}
