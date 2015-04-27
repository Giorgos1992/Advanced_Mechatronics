#ifndef _SYSTEM_CONFIG_H
#define _SYSTEM_CONFIG_H
#define _PLIB_UNSUPPORTED

#include "bsp_config.h"

// *****************************************************************************
/* Clock System Service Configuration Options
*/
#define SYS_CLK_FREQ                        40000000ul
#define SYS_CLK_BUS_PERIPHERAL_1            40000000ul
#define SYS_CLK_CONFIG_PRIMARY_XTAL         8000000ul
#define SYS_CLK_CONFIG_SECONDARY_XTAL       32768ul

/*** Common System Service Configuration ***/
#define SYS_BUFFER  false
#define SYS_QUEUE   false

/*** Interrupt System Service Configuration ***/
#define SYS_INT                     true

/*** Ports System Service Configuration ***/
#define SYS_PORT_AD1PCFG        ~0xffff
#define SYS_PORT_CNPUE          0x0
#define SYS_PORT_CNEN           0x0

#define SYS_PORT_B_TRIS         0xfff8
#define SYS_PORT_B_ODC          0x0

/*** USB Driver Configuration ***/

/* Enables Device Support */
#define DRV_USB_DEVICE_SUPPORT      true

/* Enables Device Support */
#define DRV_USB_HOST_SUPPORT        false


/* Maximum USB driver instances */
#define DRV_USB_INSTANCES_NUMBER    1

/* Interrupt mode enabled */
#define DRV_USB_INTERRUPT_MODE      true

/* Number of Endpoints used */
#define DRV_USB_ENDPOINTS_NUMBER    2

/*** USB Device Stack Configuration ***/

/* Maximum device layer instances */
#define USB_DEVICE_INSTANCES_NUMBER     1

/* EP0 size in bytes */
#define USB_DEVICE_EP0_BUFFER_SIZE      8

/* Maximum instances of HID function driver */
#define USB_DEVICE_HID_INSTANCES_NUMBER     1

/* HID Transfer Queue Size for both read and
   write. Applicable to all instances of the
   function driver */
#define USB_DEVICE_HID_QUEUE_DEPTH_COMBINED 2

#define APP_MAKE_BUFFER_DMA_READY
#define APP_USB_LED_1 BSP_LED_1
#define APP_USB_LED_2 BSP_LED_2
//#define APP_USB_LED_3 BSP_LED_3
#define APP_USB_SWITCH_1 BSP_SWITCH_1

#endif // _SYSTEM_CONFIG_H
