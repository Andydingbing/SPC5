#ifndef __ARB_READER_H__
#define __ARB_READER_H__

#ifndef _AMD64_
#define _AMD64_
#endif

#include <stdio.h>
#include <list>
#include <vector>
#include "stdint_common.h"

using namespace std;

struct ArbSeg_t
{
	uint32_t seg_num;
	uint32_t samples;
	uint32_t seg_adr;
	uint32_t inter_fltr;
	uint32_t sr;
	struct marker_t
	{
		int type;
		union
		{
			struct
			{
				int start;
				int keep;
				int period;
			} period;
			struct
			{
				int count;
				int start[1024];
				int keep[1024];
			} unperiod;
		} marker;
	} markers [4];
};

union as_reg_t
{
	struct {unsigned sample			:32;} adr0;
	struct {unsigned seg_adr		:32;} adr1;
	struct {unsigned inter_fltr		:32;} adr2;
	struct {unsigned mkr1_start		:32;} adr3;
	struct {unsigned mkr1_keep		:32;} adr4;
	struct {unsigned mkr1_period	:32;} adr5;
	struct {unsigned mkr2_start		:32;} adr6;
	struct {unsigned mkr2_keep		:32;} adr7;
	struct {unsigned mkr2_period	:32;} adr8;
	struct {unsigned mkr3_start		:32;} adr9;
	struct {unsigned mkr3_keep		:32;} adr10;
	struct {unsigned mkr3_period	:32;} adr11;
	struct {unsigned mkr4_start		:32;} adr12;
	struct {unsigned mkr4_keep		:32;} adr13;
	struct {unsigned mkr4_period	:32;} adr14;
	struct {unsigned sr				:32;} adr16;
	struct {
		unsigned mkr1_type  : 1;
		unsigned mkr2_type	: 1;
		unsigned mkr3_type	: 1;
		unsigned mkr4_type  : 1;
		unsigned mkr1_onoff : 1;
		unsigned mkr2_onoff : 1;
		unsigned mkr3_onoff : 1;
		unsigned mkr4_onoff : 1;
	}adr17;
	uint32_t data;
};

#define AS_REG(addr) as_reg[addr].adr##addr

class ArbReader
{
public:
	ArbReader();
	~ArbReader();
public:
	enum MarkerType { 
		Period,
		UnPeriod
	};
	typedef struct FileHeader_t {
		char FileName[512];
		char Date[512];
		char Version[512];
		int  Segments;
		int  Checksum;
	}FileHeader_t;

	typedef struct SegHeader_t {
		int   SegID;
		int   Samples;
		int   Duration;
		float LevelOffset;
		int   PeakOffset;
		char  Name[512];
		char  AuthInfo[32];
		int   MarkerBytes;
		float SampleRate;
	}SegHeader_t;

	typedef struct Marker_t {
		int No;
		int Type;
		union {
			struct {
				int StartPos;
				int Duration;
				int Period;
			} Period;
			struct {
				int Count;
				int StartPos[1024];
				int Duration[1024];
			} UnPeriod;
		};
	}Marker_t;

	typedef list<Marker_t> MarkerList;

	typedef struct Data_t {
		short i;
		short q;
	}Data_t;

	typedef vector<Data_t> DataVector;

	typedef struct Param_t {
		int				  FileSize;
		int				  TotalSamples;
		FileHeader_t	  FileHeader;
		list<SegHeader_t> SegHeader;
		list<ArbSeg_t>	  ArbSeg;
		list<MarkerList>  Marker;
	}Param_t;

public:
	int Load(const char *pPath);
	FILE* Load(Param_t *pParam = NULL,vector<DataVector> *pData = NULL);
	bool IsLoaded();
	int GetFileInfo(char *pBuf);
private:
	void Show(const FileHeader_t *p);
	void Show(const SegHeader_t *p);
	void Show(MarkerList &MarkerList);
	void Show(const Data_t *p);
public:
	FILE *m_pFile;
};

#endif 


