#include "cal_file_r1cd.h"
#include "cal_file_r1ab.h"
#include <string.h>
#include <errno.h>

#ifdef RD_PLATFORM_WIN32
    #include <io.h>
    #include <direct.h>
#elif defined RD_PLATFORM_LINUX
    #include <sys/stat.h>
    #include <unistd.h>
#endif

// file size before adding r1c/d data,make it easy to arrive to r1c/d data head directly
#define R1A_FILE_SIZE 5541296

using namespace sp_rd;
using namespace sp1401;

cal_file_r1cd::cal_file_r1cd(uint32_t rf_idx,uint32_t rfu_idx) :
    cal_file(rf_idx,rfu_idx),
    m_tx_lol(NULL),
    m_tx_sb(NULL),
    m_tx_pwr_op(NULL),
    m_tx_pwr_io(NULL),
    m_tx_att_op(NULL),
    m_tx_att_io(NULL),
    m_rx_ref_op(NULL),
    m_rx_ref_io(NULL),
    m_rx_att_op(NULL),
    m_rx_att_io(NULL),
    m_tx_filter_80m(NULL),
    m_tx_filter_160m(NULL),
    m_rx_filter_80m(NULL),
    m_rx_filter_160m(NULL),
    m_tx_filter_offset_op_80m(NULL),
    m_tx_filter_offset_io_80m(NULL),
    m_rx_filter_offset_op_80m(NULL),
    m_rx_filter_offset_io_80m(NULL),
    m_tx_rf_fr_0(NULL),
    m_tx_rf_fr_1(NULL),
    m_tx_if_fr(NULL),
    m_rx_rf_fr(NULL),
    m_rx_if_fr(NULL)
{
}

cal_file_r1cd::~cal_file_r1cd()
{
}

int32_t cal_file_r1cd::open()
{
    BOOST_SPTR_SAFE_MAKE(x9119_table,g_cal_table_x9119);
    BOOST_SPTR_SAFE_MAKE(tx_lol_table_r1cd,m_tx_lol);
    BOOST_SPTR_SAFE_MAKE(tx_sb_table_r1cd,m_tx_sb);
    BOOST_SPTR_SAFE_MAKE(tx_pwr_op_table_r1cd,m_tx_pwr_op);
    BOOST_SPTR_SAFE_MAKE(tx_pwr_io_table_r1cd,m_tx_pwr_io);
    BOOST_SPTR_SAFE_MAKE(tx_att_op_table_r1cd,m_tx_att_op);
    BOOST_SPTR_SAFE_MAKE(tx_att_io_table_r1cd,m_tx_att_io);
    BOOST_SPTR_SAFE_MAKE(rx_ref_op_table_r1cd,m_rx_ref_op);
    BOOST_SPTR_SAFE_MAKE(rx_ref_io_table_r1cd,m_rx_ref_io);
    BOOST_SPTR_SAFE_MAKE(rx_att_op_table_r1cd,m_rx_att_op);
    BOOST_SPTR_SAFE_MAKE(rx_att_io_table_r1cd,m_rx_att_io);

    if (_80M == m_bw) {
        BOOST_SPTR_SAFE_MAKE(tx_filter_80m_table,m_tx_filter_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_80m_table,m_rx_filter_80m);
        BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_op_80m);
        BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_io_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_op_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_io_80m);
	}
    else if (_160M == m_bw) {
        BOOST_SPTR_SAFE_MAKE(tx_filter_160m_table,m_tx_filter_160m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_160m_table,m_rx_filter_160m);
	}
    return cal_file::open();
}

