#include "sp3301.h"
#include "reg_def.h"
#include <string>

#define K7_IDX(idx)      ((MAX_RF - 1 - idx) / 2)
#define RF_IDX_2(idx)    ((idx / 2 * 2) + (idx % 2 ? 0 : 1))

using namespace sp_rd;
using namespace sp1401;

#define INS_SP1401(idx)                                                     \
    do {                                                                    \
        bool is_connected = false;											\
        char sn[24] = {0};                                                  \
        m_sp1401->at(idx)->get_sn(sn);										\
        if (basic_sp1401::is_valid_sn(sn) > SN_NULL) {						\
            is_connected = true;											\
            basic_sp1401::get_hw_ver(sn,m_hw_ver[idx]);						\
            switch (m_hw_ver[idx]) {                                        \
                case basic_sp1401::R1A :                                    \
                case basic_sp1401::R1B : {									\
                    m_sp1401->at(idx) = m_sp1401_r1a->at(idx);				\
                    break;                                                  \
                }                                                           \
                case basic_sp1401::R1C :									\
                case basic_sp1401::R1D :                                    \
                case basic_sp1401::HW_ERROR : {								\
                    m_sp1401->at(idx) = m_sp1401_r1c->at(idx);				\
                    break;                                                  \
                }                                                           \
            }                                                               \
        }                                                                   \
        else {                                                              \
            if ((is_connected = m_sp1401_r1a->at(idx)->is_connected()))		\
                m_sp1401->at(idx) = m_sp1401_r1a->at(idx);					\
            else if ((is_connected = m_sp1401_r1c->at(idx)->is_connected()))\
                m_sp1401->at(idx) = m_sp1401_r1c->at(idx);					\
        }                                                                   \
        if (is_connected) {													\
            INT_CHECK(m_sp1401->at(idx)->open_board());						\
            m_active.sp1401[idx] = true;                                    \
        }                                                                   \
	} while(0)

#define DECLARE_DYNAMIC_SP1401_SP2401		\
    basic_sp1401 *sp1401;					\
    sp2401_r1a *sp2401;						\
    if (false == m_active.sp1401[rf_idx])	\
        return -1;							\
    sp1401 = m_sp1401->at(rf_idx).get();    \
    sp2401 = m_sp2401_r1a->at(rf_idx).get();

#define DECLARE_DYNAMIC_DDR                 \
    dma_mgr *ddr = m_dma_mgr->at(K7_IDX(rf_idx)).get();

#define R1A_CAL_FILE ((cal_file_r1ab *)(sp1401->get_cal_file()))
#define R1C_CAL_FILE ((cal_file_r1cd *)(sp1401->get_cal_file()))

sp3301::sp3301(uint32_t rfu_idx):
	m_rfu_idx(rfu_idx),
    m_k7_0(ass_k7_name(0,rfu_idx).c_str()),
    m_k7_1(ass_k7_name(1,rfu_idx).c_str()),
    m_s6(ass_s6_name(rfu_idx).c_str()),
    m_is_program_k7_0(true),
    m_is_program_k7_1(true)
{
    m_sp1401 = boost::make_shared<std::vector<basic_sp1401::sptr>>();
    m_sp1401_r1a = boost::make_shared<std::vector<sp1401_r1a::sptr>>();
    m_sp1401_r1c = boost::make_shared<std::vector<sp1401_r1c::sptr>>();
    m_sp2401_r1a = boost::make_shared<std::vector<sp2401_r1a::sptr>>();

    m_dma_mgr = boost::make_shared<std::vector<dma_mgr::sptr>>();

    m_I = new std::vector<int16_t *>;
    m_Q = new std::vector<int16_t *>;

	for (int32_t i = 0;i < MAX_RF;i ++) {
        m_sp1401->push_back(NULL);
        m_sp1401_r1a->push_back(boost::make_shared<sp1401_r1a>(i,rfu_idx));
        m_sp1401_r1c->push_back(boost::make_shared<sp1401_r1c>(i,rfu_idx));
        m_sp2401_r1a->push_back(boost::make_shared<sp2401_r1a>(i));
        m_sp1401->at(i) = m_sp1401_r1a->at(i);
        m_I->push_back(NULL);
        m_Q->push_back(NULL);
	}

	for (int32_t i = 0;i < MAX_RF;i ++) {
        m_hw_ver[i] = basic_sp1401::HW_ERROR;
        m_io_mode[i] = OUTPUT;
        m_tx_freq[i] = 2400000000;
        m_tx_pwr[i] = -20.0;
        m_rx_freq[i] = 2400000000;
        m_ref[i] = -20.0;
	}
}

