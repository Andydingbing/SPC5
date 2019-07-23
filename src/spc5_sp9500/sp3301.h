#ifndef SP3301_H
#define SP3301_H

#include "sp1401.h"
#include "sp1401_r1a.h"
#include "sp1401_r1b.h"
#include "sp1401_r1c.h"
#include "sp1401_r1d.h"
#include "sp1401_r1e.h"
#include "sp1401_r1f.h"
#include "sp2401_r1a.h"
#include "dma_mgr.h"

#define SP3301_0 sp3301::instance(0)
#define SP3301_1 sp3301::instance(1)
#define SP3301_2 sp3301::instance(2)
#define SP3301_3 sp3301::instance(3)
#define SP3301_4 sp3301::instance(4)

#define K7_IDX(idx) ((MAX_RF - 1 - idx) / 2)

namespace sp_rd {

/*
 * RF_0 RF_1 RF_2 RF_3
 *   |    |    |    |
 *   ------    ------
 *   brother   brother
 */
static RD_INLINE bool is_brother_l_r(uint32_t rf_idx_l,uint32_t rf_idx_r)
{ return rf_idx_r - rf_idx_l == 1 ? (rf_idx_l != 1 ? true : false) : false; }

static RD_INLINE bool is_brother_r_l(uint32_t rf_idx_r,uint32_t rf_idx_l)
{ return is_brother_l_r(rf_idx_l,rf_idx_r); }


class RD_API sp3301
{
public:
    typedef boost::shared_ptr<std::vector<basic_sp1401::sptr>> basic_sp1401_vector_sptr;
    typedef boost::shared_ptr<std::vector<sp1401_r1a::sptr>> sp1401_r1a_vector_sptr;
    typedef boost::shared_ptr<std::vector<sp1401_r1c::sptr>> sp1401_r1c_vector_sptr;
    typedef boost::shared_ptr<std::vector<sp1401_r1e::sptr>> sp1401_r1e_vector_sptr;
    typedef boost::shared_ptr<std::vector<sp1401_r1f::sptr>> sp1401_r1f_vector_sptr;
    typedef boost::shared_ptr<std::vector<sp2401_r1a::sptr>> sp2401_r1a_vector_sptr;
    typedef boost::shared_ptr<std::vector<dma_mgr::sptr>> dma_mgr_vector_sptr;

    sp3301(uint32_t rfu_idx);
    sp3301(sp3301 &);
    virtual ~sp3301();
    static sp3301 &instance(uint32_t rfu_idx);
private:
    sp3301 & operator = (sp3301 &);
public:
    uint32_t get_rfu_idx();
    int32_t get_ocxo(uint16_t &value);
    int32_t boot();
    int32_t set_program_bit(char *path,bool k7_0 = true,bool k7_1 = true);
    int32_t program_bit();
    int32_t get_rf_port();
    sp1401::hw_ver_t get_rf_ver(uint32_t rf_idx);
    int32_t get_sn(char *sn);
    int32_t get_rf_sn(uint32_t rf_idx,char *sn);
    int32_t get_ver(char *ver);
    const char *get_driver_ver();
    static std::string ass_k7_name(uint32_t k7_idx,uint32_t rfu_idx);
    static std::string ass_s6_name(uint32_t rfu_idx);
    static char rf_ver2child_dir(sp1401::hw_ver_t ver0,sp1401::hw_ver_t ver1);

    struct active_t;
    struct rfu_info_t;
    active_t is_actived();
    rfu_info_t get_info();
public:
    int32_t rf_set_tx_state(uint32_t rf_idx,bool state);
    int32_t rf_set_tx_pwr(uint32_t rf_idx,float pwr);
    int32_t rf_set_tx_freq(uint32_t rf_idx,uint64_t freq);
    int32_t rf_get_tx_freq(uint32_t rf_idx,uint64_t &freq);
    int32_t rf_set_tx_bw(uint32_t rf_idx,sp_rd::sp1401::bw_t bw);
    int32_t rf_set_tx_delay(uint32_t rf_idx,double ns);
    int32_t rf_set_tx_src(uint32_t rf_idx,sp2401_r1a::da_src_t src);
    int32_t rf_set_src_freq(uint32_t rf_idx,uint64_t freq);
	
    int32_t arb_load(uint32_t rf_idx,const char *path);
    int32_t set_arb_en(uint32_t rf_idx,bool en);
    int32_t set_arb_trig(uint32_t rf_idx);
    int32_t set_arb_cnt(uint32_t rf_idx,int cnt);

    int32_t rf_set_rx_level(uint32_t rf_idx,double level);
    int32_t rf_set_rx_freq(uint32_t rf_idx,uint64_t freq);
    int32_t rf_get_rx_freq(uint32_t rf_idx,uint64_t &freq);
    int32_t rf_set_rx_bw(uint32_t rf_idx,sp_rd::sp1401::bw_t bw);

