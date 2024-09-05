/******************************************************************************/
/*! \file bmc/hw/hdt_hw.h  HDT HW driver
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
 * AMD Confidential Proprietary.
 * </pre>
 ******************************************************************************/

#ifndef BMC_HDT_HW_H
#define BMC_HDT_HW_H

#include "aspeed.h"
#include "hal/hdt.h"

using namespace yaap::hal;
namespace bmc
{

class HdtHw : public IHdt, public IHdt_dbrdyMask, public IHdt_dbrdySnapshot, public IHdt_dbreqOnDbrdy, public IHdt_dbreqOnReset, public IHdt_dbreqOnTrigger, public IHdt_dbreqPulseWidthSet, public IHdt_dbreqSourceSnapshot, public IHdt_pwrokGet, public IHdt_resetGet
{
   private:
    bool m_dbreq;
    bool m_dbreqOnDbrdy;
    bool m_dbreqOnReset;
    bool m_dbreqOnTrigger;
    enum yaap::hal::triggerSource m_dbreqTrigger;
    bool m_dbreqOnTriggerInvert;
    enum yaap::hal::triggerEvent m_dbreqTriggerEvent;
    uint32_t m_dbreqPulseWidth;
    uint8_t m_dbrdyMask;
    int fd;

    uint32_t m_clksPerUsec;

   public:
    HdtHw(int driver);
    uint32_t dbreqGet(bool& asserted);
    uint32_t dbreqSet(bool asserted);
    uint32_t dbrdyGet(uint8_t& asserted);
    uint32_t dbrdySnapshotGet(uint8_t& asserted);
    uint32_t dbrdySnapshotClear(void);
    uint32_t dbreqSourceSnapshotGet(enum yaap::hal::dbreqSource& source);
    uint32_t dbreqSourceSnapshotClear(void);
    uint32_t resetGet(bool& asserted);
    uint32_t pwrokGet(bool& asserted);
    uint32_t dbreqOnDbrdySet(bool enabled);
    uint32_t dbreqOnDbrdyGet(bool& enabled);
    uint32_t dbreqOnResetSet(bool enabled);
    uint32_t dbreqOnResetGet(bool& enabled);
    uint32_t dbreqOnTriggerSet(bool enabled, enum yaap::hal::triggerSource source, bool invert, enum yaap::hal::triggerEvent event);
    uint32_t dbreqOnTriggerGet(bool& enabled, enum yaap::hal::triggerSource& source, bool& invert, enum yaap::hal::triggerEvent& event);
    uint32_t dbreqPulseWidthSet(uint32_t& usec);
    uint32_t dbreqPulseWidthGet(uint32_t& usec);
    uint32_t dbrdyMaskSet(uint8_t enabled);
    uint32_t dbrdyMaskGet(uint8_t& enabled);
};
}  // namespace bmc
#endif  // BMC_HDT_HW_H
