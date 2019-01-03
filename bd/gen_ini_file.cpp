#include "gen_ini_file.h"

#ifdef RD_PLATFORM_WIN32
    #include <WinDef.h>
    #include <WinBase.h>
#elif defined RD_PLATFORM_LINUX
    #include <unistd.h>
#endif

#include <string>

using namespace sp_rd;

gen_ini_file::gen_ini_file(const char *file_name)
{
    memset(m_base_path,0,sizeof(m_base_path));

#ifdef RD_PLATFORM_WIN32
    ::GetModuleFileNameA(NULL,m_base_path,sizeof(m_base_path));
    *strrchr(m_base_path,'\\') = 0;
#elif defined RD_PLATFORM_LINUX
    strcpy(m_base_path,"/etc/sp_rd/");
#endif

    strcpy(m_file_name,file_name);
}

int gen_ini_file::get_config_str_value(const char *section, const char *key, char *value)
{
    FILE *fp = NULL;
    char exact_path[256] = {0};
    char buffer[160] = {0};
    char *str1 = NULL,*str2 = NULL;
    int ret = 0;

    memset(buffer,0,sizeof(buffer));
    memset(exact_path,0,sizeof(exact_path));
    strcpy(exact_path,m_base_path);

#ifdef RD_PLATFORM_WIN32
    strcat(exact_path,"\\");
#elif defined RD_PLATFORM_LINUX
    strcat(exact_path,"/");
#endif

    strcat(exact_path,m_file_name);
    if (NULL == (fp = fopen(exact_path,"r")))
		return FILENAME_NOTEXIST;

    while (!feof(fp)) {                                     //if end of the file
        if (NULL == fgets(buffer,150,fp))
			break;
        str1 = buffer;
        while ((' ' == *str1) || ('\t' == *str1))			//skip 'space' or 'tab' before a section
            str1 ++;
        if ('[' == *str1) {
            str1 ++;
            while ((' ' == *str1) || ('\t' == *str1))		//skip 'space' or 'tab' in a section such as [    ??]
                str1 ++;
            str2 = str1;									//first meanful character of section
            while ((']' != *str1) && ('\0' != *str1))
                str1 ++;
            if ('\0' == *str1)								//if section do not end,keep reading the left characters
				continue;
            while (' ' == *(str1 - 1))						//skip 'spcee' or 'tab' in a section such as [??    ]
                str1 --;
            *str1 = '\0';

            ret = compare_string(str2,section);
            if (!ret) {
                ret = get_key_value(fp,key,value);
                fclose(fp);
                return ret;
			}
		}
	}
    fclose(fp);
	return SECTIONNAME_NOTEXIST;
}

int gen_ini_file::get_config_int_value(const char *section, const char *key, int *value)
{
    int ret = 0;
    char str_key_value[16] = {0};
    char *str = NULL;

    memset(str_key_value,0,sizeof(str_key_value));
    ret = get_config_str_value(section,key,str_key_value);
    if (ret)
        return ret;
    str = str_key_value;
    while ((' ' == *str) || ('\t' == *str))
        str ++;
    if ((('0' == *str) && ('x' == *(str + 1))) || ('X' == *(str + 1)))		//if using hex
        sscanf(str + 2,"%x",value);
	else
        sscanf(str,"%d",value);

	return 0;
}

//exact equal
int gen_ini_file::compare_string(const char *str1,const char *str2)
{
    if (strlen(str1) != strlen(str2))
		return STRING_LENNOTEQUAL;

    while (*str1 == *str2) {	//compare each character
        if ('\0' == *str1)
			break;
        str1 ++;
        str2 ++;
	}

    if ('\0' == *str1)
		return STRING_EQUAL;
	
	return STRING_NOTEQUAL;
}

int gen_ini_file::get_key_value(FILE *fp, const char *key, char *value)
{
    char buffer[160] = {0};
    char *str1 = NULL,*str2 = NULL,*str3 = NULL;
    unsigned int len = 0;
    int ret = 0;

    memset(buffer,0,sizeof(buffer));
    while (!feof(fp)) {
        if (NULL == fgets(buffer,150,fp))
			break;
        str1 = buffer;
        while ((' ' == *str1) || ('\t' == *str1))
            str1 ++;
        if ('#' == *str1)												//using '#'  as the annotate in section
			continue;
        if (('/' == *str1) && ('/' == *(str1 + 1)))                     //using '//' as the annotate in section
			continue;
        if (('\0' == *str1) || (0x0d == *str1) || (0x0a == *str1))      //if end of the line or meeting '\r' or '\n',keep reading the next line
			continue;
        if ('[' == *str1) {                                             //avoid the case when another section was inserted between the right section and it's keys
            str2 = str1;
            while ((']' != *str1) && ('\0' != *str1))
                str1 ++;
            if (']' == *str1)
				break;
            str1 = str2;
		}
        str2 = str1;													//find the first meanful character of one key
        while (('=' != *str1) && ('\0' != *str1))						//find the last character of one key
            str1 ++;
        if ('\0' == *str1)
			continue;
        str3 = str1 + 1;
        if (str2 == str1)												//if no key name
			continue;
        *str1 = '\0';
        str1 --;
        while ((' ' == *str1) || ('\t' == *str1)) {
            *str1 = '\0';
            str1 --;
		}

        ret = compare_string(str2,key);
        if (!ret) {
            str1 = str3;												//jump over '='
            while ((' ' == *str1) || ('\t' == *str1))
                str1 ++;
            str3 = str1;
            while (('\0' != *str1) && (0x0d != *str1) && (0x0a != *str1)) {
                if (('/' == *str1) && ('/' == *(str1 + 1)))             //also use '//' as the annotate in key
					break;
                str1 ++;
			}
            *str1 = '\0';
            len = (unsigned int)strlen(str3);
            memcpy(value,str3,len);
            *(value + len) = '\0';
			return 0;
		}
	}
	return KEYNAME_NOTEXIST;
}