sp3301::~sp3301()
{
}

sp3301 &sp3301::operator = (sp3301 &sp3301)
{
	return sp3301;
}

sp3301 &sp3301::instance(uint32_t rfu_idx)
{
    switch (rfu_idx) {
        case 0 : {static sp3301 sp3301_0(0);return sp3301_0;}
        case 1 : {static sp3301 sp3301_1(1);return sp3301_1;}
        case 2 : {static sp3301 sp3301_2(2);return sp3301_2;}
        case 3 : {static sp3301 sp3301_3(3);return sp3301_3;}
        case 4 : {static sp3301 sp3301_4(4);return sp3301_4;}
        default: {static sp3301 reserve(-1);return reserve;}
	}
}

sp3301::rfu_info_t::rfu_info_t(const char *k7_0,const char *k7_1,const char *s6,const char *sn):
    k7_0_ver(0),
    k7_1_ver(0),
    s6_ver(0)
{
    strcpy(this->rsrc_name.k7_0,k7_0);
    strcpy(this->rsrc_name.k7_1,k7_1);
    strcpy(this->rsrc_name.s6,s6);
    strcpy(this->sn,sn);
}

sp3301::active_t sp3301::is_actived()
{
    return m_active;
}

basic_sp1401 *sp3301::get_sp1401(int32_t rf_idx)
{
    return m_sp1401->at(rf_idx).get();
}

sp1401_r1a *sp3301::get_sp1401_r1a(int32_t rf_idx)
{
    return m_sp1401_r1a->at(rf_idx).get();
}

sp1401_r1c *sp3301::get_sp1401_r1c(int32_t rf_idx)
{
    return m_sp1401_r1c->at(rf_idx).get();
}

sp2401_r1a *sp3301::get_sp2401(int32_t rf_idx)
{
    return m_sp2401_r1a->at(rf_idx).get();
}

sp3301::rfu_info_t sp3301::get_info()
{
    return m_rfu_info;
}

uint32_t sp3301::get_rfu_idx()
{
    return m_rfu_idx;
}

int32_t sp3301::get_ocxo(int16_t &value)
{
    cal_file::get_x9119()->get(value);
	return 0;
}

