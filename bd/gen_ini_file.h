//
// Copyright 2018 StarPoint Inc.,Ltd
//

#ifndef GEN_INI_FILE_H
#define GEN_INI_FILE_H

#include "rd.h"
#include <stdio.h>

/*!
 * A general ini file operation class.
 * 1.use '#' or '//' to annotate one section
 * 2.use "//"	     to annotate one key
 * 3.not support '\r' or '\n' in both section and key name
 * 4.construction is different depend on OS
 * 5.in windows platform,the ini file should be placed directly in the module path
*/

#define FILENAME_NOTEXIST		0x01	//missing ini file
#define SECTIONNAME_NOTEXIST	0x02	//missing section name
#define KEYNAME_NOTEXIST		0x03	//missing key name
#define STRING_LENNOTEQUAL		0x04	//different string length
#define STRING_NOTEQUAL			0x05	//different strings
#define STRING_EQUAL			0x00	//same string

namespace sp_rd {

class RD_API gen_ini_file
{
public:
    gen_ini_file(const char *file_name);
public:
    int get_config_str_value(const char *section,const char *key,char *value);
    int get_config_int_value(const char *section,const char *key,int *value);
private:
    int compare_string(const char *str1,const char *str2);
    int get_key_value(FILE *fp, const char *key, char *value);
private:
    char m_base_path[256];
    char m_file_name[64];
};

} //namespace sp_rd

#endif // GEN_INI_FILE_H
