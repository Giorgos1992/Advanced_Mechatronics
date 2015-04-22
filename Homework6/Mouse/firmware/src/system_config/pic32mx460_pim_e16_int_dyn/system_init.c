/*******************************************************************************
  System Initialization File

  File Name:
    system_init.c

  Summary:
    This file contains source code necessary to initialize the system.

  Description:
    This file contains source code necessary to initialize the system.  It
    implements the "SYS_Initialize" function, configuration bits, and allocates
    any necessary global system resources, such as the systemObjects structure
    that contains the object handles to all the MPLAB Harmony module objects in
    the system.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED AS IS WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include "system_config.h"
#include "system_definitions.h"
#include "app.h"


// ****************************************************************************
// ****************************************************************************
// Section: Configuration Bits
// ****************************************************************************
// ****************************************************************************

/*** DEVCFG0 ***/

#pragma config DEBUG =      OFF
#pragma config ICESEL =     ICS_PGx2
#pragma config PWP =        0xff
#pragma config BWP =        OFF
#pragma config CP =         OFF

/*** DEVCFG1 ***/

#pragma config FNOSC =      PRIPLL
#pragma config FSOSCEN =    OFF
#pragma config IESO =       OFF
#pragma config POSCMOD =    XT
#pragma config OSCIOFNC =   OFF
#pragma config FPBDIV =     DIV_1
#pragma config FCKSM =      CSDCMD
#pragma config WDTPS =      PS1048576
#pragma config FWDTEN =     OFF

/*** DEVCFG2 ***/

#pragma config FPLLIDIV =   DIV_2
#pragma config FPLLMUL =    MUL_15
#pragma config FPLLODIV =   DIV_1
#pragma config UPLLIDIV =   DIV_2
#pragma config UPLLEN =     ON

/*** DEVCFG3 ***/

#pragma config USERID =     0xffff


// *****************************************************************************
// *****************************************************************************
// Section: Library/Stack Initialization Data
// *****************************************************************************
// *****************************************************************************/


/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/
const uint8_t hid_rpt0[] =
{
   0x05, 0x01, /* Usage Page (Generic Desktop)        */
   0x09, 0x02, /* Usage (Mouse)                       */
   0xA1, 0x01, /* Collection (Application)            */
   0x09, 0x01, /* Usage (Pointer)                     */
   0xA1, 0x00, /* Collection (Physical)               */
   0x05, 0x09, /* Usage Page (Buttons)                */
   0x19, 0x01, /* Usage Minimum (01)                  */
   0x29, 0x03, /* Usage Maximum (03)                  */
   0x15, 0x00, /* Logical Minimum (0)                 */
   0x25, 0x01, /* Logical Maximum (1)                 */
   0x95, 0x03, /* Report Count (3)                    */
   0x75, 0x01, /* Report Size (1)                     */
   0x81, 0x02, /* Input (Data, Variable, Absolute)    */
   0x95, 0x01, /* Report Count (1)                    */
   0x75, 0x05, /* Report Size (5)                     */
   0x81, 0x01, /* Input (Constant)    ;5 bit padding  */
   0x05, 0x01, /* Usage Page (Generic Desktop)        */
   0x09, 0x30, /* Usage (X)                           */
   0x09, 0x31, /* Usage (Y)                           */
   0x15, 0x81, /* Logical Minimum (-127)              */
   0x25, 0x7F, /* Logical Maximum (127)               */
   0x75, 0x08, /* Report Size (8)                     */
   0x95, 0x02, /* Report Count (2)                    */
   0x81, 0x06, /* Input (Data, Variable, Relative)    */
   0xC0, 0xC0
};
/**************************************************
 * USB Device Function Driver Init Data
 **************************************************/
    const USB_DEVICE_HID_INIT hidInit0 = 
    {
        .hidReportDescriptorSize = sizeof(hid_rpt0),
        .hidReportDescriptor = &hid_rpt0,
        .queueSizeReportReceive = 1,
        .queueSizeReportSend = 1
    };
/**************************************************
 * USB Device Layer Function Driver Registration 
 * Table
 **************************************************/
const USB_DEVICE_FUNCTION_REGISTRATION_TABLE funcRegistrationTable[1] =
{ 
    /* Function 1 */
    { 
        .configurationValue = 1,    /* Configuration value */ 
        .funcDriverIndex = 0,       /* Function driver index */     
        .interfaceNumber = 0,       /* First interfaceNumber of this function */ 
        .numberOfInterfaces = 1,    /* Number of interfaces */
        .speed = USB_SPEED_FULL,    /* Function Speed */ 
        .driver = (void*)USB_DEVICE_HID_FUNCTION_DRIVER,    /* USB HID function data exposed to device layer */
        .funcDriverInit = (void*)&hidInit0,    /* Function driver init data*/
    },
};