int32_t sp3301::boot()
{
    std::vector<std::string> str_rsrc_rfu;
    std::vector<std::string>::iterator iter_rsrc_rfu;
    vi_pci_dev pci_dev;

    char str_product_form[64];
    char str_rsrc_section[64];
    char str_dev_key[64];
    memset(str_product_form,0,sizeof(str_product_form));
    memset(str_rsrc_section,0,sizeof(str_rsrc_section));
    memset(str_dev_key,0,sizeof(str_dev_key));

    gen_ini_file ini_file(CONFIG_FILE_PATH);

    ini_file.get_config_str_value("Product Form","Form",str_product_form);
    strcpy(str_rsrc_section,"RESOURCE");
    strcat(str_rsrc_section,str_product_form);

    sprintf(str_dev_key,"RFU%u_K7_0",m_rfu_idx);
    ini_file.get_config_str_value(str_rsrc_section,str_dev_key,m_rfu_info.rsrc_name.k7_0);
    sprintf(str_dev_key,"RFU%u_K7_1",m_rfu_idx);
    ini_file.get_config_str_value(str_rsrc_section,str_dev_key,m_rfu_info.rsrc_name.k7_1);
    sprintf(str_dev_key,"RFU%u_S6",m_rfu_idx);
    ini_file.get_config_str_value(str_rsrc_section,str_dev_key,m_rfu_info.rsrc_name.s6);

    pci_dev.get_devs(str_rsrc_rfu);

    for (iter_rsrc_rfu = str_rsrc_rfu.begin();iter_rsrc_rfu != str_rsrc_rfu.end();iter_rsrc_rfu ++ ) {
        if (!strcmp(m_rfu_info.rsrc_name.k7_0,iter_rsrc_rfu->c_str()))
            m_active.k7_0 = true;
        if (!strcmp(m_rfu_info.rsrc_name.k7_1,iter_rsrc_rfu->c_str()))
            m_active.k7_1 = true;
        if (!strcmp(m_rfu_info.rsrc_name.s6,iter_rsrc_rfu->c_str()))
            m_active.s6 = true;
	}

    if (false == m_active.k7_0 && false == m_active.k7_1 && false == m_active.s6) {
        Log.set_last_err("missing rfu%d",m_rfu_idx);
		return -1;
	}

    if (m_active.k7_0) {
        INT_CHECK(m_k7_0.init(m_rfu_info.rsrc_name.k7_0,0x10ee,0x0007));

        m_sp1401_r1a->at(2)->connect(&m_k7_0);
        m_sp1401_r1a->at(3)->connect(&m_k7_0);
        m_sp1401_r1c->at(2)->connect(&m_k7_0);
        m_sp1401_r1c->at(3)->connect(&m_k7_0);
		INS_SP1401(2);
		INS_SP1401(3);

        m_dma_mgr->push_back(boost::make_shared<dma_mgr>(&m_k7_0));
		INT_CHECK(m_dma_mgr->at(0)->init());
        INT_CHECK(m_sp1401->at(2)->get_k7_ver(m_rfu_info.k7_0_ver));
	}

    if (m_active.k7_1) {
        INT_CHECK(m_k7_1.init(m_rfu_info.rsrc_name.k7_1,0x10ee,0x0007));

        m_sp1401_r1a->at(0)->connect(&m_k7_1);
        m_sp1401_r1a->at(1)->connect(&m_k7_1);
        m_sp1401_r1c->at(0)->connect(&m_k7_1);
        m_sp1401_r1c->at(1)->connect(&m_k7_1);
		INS_SP1401(0);
		INS_SP1401(1);

        m_dma_mgr->push_back(boost::make_shared<dma_mgr>(&m_k7_1));
		INT_CHECK(m_dma_mgr->at(1)->init());
        INT_CHECK(m_sp1401->at(0)->get_k7_ver(m_rfu_info.k7_1_ver));
	}
	
    if (m_active.s6) {
        INT_CHECK(m_s6.init(m_rfu_info.rsrc_name.s6,0x10ee,0x2411));
        if (m_active.k7_0) {
            INT_CHECK(m_sp2401_r1a->at(2)->open_board(&m_k7_0,&m_s6));
            INT_CHECK(m_sp2401_r1a->at(3)->open_board(&m_k7_0,&m_s6));
            INT_CHECK(m_sp2401_r1a->at(2)->get_s6_ver(m_rfu_info.s6_ver));
		}
        if (m_active.k7_1) {
            INT_CHECK(m_sp2401_r1a->at(0)->open_board(&m_k7_1,&m_s6));
            INT_CHECK(m_sp2401_r1a->at(1)->open_board(&m_k7_1,&m_s6));
            INT_CHECK(m_sp2401_r1a->at(0)->get_s6_ver(m_rfu_info.s6_ver));
		}
	}

    sprintf(m_rfu_info.sn,"SP2401R1BRFUD160%08x%08x",m_rfu_info.k7_0_ver,m_rfu_info.k7_1_ver);

	for (int32_t i = 0;i < 4;i ++) {
        rf_set_io_mode(i,OUTPUT);
        rf_set_tx_freq(i,m_tx_freq[i]);
        rf_set_tx_pwr(i,(float)m_tx_pwr[i]);
        rf_set_rx_freq(i,m_rx_freq[i]);
        rf_set_rx_level(i,m_ref[i]);
        m_sp1401->at(i)->set_pwr_meas_timeout(6553600);
	}

	return 0;
}

int32_t sp3301::set_program_bit(bool k7_0,bool k7_1,char *path_0,char *path_1)
{
    m_is_program_k7_0 = k7_0;
    m_is_program_k7_1 = k7_1;
    if (path_0)
        strcpy(m_bit_path_0,path_0);
    if (path_1)
        strcpy(m_bit_path_1,path_1);
	return 0;
}

int32_t sp3301::program_bit()
{
    if (m_active.s6) {
        if (m_is_program_k7_0) {
            INT_CHECK(m_sp2401_r1a->at(2)->set_fpga_bit(m_bit_path_0));
		}
        if (m_is_program_k7_1) {
            INT_CHECK(m_sp2401_r1a->at(0)->set_fpga_bit(m_bit_path_1));
		}
	}
	return 0;
}

int32_t sp3301::get_rf_port()
{
    int32_t no = 0;
	for (int i = 0;i < 4;i ++) {
        if (m_active.sp1401[i] == true)
            no ++;
	}
    return no;
}

int32_t sp3301::get_sn(char *sn)
{
    strcpy(sn,m_rfu_info.sn);
	return 0;
}

