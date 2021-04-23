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
using namespace rd;

#define INSTR_CHECK(func) \
{   if (!(func)) { \
        if (_en_expt) { throw rd::runtime_error(__FUNCTION__); } \
        else { return false; } \
    } \
}


instr::instr(void) :
    _des_sa("\0"),
    _des_sg("\0"),
    _des_pm("\0")
{
    _all_sa = boost::make_shared<std::vector<sa_sptr>>();
    boost::assign::push_back(*_all_sa)
            (boost::make_shared<ks_n9010a>())
            (boost::make_shared<ks_n9020a>())
            (boost::make_shared<ks_n9020b>())
            (boost::make_shared<ks_n9030a>())
            (boost::make_shared<ks_e4440a>())
            (boost::make_shared<ks_e4443a>())
            (boost::make_shared<ks_e4445a>())
            (boost::make_shared<rs_fsup>());

    _all_sg = boost::make_shared<std::vector<sg_sptr>>();
    boost::assign::push_back(*_all_sg)
            (boost::make_shared<ks_n5182a>())
            (boost::make_shared<ks_n5182b>())
            (boost::make_shared<ks_e4438c>())
            (boost::make_shared<ks_e8267d>())
            (boost::make_shared<rs_smf100a>());

    _all_pm = boost::make_shared<std::vector<pm_sptr>>();
    boost::assign::push_back(*_all_pm)
            (boost::make_shared<rs_nrp>());

    _en_expt = true;
    _sa = nullptr;
    _sg = nullptr;
    _pm = nullptr;

    vi_pci_dev::open_default_rm();
}

instr& instr::instance()
{
    static instr g_instr;
    return g_instr;
}

void instr::set_en_expt(bool en)
{
    _en_expt = en;
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

    _sa = nullptr;
    _sg = nullptr;
    _pm = nullptr;

    if (viFindRsrc(vi_pci_dev::get_default_rm(),expr,&find_list,&ret_cnt,des) < VI_SUCCESS) {
        if (_en_expt) {
            throw rd::runtime_error("vi find rsrc GPIB?*INSTR");
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
            if (_en_expt) {
                ViChar msg[256] = {0};
                rsnrpz_error_message(VI_NULL,status,msg);
                throw rd::runtime_error(msg);
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
            if (_en_expt) {
                ViChar msg[256] = {0};
                rsnrpz_error_message(VI_NULL,status,msg);
                throw rd::runtime_error(msg);
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
    bool specified_sa = !_des_sa.empty();
    bool specified_sg = !_des_sg.empty();
    bool specified_pm = !_des_pm.empty();
    string descriptor;

    for (size_t i = 0;i < _all_sa->size();i ++) {
        descriptor = (specified_sa ? _des_sa : _all_sa->at(i)->get_descriptor());
        if (idn.find(descriptor) != string::npos) {
            if (!_all_sa->at(i)->init(des)) {
				continue;
            } else {
                _sa = _all_sa->at(i).get();
				return true;
			}
		}
	}

    for (size_t i = 0;i < _all_sg->size();i ++) {
        descriptor = (specified_sg ? _des_sg : _all_sg->at(i)->get_descriptor());
        if (idn.find(descriptor) != string::npos) {
            if (!_all_sg->at(i)->init(des)) {
				continue;
            } else {
                _sg = _all_sg->at(i).get();
				return true;
			}
		}
	}

    for (size_t i = 0;i < _all_pm->size();i ++) {
        descriptor = (specified_pm ? _des_pm : _all_pm->at(i)->get_descriptor());
        if (idn.find(descriptor) != string::npos) {
            if (!_all_pm->at(i)->init(des)) {
				continue;
            } else {
                _pm = _all_pm->at(i).get();
				return true;
			}
		}
	}

	return true;
}

bool instr::close()
{
    if (_pm) {
        INSTR_CHECK(_pm->close());
    }

	return true;
}

bool instr::has_sa()
{
    if (_sa == nullptr) {
        if (_en_expt) {
            throw rd::runtime_error((_des_sa.empty() ? "sa" : _des_sa) + " disconnect");
        } else {
			return false;
		}
	}
	return true;
}

bool instr::has_sg()
{
    if (_sg == nullptr) {
        if (_en_expt) {
            throw rd::runtime_error((_des_sg.empty() ? "sg" : _des_sg) + " disconnect");
        } else {
			return false;
		}
	}
	return true;
}

bool instr::has_pm()
{
    if (_pm == nullptr) {
        if (_en_expt) {
            throw rd::runtime_error((_des_pm.empty() ? "pm" : _des_pm) + " disconnect");
        } else {
			return false;
		}
	}
	return true;
}

void instr::set_sa(std::string des)
{
    _des_sa = des;
}

void instr::set_sg(std::string des)
{
    _des_sg = des;
}

void instr::set_pm(std::string des)
{
    _des_pm = des;
}

bool instr::sa_reset()
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->reset());
    INSTR_CHECK(_sa->set_cal(sa::AUTO_OFF));
	return true;
}

bool instr::sa_set_cf(double freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_cf(freq));
	return true;
}

bool instr::sa_set_cf(uint64_t freq)
{
    return sa_set_cf(double(freq));
}

bool instr::sa_set_span(double freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_span(freq));
	return true;
}

