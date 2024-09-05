/******************************************************************************/
/*! \file bmc/hw/triggers_hw.h BMC triggers HW driver
 *
 * <pre>
 * Copyright (C) 2011-2020, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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

#ifndef BMC_TRIGGERS_HW_H
#define BMC_TRIGGERS_HW_H

#include "hal/trigger.h"
#include <map>

using namespace yaap::hal;
namespace bmc
{
class TriggersHw : public ITriggers, public ITriggers_manual
{
   private:
    map<int, struct yaap::hal::triggerConfig> m_triggers;
    map<int, bool> m_asserted;

    int m_channel;                          //!< Trigger channel
    enum triggerDirection m_direction;      //!< Trigger direction (input/output)
    enum triggerLevel m_voltage;            //!< Voltage level
    enum triggerPolarity m_inputPolarity;   //!< Polarity (input mode; only applicable with TRIG_EVENT_LEVEL)
    enum triggerPolarity m_outputPolarity;  //!< Polarity (output mode; only applicable with TRIG_EVENT_LEVEL)
    enum triggerOutputSrc m_outputSource;   //!< If output, the source of trigger value (manual or DBRDY)
    enum triggerEvent m_outputEvent;        //!< Output event type (edge/level)

    uint32_t m_busFreqHz;
    uint32_t m_nsecPerClk;

    const static int m_numTrigCh = 2;

   public:
    TriggersHw();
    uint32_t triggerSetupSet(struct yaap::hal::triggerConfig& config);
    uint32_t triggerSetupGet(int channel, struct yaap::hal::triggerConfig& config);
    uint32_t triggerSet(int channel, bool asserted);
    uint32_t triggerGet(int channel, bool& asserted);

    uint32_t m_PulseWidth;
};
}  // namespace bmc
#endif  // BMC_TRIGGERS_HW_H