int32_t sp3301::get_ver(char *ver)
{
    strcpy(ver,"SP2401R1BRFUD160");
	return 0;
}

const char *sp3301::get_driver_ver()
{
    extern const char *drive_ver;
    return drive_ver;
}

std::string sp3301::ass_k7_name(uint32_t k7_idx,uint32_t rfu_idx)
{
    char name[16] = {0};
    memset(name,0,sizeof(name));
    sprintf(name,"rfu%u_k7_%u",rfu_idx,k7_idx);
    return std::string(name);
}

string sp3301::ass_s6_name(uint32_t rfu_idx)
{
    char name[16] = {0};
    memset(name,0,sizeof(name));
    sprintf(name,"rfu%u_s6",rfu_idx);
    return string(name);
}

int32_t sp3301::rf_set_tx_state(uint32_t rf_idx,bool state)
{
    DECLARE_DYNAMIC_SP1401_SP2401
	
    switch (m_hw_ver[rf_idx]) {
        case basic_sp1401::R1A :
        case basic_sp1401::R1B : {
            INT_CHECK(((sp1401_r1a *)sp1401)->set_pwr_en(state));
            INT_CHECK(((sp1401_r1a *)sp1401)->set_tx_modulator_en(state));
			return 0;
							}
        case basic_sp1401::R1C :
        case basic_sp1401::R1D : {
            INT_CHECK(((sp1401_r1c *)sp1401)->set_pwr_en(state));
            INT_CHECK(((sp1401_r1c *)sp1401)->set_tx_modulator_en(state));
			return 0;
							}
		default:return 0;
	}
	return 0;
}

int32_t sp3301::rf_set_tx_pwr(uint32_t rf_idx,float pwr)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    uint64_t freq = m_tx_freq[rf_idx];
    io_mode_t mode = m_io_mode[rf_idx];

    switch (m_hw_ver[rf_idx]) {
        case basic_sp1401::R1A :
        case basic_sp1401::R1B : {
            double d_gain = 0.0,att_offset = 0.0;
            int32_t att1 = 0,att2 = 0,att3 = 0;
            sp1401::tx_pa_att_t pa_att = TX_ATT;

            R1A_CAL_FILE->get_tx_pwr(freq,pwr,mode,d_gain,att1,att2,pa_att,att3);
            R1A_CAL_FILE->get_tx_att(freq,mode,att1 + att2 + att3,att_offset);

            INT_CHECK(((sp1401_r1a *)sp1401)->set_tx_pa_att_sw(pa_att));
            INT_CHECK(((sp1401_r1a *)sp1401)->sync_set_tx_gain(att1,att2,att3,d_gain + att_offset));
			break;
							}
        case basic_sp1401::R1C :
        case basic_sp1401::R1D : {
            double att0 = 0.0,att1 = 0.0,att2 = 0.0,att3 = 0.0,d_gain = 0.0;

            if (OUTPUT == mode) {
                tx_pwr_op_table_r1cd::data_m_t data;
                R1C_CAL_FILE->m_tx_pwr_op->get(freq,pwr,&data);
                att0 = data.att0 / 2.0;
                att1 = data.att1 / 2.0;
                att2 = data.att2 / 2.0;
                att3 = data.att3 / 2.0;
                d_gain = (double)data.d_gain + R1C_CAL_FILE->m_tx_att_op->get(freq,pwr);
			}
            else if (IO == mode) {
                tx_pwr_io_table_r1cd::data_m_t data;
                R1C_CAL_FILE->m_tx_pwr_io->get(freq,pwr,&data);
                att0 = data.att0 / 2.0;
                att1 = data.att1 / 2.0;
                att2 = data.att2 / 2.0;
                att3 = data.att3 / 2.0;
                d_gain = (double)data.d_gain + R1C_CAL_FILE->m_tx_att_io->get(freq,pwr);
			}
// 			sp1401->SetTxAtt(dAtt0,dAtt1,dAtt2,dAtt3);	//need to sync with baseband
// 			sp2401->SetTxPowerComp(dDGain);
			break;
							}
		default:break;
	}
    m_tx_pwr[rf_idx] = (double)pwr;
	return 0;
}

