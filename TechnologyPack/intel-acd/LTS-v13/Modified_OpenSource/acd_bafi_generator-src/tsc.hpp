#pragma once
#include <string>

struct TscData
{
    // All variables below are used just for GNR compatibility
    std::string pcu_first_ierr_tsc_lo_cfg = "0x0";
    std::string pcu_first_ierr_tsc_hi_cfg = "0x0";
    std::string pcu_first_mcerr_tsc_lo_cfg = "0x0";
    std::string pcu_first_mcerr_tsc_hi_cfg = "0x0";
    std::string pcu_first_rmca_tsc_lo_cfg = "0x0";
    std::string pcu_first_rmca_tsc_hi_cfg = "0x0";

    uint64_t pcu_first_ierr_tsc_cfg = 0;
    uint64_t pcu_first_mcerr_tsc_cfg = 0;
    uint64_t pcu_first_rmca_tsc_cfg = 0;
    bool ierr_on_socket = false;
    bool mcerr_on_socket = false;
    bool rmca_on_socket = false;
};

struct TscVariablesNames
{
    std::string pcu_first_ierr_tsc_lo_cfg_varname;
    std::string pcu_first_ierr_tsc_hi_cfg_varname;
    std::string pcu_first_mcerr_tsc_lo_cfg_varname;
    std::string pcu_first_mcerr_tsc_hi_cfg_varname;
    std::string pcu_first_rmca_tsc_lo_cfg_varname;
    std::string pcu_first_rmca_tsc_hi_cfg_varname;
};

void countTscCfg(TscData& socketTscData)
{
    if (socketTscData.pcu_first_ierr_tsc_cfg != 0)
    {
        socketTscData.ierr_on_socket = true;
    }

    if (socketTscData.pcu_first_mcerr_tsc_cfg != 0)
    {
        socketTscData.mcerr_on_socket = true;
    }

    if (socketTscData.pcu_first_rmca_tsc_cfg != 0)
    {
        socketTscData.rmca_on_socket = true;
    }
}