/**************************************************************************/
/*! \file hal/header.h HAL definition for "virtual" connector.
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

#ifndef HAL_HEADER_H
#define HAL_HEADER_H

#include <stdint.h>
#include "hal/errs.h"

using namespace std;

namespace yaap {
namespace hal {

/*******************************************************************************/
/*! This interface provides access to single header or target connection.
 * 
 * It is to be used even if there is no way to disconnect the connection; HDT uses 
 * it to manage the target connection.
 * 
 * This class includes some optional extensions, which are defined in their own
 * classes (prefixed with \a IHeader_).  If the hardware implements any of those
 * optional extensions, the associated interfaces must also be implemented by the class
 * implementing \a IHeader.
 * 
 * \see IHeader_changeDetect, IHeader_setEnabled
 *******************************************************************************/
class IHeader
{
public:

    static const char *HEADER_MODE_HDT;             //!< HDT header (black) CPU JTAG mode
    static const char *HEADER_MODE_MPHDT;           //!< MPHDT and HDT headers (both black) CPU JTAG mode
    static const char *HEADER_MODE_HDT_PLUS;        //!< HDT+ header (blue) CPU/APU JTAG mode
    static const char *HEADER_MODE_CSJTAG_NB;       //!< CSJTAG header (red) NB mode
    static const char *HEADER_MODE_CSJTAG_SB;       //!< CSJTAG header (red) SB mode
    static const char *HEADER_MODE_CSJTAG_GPU_JTAG; //!< CSJTAG header (red) GPU JTAG mode
    static const char *HEADER_MODE_CSJTAG_GPU_SCAN; //!< CSJTAG header (red) GPU scan mode
    static const char *HEADER_MODE_CSJTAG_I2C;      //!< CSJTAG header (red) I2C mode

    /*! Check whether a connection to the target is sensed.
     * 
     * This method should indicate that the target is connected if the target is connected and powered.  
     * This may be done by checking for voltage on VDDIO.  If connected, it must also return the type of
     * connection using a predefined string, i.e. #HEADER_MODE_HDT_PLUS, #HEADER_MODE_CSJTAG_NB, etc.
     * 
     * \param[out] connected On return, shall indicate whether a target connection was sensed on this interface.
     * \param[out] mode If \a connected is \b true, shall indicate the current connection mode on return.
     * 
     * The CPU and chipset header modes that HDT recognizes (i.e. in the \a mode parameter) are:
     *    - #HEADER_MODE_HDT             - Legacy (black) HDT connector (use #HEADER_MODE_HDT)
     *    - #HEADER_MODE_MPHDT           - Both legacy (black) HDT connecter and MPHDT connector (use #HEADER_MODE_MPHDT)
     *    - #HEADER_MODE_HDT_PLUS        - HDT+ (blue) HDT connector (use #HEADER_MODE_HDT_PLUS)
     *    - #HEADER_MODE_CSJTAG_NB       - Discrete northbridge JTAG connection (use #HEADER_MODE_CSJTAG_NB)
     *    - #HEADER_MODE_CSJTAG_SB       - Discrete southbridge JTAG connection (use #HEADER_MODE_CSJTAG_SB)
     *    - #HEADER_MODE_CSJTAG_GPU_JTAG - Discrete GPU JTAG connection (use #HEADER_MODE_CSJTAG_GPU_JTAG)
     * 
     * \note If the header is in a conflicted state, it should be reported as not connected.
     * 
     * \retval #E_SUCCESS Call succeeded.
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */            
    virtual uint32_t isConnected(bool& connected, const char *& mode) = 0;
    
    /*! Check if the header is currently enabled (i.e. pins are actively driven).
     * 
     * A disabled header may be tristated to help avoid "hot plug" problems when connecting and disconnecting
     * cables.  If an implementation does not support enabling or disabling of the header, this method
     * must always set \a enabled to true.
     * 
     * \param[out] enabled On return, shall indicate whether the header is enabled (not tristated).
     * 
     * \retval #E_SUCCESS Call succeeded.
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */
    virtual uint32_t isEnabled(bool& enabled) = 0;
    
    /*! Check whether a conflict is detected on this header.
     * 
     * If a header supports multiple connection modes, it may be able to detect conflicting connections.
     * If a header is in conflict, it must report itself as not connected, and can set \a conflicted to
     * \a true when this method is called.  If an implementation does not support multiple connection modes, 
     * this method must always return \a false.
     * 
     * \param[out] conflicted On return, shall indicate whether the header is currently in conflict.
     * 
     * \retval #E_SUCCESS Call succeeded.
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     */            
    virtual uint32_t isInConflict(bool& conflicted) = 0;
};

/******************************************************************************/
/*! Interface for header enable/disable hardware.
 * 
 * If the hardware supports the disabling and enabling of a header,
 * then this interface should be implemented by same class implementing IHeader.
 ******************************************************************************/
class IHeader_setEnabled
{
  public:

    /*! Enable or disable (tristate) the header.
     * 
     * \param[in] enabled Indicates whether the connector should be enabled or disabled.
     * 
     * \retval #E_SUCCESS Call succeeded.
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * 
     * \see IHeader::getEnabled
     */
    virtual uint32_t setEnabled(bool enabled) = 0;
};

/******************************************************************************/
/*! Interface for header change detection hardware.
 * 
 * If the hardware supports detection of header status changes,
 * then this interface should be implemented by same class implementing IHeader.
 ******************************************************************************/
class IHeader_changeDetect
{
  public:

    /*! Check whether a change has been detected on the header since the last call to \a clearChanged().
     * 
     * This is part of the header change detection logic.  The purpose of this is to warn users when an event likely to
     * have changed significant target state occurs, such as power-cycle, cable unplug/plug, or reset.  The YAAP code
     * will call this method to determine if such a change has been detected, and if so will return an error for HDT to handle.
     * 
     * \param[out] changed On return, shall indicate whether the header status has changed (i.e. power has cycled or reset
     *                     has occurred) since the last call to clearChanged().
     * 
     * \retval #E_SUCCESS Call succeeded.
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * 
     * \see clearChanged()
     */            
    virtual uint32_t isChanged(bool& changed) = 0;
    
    /*! Clear the header-changed flag which is returned by \a isChanged().
     * 
     * \retval #E_SUCCESS Call succeeded.
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     * 
     * \see isChanged()
     */            
    virtual uint32_t clearChanged(void) = 0;
};

/******************************************************************************/
/*! Interface for controlling TRST_L
 * 
 * If the hardware supports a setting/getting TRST_L, then this 
 * interface should be implemented by the same class implementing IJtag.
 ******************************************************************************/
class IHeader_trst
{
  public:
  
    /*! Sets (or clears) TRST_L
     *
     * \param[in] assert true asserts TRSL_L (by driving it low), false deasserts it.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see getTrst()
     */
    virtual uint32_t trstSet(bool assert) = 0;

    /*! Gets the TRST_L setting.
     *
     * \param[out] asserted On return, shall be true if TRST_L is asserted (low),
     *                      false if TRST_L is deasserted.
     *
     * \retval #E_SUCCESS Call succeeded
     * \retval #E_UNSPECIFIED_HW_ERROR Unspecified low-level error encountered
     *
     * \see trstGet()
     */
    virtual uint32_t trstGet(bool& assert) = 0;
};

} // namespace hal
} // namespace yaap

#endif // HAL_HEADER_H