int32_t sp3301::rf_set_tx_freq(uint32_t rf_idx,uint64_t freq)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    uint64_t freq_rf = freq / (int64_t)RF_FREQ_SPACE * (int64_t)RF_FREQ_SPACE;
    double freq_duc = double(freq - freq_rf);

    switch (m_hw_ver[rf_idx]) {
        case basic_sp1401::R1A :
        case basic_sp1401::R1B : {
            double th = 0.0;
            uint16_t am_i = 0,am_q = 0;
            int16_t dc_i = 0,dc_q = 0;

            R1A_CAL_FILE->m_tx_sb->get(freq_rf,th,am_i,am_q);
            R1A_CAL_FILE->m_tx_lol->get(freq_rf,dc_i,dc_q);
            INT_CHECK(sp2401->set_tx_phase_rotate_I(th));
            INT_CHECK(sp2401->set_tx_amplitude_balance(am_i,am_q));
            INT_CHECK(sp2401->set_tx_dc_offset(dc_i,dc_q));
			break;
							}
        case basic_sp1401::R1C :
        case basic_sp1401::R1D : {
            tx_lol_table_r1cd::data_m_t data_lol;
            tx_sb_table_r1cd::data_m_t data_sb;

            R1C_CAL_FILE->m_tx_lol->get(freq_rf,&data_lol);
            R1C_CAL_FILE->m_tx_sb->get(freq_rf,&data_sb);
            INT_CHECK(sp2401->set_tx_dc_offset((uint16_t)(data_lol.dc_i),(uint16_t)(data_lol.dc_q)));
            INT_CHECK(sp2401->set_tx_phase_rotate_I((double)(data_sb.th)));
            INT_CHECK(sp2401->set_tx_amplitude_balance(data_sb.am_i,data_sb.am_q));
			break;
							}
		default:break;
	}
    INT_CHECK(sp1401->set_tx_freq(freq_rf));
    INT_CHECK(sp2401->set_duc_dds(freq_duc));
    m_tx_freq[rf_idx] = freq;
    INT_CHECK(rf_set_tx_pwr(rf_idx,(float)m_tx_pwr[rf_idx]));
    INT_CHECK(rf_set_tx_bw(rf_idx,sp1401->get_bw()));
	return 0;
}

int32_t sp3301::rf_set_tx_bw(uint32_t rf_idx,bw_t bw)
{
    DECLARE_DYNAMIC_SP1401_SP2401

    switch (m_hw_ver[rf_idx]) {
        case basic_sp1401::R1A :
        case basic_sp1401::R1B : {
			return 0;
							}
        case basic_sp1401::R1C :
        case basic_sp1401::R1D : {
            double real[TX_FILTER_ORDER_2I] = {0.0};
            double imag[TX_FILTER_ORDER_2I] = {0.0};

            R1C_CAL_FILE->set_bw(bw);
            if (_80M == bw) {
                tx_filter_80m_table::data_m_t data;
                R1C_CAL_FILE->m_tx_filter_80m->get(m_tx_freq[rf_idx],&data);
                data._2double(real,imag);
                sp2401->set_tx_filter(real,imag);
			}
            else if (_160M == bw) {
                tx_filter_160m_table::data_m_t data;
                R1C_CAL_FILE->m_tx_filter_160m->get(m_tx_freq[rf_idx],&data);
                data._2double(real,imag);
                sp2401->set_tx_filter(real,imag);
			}
			return 0;
							}
		default:return 0;
	}
	return 0;
}

int32_t sp3301::rf_set_tx_delay(uint32_t rf_idx,double ns)
{
	return 0;
}

int32_t sp3301::rf_set_tx_src(uint32_t rf_idx,sp2401_r1a::da_src_t src)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    INT_CHECK(sp2401->set_dds_src(src));
	return 0;
}

int32_t sp3301::rf_set_src_freq(uint32_t rf_idx,uint64_t freq)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    INT_CHECK(sp2401->set_dds1((double)freq));
	return 0;
}