int32_t cal_file_r1cd::set_bw(bw_t bw)
{
    if (m_bw == bw)
		return 0;
	if (_80M == bw) {
        m_tx_filter_160m.reset();
        m_rx_filter_160m.reset();

        BOOST_SPTR_SAFE_MAKE(tx_filter_80m_table,m_tx_filter_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_80m_table,m_rx_filter_80m);
        BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_op_80m);
        BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_io_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_op_80m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_io_80m);

        MAP_FILE_DATAM(TX_FILTER_80,m_tx_filter_80m);
        MAP_FILE_DATAM(RX_FILTER_80,m_rx_filter_80m);
        MAP_FILE_DATAM(TX_FILTER_OFFSET_OP_80,m_tx_filter_offset_op_80m);
        MAP_FILE_DATAM(TX_FILTER_OFFSET_IO_80,m_tx_filter_offset_io_80m);
        MAP_FILE_DATAM(RX_FILTER_OFFSET_OP_80,m_rx_filter_offset_op_80m);
        MAP_FILE_DATAM(RX_FILTER_OFFSET_IO_80,m_rx_filter_offset_io_80m);
	}
	else if (_160M == bw) {
        m_tx_filter_offset_op_80m.reset();
        m_tx_filter_offset_io_80m.reset();
        m_rx_filter_offset_op_80m.reset();
        m_rx_filter_offset_io_80m.reset();
        m_tx_filter_80m.reset();
        m_rx_filter_80m.reset();

        BOOST_SPTR_SAFE_MAKE(tx_filter_160m_table,m_tx_filter_160m);
        BOOST_SPTR_SAFE_MAKE(rx_filter_160m_table,m_rx_filter_160m);

        MAP_FILE_DATAM(TX_FILTER_160,m_tx_filter_160m);
        MAP_FILE_DATAM(RX_FILTER_160,m_rx_filter_160m);
	}
    return cal_file::set_bw(bw);
}

int32_t cal_file_r1cd::add(cal_item_t item,void *data)
{
    map2buf(item);
    switch (item) {
        case cal_file::X9119                    : ADD_DATAF(g_cal_table_x9119,x9119_table::data_t);
        case cal_file::TX_LOL                   : ADD_DATAF(m_tx_lol,tx_lol_table_r1cd::data_f_t);
        case cal_file::TX_SB                    : ADD_DATAF(m_tx_sb,tx_sb_table_r1cd::data_f_t);
        case cal_file::TX_PWR_OP                : ADD_DATAF(m_tx_pwr_op,tx_pwr_op_table_r1cd::data_f_t);
        case cal_file::TX_PWR_IO                : ADD_DATAF(m_tx_pwr_io,tx_pwr_io_table_r1cd::data_f_t);
        case cal_file::TX_ATT_OP                : ADD_DATAF(m_tx_att_op,tx_att_op_table_r1cd::data_f_t);
        case cal_file::TX_ATT_IO                : ADD_DATAF(m_tx_att_io,tx_att_io_table_r1cd::data_f_t);
        case cal_file::RX_REF_OP                : ADD_DATAF(m_rx_ref_op,rx_ref_op_table_r1cd::data_f_t);
        case cal_file::RX_REF_IO                : ADD_DATAF(m_rx_ref_io,rx_ref_io_table_r1cd::data_f_t);
        case cal_file::RX_ATT_OP                : ADD_DATAF(m_rx_att_op,rx_att_op_table_r1cd::data_f_t);
        case cal_file::RX_ATT_IO                : ADD_DATAF(m_rx_att_io,rx_att_io_table_r1cd::data_f_t);
        case cal_file::TX_FILTER_80             : ADD_DATAF(m_tx_filter_80m,tx_filter_80m_table::data_f_t);
        case cal_file::TX_FILTER_160            : ADD_DATAF(m_tx_filter_160m,tx_filter_160m_table::data_f_t);
        case cal_file::RX_FILTER_80             : ADD_DATAF(m_rx_filter_80m,rx_filter_80m_table::data_f_t);
        case cal_file::RX_FILTER_160            : ADD_DATAF(m_rx_filter_160m,rx_filter_160m_table::data_f_t);
        case cal_file::TX_FILTER_OFFSET_OP_80   : ADD_DATAF(m_tx_filter_offset_op_80m,tx_filter_offset_table::data_f_t);
        case cal_file::TX_FILTER_OFFSET_IO_80   : ADD_DATAF(m_tx_filter_offset_io_80m,tx_filter_offset_table::data_f_t);
        case cal_file::RX_FILTER_OFFSET_OP_80   : ADD_DATAF(m_rx_filter_offset_op_80m,rx_filter_offset_table::data_f_t);
        case cal_file::RX_FILTER_OFFSET_IO_80   : ADD_DATAF(m_rx_filter_offset_io_80m,rx_filter_offset_table::data_f_t);

        case cal_file::TX_RF_FR_0	: ADD_DATAF(m_tx_rf_fr_0,tx_rf_fr_0_table_r1cd::data_f_t);
        case cal_file::TX_RF_FR_1	: ADD_DATAF(m_tx_rf_fr_1,tx_rf_fr_1_table_r1cd::data_f_t);
        case cal_file::TX_IF_FR     : ADD_DATAF(m_tx_if_fr,tx_if_fr_table::data_f_t);
        case cal_file::RX_RF_FR     : ADD_DATAF(m_rx_rf_fr,rx_rf_fr_table::data_f_t);
        case cal_file::RX_IF_FR     : ADD_DATAF(m_rx_if_fr,rx_if_fr_table::data_f_t);
		default:return 0;
	}
	return 0;
}

