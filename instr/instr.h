#ifndef INSTR_INSTR_H
#define INSTR_INSTR_H

#include "sa.h"
#include "sg.h"
#include "pm.h"
#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/make_shared.hpp>

#define Instr instr::instance()

class RD_API instr : sp_rd::noncopyable
{
public:
    typedef boost::shared_ptr<sa> sa_sptr;
    typedef boost::shared_ptr<sg> sg_sptr;
    typedef boost::shared_ptr<pm> pm_sptr;

    typedef boost::shared_ptr<std::vector<sa_sptr>> sa_vector_sptr;
    typedef boost::shared_ptr<std::vector<sg_sptr>> sg_vector_sptr;
    typedef boost::shared_ptr<std::vector<pm_sptr>> pm_vector_sptr;

protected:
    instr(void);
public:
    static instr &instance();
    void set_en_expt(bool en);
public:
    bool init();
    bool close();
    bool has_sa();
    bool has_sg();
    bool has_pm();
    void set_sa(std::string des);
    void set_sg(std::string des);
    void set_pm(std::string des);
public:  
    bool sa_reset();
    bool sa_set_cf(double freq);
    bool sa_set_cf(uint64_t freq);
    bool sa_set_span(double freq);
    bool sa_set_rbw(double freq,bool is_auto);
    bool sa_set_vbw(double freq,bool is_auto);
    bool sa_set_det(sa::det_type_t type);
    bool sa_set_peak_search(sa::peak_search_type_t type);
    bool sa_set_marker(sa::marker_type_t type);
    bool sa_set_cal(sa::cal_type_t type);
    bool sa_set_avg_trace(bool en,uint32_t cnt);
    bool sa_set_ref(double ref);
    bool sa_get_ref(double &ref);
    bool sa_get_marker_pwr(double &pwr);
    bool sa_set_en_preamp(bool en);
    bool sa_get_marker_freq(double &freq);
    bool sa_sweep_once();
    bool sa_marker_move(sa::marker_move_type_t type);
    bool sa_marker_move(double freq);
    bool sa_set_avg_trace_get_data(uint32_t avg_cnt,uint32_t pt_cnt,double *data);
    bool sa_set_mech_att(bool is_auto,int32_t att = 0);
    bool sa_set_ref_auto();
public:
    bool sg_reset();
    bool sg_set_cw(double freq);
    bool sg_get_cw(double &freq);
    bool sg_set_pl(double pwr);
    bool sg_get_pl(double &pwr);
    bool sg_set_en_output(bool en);
    bool sg_set_en_mod(bool en);
public:
    bool pm_reset();
    bool pm_set_freq(double freq);
    bool pm_get_pwr(double &pwr);
    bool pm_get_pwr(double freq,double &pwr);
private:
    bool ins_instr(const std::string &idn, ViRsrc des);
private:
    bool m_en_expt;
    std::string m_des_sa;
    std::string m_des_sg;
    std::string m_des_pm;
    sa* m_sa;
    sg* m_sg;
    pm* m_pm;
    sa_vector_sptr m_all_sa;
    sg_vector_sptr m_all_sg;
    pm_vector_sptr m_all_pm;

};

#endif // INSTR_INSTR_H
