from ctypes import Structure
import pyads

class RxData(Structure):
    _fields_ = [
        ('eMode', pyads.PLCTYPE_UDINT),
        ('t', pyads.PLCTYPE_REAL),

        ('comau_q', pyads.PLCTYPE_ARR_REAL(6)),
        ('comau_q_t', pyads.PLCTYPE_ARR_REAL(6)),

        ('winch_l', pyads.PLCTYPE_REAL),
        ('winch_l_t', pyads.PLCTYPE_REAL),
        ('winch_l_tt', pyads.PLCTYPE_REAL),

        ('hook_p', pyads.PLCTYPE_ARR_REAL(3)),
        ('hook_p_t', pyads.PLCTYPE_ARR_REAL(3)),
        ('hook_p_tt', pyads.PLCTYPE_ARR_REAL(3)),

        ('em8000_eta', pyads.PLCTYPE_ARR_REAL(6)),
        ('em8000_eta_t', pyads.PLCTYPE_ARR_REAL(6)),
        ('em8000_eta_tt', pyads.PLCTYPE_ARR_REAL(6)),
        ('mru1_eta', pyads.PLCTYPE_ARR_REAL(6)),
        ('mru1_v', pyads.PLCTYPE_ARR_REAL(6)),

        ('em1500_eta', pyads.PLCTYPE_ARR_REAL(6)),
        ('em1500_eta_t', pyads.PLCTYPE_ARR_REAL(6)),
        ('em1500_eta_tt', pyads.PLCTYPE_ARR_REAL(6)),
        ('mru2_eta', pyads.PLCTYPE_ARR_REAL(6)),
        ('mru2_v', pyads.PLCTYPE_ARR_REAL(6))
    ]