int32_t sp3301::arb_load(uint32_t rf_idx, const char *path)
{
    DECLARE_DYNAMIC_DDR
    uint32_t rf_idx_2 = RF_IDX_2(rf_idx);

    basic_sp1401 *sp1401[2] = {m_sp1401->at(rf_idx).get(),m_sp1401->at(rf_idx_2).get()};
	FILE *fp[2] = {NULL,NULL};
    vi_pci_dev *m_k7 = sp1401[0]->get_k7();

    uint32_t spls_total = 0;
    uint32_t spls_left = 0;
    uint32_t spls_trsed = 0;
    uint32_t spls_trsing = 0;
    uint32_t ddr_addr = 0;
    uint32_t cnt = 0;
    unsigned inter = 1;

    arb_reader::param_t arb_param[2];
    list<arb_reader::seg_header_t>::iterator iter_seg_header;
    list<arb_seg_t>::iterator iter_arb_seg;

    INT_CHECK(sp1401[0]->get_arb_reader()->load(path));
    if (false == sp1401[1]->get_arb_reader()->is_loaded()) {
        INT_CHECK(sp1401[1]->get_arb_reader()->load(path));
	}

    if ((fp[0] = sp1401[0]->get_arb_reader()->load(&arb_param[0],NULL)) == NULL)
		return -1;
    if ((fp[1] = sp1401[1]->get_arb_reader()->load(&arb_param[1],NULL)) == NULL)
		return -1;

    if (arb_param[0].total_samples != arb_param[1].total_samples) {
        Log.set_last_err("samples file0:%d != file1:%d",arb_param[0].total_samples,arb_param[1].total_samples);
		return -1;
	}

    iter_seg_header = arb_param[0].seg_header.begin();
    iter_arb_seg = arb_param[0].arb_seg.begin();

    INT_CHECK(sp1401[0]->arb_stop());
    INT_CHECK(sp1401[1]->arb_stop());
    INT_CHECK(sp1401[0]->set_arb_segments(arb_param[0].file_header.segments));
    INT_CHECK(sp1401[1]->set_arb_segments(arb_param[1].file_header.segments));

    RFU_K7_REG_DECLARE(0x0544);
    for (iter_seg_header = arb_param[0].seg_header.begin();iter_seg_header != arb_param[0].seg_header.end();iter_seg_header ++) {
        inter = (unsigned)(245.76e6 / iter_seg_header->sr);
        RFU_K7_REG(0x0544).inter = inter;
        RFU_K7_W(0x0544);
    }

    RFU_K7_REG_DECLARE(0x0028);
    RFU_K7_REG_DECLARE(0x002f);
    RFU_K7_REG(0x0028).flag = 0;

    spls_total = arb_param[0].total_samples + arb_param[1].total_samples;
    spls_left = spls_total;
    Log.stdprintf("prepare fpga dma read,total samples(*2ed):%d\n",spls_total);
    while(spls_left > 0)
	{
        cnt ++;
        spls_trsing = spls_left > T_BLOCK_SAMPLES ? T_BLOCK_SAMPLES : spls_left;

        ddr_addr = spls_trsed >> 1;
        RFU_K7_REG(0x002f).samples = spls_trsing;
        RFU_K7_W(0x002f);
        RFU_K7_REG(0x0028).addr = ddr_addr;
        RFU_K7_W(0x0028);

        for (uint32_t i = 0;i < spls_trsing / 2;i ++) {
            fread(ddr->usr_spc() + i * 2,    sizeof(iq_data_t),1,fp[0]);
            fread(ddr->usr_spc() + i * 2 + 1,sizeof(iq_data_t),1,fp[1]);
		}

        INT_CHECK(ddr->fpga_r(spls_trsing));
		
        spls_trsed += spls_trsing;
        spls_left = spls_total - spls_trsed;
        Log.stdprintf("dma%-4d done,samples(*2ed):%-8d total(*2ed):%-10d\r",cnt,spls_trsing,spls_trsed);
	}
	sp1401[0]->set_arb_bc_star_pos(0);
	sp1401[0]->set_arb_bc_samples(spls_total);

    Log.stdprintf("\n");
    Log.stdprintf("all done\n");
	return 0;
}

int32_t sp3301::set_arb_en(uint32_t rf_idx,bool en)
{
	return 0;
}

int32_t sp3301::set_arb_trig(uint32_t rf_idx)
{
	return 0;
}

int32_t sp3301::set_arb_cnt(uint32_t rf_idx,int cnt)
{
	return 0;
}