/*******************************************
 * USB Device Layer Descriptors
 *******************************************/
/*******************************************
 *  USB Device Descriptor for this demo
 *******************************************/
const USB_DEVICE_DESCRIPTOR fullSpeedDeviceDescriptor =
{
    0x12,                       // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,      // DEVICE descriptor type
    0x0200,                     // USB Spec Release Number in BCD format
    0x00,                       // Class Code
    0x00,                       // Subclass code
    0x00,                       // Protocol code
    USB_DEVICE_EP0_BUFFER_SIZE, // Max packet size for EP0, see usb_config.h
    0x0458,                     // Vendor ID
    0x0000,                     // Product ID: Custom HID device demo
    0x0003,                     // Device release number in BCD format
    0x01,                       // Manufacturer string index
    0x02,                       // Product string index
    0x00,                       // Device serial number string index
    0x01                        // Number of possible configurations
};

/*******************************************
 *  Device Configuration Descriptor
 *******************************************/
const uint8_t fullSpeedConfigurationDescriptor[] =
{
    /* Configuration Descriptor */

    0x09,                                                // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                        // CONFIGURATION descriptor type
    0x22,0x00,                                           // Total length of data for this cfg
    1,                                                   // Number of interfaces in this cfg
    1,                                                   // Index value of this configuration
    0,                                                   // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED,  // Attributes, see usb_device.h
    50,                                                  // Max power consumption (2X mA)

    /* Interface Descriptor */

    0x09,                                            // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,                        // INTERFACE descriptor type
    0,                                               // Interface Number
    0,                                               // Alternate Setting Number
    1,                                               // Number of endpoints in this intf
    USB_HID_CLASS_CODE,                              // Class code
    USB_HID_SUBCLASS_CODE_BOOT_INTERFACE_SUBCLASS ,  // Subclass code
    USB_HID_PROTOCOL_CODE_MOUSE,                     // Protocol code
    0,                                               // Interface string index

    /* HID Class-Specific Descriptor */

    0x09,                           // Size of this descriptor in bytes
    USB_HID_DESCRIPTOR_TYPES_HID,   // HID descriptor type
    0x11, 0x01,                     // HID Spec Release Number in BCD format (1.11)
    0x00,                           // Country Code (0x00 for Not supported)
    0x1,                            // Number of class descriptors, see usbcfg.h
    USB_HID_DESCRIPTOR_TYPES_REPORT,// Report descriptor type
    0x32,0x00,                      // Size of the report descriptor

    /* Endpoint Descriptor */

    0x07,                           // Size of this descriptor
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    0x1 | USB_EP_DIRECTION_IN,      // EndpointAddress
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x40, 0x00,                     // size
    0x01                            // Interval
};

/**************************************
 *  String descriptors.
 *************************************/

/* Language code string descriptor 0 */
const struct
{
    uint8_t bLength;    // Length of this descriptor
    uint8_t bDscType;   // String type descriptor
    uint16_t string[1]; // String
}
sd000 =
{
    sizeof(sd000),
    USB_DESCRIPTOR_STRING,
    {0x0409 }
};

/* Manufacturer string descriptor 1 */
const struct
{
    uint8_t bLength;    // Length of this descriptor
    uint8_t bDscType;   // String type descriptor
    uint16_t string[25];// String
}
sd001 =
{
    sizeof(sd001),
    USB_DESCRIPTOR_STRING,
    {'M','i','c','r','o','c','h','i','p',' ',
    'T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.' }
};

/* Product string descriptor 2 */
const struct
{
    uint8_t bLength;    // Length of this descriptor
    uint8_t bDscType;   // String type descriptor
    uint16_t string[14];// String
}
sd002 =
{
    sizeof(sd002),
    USB_DESCRIPTOR_STRING,
    {'H','I','D',' ',
    'M','o','u','s','e',' ','D','e','m','o' }
};

/*************************************
 *  String descriptors.
 *************************************/
USB_DEVICE_STRING_DESCRIPTORS_TABLE stringDescriptors[3]=
{
    (const uint8_t *const)&sd000,
    (const uint8_t *const)&sd001,
    (const uint8_t *const)&sd002
};

/*******************************************
 * Array of full speed config descriptors
 *******************************************/
USB_DEVICE_CONFIGURATION_DESCRIPTORS_TABLE fullSpeedConfigDescSet[]=
{
    fullSpeedConfigurationDescriptor
};