int32_t cal_file_r1cd::map2buf(cal_item_t item)
{
    if (NULL == g_cal_item_buf.buf) {
        cal_file::file_info_t Info;
        uint32_t max_byte = cal_file::get_max_item_byte();

        g_cal_item_buf.buf = new char[max_byte / sizeof(char)];
        if (NULL == g_cal_item_buf.buf) {
            Log.set_last_err("%s:%s:%d",__FILE__,__FUNCTION__,__LINE__);
			return -1;
		}
        memset(g_cal_item_buf.buf,0,max_byte);
	}

    if (item != g_cal_item_buf.item || this->m_rf_idx != g_cal_item_buf.rf_idx || this->m_rfu_idx != g_cal_item_buf.rfu_idx) {
        INT_CHECK(r(item,g_cal_item_buf.buf));
        g_cal_item_buf.item = item;
        g_cal_item_buf.rf_idx = this->m_rf_idx;
        g_cal_item_buf.rfu_idx = this->m_rfu_idx;
        switch (item) {
            case cal_file::X9119     : MAP_GLOBAL_DATAF(g_cal_table_x9119,x9119_table::data_t);
            case cal_file::TX_LOL    : MAP_GLOBAL_DATAF(m_tx_lol,tx_lol_table_r1cd::data_f_t );
            case cal_file::TX_SB     : MAP_GLOBAL_DATAF(m_tx_sb,tx_sb_table_r1cd::data_f_t);
            case cal_file::TX_PWR_OP : MAP_GLOBAL_DATAF(m_tx_pwr_op,tx_pwr_op_table_r1cd::data_f_t);
            case cal_file::TX_PWR_IO : MAP_GLOBAL_DATAF(m_tx_pwr_io,tx_pwr_io_table_r1cd::data_f_t);
            case cal_file::TX_ATT_OP : MAP_GLOBAL_DATAF(m_tx_att_op,tx_att_op_table_r1cd::data_f_t);
            case cal_file::TX_ATT_IO : MAP_GLOBAL_DATAF(m_tx_att_io,tx_att_io_table_r1cd::data_f_t);
            case cal_file::RX_REF_OP : MAP_GLOBAL_DATAF(m_rx_ref_op,rx_ref_op_table_r1cd::data_f_t);
            case cal_file::RX_REF_IO : MAP_GLOBAL_DATAF(m_rx_ref_io,rx_ref_io_table_r1cd::data_f_t);
            case cal_file::RX_ATT_OP : MAP_GLOBAL_DATAF(m_rx_att_op,rx_att_op_table_r1cd::data_f_t);
            case cal_file::RX_ATT_IO : MAP_GLOBAL_DATAF(m_rx_att_io,rx_att_io_table_r1cd::data_f_t);
            case cal_file::TX_FILTER_80 : {
                BOOST_SPTR_SAFE_MAKE(tx_filter_80m_table,m_tx_filter_80m);
                MAP_GLOBAL_DATAF(m_tx_filter_80m,tx_filter_80m_table::data_f_t); }

            case cal_file::TX_FILTER_160 : {
                BOOST_SPTR_SAFE_MAKE(tx_filter_160m_table,m_tx_filter_160m);
                MAP_GLOBAL_DATAF(m_tx_filter_160m,tx_filter_160m_table::data_f_t); }

            case cal_file::RX_FILTER_80 : {
                BOOST_SPTR_SAFE_MAKE(rx_filter_80m_table,m_rx_filter_80m);
                MAP_GLOBAL_DATAF(m_rx_filter_80m,rx_filter_80m_table::data_f_t); }

            case cal_file::RX_FILTER_160 : {
                BOOST_SPTR_SAFE_MAKE(rx_filter_160m_table,m_rx_filter_160m);
                MAP_GLOBAL_DATAF(m_rx_filter_160m,rx_filter_160m_table::data_f_t); }

            case cal_file::TX_FILTER_OFFSET_OP_80 : {
                BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_op_80m);
                MAP_GLOBAL_DATAF(m_tx_filter_offset_op_80m,tx_filter_offset_table::data_f_t); }

            case cal_file::TX_FILTER_OFFSET_IO_80 : {
                BOOST_SPTR_SAFE_MAKE(tx_filter_offset_table,m_tx_filter_offset_io_80m);
                MAP_GLOBAL_DATAF(m_tx_filter_offset_io_80m,tx_filter_offset_table::data_f_t); }

            case cal_file::RX_FILTER_OFFSET_OP_80 : {
                BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_op_80m);
                MAP_GLOBAL_DATAF(m_rx_filter_offset_op_80m,rx_filter_offset_table::data_f_t); }

            case cal_file::RX_FILTER_OFFSET_IO_80 : {
                BOOST_SPTR_SAFE_MAKE(rx_filter_offset_table,m_rx_filter_offset_io_80m);
                MAP_GLOBAL_DATAF(m_rx_filter_offset_io_80m,rx_filter_offset_table::data_f_t); }

            case cal_file::TX_RF_FR_0 : {
                BOOST_SPTR_SAFE_MAKE(tx_rf_fr_0_table_r1cd,m_tx_rf_fr_0);
                MAP_GLOBAL_DATAF(m_tx_rf_fr_0,tx_rf_fr_table::data_f_t); }

            case cal_file::TX_RF_FR_1 : {
                BOOST_SPTR_SAFE_MAKE(tx_rf_fr_1_table_r1cd,m_tx_rf_fr_1);
                MAP_GLOBAL_DATAF(m_tx_rf_fr_1,tx_rf_fr_table::data_f_t); }

            case cal_file::TX_IF_FR : {
                BOOST_SPTR_SAFE_MAKE(tx_if_fr_table,m_tx_if_fr);
                MAP_GLOBAL_DATAF(m_tx_if_fr,tx_if_fr_table::data_f_t); }

            case cal_file::RX_RF_FR : {
                BOOST_SPTR_SAFE_MAKE(rx_rf_fr_table,m_rx_rf_fr);
                MAP_GLOBAL_DATAF(m_rx_rf_fr,rx_rf_fr_table::data_f_t); }

            case cal_file::RX_IF_FR : {
                BOOST_SPTR_SAFE_MAKE(rx_if_fr_table,m_rx_if_fr);
                MAP_GLOBAL_DATAF(m_rx_if_fr,rx_if_fr_table::data_f_t); }
			default:return 0;
		}
	}
	return 0;
}