int32_t sp3301::rf_set_rx_level(uint32_t rf_idx,double level)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    uint64_t freq = m_rx_freq[rf_idx];
    io_mode_t mode = m_io_mode[rf_idx];

    switch (m_hw_ver[rf_idx]) {
        case basic_sp1401::R1A :
        case basic_sp1401::R1B : {
            sp1401::rx_lna_att_t lna_att = sp1401::RX_ATT;
            double att1 = 0.0;
            int32_t att2 = 0;
            int64_t ad_0dbfs = _0dBFS;

            R1A_CAL_FILE->m_rx_ref->get(freq,level,mode,ad_0dbfs,lna_att,att1,att2);
            INT_CHECK(((sp1401_r1a *)sp1401)->set_rx_lna_att_sw(lna_att));
            INT_CHECK(((sp1401_r1a *)sp1401)->set_rx_att(att1,att2));
            INT_CHECK(sp2401->set_rx_pwr_comp((int32_t)(_0dBFS - ad_0dbfs)));
			break;
							}
        case basic_sp1401::R1C :
        case basic_sp1401::R1D : {
            rx_ref_op_table_r1cd::rx_state_m_t rx_state;
            int32_t offset = 0;

            if (OUTPUT == mode) {
                R1C_CAL_FILE->m_rx_ref_op->get(freq,level,&rx_state);
                offset = R1C_CAL_FILE->m_rx_att_op->get(freq,level);
                offset += rx_state.ad_offset;
			}
            if (IO == mode) {
                R1C_CAL_FILE->m_rx_ref_io->get(freq,level,&rx_state);
                offset = R1C_CAL_FILE->m_rx_att_io->get(freq,level);
                offset += rx_state.ad_offset;
			}
            INT_CHECK(((sp1401_r1c *)sp1401)->set_rx_lna_att_sw((sp1401::rx_lna_att_t)(rx_state.lna_att)));
            INT_CHECK(((sp1401_r1c *)sp1401)->set_rx_att_019_sw((sp1401::rx_att_019_t)(rx_state.att_019)));
            INT_CHECK(((sp1401_r1c *)sp1401)->set_rx_att((double)rx_state.att1,(double)rx_state.att2,(double)rx_state.att3));
            INT_CHECK(sp2401->set_rx_pwr_comp(offset));
			break;
							}
		default:break;
	}
    m_ref[rf_idx] = level;
	return 0;
}

int32_t sp3301::rf_set_rx_freq(uint32_t rf_idx,uint64_t freq)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    uint64_t freq_rf = freq / RF_FREQ_SPACE * RF_FREQ_SPACE;
    double freq_ddc = -92640000.0 - double(freq - freq_rf);

    INT_CHECK(sp1401->set_rx_freq(freq_rf));
    INT_CHECK(sp2401->set_ddc(freq_ddc));
    m_rx_freq[rf_idx] = freq;
    INT_CHECK(rf_set_rx_level(rf_idx,m_ref[rf_idx]));
    INT_CHECK(rf_set_rx_bw(rf_idx,sp1401->get_bw()));
	return 0;
}

int32_t sp3301::rf_set_rx_bw(uint32_t rf_idx,bw_t bw)
{
    DECLARE_DYNAMIC_SP1401_SP2401

    switch (m_hw_ver[rf_idx]) {
        case basic_sp1401::R1A :
        case basic_sp1401::R1B : {
			return 0;
							}
        case basic_sp1401::R1C :
        case basic_sp1401::R1D : {
            R1C_CAL_FILE->set_bw(bw);
            double real[RX_FILTER_ORDER] = {0.0};
            double imag[RX_FILTER_ORDER] = {0.0};

            if (_80M == bw) {
                rx_filter_80m_table::data_m_t data;

                R1C_CAL_FILE->m_rx_filter_80m->get(m_tx_freq[rf_idx],&data);
                data._2double(real,imag);
                sp2401->set_rx_filter(real,imag);
			}
            else if (_160M == bw) {
                rx_filter_160m_table::data_m_t data;

                R1C_CAL_FILE->m_rx_filter_160m->get(m_tx_freq[rf_idx],&data);
                data._2double(real,imag);
                sp2401->set_rx_filter(real,imag);
			}
			return 0;
							}
		default:return 0;
	}
	return 0;
}

int32_t sp3301::rf_set_io_mode(uint32_t rf_idx,io_mode_t mode)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    INT_CHECK(sp1401->set_io_mode(mode));
    m_io_mode[rf_idx] = mode;
	return 0;
}

int32_t sp3301::rf_set_trig_mode(uint32_t rf_idx, basic_sp1401::pwr_meas_src_t src)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    INT_CHECK(sp1401->set_pwr_meas_src(src,true));
	return 0;
}

int32_t sp3301::rf_set_trig_level(uint32_t rf_idx,float level)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    INT_CHECK(sp1401->set_pwr_mesa_trig_threshold((double)level));
	return 0;
}

int32_t sp3301::rf_set_trig_offset(uint32_t rf_idx,uint32_t offset)
{
	return 0;
}

