#ifndef __INIFILE_H__
#define __INIFILE_H__

// #ifndef _IA64_
// #define _IA64_
// #endif

#ifndef _AMD64_
#define _AMD64_
#endif

#ifdef _MSC_VER
#pragma warning( disable : 4996 )
#endif

#include <stdio.h>

/* a general ini file operation class						*
 * 1 using '#' or '//' to annotate one section				*
 * 2 using "//"	       to annotate one key					*			
 * 3 not support '\r' or '\n' in both section and key name	*
 * 4 construction is different depend on os					*
 * 5 "Log" need the libLog link								*
 * 6 the ini file should be placed directly in the module path	    */

#define FILENAME_NOTEXIST		0x01	//missing ini file
#define SECTIONNAME_NOTEXIST	0x02	//missing section name
#define KEYNAME_NOTEXIST		0x03	//missing key name
#define STRING_LENNOTEQUAL		0x04	//different string length
#define STRING_NOTEQUAL			0x05	//different strings
#define STRING_EQUAL			0x00	//same string

class CGeneralIniFile
{
public:
    CGeneralIniFile(const char *pFileName);
public:
    int GetConfigStringValue(const char *pInSectionName,const char *pInKeyName,char *pOutKeyValue);
	int GetConfigIntValue(char *pInSectionName,char *pInKeyName,int *pOutKeyValue);
private:
    int CompareString(const char *pInStr1,const char *pInStr2);
    int GetKeyValue(FILE *fpConfig,const char *pInKeyName,char *pOutKeyValue);
private:
    char m_szBasePath[256];
    char m_szFileName[64];
};

#endif