int32_t cal_file_r1cd::map2mem()
{
    MAP_FILE_DATAM(cal_file::X9119,     g_cal_table_x9119);
    MAP_FILE_DATAM(cal_file::TX_LOL,    m_tx_lol);
    MAP_FILE_DATAM(cal_file::TX_SB,     m_tx_sb);
    MAP_FILE_DATAM(cal_file::TX_PWR_OP, m_tx_pwr_op);
    MAP_FILE_DATAM(cal_file::TX_PWR_IO, m_tx_pwr_io);
    MAP_FILE_DATAM(cal_file::TX_ATT_OP, m_tx_att_op);
    MAP_FILE_DATAM(cal_file::TX_ATT_IO, m_tx_att_io);
    MAP_FILE_DATAM(cal_file::RX_REF_OP, m_rx_ref_op);
    MAP_FILE_DATAM(cal_file::RX_REF_IO, m_rx_ref_io);
    MAP_FILE_DATAM(cal_file::RX_ATT_OP, m_rx_att_op);
    MAP_FILE_DATAM(cal_file::RX_ATT_IO, m_rx_att_io);
    if (_80M == m_bw) {
        MAP_FILE_DATAM(cal_file::TX_FILTER_80,m_tx_filter_80m);
        MAP_FILE_DATAM(cal_file::TX_FILTER_OFFSET_OP_80,m_tx_filter_offset_op_80m);
        MAP_FILE_DATAM(cal_file::TX_FILTER_OFFSET_IO_80,m_tx_filter_offset_io_80m);
        MAP_FILE_DATAM(cal_file::RX_FILTER_80,m_rx_filter_80m);
        MAP_FILE_DATAM(cal_file::RX_FILTER_OFFSET_OP_80,m_rx_filter_offset_op_80m);
        MAP_FILE_DATAM(cal_file::RX_FILTER_OFFSET_IO_80,m_rx_filter_offset_io_80m);
	}
    else if (_160M == m_bw) {
        MAP_FILE_DATAM(cal_file::TX_FILTER_160,m_tx_filter_160m);
        MAP_FILE_DATAM(cal_file::RX_FILTER_160,m_rx_filter_160m);
	}
    return 0;
}

