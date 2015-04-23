#include <xc.h>
#include <sys/attribs.h>
#include "app.h"
#include "system_definitions.h"

void __ISR(_USB_1_VECTOR, ipl4) _IntHandlerUSBInstance0(void)
{
    USB_DEVICE_Tasks_ISR(sysObj.usbDevObject0);            
}