    int32_t rf_set_io_mode(uint32_t rf_idx,sp_rd::sp1401::io_mode_t mode);
    int32_t rf_get_temp(uint32_t rf_idx,double &tx_temp,double &rx_temp);

    int32_t rf_init_pwr_meas(uint32_t rf_idx);
    int32_t rf_abort_pwr_meas(uint32_t rf_idx);
    int32_t rf_get_pwr_meas_proc(uint32_t rf_idx,basic_sp1401::pwr_meas_state_t &proc);
    int32_t rf_get_pwr_meas_result(uint32_t rf_idx,float &pwr,float &crest);

    int32_t set_iq_cap_samples(uint32_t rf_idx,uint32_t samples);
    int32_t set_iq_cap_buffer(uint32_t rf_idx,int16_t *I,int16_t *Q);
    int32_t set_iq_cap_trig_src(uint32_t rf_idx,basic_sp1401::iq_cap_src_t src);
    int32_t set_iq_cap_trig_level(uint32_t rf_idx,float level);
    int32_t set_iq_cap_frame_trig_src(uint32_t rf_idx,sp2401_r1a::frame_trig_src_t src);
    int32_t set_iq_cap_frame_trig_frame(uint32_t rf_idx,uint32_t frame);
    int32_t set_iq_cap_frame_trig_offset(uint32_t rf_idx,int32_t offset);
    int32_t set_iq_cap_frame_trig_mod_x_y(uint32_t rf_idx,uint16_t x,uint16_t y);
    int32_t iq_cap(uint32_t rf_idx);
    int32_t iq_cap_abort(uint32_t rf_idx);
    int32_t iq_cap_iq2buf(uint32_t rf_idx);
    int32_t iq_cap_iq2buf(uint32_t rf_idx,uint32_t samples);

public:
    basic_sp1401_vector_sptr m_sp1401;
    sp1401_r1a_vector_sptr m_sp1401_r1a;
    sp1401_r1c_vector_sptr m_sp1401_r1c;
    sp1401_r1e_vector_sptr m_sp1401_r1e;
    sp1401_r1f_vector_sptr m_sp1401_r1f;
    sp2401_r1a_vector_sptr m_sp2401_r1a;
    dma_mgr_vector_sptr m_dma_mgr;
    std::vector<int16_t *> *m_I;
    std::vector<int16_t *> *m_Q;
public:
    basic_sp1401 *get_sp1401(int32_t rf_idx);
    sp1401_r1a *get_sp1401_r1a(int32_t rf_idx);
    sp1401_r1c *get_sp1401_r1c(int32_t rf_idx);
    sp1401_r1e *get_sp1401_r1e(int32_t rf_idx);
    sp1401_r1f *get_sp1401_r1f(int32_t rf_idx);
    sp2401_r1a *get_sp2401(int32_t rf_idx);
    dma_mgr *ddr(int32_t rf_idx) { return m_dma_mgr->at(K7_IDX(rf_idx)).get(); }
public:
    vi_pci_dev m_k7_0;
    vi_pci_dev m_k7_1;
    vi_pci_dev m_s6;
public:
    typedef struct RD_API rfu_info_t {
        typedef struct rsrc_name_t {
            char k7_0[32];
            char k7_1[32];
            char s6[32];
        }rsrc_name_t;

        rsrc_name_t rsrc_name;
        char        sn[128];
        uint32_t    k7_0_ver;
        uint32_t    k7_1_ver;
        uint32_t    s6_ver;
	public:
        rfu_info_t(const char *k7_0 = "",const char *k7_1 = "",const char *s6 = "",const char *sn = "");
    }rfu_info_t;
public:
    rfu_info_t m_rfu_info;
    uint32_t m_rfu_idx;

    typedef struct RD_API active_t{
        bool k7_0;
        bool k7_1;
        bool s6;
        bool sp1401[4];
        active_t() {
            k7_0 = false;
            k7_1 = false;
            s6   = false;
            for (uint32_t i = 0;i < ARRAY_SIZE(sp1401);i ++)
                sp1401[i] = false;
		}
    }active_t;
private:
    int32_t instance_sp1401(uint32_t rf_idx);
private:
    sp_rd::sp1401::io_mode_t m_io_mode[MAX_RF];
    uint64_t m_tx_freq[MAX_RF];
    uint64_t m_rx_freq[MAX_RF];
    double m_tx_pwr[MAX_RF];
    double m_ref[MAX_RF];
    bool m_is_program_k7_0;
    bool m_is_program_k7_1;
    char m_bit_path[128];
    active_t m_active;
};

} //namespace sp_rd

#endif // SP3301_H