int32_t cal_file_r1cd::create()
{
    cal_file::file_ver_t ver;
    cal_file::file_info_t r1ab_info;
    cal_file::file_info_t r1cd_info;
    uint32_t item_idx_r1ab = 0;
    uint32_t item_idx_r1cd = 0;
    uint32_t max_byte_r1ab = 0;
    uint32_t max_byte_r1cd = 0;
    uint32_t max_byte = 0;
    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",m_rfu_idx,m_rf_idx);

    ver.head = CAL_FILE_VER_HEAD;
    ver.ver  = CAL_FILE_VER;
    ver.tail = CAL_FILE_VER_TAIL;

    r1ab_info.head = CAL_FILE_INFO_HEAD;
    r1ab_info.tail = CAL_FILE_INFO_TAIL;
    memset(r1ab_info.sn,0,sizeof(r1ab_info.sn));
    sprintf(r1ab_info.sn,"SP1401R1B%08dRFU%uRF%u",m_ordinal,m_rfu_idx,m_rf_idx);

    max_byte_r1ab = cal_file_r1ab::get_fix_item_table_r1ab(r1ab_info.item_info,r1ab_info.items);
    max_byte_r1cd = cal_file_r1cd::get_fix_item_table_r1cd(r1cd_info.item_info,r1cd_info.items);
    max_byte = max_byte_r1ab >= max_byte_r1cd ? max_byte_r1ab : max_byte_r1cd;

    r1ab_info.items -= 1;
    for (uint32_t i = 0;i < r1ab_info.items;i ++) {
		if (X9119 == i)		//r1a's cal item table = enum CalItem,so it is safe here
            item_idx_r1ab ++;
        r1ab_info.item_info[i] = r1ab_info.item_info[item_idx_r1ab];
        item_idx_r1ab ++;
	}

    FILE *fp;
    if (access("C:\\CSECal",0) == -1) {
#ifdef RD_PLATFORM_WIN32
        if (mkdir("C:\\CSECal") == -1) {
#elif defined RD_PLATFORM_LINUX
        if (mkdir("/var/lib/CSECal",0777) == -1) {
#endif
            Log.set_last_err("%s:%s:%d(%s)",__FILE__,__FUNCTION__,__LINE__,strerror(errno));
            return -1;
        }
    }
    if (access("C:\\CSECal\\cxu.cal",0) == -1) {
        x9119_table::data_t data;
        data.value = 0;

        fp = fopen("C:\\CSECal\\cxu.cal","w+");
        CFO_ASSERT(fp,NULL == fp);
        CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
        CFO_ASSERT(fp,fwrite(&data,sizeof(x9119_table::data_t),1,fp) == 0);
        CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
        fclose(fp);
    }
    //r1a/b
    fp = fopen(path,"w+");
    CFO_ASSERT(fp,NULL == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fwrite(&ver,sizeof(cal_file::file_ver_t),1,fp) == 0);
    CFO_ASSERT(fp,fwrite(&r1ab_info,sizeof(cal_file::file_info_t),1,fp) == 0);
    for (uint32_t i = 0;i < r1ab_info.items;i ++)
        CFO_ASSERT(fp,fwrite(&(r1ab_info.item_info[i]),sizeof(cal_file::item_info_t),1,fp) == 0);

    unsigned char *buf = new unsigned char[max_byte];
    for (uint32_t i = 0;i < r1ab_info.items;i ++) {
        uint32_t size = ((r1ab_info.item_info[i]).size) * ((r1ab_info.item_info[i]).pt);
        memset(buf,0,max_byte * sizeof(unsigned char));
        CFO_ASSERT(fp,fwrite(buf,1,size,fp) == 0);
    }

    //r1c/d
    r1cd_info.head = CAL_FILE_INFO_HEAD;
    r1cd_info.tail = CAL_FILE_INFO_TAIL;
    memset(r1cd_info.sn,0,sizeof(r1cd_info.sn));
    sprintf(r1cd_info.sn,"SP1401R1C%08dRFU%uRF%u",m_ordinal,m_rfu_idx,m_rf_idx);

    r1cd_info.items -= 1;
    for (uint32_t i = 0;i < r1cd_info.items;i ++) {
        if (X9119 == r1cd_info.item_info[item_idx_r1cd].item)	//r1c may add or only use a part of cal item which defined before
            item_idx_r1cd ++;
        r1cd_info.item_info[i] = r1cd_info.item_info[item_idx_r1cd];
        item_idx_r1cd ++;
	}
	
    CFO_ASSERT(fp,fwrite(&r1cd_info,sizeof(cal_file::file_info_t),1,fp) == 0);
    for (uint32_t i = 0;i < r1cd_info.items;i ++)
        CFO_ASSERT(fp,fwrite(&(r1cd_info.item_info[i]),sizeof(cal_file::item_info_t),1,fp) == 0);

    for (uint32_t i = 0;i < r1cd_info.items;i ++) {
        uint32_t size = ((r1cd_info.item_info[i]).size) * ((r1cd_info.item_info[i]).pt);
        memset(buf,0,max_byte * sizeof(unsigned char));
        CFO_ASSERT(fp,fwrite(buf,1,size,fp) == 0);
	}
    delete []buf;
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);
	return 0;
}

