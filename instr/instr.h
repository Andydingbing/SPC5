#ifndef INSTR_H
#define INSTR_H

#include "sa.h"
#include "sg.h"
#include "pm.h"
#include <vector>
#include <boost/smart_ptr.hpp>
#include <boost/make_shared.hpp>

class RD_API instr_expt
{
public:
    instr_expt(const char* format,...);
    char* get_expt();
private:
    char m_buf[256];
};

#define Instr instr::instance()

class RD_API instr
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
    instr(instr &){}
public:
    static instr &instance();
    void set_en_expt(bool en);
public:
    virtual bool init();
    virtual bool close();
    virtual bool has_sa();
    virtual bool has_sg();
    virtual bool has_pm();
    virtual void set_sa(std::string des);
    virtual void set_sg(std::string des);
    virtual void set_pm(std::string des);
public:  
    virtual bool sa_reset();
    virtual bool sa_set_cf(double freq);
    virtual bool sa_set_span(double freq);
    virtual bool sa_set_rbw(double freq,bool is_auto);
    virtual bool sa_set_vbw(double freq,bool is_auto);
    virtual bool sa_set_det(sa::det_type_t type);
    virtual bool sa_set_peak_search(sa::peak_search_type_t type);
    virtual bool sa_set_marker(sa::marker_type_t type);
    virtual bool sa_set_cal(sa::cal_type_t type);
    virtual bool sa_set_avg_trace(bool en,int32_t cnt);
    virtual bool sa_set_ref(double ref);
    virtual bool sa_get_ref(double &ref);
    virtual bool sa_get_marker_pwr(double &pwr);
    virtual bool sa_set_en_preamp(bool en);
    virtual bool sa_get_marker_freq(double &freq);
    virtual bool sa_sweep_once();
    virtual bool sa_marker_move(sa::marker_move_type_t type);
    virtual bool sa_marker_move(double freq);
    virtual bool sa_set_avg_trace_get_data(int32_t avg_cnt,int32_t pt_cnt,double *data);
    virtual bool sa_set_mech_att(bool is_auto,int32_t att = 0);
    virtual bool sa_set_ref_auto();
public:
    virtual bool sg_reset();
    virtual bool sg_set_cw(double freq);
    virtual bool sg_get_cw(double &freq);
    virtual bool sg_set_pl(double pwr);
    virtual bool sg_get_pl(double &pwr);
    virtual bool sg_set_en_output(bool en);
    virtual bool sg_set_en_mod(bool en);
public:
    virtual bool pm_reset();
    virtual bool pm_set_freq(double freq);
    virtual bool pm_get_pwr(double &pwr);
    virtual bool pm_get_pwr(double freq,double &pwr);
private:
    virtual bool ins_instr(char *idn,ViRsrc des);
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

#endif // INSTR_H
