#include "libbd.h"
#include "instr.h"
#include "ks_sa.h"
#include "ks_sg.h"
#include "rs_fsup.h"
#include "rs_smf100a.h"
#include "rs_nrp.h"
#include "rsnrpz.h"
#include "exception.hpp"
#include <string>
#include <boost/assign.hpp>
#include <boost/format.hpp>

using namespace std;
using namespace sp_rd;

#define INSTR_CHECK(func) \
{   if (!(func)) { \
        if (m_en_expt) { throw sp_rd::runtime_error(__FUNCTION__); } \
        else { return false; } \
    } \
}


instr::instr(void) :
    m_des_sa("\0"),
    m_des_sg("\0"),
    m_des_pm("\0")
{
    m_all_sa = boost::make_shared<std::vector<sa_sptr>>();
    boost::assign::push_back(*m_all_sa)
            (boost::make_shared<ks_n9010a>())
            (boost::make_shared<ks_n9020a>())
            (boost::make_shared<ks_n9030a>())
            (boost::make_shared<ks_e4440a>())
            (boost::make_shared<ks_e4443a>())
            (boost::make_shared<ks_e4445a>())
            (boost::make_shared<rs_fsup>());

    m_all_sg = boost::make_shared<std::vector<sg_sptr>>();
    boost::assign::push_back(*m_all_sg)
            (boost::make_shared<ks_n5182a>())
            (boost::make_shared<ks_n5182b>())
            (boost::make_shared<ks_e4438c>())
            (boost::make_shared<ks_e8267d>())
            (boost::make_shared<rs_smf100a>());

    m_all_pm = boost::make_shared<std::vector<pm_sptr>>();
    boost::assign::push_back(*m_all_pm)
            (boost::make_shared<rs_nrp>());

    m_en_expt = true;
    m_sa = nullptr;
    m_sg = nullptr;
    m_pm = nullptr;

    vi_pci_dev::open_default_rm();
}

instr& instr::instance()
{
    static instr g_instr;
    return g_instr;
}

void instr::set_en_expt(bool en)
{
    m_en_expt = en;
}

bool instr::init()
{
    ViFindList find_list = 0;
    ViUInt32 ret_cnt = 0;
    ViUInt32 ret = 0;
    ViChar des[256] = {0};
    ViChar idn[256] = {0};
    ViSession session = 0;
    ViChar expr[32] = "GPIB?*INSTR";

    m_sa = nullptr;
    m_sg = nullptr;
    m_pm = nullptr;

    if (viFindRsrc(vi_pci_dev::get_default_rm(),expr,&find_list,&ret_cnt,des) < VI_SUCCESS) {
        if (m_en_expt) {
            throw sp_rd::runtime_error("vi find rsrc GPIB?*INSTR");
        } else {
            return false;
        }
	}
    while (ret_cnt --) {
        if (viOpen(vi_pci_dev::get_default_rm(),des,VI_NULL,VI_NULL,&session) < VI_SUCCESS) {
            viFindNext(find_list,des);
			continue;
		}
        if (viWrite(session,ViBuf("*IDN?"),5,&ret) < VI_SUCCESS) {
            viClose(session);
            viFindNext(find_list,des);
			continue;
		}
        if (viRead(session,ViPBuf(idn),256,&ret) < VI_SUCCESS) {
            viClose(session);
            viFindNext(find_list,des);
			continue;
		}
        viClose(session);
        ins_instr(string(idn),des);
        viFindNext(find_list,des);
	}
    viClose(find_list);

    ViStatus status = VI_SUCCESS;
    ViInt32 num_sensors  = 0;
    char sensor_name[256] = {0};
    char sensor_type[256] = {0};
    char sensor_sn[256] = {0};

    for (uint32_t retry = 0;retry < 2;retry ++) {
        if ((status = rsnrpz_GetSensorCount(0,&num_sensors)) != VI_SUCCESS) {
            if (m_en_expt) {
                ViChar msg[256] = {0};
                rsnrpz_error_message(VI_NULL,status,msg);
                throw sp_rd::runtime_error(msg);
            } else {
                return false;
            }
		}
        if (num_sensors > 0) {
			break;
        }
	}

    if (num_sensors < 1) {
		return true;
    }

    for (ViInt32 i = 1;i <= num_sensors;i ++) {
        if ((status = rsnrpz_GetSensorInfo(0,i,sensor_name,sensor_type,sensor_sn)) != VI_SUCCESS) {
            if (m_en_expt) {
                ViChar msg[256] = {0};
                rsnrpz_error_message(VI_NULL,status,msg);
                throw sp_rd::runtime_error(msg);
            } else {
                return false;
            }
		}
        ins_instr(string("ROHDE&SCHWARZ,NRP"),sensor_name);
	}

	return true;
}

