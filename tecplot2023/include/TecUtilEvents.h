#include "MASTER.h"
#include "GLOBAL.h"
#include "tp360event_Exports.h"

#if defined LINKTOADDON
  #undef LINKTOADDON
#endif

#define LINKTOADDON extern "C" tp360event_API

/**
 * This is called when a registered timer fires.
 *
 * @par Limitation:
 *   Unix and Linux versions of Tecplot currently do not fire timer events when
 *   Tecplot is running in batch mode (with the -b flag). This behavior
 *   limitation is subject to change.
 *
 * @param ClientData
 *   Arbitrary client data.
 *
 * @return
 *   Return TRUE if the timer should be reinstated.   Return FALSE
 *   to stop subsequent callbacks.
 */
typedef Boolean_t (*AddOnTimerCallback_pf)(ArbParam_t ClientData);


/**
 * Adds a timer callback, a function which is called by Tecplot at a set
 * interval of time. The interval return time can be affected by the event 
 * processing in the UI and may waver. In general you should receive a 
 * callback no sooner than the prescribed timeout interval.
 *
 * @par Note:
 *   In your timer callback you must check to see if Tecplot is locked before
 *   calling any TecUtil functions, as illustrated in the source code example.
 *
 * @param Interval
 *   Timeout interval in milliseconds.
 *
 * @param ClientData
 *   This can be any 32-bit value and will be passed to the timer callback.
 *   Typically this is a pointer to a structure
 *
 * @param TimerCallback
 *   Function to be called at each timeout interval. This function should
 *   return TRUE to continue the timer, or FALSE to stop it.
 *
 * @return
 *   TRUE if the callback was successfully added, FALSE otherwise. This
 *   function returns FALSE only if Tecplot is unable to obtain a timer from
 *   the operating system.
 *
 * <FortranSyntax>
 *    INTEGER*4 FUNCTION TecUtilTimerAddCallback(
 *   &                   Interval,
 *   &                   ClientDataPtr,
 *   &                   TimerCallback)
 *    INTEGER*4          Interval
 *    POINTER            (ClientDataPtr, ClientData)
 *    EXTERNAL           TimerCallback
 * </FortranSyntax>
 *
 * <PythonSyntax>
 * </PythonSyntax>
 *
 *   Set up a timer with an interval of one second.
 *
 * @code
 *   static Boolean_t MyTimerCallback(ArbParam_t MyData)
 *   {
 *     if (!TecUtilLockIsOn())
 *       {
 *         // Tecplot isn't currently locked, so it's safe to call TecUtil functions
 *         TecUtilLockStart(AddOnID); // Lock Tecplot for ourselves
 *
 *         // ... Do processing with TecUtil functions
 *
 *         // Release Tecplot for other addons
 *         TecUtilLockFinish(AddOnID);
 *       }
 *     else
 *       {
 *         // Another add-on has locked Tecplot. It is NOT safe to
 *         // call any TecUtil functions, so do nothing.
 *       }
 *
 *     // Return TRUE to continue the timer, return FALSE to stop the timer.
 *     return TRUE;
 *   }
 *   // Make the above function called every second.
 *   TecUtilTimerAddCallback(1000,NULL,MyTimerCallback);
 * @endcode
 *
 * @par Note:
 *   For a complete example of an add-on which uses timers, see the timetest
 *   sample add-on.
 *
 * @sa TecUtilOnIdleQueueAddCallback()
 *
 * @ingroup Utilities
 *
 * #internalattributes exclude_fglue, exclude_sdkdoc, exclude_tcl
 */

LINKTOADDON Boolean_t TecUtilTimerAddCallback(
    uint32_t              Interval,
    ArbParam_t            ClientData,
    AddOnTimerCallback_pf Callback);