bool instr::sa_set_rbw(double freq,bool is_auto)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_rbw(freq,is_auto));
	return true;
}

bool instr::sa_set_vbw(double freq,bool is_auto)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_vbw(freq,is_auto));
	return true;
}

bool instr::sa_set_det(sa::det_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_det(type));
	return true;
}

bool instr::sa_set_peak_search(sa::peak_search_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_peak_search(type));
	return true;
}

bool instr::sa_set_ref(double ref)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_ref(ref));
	return true;
}

bool instr::sa_get_ref(double &ref)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->get_ref(ref));
	return true;
}

bool instr::sa_set_ref_auto()
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_ref_auto());
	return true;
}

bool instr::sa_set_ref_auto(double pwr,double &ref)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_ref_auto(pwr,ref));
    return true;
}

bool instr::sa_set_ref_auto(double pwr,double &ref,double &offset)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_ref_auto(pwr,ref,offset));
    return true;
}

bool instr::sa_set_marker(sa::marker_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_marker(type));
	return true;
}

bool instr::sa_get_marker_pwr(double &pwr)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->get_marker_pwr(pwr));
	return true;
}

bool instr::sa_get_marker_freq(double &freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->get_marker_freq(freq));
	return true;
}

bool instr::sa_set_en_preamp(bool en)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_en_preamp(en));
	return true;
}

bool instr::sa_marker_move(sa::marker_move_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->marker_move(type));
	return true;
}

bool instr::sa_marker_move(double freq)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->marker_move(freq));
	return true;
}

bool instr::sa_sweep_once()
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->sweep_once());
	return true;
}

bool instr::sa_set_cal(sa::cal_type_t type)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_cal(type));
	return true;
}

bool instr::sa_set_avg_trace(bool en, uint32_t cnt)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_avg_trace(en,cnt));
	return true;
}

bool instr::sa_set_avg_trace_get_data(uint32_t avg_cnt, uint32_t pt_cnt, double *data)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_avg_trace_get_data(avg_cnt,pt_cnt,data));
	return true;
}

bool instr::sa_set_mech_att(bool is_auto,int32_t att)
{
    BOOL_CHECK(has_sa());
    INSTR_CHECK(_sa->set_mech_att(is_auto,att));
	return true;
}

bool instr::sg_reset()
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->reset());
	return true;
}

bool instr::sg_set_cw(double freq)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->set_cw(freq));
	return true;
}

bool instr::sg_get_cw(double &freq)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->get_cw(freq));
	return true;
}

bool instr::sg_set_pl(double pwr)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->set_pl(pwr));
	return true;
}

bool instr::sg_get_pl(double &pwr)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->get_pl(pwr));
	return true;
}

bool instr::sg_set_en_output(bool en)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->set_en_output(en));
	return true;
}

bool instr::sg_set_en_mod(bool en)
{
    BOOL_CHECK(has_sg());
    INSTR_CHECK(_sg->set_en_mod(en));
	return true;
}

bool instr::pm_reset()
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(_pm->reset());
	return true;
}

bool instr::pm_set_freq(double freq)
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(_pm->set_freq(freq));
	return true;
}

bool instr::pm_get_pwr(double &pwr)
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(_pm->get_pwr(pwr));
	return true;
}

bool instr::pm_get_pwr(double freq,double &pwr)
{
    BOOL_CHECK(has_pm());
    INSTR_CHECK(_pm->get_pwr(freq,pwr));
	return true;
}