int32_t cal_file_r1cd::load(file_info_t &info)
{
    file_ver_t ver;
    char path[64] = {0};
    sprintf(path,"C:\\CSECal\\rfu%drf%d.cal",m_rfu_idx,m_rf_idx);

    FILE *fp = fopen(path,"rb");
    CFO_ASSERT(fp,NULL == fp);
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    CFO_ASSERT(fp,fread(&ver,sizeof(file_ver_t),1,fp) == 0);
    show_file_ver(&ver);

    CFO_ASSERT(fp,fseek(fp,R1A_FILE_SIZE,SEEK_SET));

    item_info_t *item_info = info.item_info;
    CFO_ASSERT(fp,fread(&info,sizeof(cal_file::file_info_t),1,fp) == 0);
    info.item_info = item_info;
    show_file_info(&info);

    for (uint32_t i = 0;i < info.items;i ++) {
        CFO_ASSERT(fp,fread(&(info.item_info[i]),sizeof(cal_file::item_info_t),1,fp) == 0);
        show_item_info(&(info.item_info[i]));
	}
    CFO_ASSERT(fp,fseek(fp,0,SEEK_SET));
    fclose(fp);
	return 0;
}

int32_t cal_file_r1cd::get_item_size(cal_item_t item,uint32_t &pos,uint32_t &size)
{
    cal_file::file_info_t info;
    INT_CHECK(load(info));

    uint32_t pt = 0;
    for (uint32_t i = 0;i < info.items;i ++) {
        if (info.item_info[i].item == item) {
            size = info.item_info[i].size;
            pt = info.item_info[i].pt;
			break;
		}
	}
    size = size * pt;

    if (cal_file::X9119 == item) {
        pos = 0;
        size = sizeof(x9119_table::data_t);
		return 0;
	}

    pos = R1A_FILE_SIZE;
    pos += sizeof(cal_file::file_info_t);
    for (uint32_t i = 0;i < info.items;i ++)
        pos += sizeof(cal_file::item_info_t);
    for (uint32_t i = 0;i < info.items;i ++) {
        if (item != info.item_info[i].item)
            pos += ((info.item_info[i]).size) * ((info.item_info[i]).pt);
		else
			break;
	}
	return 0;
}
