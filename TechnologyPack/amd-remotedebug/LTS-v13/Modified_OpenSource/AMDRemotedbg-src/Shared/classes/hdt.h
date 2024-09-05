/******************************************************************************/
/*! \file classes/hdt.h YAAP HDT class
 *
 * <pre>
 * Copyright (C) 2009-2012, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
 * AMD Confidential Proprietary
 *
 * AMD is granting you permission to use this software (the Materials)
 * pursuant to the terms and conditions of your Software License Agreement
 * with AMD.  This header does *NOT* give you permission to use the Materials
 * or any rights under AMD's intellectual property.  Your use of any portion
 * of these Materials shall constitute your acceptance of those terms and
 * conditions.  If you do not agree to the terms and conditions of the Software
 * License Agreement, please do not use any portion of these Materials.
 *
 * CONFIDENTIALITY:  The Materials and all other information, identified as
 * confidential and provided to you by AMD shall be kept confidential in
 * accordance with the terms and conditions of the Software License Agreement.
 *
 * LIMITATION OF LIABILITY: THE MATERIALS AND ANY OTHER RELATED INFORMATION
 * PROVIDED TO YOU BY AMD ARE PROVIDED "AS IS" WITHOUT ANY EXPRESS OR IMPLIED
 * WARRANTY OF ANY KIND, INCLUDING BUT NOT LIMITED TO WARRANTIES OF
 * MERCHANTABILITY, NONINFRINGEMENT, TITLE, FITNESS FOR ANY PARTICULAR PURPOSE,
 * OR WARRANTIES ARISING FROM CONDUCT, COURSE OF DEALING, OR USAGE OF TRADE.
 * IN NO EVENT SHALL AMD OR ITS LICENSORS BE LIABLE FOR ANY DAMAGES WHATSOEVER
 * (INCLUDING, WITHOUT LIMITATION, DAMAGES FOR LOSS OF PROFITS, BUSINESS
 * INTERRUPTION, OR LOSS OF INFORMATION) ARISING OUT OF AMD'S NEGLIGENCE,
 * GROSS NEGLIGENCE, THE USE OF OR INABILITY TO USE THE MATERIALS OR ANY OTHER
 * RELATED INFORMATION PROVIDED TO YOU BY AMD, EVEN IF AMD HAS BEEN ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGES.  BECAUSE SOME JURISDICTIONS PROHIBIT THE
 * EXCLUSION OR LIMITATION OF LIABILITY FOR CONSEQUENTIAL OR INCIDENTAL DAMAGES,
 * THE ABOVE LIMITATION MAY NOT APPLY TO YOU.
 *
 * AMD does not assume any responsibility for any errors which may appear in
 * the Materials or any other related information provided to you by AMD, or
 * result from use of the Materials or any related information.
 *
 * You agree that you will not reverse engineer or decompile the Materials.
 *
 * NO SUPPORT OBLIGATION: AMD is not obligated to furnish, support, or make any
 * further information, software, technical information, know-how, or show-how
 * available to you.  Additionally, AMD retains the right to modify the
 * Materials at any time, without notice, and is not obligated to provide such
 * modified Materials to you.
 *
 * U.S. GOVERNMENT RESTRICTED RIGHTS: The Materials are provided with
 * "RESTRICTED RIGHTS." Use, duplication, or disclosure by the Government is
 * subject to the restrictions as set forth in FAR 52.227-14 and
 * DFAR252.227-7013, et seq., or its successor.  Use of the Materials by the
 * Government constitutes acknowledgement of AMD's proprietary rights in them.
 *
 * EXPORT ASSURANCE:  You agree and certify that neither the Materials, nor any
 * direct product thereof will be exported directly or indirectly, into any
 * country prohibited by the United States Export Administration Act and the
 * regulations thereunder, without the required authorization from the U.S.
 * government nor will be used for any purpose prohibited by the same.
 * </pre>
 ******************************************************************************/

#ifndef YAAP_HDT_CLASS_H
#define YAAP_HDT_CLASS_H

#include "classes/base.h"
#include "hal/hdt.h"
#include "hal/trigger.h"
#include "hal/header.h"

using namespace yaap;

namespace yaap {
namespace classes {

/******************************************************************************/
/*! The YAAP \a hdt class.
 * 
 * This class covers the CPU debug sideband signals.
 ******************************************************************************/
class hdt : public base
{
public:

    /*! Constructor.
     * 
     * \param[in] name    The name of the object.  HDT expects this to be "hdt".
     * \param[in] hdtHw   Pointer to the low-level HDT driver associated with this instance.
     * \param[in] trigHw  Pointer to the low-level debug module triggers driver associated with this instance (NULL for none).
     * \param[in] hdrHw   Pointer to the low-level header driver associated with this instance.
     */
    hdt(const char *name, hal::IHdt *hdtHw, hal::ITriggers *trigHw, hal::IHeader *hdrHw);

protected:

    //! YAAP Method: Get state of DBREQ signal.
    uint32_t yaap_ioDbreqGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set state of DBREQ signal.
    uint32_t yaap_ioDbreqSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the state of the DBRDY signals.
    uint32_t yaap_ioDbrdyGet(YAAP_METHOD_PARAMS);
 
