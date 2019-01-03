#ifndef CAL_FILE_H
#define CAL_FILE_H

#include "liblog.h"
#include "libbd.h"
#include "tx_sb_table.h"
#include "tx_lol_table.h"
#include "tx_att_table.h"
#include "tx_pwr_table.h"
#include "rx_ref_table.h"
#include "rx_att_table.h"
#include "tx_filter_table.h"
#include "rx_filter_table.h"
#include "tx_filter_offset_table.h"
#include "rx_filter_offset_table.h"
#include <boost/smart_ptr.hpp>

/*update calibration file version here*/
/*must add the related function pointer such as "t_GetItemSize";"t_ForwardConvert"*/
//#define CAL_FILE_VER 0x0000	// the very first version
//#define CAL_FILE_VER 0x0001	// add the version number of the file at the beginning,add head and tail before and after the CalFileInfo structure
//#define CAL_FILE_VER 0x0002	// get the probably correct value of x9119 from all calibration files,then create a new file named "cxu.cal" to save this value
//#define CAL_FILE_VER 0x0003	// change receiver's reference range : "output mode : "+30~-30dBm"--->"+30~-40dBm"
								//									   "in/out mode : "+30~-20dBm"--->"+30~-30dBm"
//#define CAL_FILE_VER 0x0004	// add an item:attenuation deviation in in/out mode
								// note that the driver has changed the meaning of the data,each 9 float data represents a status of the attenuation from 0dB to 90dB,step 10dB,so it's necessary to calibrate tx att and tx power again
//#define CAL_FILE_VER 0x0005	// r1c first version
#define CAL_FILE_VER 0x0006		// change all r1c/d's table's data member "m_EndTime" from struct "COleDateTime" to struct "tm"

/*define all info's head and tail,which are used to locate the related structure*/
/*GUID_1 = {44A4AE51-6470-4C83-A568-34EDDA974411}*/
/*GUID_2 = {1E8EB58C-791E-42AA-8124-C197398944FF}*/
#define CAL_FILE_VER_HEAD  0x44A4AE51
#define CAL_FILE_VER_TAIL  0x64704C83

#define CAL_FILE_INFO_HEAD 0xA56834ED
#define CAL_FILE_INFO_TAIL 0xDA974411

#define CAL_ITEM_INFO_HEAD 0x1E8EB58C	//not used actually,replace head with 0xaa and tail with 0xff
#define CAL_ITEM_INFO_TAIL 0x791E42AA

#define CFO_ASSERT(stream,func)    /*calibration file operation assert*/                        \
    do {                                                                                        \
        if (func) {                                                                             \
            Log.set_last_err("%s:%s:%d(%s)",__FILE__,__FUNCTION__,__LINE__,strerror(errno));    \
            if (stream)                                                                         \
                fclose(stream);                                                                 \
            return -1;                                                                          \
        }                                                                                       \
    } while(0)

#define CFO_ASSERT_S(stream,func,post) /*safe calibration file operation assert*/               \
    do {                                                                                        \
        if (func) {                                                                             \
            Log.set_last_err("%s:%s:%d(%s)",__FILE__,__FUNCTION__,__LINE__,strerror(errno));	\
            if (stream)                                                                         \
                fclose(stream);                                                                 \
            post;                                                                               \
            return -1;                                                                          \
        }                                                                                       \
    } while(0)

// map global buf(g_cal_item_buf.buf) to item(table)'s data(d) buf
#define MAP_GLOBAL_DATAF(table,d) { table->map2buf((d *)(g_cal_item_buf.buf));return 0; }

// mpa data from calibration file to memory
#define MAP_FILE_DATAM(item,table) { map2buf(item);table->map2mem(); }

// add data from (d) to item(table)'s data buf
#define ADD_DATAF(table,d) { table->add((d *)data);return 0; }

