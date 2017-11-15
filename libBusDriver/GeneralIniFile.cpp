#include "GeneralIniFile.h"

#ifdef _WIN64
#include <WinDef.h>
#include <WinBase.h>
#elif defined(__unix__) || defined(__linux__)
#include <unistd.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CGeneralIniFile::CGeneralIniFile(const char *pFileName)
{
	memset(m_szBasePath,0,sizeof(m_szBasePath));
#ifdef _MSC_VER
	::GetModuleFileName(NULL,m_szBasePath,256);
	*strrchr(m_szBasePath,'\\') = 0;
#elif defined __GNUC__
    strcpy(m_szBasePath,"/etc/cse/");
#endif
	strcpy(m_szFileName,pFileName);
}

int CGeneralIniFile::GetConfigStringValue(const char *pInSectionName,const char *pInKeyName,char *pOutKeyValue)
{
	FILE *fpConfig;
    char szExactPath[256];
	char szBuffer[150];
	char *pStr1,*pStr2;
	int iRetCode = 0;

// 	printf("pInFileName : %s !\n",pInFileName);
// 	printf("pInSectionName : %s !\n",pInSectionName);
// 	printf("pInKeyName : %s !\n",pInKeyName);

	memset(szBuffer,0,sizeof(szBuffer));
	memset(szExactPath,0,sizeof(szExactPath));
    strcpy(szExactPath,m_szBasePath);
#ifdef _WIN64
    strcat(szExactPath,"\\");
#elif defined(__unix__) || defined(__linux__)
    strcat(szExactPath,"/");
#endif
    strcat(szExactPath,m_szFileName);
	if (NULL == (fpConfig = fopen(szExactPath,"r")))
		return FILENAME_NOTEXIST;

	while (!feof(fpConfig)) {								//if end of the file
		if (NULL == fgets(szBuffer,150,fpConfig))
			break;
		pStr1 = szBuffer;
		while ((' ' == *pStr1) || ('\t' == *pStr1))			//skip 'space' or 'tab' before a section
			pStr1 ++;
		if ('[' == *pStr1) {
			pStr1 ++;
			while ((' ' == *pStr1) || ('\t' == *pStr1))		//skip 'space' or 'tab' in a section such as [    ??]
				pStr1 ++;
			pStr2 = pStr1;									//first meanful character of section
			while ((']' != *pStr1) && ('\0' != *pStr1))	
				pStr1 ++;
			if ('\0' == *pStr1)								//if section do not end,keep reading the left characters
				continue;
			while (' ' == *(pStr1 - 1))						//skip 'spcee' or 'tab' in a section such as [??    ]
				pStr1 --;
			*pStr1 = '\0';									

			iRetCode = CompareString(pStr2,pInSectionName);
			if (!iRetCode) {
				iRetCode = GetKeyValue(fpConfig,pInKeyName,pOutKeyValue);
				fclose(fpConfig);
				return iRetCode;
			}
		}
	}
	fclose(fpConfig);
	return SECTIONNAME_NOTEXIST;
}

int CGeneralIniFile::GetConfigIntValue(char *pInSectionName,char *pInKeyName,int *pOutKeyValue)
{
	int iRetCode = 0;
	char szKeyValue[16],*pStr;

	memset(szKeyValue,0,sizeof(szKeyValue));
	iRetCode = GetConfigStringValue(pInSectionName,pInKeyName,szKeyValue);
	if (iRetCode)
		return iRetCode;
	pStr = szKeyValue;
	while ((' ' == *pStr) || ('\t' == *pStr))
		pStr ++;
    if ((('0' == *pStr) && ('x' == *(pStr + 1))) || ('X' == *(pStr + 1)))		//if using hex
		sscanf(pStr + 2,"%x",pOutKeyValue);
	else
		sscanf(pStr,"%d",pOutKeyValue);

	return 0;
}

//exact equal
int CGeneralIniFile::CompareString(const char *pInStr1,const char *pInStr2)
{
	if (strlen(pInStr1) != strlen(pInStr2))
		return STRING_LENNOTEQUAL;

	while (*pInStr1 == *pInStr2) {	//compare each character
		if ('\0' == *pInStr1)
			break;
		pInStr1 ++;
		pInStr2 ++;
	}

	if ('\0' == *pInStr1)
		return STRING_EQUAL;
	
	return STRING_NOTEQUAL;
}

int CGeneralIniFile::GetKeyValue(FILE *fpConfig,const char *pInKeyName,char *pOutKeyValue)
{
	char szBuffer[150];
	char *pStr1,*pStr2,*pStr3;
	unsigned int uiLen;
	int iRetCode = 0;

	memset(szBuffer,0,sizeof(szBuffer));
	while (!feof(fpConfig)) {
		if (NULL == fgets(szBuffer,150,fpConfig))
			break;
		pStr1 = szBuffer;
		while ((' ' == *pStr1) || ('\t' == *pStr1))
			pStr1 ++;
		if ('#' == *pStr1)												//using '#'  as the annotate in section
			continue;
		if (('/' == *pStr1) && ('/' == *(pStr1 + 1)))					//using '//' as the annotate in section
			continue;
		if (('\0' == *pStr1) || (0x0d == *pStr1) || (0x0a == *pStr1))	//if end of the line or meeting '\r' or '\n',keep reading the next line
			continue;
		if ('[' == *pStr1) {											//avoid the case when another section was inserted between the right section and it's keys
			pStr2 = pStr1;
			while ((']' != *pStr1) && ('\0' != *pStr1))
				pStr1 ++;
			if (']' == *pStr1)
				break;
			pStr1 = pStr2;
		}
		pStr2 = pStr1;													//find the first meanful character of one key
		while (('=' != *pStr1) && ('\0' != *pStr1))						//find the last character of one key
			pStr1 ++;
		if ('\0' == *pStr1)
			continue;
		pStr3 = pStr1 + 1;
		if (pStr2 == pStr1)												//if no key name
			continue;
		*pStr1 = '\0';
		pStr1 --;
		while ((' ' == *pStr1) || ('\t' == *pStr1)) {
			*pStr1 = '\0';
			pStr1 --;
		}

		iRetCode = CompareString(pStr2,pInKeyName);
		if (!iRetCode) {
			pStr1 = pStr3;												//jump over '='
			while ((' ' == *pStr1) || ('\t' == *pStr1))
				pStr1 ++;
			pStr3 = pStr1;
			while (('\0' != *pStr1) && (0x0d != *pStr1) && (0x0a != *pStr1)) {
				if (('/' == *pStr1) && ('/' == *(pStr1 + 1)))			//also use '//' as the annotate in key
					break;
				pStr1 ++;
			}
			*pStr1 = '\0';
			uiLen = (unsigned int)strlen(pStr3);
			memcpy(pOutKeyValue,pStr3,uiLen);
			*(pOutKeyValue + uiLen) = '\0';
			return 0;
		}
	}
	return KEYNAME_NOTEXIST;
}
