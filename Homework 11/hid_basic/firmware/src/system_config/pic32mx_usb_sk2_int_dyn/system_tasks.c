#include "system_config.h"
#include "system_definitions.h"
#include "app.h"

void SYS_Tasks ( void )
{
    /* Maintain the state machines of all library modules executing polled in
    the system. */

    /* Maintain system services */
    SYS_DEVCON_Tasks(sysObj.sysDevcon);

    /* Maintain Device Drivers */

    /* Maintain USB Stack */
    /* Device layer tasks routine */ 
    USB_DEVICE_Tasks(sysObj.usbDevObject0);

    /* Maintain the application's state machine. */
    APP_Tasks();
}