namespace sp_rd {

class RD_API cal_file
{
public:
#define CAL_TOTAL_ITEMS 25
    // list all the calibration items,the value is only allowed to accumulate
    enum cal_item_t {
        TX_SB                   = 0x00000000,	//r1a/b/c/d
        TX_LOL                  = 0x00000001,	//r1a/b/c/d
        TX_ATT_OP               = 0x00000002,	//r1a/b/c/d
        TX_PWR_OP               = 0x00000003,	//r1a/b/c/d		//default bw 160M
        TX_PWR_IO               = 0x00000004,	//r1a/b/c/d		//default bw 160M
        RX_REF                  = 0x00000005,	//r1a/b
        X9119                   = 0x00000006,	//r1a/b/c/d
        TX_ATT_IO               = 0x00000007,	//r1a/b/c/d
        RX_REF_OP               = 0x00000008,	//r1c/d			//default bw 160M
        RX_REF_IO               = 0x00000009,	//r1c/d			//default bw 160M
        RX_ATT_OP               = 0x0000000a,	//r1c/d
        RX_ATT_IO               = 0x0000000b,	//r1c/d
        TX_FILTER_80            = 0x0000000c,	//r1c/d
        TX_FILTER_160           = 0x0000000d,	//r1c/d
        TX_RF_FR_0              = 0x0000000e,	//r1c/d
        TX_RF_FR_1              = 0x0000000f,	//r1c/d
        TX_IF_FR                = 0x00000010,	//r1c/d
        RX_FILTER_80            = 0x00000011,	//r1c/d
        RX_FILTER_160           = 0x00000012,	//r1c/d
        RX_RF_FR                = 0x00000013,	//r1c/d
        RX_IF_FR                = 0x00000014,	//r1c/d
        TX_FILTER_OFFSET_OP_80	= 0x00000015,	//r1c/d
        TX_FILTER_OFFSET_IO_80	= 0x00000016,	//r1c/d
        RX_FILTER_OFFSET_OP_80	= 0x00000017,	//r1c/d
        RX_FILTER_OFFSET_IO_80	= 0x00000018	//r1c/d
	};

    typedef struct file_ver_t {
        uint32_t head;
        uint32_t ver;
        uint32_t tail;
    }file_ver_t;

    struct item_info_t;
    typedef struct file_info_t {
        uint32_t    head;
        char        sn[32];
        uint32_t    items;
        item_info_t *item_info;
        uint32_t    tail;
	public:
        file_info_t();
        ~file_info_t();
    }file_info_t;

    STRUCT_ALIGN_S(item_info_t,4)
        uint32_t    head : 32;
        cal_item_t  item : 32;
        uint32_t    size : 32;		//each frequency's data type(byte)
        uint32_t    pt   : 32;		//points
        uint32_t    tail : 32;
    STRUCT_ALIGN_E(item_info_t,4)

    typedef struct RD_API item_buf_t {
        uint32_t    rf_idx;
        uint32_t    rfu_idx;
        void        *buf;			//point to each table dynamically
        cal_item_t  item;			//point out pointing to which table
	public:
        item_buf_t();
        ~item_buf_t();
        item_buf_t operator = (const item_buf_t &buf);
    }item_buf_t;

#define DECLARE_CAL_FILE()                              \
public:													\
    virtual int32_t add(cal_item_t item,void *data);	\
    virtual int32_t map2buf(cal_item_t item);           \
    virtual int32_t map2mem();                          \
    virtual int32_t create();							\
    virtual int32_t load(file_info_t &info);			\
    virtual int32_t get_item_size(cal_item_t item,uint32_t &pos,uint32_t &size);

public:
    cal_file(uint32_t rf_idx,uint32_t rfu_idx);
    virtual ~cal_file();
    friend class cfv_ctrl;
public:
    virtual int32_t add(cal_item_t item,void *data) = 0;
    virtual int32_t map2buf(cal_item_t item) = 0;
    virtual int32_t map2mem() = 0;
    virtual int32_t create() = 0;
    virtual int32_t load(file_info_t &info) = 0;
    //getting every item's size may differ from each file version
    virtual int32_t get_item_size(cal_item_t item,uint32_t &pos,uint32_t &size) = 0;
public:
    sp_rd::sp1401::bw_t get_bw();
    uint32_t get_rf_idx();
    uint32_t get_rfu_idx();
    uint32_t get_ordinal();
    virtual int32_t get_file_ver();
    static uint32_t get_fix_item_table_r1ab(item_info_t *info,uint32_t &items);
    static uint32_t get_fix_item_table_r1cd(item_info_t *info,uint32_t &items);
    static uint32_t get_max_item_byte(item_info_t *info,uint32_t items);
    static uint32_t get_max_item_byte();
    static sp_rd::x9119_table *get_x9119();
    virtual int32_t open();
    virtual bool is_file_valid();
    virtual int32_t w_from_pos(char *path,uint32_t pos,uint32_t size,void *data);
    virtual int32_t r_from_pos(char *path,uint32_t pos,uint32_t size,void *data);
    virtual int32_t w(cal_item_t item);
    virtual int32_t r(cal_item_t item,void *data);
    virtual int32_t set_bw(sp_rd::sp1401::bw_t bw);

    virtual void show_file_ver(file_ver_t *ver);
    virtual void show_file_info(file_info_t *info);
    virtual void show_item_info(item_info_t *info);
public:
    static boost::shared_ptr<x9119_table> g_cal_table_x9119;
protected:
    static item_buf_t g_cal_item_buf;
    uint32_t m_rf_idx;
    uint32_t m_rfu_idx;
    uint32_t m_ordinal;
    sp_rd::sp1401::bw_t m_bw;
};

} //namespace sp_rd

#endif // CAL_FILE_H