int32_t sp3301::set_iq_cap_samples(uint32_t rf_idx,uint32_t samples)
{
    DECLARE_DYNAMIC_DDR
    INT_CHECK(ddr->set_fpga_w_samples(samples));
	return 0;
}

int32_t sp3301::set_iq_cap_buffers(uint32_t rf_idx, int16_t *I, int16_t *Q)
{
    m_I->at(rf_idx) = I;
    m_Q->at(rf_idx) = Q;
    return 0;
}

int32_t sp3301::iq_cap_start(uint32_t rf_idx)
{
    DECLARE_DYNAMIC_DDR
    INT_CHECK(ddr->fpga_w());
    return 0;
}

int32_t sp3301::iq_cap_abort(uint32_t rf_idx)
{
    DECLARE_DYNAMIC_DDR
    INT_CHECK(ddr->fpga_w_abort());
    return 0;
}

int32_t sp3301::iq_cap_iq2buf(uint32_t rf_idx, uint32_t samples)
{
    DECLARE_DYNAMIC_DDR
    uint32_t rf_idx_2 = RF_IDX_2(rf_idx);
    INT_CHECK(ddr->iq2buf(rf_idx, m_I->at(rf_idx), m_Q->at(rf_idx), samples, m_I->at(rf_idx_2), m_Q->at(rf_idx_2)));
    return 0;
}

int32_t sp3301::rf_init_pwr_meas(uint32_t rf_idx)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    INT_CHECK(sp1401->pwr_meas_abort());
    INT_CHECK(sp1401->pwr_meas_start());
	return 0;
}

int32_t sp3301::rf_abort_pwr_meas(uint32_t rf_idx)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    INT_CHECK(sp1401->pwr_meas_abort());
	return 0;
}

int32_t sp3301::rf_get_pwr_meas_proc(uint32_t rf_idx,basic_sp1401::pwr_meas_state_t &proc)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    INT_CHECK(sp1401->get_pwr_meas_state(proc));
	return 0;
}

int32_t sp3301::rf_get_pwr_meas_result(uint32_t rf_idx,float &pwr,float &crest)
{
    DECLARE_DYNAMIC_SP1401_SP2401
    double pwr_avg = 0.0,pwr_peak = 0.0;
    pwr = -100.0;
    crest = 0.0;
    INT_CHECK(sp1401->get_pwr_meas_pwr(pwr_avg));
    INT_CHECK(sp1401->get_pwr_meas_peak(pwr_peak));
    pwr = (float)pwr_avg;
    crest = (float)(pwr_peak - pwr_avg);
	return 0;
}

int32_t sp3301::rf_get_temp(uint32_t rf_idx,double &tx_temp,double &rx_temp)
{
    DECLARE_DYNAMIC_SP1401_SP2401

    tx_temp = 0.0;
    rx_temp = 0.0;
    switch (m_hw_ver[rf_idx]) {
        case basic_sp1401::R1A :
        case basic_sp1401::R1B : {
            INT_CHECK(((sp1401_r1a *)sp1401)->get_tx_temp(tx_temp));
            INT_CHECK(((sp1401_r1a *)sp1401)->get_rx_temp(rx_temp));
			return 0;
							}
        case basic_sp1401::R1C : {
            double tx_t[4] = {0.0,0.0,0.0,0.0};
            double rx_t[4] = {0.0,0.0,0.0,0.0};
            ((sp1401_r1c *)sp1401)->get_temp4_tx_lo2(&tx_t[0]);
            ((sp1401_r1c *)sp1401)->get_temp5_tx_lo1(&tx_t[1]);
            ((sp1401_r1c *)sp1401)->get_temp6_tx_pa4(&tx_t[2]);
            ((sp1401_r1c *)sp1401)->get_temp7_tx_pa3(&tx_t[3]);
            ((sp1401_r1c *)sp1401)->get_temp0_rx_lo2(&rx_t[0]);
            ((sp1401_r1c *)sp1401)->get_temp1_rx_lna(&rx_t[1]);
            ((sp1401_r1c *)sp1401)->get_temp2_rx_lo1(&rx_t[2]);
            ((sp1401_r1c *)sp1401)->get_temp3_rx_pa1(&rx_t[3]);

            for (int32_t i = 0;i < 4;i ++) {
                tx_temp += tx_t[i];
                rx_temp += rx_t[i];
            }
            tx_temp /= 4.0;
            rx_temp /= 4.0;
			return 0;
							}
		default:return 0;
	}
	return 0;
}