    //! YAAP Method: Get a snapshot of the DBRDY values at the moment the first one asserted.
    uint32_t yaap_ioDbrdySnapshotGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Clear the last-captured DBRDY snapshot logic.
    uint32_t yaap_ioDbrdySnapshotClear(YAAP_METHOD_PARAMS);
 
    //! YAAP Method: Get the source of the last DBREQ assertion.
    uint32_t yaap_ioDbreqSourceSnapshotGet(YAAP_METHOD_PARAMS);
 
    //! YAAP Method: Clear the DBREQ source snapshot logic.
    uint32_t yaap_ioDbreqSourceSnapshotClear(YAAP_METHOD_PARAMS);
 
    //! YAAP Method: Get the state of the reset signal.
    uint32_t yaap_ioResetGet(YAAP_METHOD_PARAMS);
 
    //! YAAP Method: Get the state of the PWROK signal.
    uint32_t yaap_ioPwrokGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the state of a trigger on the debug module.
    uint32_t yaap_ioTriggerSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the state of a trigger on the debug module.
    uint32_t yaap_ioTriggerGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Configure the debug module DBREQ-on-DBRDY option.
    uint32_t yaap_optionDbreqOnDbrdySet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the current debug module DBREQ-on-DBRDY option setting.
    uint32_t yaap_optionDbreqOnDbrdyGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Configure the debug module DBREQ-on-Reset option.
    uint32_t yaap_optionDbreqOnResetSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the current debug module DBREQ-on-Reset option setting.
    uint32_t yaap_optionDbreqOnResetGet(YAAP_METHOD_PARAMS);
 
    //! YAAP Method: Configure the debug module DBREQ-on-Trigger option.
    uint32_t yaap_optionDbreqOnTriggerSet(YAAP_METHOD_PARAMS);
 
    //! YAAP Method: Get the current debug module DBREQ-on-Trigger option setting.
    uint32_t yaap_optionDbreqOnTriggerGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the DBREQ pulse width.
    uint32_t yaap_optionDbreqPulseWidthSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the DBREQ pulse width.
    uint32_t yaap_optionDbreqPulseWidthGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Set the DBRDY mask.
    uint32_t yaap_optionDbrdyMaskSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the DBRDY mask.
    uint32_t yaap_optionDbrdyMaskGet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the HW frequency.
    uint32_t yaap_optionFrequencyGet(YAAP_METHOD_PARAMS);   // !!! This should not be here in this class

    //! YAAP Method: Configure a debug module trigger.
    uint32_t yaap_optionTriggerSetupSet(YAAP_METHOD_PARAMS);

    //! YAAP Method: Get the current configuration of a debug module trigger.
    uint32_t yaap_optionTriggerSetupGet(YAAP_METHOD_PARAMS);
    
    //! YAAP Method: Get the header enabled status.
    uint32_t yaap_enableGet(YAAP_METHOD_PARAMS);            // !!! Code duplication; move this to a new YAAP header class
    
    //! YAAP Method: Set the header enabled status.
    uint32_t yaap_enableSet(YAAP_METHOD_PARAMS);            // !!! Code duplication; move this to a new YAAP header class
    
    //! YAAP Method: Get the header status.
    uint32_t yaap_headerStatusGet(YAAP_METHOD_PARAMS);      // !!! Code duplication; move this to a new YAAP header class
    
    //! YAAP Method: Clear the header changed flag.
    uint32_t yaap_headerStatusClear(YAAP_METHOD_PARAMS);    // !!! Code duplication; move this to a new YAAP header class 

    hal::IHeader *m_hdrHw;                              //!< The header HAL
    hal::IHeader_setEnabled *m_hdrEnable;               //!< Header enable/disable HAL
    hal::IHeader_changeDetect *m_hdrChange;             //!< Header change detect HAL
    
    hal::IHdt *m_hdtHw;                                 //!< The HDT HAL
    hal::IHdt_dbrdyMask *m_dbrdyMask;                   //!< DBRDY Mask HAL
    hal::IHdt_dbrdySnapshot *m_dbrdySnapshot;           //!< DBRDY snapshot HAL
    hal::IHdt_dbreqOnDbrdy *m_dmod;                     //!< DBREQ-on-DBRDY HAL
    hal::IHdt_dbreqOnReset *m_dbreqOnReset;             //!< DBREQ-on-reset HAL
    hal::IHdt_dbreqOnTrigger *m_dbreqOnTrig;            //!< DBREQ-on-trigger HAL
    hal::IHdt_dbreqPulseWidthSet *m_dbreqPulseWidth;    //!< DBREQ pulse width HAL
    hal::IHdt_dbreqSourceSnapshot *m_dbreqSnapshot;     //!< DBREQ snapshot HAL
    hal::IHdt_pwrokGet *m_pwrOkGet;                     //!< PWROK getter HAL
    hal::IHdt_resetGet *m_resetGet;                     //!< RESET_L getter HAL
    
    hal::ITriggers *m_triggersHw;                       //!< The Triggers HAL
    hal::ITriggers_manual *m_trigManual;                //!< Triggers manual control HAL

};

} // namespace classes
} // namespace yaap

#endif 