/*******************************************
 * USB Device Layer Master Descriptor Table 
 *******************************************/
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    &fullSpeedDeviceDescriptor, /* Full speed descriptor */
    1,                          /* Total number of full speed configurations available */
    &fullSpeedConfigDescSet[0], /* Pointer to array of full speed configurations descriptors*/

    NULL,                       /* High speed device desc is supported*/
    0,                          /* Total number of high speed configurations available */
    NULL,                       /* Pointer to array of high speed configurations descriptors. */

    3,                          /* Total number of string descriptors available */
    stringDescriptors,          /* Pointer to array of string descriptors */

    NULL,                       /* Pointer to full speed dev qualifier. Not supported */
    NULL,                       /* Pointer to high speed dev qualifier. Not supported */
};

/****************************************************
 * Endpoint Table needed by the Device Layer.
 ****************************************************/
uint8_t __attribute__((aligned(512))) endPointTable[USB_DEVICE_ENDPOINT_TABLE_SIZE];

/****************************************************
 * USB Device Layer Initialization Data
 ****************************************************/

const USB_DEVICE_INIT usbDevInitData =
{
    /* System module initialization */
    .moduleInit = {SYS_MODULE_POWER_RUN_FULL},

	/* Identifies peripheral (PLIB-level) ID */
    .usbID = USB_ID_1,

    /* Stop in idle */
    .stopInIdle = false,

    /* Suspend in sleep */
    .suspendInSleep = false,
    /* Interrupt Source for USB module */
    .interruptSource = INT_SOURCE_USB_1,

    /* Endpoint table */
    .endpointTable= endPointTable,

    /* Number of function drivers registered to this instance of the
       USB device layer */
    .registeredFuncCount = 1,

    /* Function driver table registered to this instance of the USB device layer*/
    .registeredFunctions = (USB_DEVICE_FUNCTION_REGISTRATION_TABLE*)funcRegistrationTable,

    /* Pointer to USB Descriptor structure */
    .usbMasterDescriptor = (USB_DEVICE_MASTER_DESCRIPTOR*)&usbMasterDescriptor,

    /* USB Device Speed */
    .deviceSpeed = USB_SPEED_FULL,


};



// *****************************************************************************
// *****************************************************************************
// Section: Driver Initialization Data
// *****************************************************************************
// *****************************************************************************




// *****************************************************************************
// *****************************************************************************
// Section: System Data
// *****************************************************************************
// *****************************************************************************

/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

// *****************************************************************************
// *****************************************************************************
// Section: Module Initialization Data
// *****************************************************************************
// *****************************************************************************


/*** System Device Control Initialization Data ***/

const SYS_DEVCON_INIT sysDevconInit =
{
    .moduleInit = {0},
};


// *****************************************************************************
// *****************************************************************************
// Section: Static Initialization Functions
// *****************************************************************************
// *****************************************************************************



// *****************************************************************************
// *****************************************************************************
// Section: System Initialization
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void SYS_Initialize ( SYS_INIT_DATA *data )

  Summary:
    Initializes the board, services, drivers, application and other modules.

  Remarks:
    See prototype in system/common/sys_module.h.
 */

void SYS_Initialize ( void* data )
{
    /* Core Processor Initialization */
    SYS_CLK_Initialize( NULL );
    sysObj.sysDevcon = SYS_DEVCON_Initialize(SYS_DEVCON_INDEX_0, (SYS_MODULE_INIT*)&sysDevconInit);
    SYS_DEVCON_PerformanceConfig(SYS_CLK_SystemFrequencyGet());
    SYS_DEVCON_JTAGDisable();

    /* Board Support Package Initialization */
    BSP_Initialize();

    /* System Services Initialization */    
    SYS_INT_Initialize();  
    SYS_PORTS_Initialize();

    /* Initialize Drivers */

    /* Initialize System Services */


    /* Initialize Middleware */
    /* Set priority of USB interrupt source */
    SYS_INT_VectorPrioritySet(INT_VECTOR_USB1, INT_PRIORITY_LEVEL4);

    /* Set Sub-priority of USB interrupt source */
    SYS_INT_VectorSubprioritySet(INT_VECTOR_USB1, INT_SUBPRIORITY_LEVEL0);


    /* Initialize the USB device layer */
    sysObj.usbDevObject0 = USB_DEVICE_Initialize (USB_DEVICE_INDEX_0 , ( SYS_MODULE_INIT* ) & usbDevInitData);

    /* Enable Global Interrupts */
    SYS_INT_Enable();


    /* Initialize the Application */
    APP_Initialize();
}

/*******************************************************************************
 End of File
*/

