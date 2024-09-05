/**************************************************************************/
/*! \file trigger.h Debug module triggers HAL interface definition.
 *
 * <pre>
 * Copyright (C) 2011-2012, ADVANCED MICRO DEVICES, INC.  All Rights Reserved.
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
 **************************************************************************/

#ifndef HAL_TRIGGER_H
#define HAL_TRIGGER_H

#include <stdint.h>
#include "hal/errs.h"

using namespace std;

namespace yaap {
namespace hal {

//! Specifies the possible trigger sensitivities.
enum triggerEvent
{
    TRIG_EVENT_LEVEL = 0,       //!< Sensitive to level
    TRIG_EVENT_POSEDGE = 1,     //!< Sensitive to rising edge
    TRIG_EVENT_NEGEDGE = 2,     //!< Sensitive to falling edge
    TRIG_EVENT_BOTH_EDGES = 3,  //!< Sensitive to either edge
};

//! Specifies the possible trigger directions.
enum triggerDirection
{
    TRIG_INTPUT = 0,            //!< Input trigger
    TRIG_OUTPUT = 1,            //!< Output trigger
};

//! Specifies the possible trigger voltage levels.
enum triggerLevel
{
    TRIG_LEVEL_3_3V = 0,        //!< 3.3V
    TRIG_LEVEL_5V = 1,          //!< 5V
};

//! Specifies the possible trigger polarities.
enum triggerPolarity
{
    TRIG_POL_POSITIVE = 0,      //!< Positive polarity
    TRIG_POL_NEGATIVE = 1,      //!< Negative polarity
};

//! Specifies the possible trigger value sources when configured as an output trigger.
enum triggerOutputSrc
{
    TRIG_OUTPUT_DBRDY = 0,      //!< Trigger value reflects DBRDY
    TRIG_OUTPUT_MANUAL = 1,     //!< Trigger value is set manually
};

//! Carries configuration information for a trigger channel.
struct triggerConfig
{
    int channel;                          //!< Trigger channel
    enum triggerDirection direction;      //!< Trigger direction (input/output)
    enum triggerLevel voltage;            //!< Voltage level
    enum triggerPolarity inputPolarity;   //!< Polarity (input mode; only applicable with TRIG_EVENT_LEVEL)
    enum triggerPolarity outputPolarity;  //!< Polarity (output mode; only applicable with TRIG_EVENT_LEVEL)
    enum triggerOutputSrc outputSource;   //!< If output, the source of trigger value (manual or DBRDY)
    enum triggerEvent outputEvent;        //!< Output event type (edge/level)
    uint32_t outputPulseNsec;             //!< Output pulse width (nsec)
};

/******************************************************************************/
/*! This interface provides access to the trigger(s) on a debug module.
 *
 * A debug module may provide triggers for allowing connectivity between the 
 * system-under-test and other test equipment.  Such triggers are typically used
 * to interface DBREQ and DBRDY with equipment such as a logic analyzer.
 * 
 * This class includes some optional extensions, which are defined in their own
 * classes (prefixed with \a ITriggers_).  If the hardware implements any of those
 * optional extensions, the associated interfaces must also be implemented by the class
 * implementing \a ITriggers.
 * 
 * \see ITriggers_manual
 *******************************************************************************/
class ITriggers
{
 public: 

    /*! Configure a trigger.
     *
     * This method is used to configure a trigger.
     * 
     * \param[in] config Requested trigger configuration.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_HDT_INVALID_TRIGGER_CHAN The specified channel is invalid in this implementation
     * \retval #E_HDT_INVALID_TRIGGER_CFG The requested configuration is unsupported in this implementation
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see triggerSetupGet()
     */
    virtual uint32_t triggerSetupSet(struct triggerConfig& config) = 0;

    /*! Retrieve a trigger configuration.
     *
     * This method is used to read out the current configuration of a trigger.
     * 
     * \param[in] channel Trigger channel (0 = A, 1 = B).
     * \param[out] config On return, this struct shall be populated with the trigger configuration.
     *                    The callee may populate the structure passed-in, or may change the reference
     *                    to an already-populated structure.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_HDT_INVALID_TRIGGER_CHAN The specified channel is invalid in this implementation
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see triggerSetupSet()
     */
    virtual uint32_t triggerSetupGet(int channel, struct triggerConfig& config) = 0;

};

/******************************************************************************/
/*! Interface for manually controlling or observing a trigger.
 * 
 * If the debug device supports manual access to a trigger via YAAP, then this
 * interface should be implemented by the same class implementing ITriggers.
 ******************************************************************************/
class ITriggers_manual
{
  public:
  
    /*! Set the value of a manually-controlled output trigger.
     *
     * This method is used to control a trigger configured as a manually-controlled output
     * (i.e. ::TRIG_OUTPUT and ::TRIG_OUTPUT_MANUAL).  By passing in \b TRUE for \a asserted, the 
     * trigger will assert according to its current configuration (polarity, sensitivity, etc.).
     *
     * If the trigger is configured for ::TRIG_EVENT_LEVEL, \a assert = TRUE will set the output 
     * high for ::TRIG_POL_POSITIVE, and low for ::TRIG_POL_NEGATIVE; \a assert = FALSE will set it
     * the opposite.
     *
     * If the trigger is configured for edge output, only values of \a assert = TRUE have any effect.
     * 
     * If the trigger is configured for ::TRIG_EVENT_POSEDGE and the signal is currently low,
     * \a assert = TRUE will cause a rising edge, followed by a falling edge after the programmed 
     * pulse width.  If the signal is currently high, the call has no effect.
     *
     * If the trigger is configured for ::TRIG_EVENT_NEGEDGE and the signal is currently high,
     * \a assert = TRUE will cause a falling edge, followed by a rising edge after the programmed 
     * pulse width.  If the signal is currently low, the call has no effect.
     *
     * If the trigger is configured for ::TRIG_EVENT_BOTH_EDGES, \a assert = TRUE will cause the signal
     * to toggle, followed by another toggle after the programmed pulse width.  
     * 
     * \param[in] channel Trigger channel (0 = A, 1 = B).
     * \param[in] asserted Whether to assert or deassert the trigger.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_HDT_INVALID_TRIGGER_CHAN The specified channel is invalid in this implementation
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see ITrigger::triggerSetupSet()
     * \see ITrigger::triggerSetupGet()
     * \see triggerConfig::outputPulseMicroseconds
     */
    virtual uint32_t triggerSet(int channel, bool asserted) = 0;
    
    /*! Get the current state of a trigger.
     *
     * This method is used to read the current state of a trigger.  Note that the
     * current trigger configuration is used when determining whether a trigger is asserted
     * or not, so this method will always return FALSE for edge-sensitive triggers.
     * 
     * \param[in] channel Trigger channel (0 = A, 1 = B).
     * \param[out] asserted On return, shall indicate whether the trigger is currently asserted.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_HDT_INVALID_TRIGGER_CHAN The specified channel is invalid in this implementation
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see ITrigger::triggerSetupSet()
     * \see ITrigger::triggerSetupGet()
     */
    virtual uint32_t triggerGet(int channel, bool& asserted) = 0;
};

} // namespace hal
} // namespace yaap

#endif // HAL_TRIGGER_H