bool instr::ins_instr(const std::string &idn, ViRsrc des)
{
    bool specified_sa = !m_des_sa.empty();
    bool specified_sg = !m_des_sg.empty();
    bool specified_pm = !m_des_pm.empty();
    string descriptor;

    for (size_t i = 0;i < m_all_sa->size();i ++) {
        descriptor = (specified_sa ? m_des_sa : m_all_sa->at(i)->get_descriptor());
        if (idn.find(descriptor) != string::npos) {
            if (!m_all_sa->at(i)->init(des)) {
				continue;
            } else {
                m_sa = m_all_sa->at(i).get();
				return true;
			}
		}
	}

    for (size_t i = 0;i < m_all_sg->size();i ++) {
        descriptor = (specified_sg ? m_des_sg : m_all_sg->at(i)->get_descriptor());
        if (idn.find(descriptor) != string::npos) {
            if (!m_all_sg->at(i)->init(des)) {
				continue;
            } else {
                m_sg = m_all_sg->at(i).get();
				return true;
			}
		}
	}

    for (size_t i = 0;i < m_all_pm->size();i ++) {
        descriptor = (specified_pm ? m_des_pm : m_all_pm->at(i)->get_descriptor());
        if (idn.find(descriptor) != string::npos) {
            if (!m_all_pm->at(i)->init(des)) {
				continue;
            } else {
                m_pm = m_all_pm->at(i).get();
				return true;
			}
		}
	}

	return true;
}

bool instr::close()
{
    if (m_pm) {
        INSTR_CHECK(m_pm->close());
    }

	return true;
}

bool instr::has_sa()
{
    if (m_sa == nullptr) {
        if (m_en_expt) {
            throw sp_rd::runtime_error((m_des_sa.empty() ? "sa" : m_des_sa) + " disconnect");
        } else {
			return false;
		}
	}
	return true;
}

bool instr::has_sg()
{
    if (m_sg == nullptr) {
        if (m_en_expt) {
            throw sp_rd::runtime_error((m_des_sg.empty() ? "sg" : m_des_sg) + " disconnect");
        } else {
			return false;
		}
	}
	return true;
}

bool instr::has_pm()
{
    if (m_pm == nullptr) {
        if (m_en_expt) {
            throw sp_rd::runtime_error((m_des_pm.empty() ? "pm" : m_des_pm) + " disconnect");
        } else {
			return false;
		}
	}
	return true;
}

void instr::set_sa(std::string des)
{
    m_des_sa = des;
}

void instr::set_sg(std::string des)
{
    m_des_sg = des;
}

void instr::set_pm(std::string des)
{
    m_des_pm = des;
}

bool instr::sa_reset()
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->reset());
    INSTR_CHECK(m_sa->set_cal(sa::AUTO_OFF));
	return true;
}

bool instr::sa_set_cf(double freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_cf(freq));
	return true;
}

bool instr::sa_set_span(double freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_span(freq));
	return true;
}

bool instr::sa_set_rbw(double freq,bool is_auto)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_rbw(freq,is_auto));
	return true;
}

bool instr::sa_set_vbw(double freq,bool is_auto)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_vbw(freq,is_auto));
	return true;
}

bool instr::sa_set_det(sa::det_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_det(type));
	return true;
}

bool instr::sa_set_peak_search(sa::peak_search_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_peak_search(type));
	return true;
}

bool instr::sa_set_ref(double ref)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_ref(ref));
	return true;
}

bool instr::sa_get_ref(double &ref)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->get_ref(ref));
	return true;
}

bool instr::sa_set_ref_auto()
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_ref_auto());
	return true;
}

bool instr::sa_set_marker(sa::marker_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_marker(type));
	return true;
}

bool instr::sa_get_marker_pwr(double &pwr)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->get_marker_pwr(pwr));
	return true;
}

bool instr::sa_get_marker_freq(double &freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->get_marker_freq(freq));
	return true;
}

bool instr::sa_set_en_preamp(bool en)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_en_preamp(en));
	return true;
}

bool instr::sa_marker_move(sa::marker_move_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->marker_move(type));
	return true;
}

bool instr::sa_marker_move(double freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->marker_move(freq));
	return true;
}

bool instr::sa_sweep_once()
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->sweep_once());
	return true;
}

bool instr::sa_set_cal(sa::cal_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_cal(type));
	return true;
}

bool instr::sa_set_avg_trace(bool en, uint32_t cnt)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_avg_trace(en,cnt));
	return true;
}

bool instr::sa_set_avg_trace_get_data(uint32_t avg_cnt, uint32_t pt_cnt, double *data)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_avg_trace_get_data(avg_cnt,pt_cnt,data));
	return true;
}

bool instr::sa_set_mech_att(bool is_auto,int32_t att)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(m_sa->set_mech_att(is_auto,att));
	return true;
}

bool instr::sg_reset()
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(m_sg->reset());
	return true;
}

bool instr::sg_set_cw(double freq)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(m_sg->set_cw(freq));
	return true;
}

bool instr::sg_get_cw(double &freq)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(m_sg->get_cw(freq));
	return true;
}

bool instr::sg_set_pl(double pwr)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(m_sg->set_pl(pwr));
	return true;
}

bool instr::sg_get_pl(double &pwr)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(m_sg->get_pl(pwr));
	return true;
}

bool instr::sg_set_en_output(bool en)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(m_sg->set_en_output(en));
	return true;
}

bool instr::sg_set_en_mod(bool en)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(m_sg->set_en_mod(en));
	return true;
}

bool instr::pm_reset()
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(m_pm->reset());
	return true;
}

bool instr::pm_set_freq(double freq)
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(m_pm->set_freq(freq));
	return true;
}

bool instr::pm_get_pwr(double &pwr)
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(m_pm->get_pwr(pwr));
	return true;
}

bool instr::pm_get_pwr(double freq,double &pwr)
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(m_pm->get_pwr(freq,pwr));
	return true;
}
