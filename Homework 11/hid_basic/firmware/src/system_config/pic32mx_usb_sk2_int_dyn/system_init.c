#include "system_config.h"
#include "system_definitions.h"
#include "app.h"

// <editor-fold defaultstate="collapsed" desc="DEVCFGs here">
/* ***********************DEVCFGs here ********************** */
// DEVCFG0
#pragma config DEBUG = OFF // no debugging
#pragma config JTAGEN = OFF // no jtag
#pragma config ICESEL = ICS_PGx1 // use PGED1 and PGEC1
#pragma config PWP = OFF // no write protect
#pragma config BWP = OFF // not boot write protect
#pragma config CP = OFF // no code protect
// DEVCFG1
#pragma config FNOSC = PRIPLL // use primary oscillator with pll
#pragma config FSOSCEN = OFF // turn off secondary oscillator
#pragma config IESO = OFF // no switching clocks
#pragma config POSCMOD = HS // high speed crystal mode
#pragma config OSCIOFNC = OFF // free up secondary osc pins by turning sosc off
#pragma config FPBDIV = DIV_1 // divide CPU freq by 1 for peripheral bus clock
#pragma config FCKSM = CSDCMD // do not enable clock switch
#pragma config WDTPS = PS1048576 // slowest wdt
#pragma config WINDIS = OFF // no wdt window
#pragma config FWDTEN = OFF // wdt off by default
#pragma config FWDTWINSZ = WINSZ_25 // wdt window at 25%
// DEVCFG2 - get the CPU clock to 40MHz
#pragma config FPLLIDIV = DIV_2 // divide input clock to be in range 4-5MHz:8MHz/2
#pragma config FPLLMUL = MUL_20 // multiply clock after FPLLIDIV:4*20 = 80MHz
#pragma config FPLLODIV = DIV_2 // divide clock after FPLLMUL to get 40MHz:80/2 = 40MHz
#pragma config UPLLIDIV = DIV_2 // divide 8MHz input clock, then multiply by 12 to get 48MHz for USB
#pragma config UPLLEN = ON // USB clock on
// DEVCFG3
#pragma config USERID = 0 // some 16bit userid, doesn't matter what
#pragma config PMDL1WAY = ON // allow only one reconfiguration
#pragma config IOL1WAY = ON // allow only one reconfiguration
#pragma config FUSBIDIO = OFF // USB pins controlled by USB module
#pragma config FVBUSONIO = OFF // controlled by USB module
// </editor-fold>

/****************************************************
 * Class specific descriptor - HID Report descriptor
 ****************************************************/

#define HID_RPT01_SIZE          28

const uint8_t hid_rpt0[] =
{
    0x06, 0x00, 0xFF,   // Usage Page = 0xFF00 (Vendor Defined Page 1)
    0x09, 0x01,             // Usage (Vendor Usage 1)
    0xA1, 0x01,             // Colsslection (Application)
    0x19, 0x01,             // Usage Minimum
    0x29, 0x40,             // Usage Maximum 	//64 input usages total (0x01 to 0x40)
    0x15, 0x01,             // Logical Minimum (data bytes in the report may have minimum value = 0x00)
    0x25, 0x40,      	    // Logical Maximum (data bytes in the report may have maximum value = 0x00FF = unsigned 255)
    0x75, 0x08,             // Report Size: 8-bit field size
    0x95, 0x40,             // Report Count: Make sixty-four 8-bit fields (the next time the parser hits an "Input", "Output", or "Feature" item)
    0x81, 0x00,             // Input (Data, Array, Abs): Instantiates input packet fields based on the above report size, count, logical min/max, and usage.
    0x19, 0x01,             // Usage Minimum
    0x29, 0x40,             // Usage Maximum 	//64 output usages total (0x01 to 0x40)
    0x91, 0x00,             // Output (Data, Array, Abs): Instantiates output packet fields.  Uses same report size and count as "Input" fields, since nothing new/different was specified to the parser since the "Input" item.
    0xC0                    // End Collection
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
 *  USB Device Desciptor for this demo
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
    0x04D8,                     // Vendor ID
    0x003F,                     // Product ID: Custom HID device demo
    0x0002,                     // Device release number in BCD format
    0x01,                       // Manufacturer string index
    0x02,                       // Product string index
    0x00,                       // Device serial number string index
    0x01                        // Number of possible configurations
};

/*******************************************
 *  Device Configuration Decriptor
 *******************************************/
const uint8_t fullSpeedConfigurationDescriptor1[] =
{
    /* Configuration Descriptor Header */
    0x09,			                        			// Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,                       // CONFIGURATION descriptor type
    0x29,0x00,            	                        	// Total length of data for this cfg
    1,                                                  // Number of interfaces in this cfg
    1,                                                  // Index value of this configuration
    0,                                                  // Configuration string index
    USB_ATTRIBUTE_DEFAULT | USB_ATTRIBUTE_SELF_POWERED, // Attributes, see usb_chapter_9.h
    50,                                                 // Max power consumption (2X mA)

    /* Interface Descriptor */
    0x09,						// Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,   // INTERFACE descriptor type
    0,                          // Interface Number
    0,                          // Alternate Setting Number
    2,                          // Number of endpoints in this intf
    USB_HID_CLASS_CODE,         // Class code
    0,     						// Subclass code
    0,     						// Protocol code
    0,                          // Interface string index

    /* HID Class-Specific Descriptor */

    0x09,			    			// Size of this descriptor in bytes
    USB_HID_DESCRIPTOR_TYPES_HID,   // HID descriptor type
    0x11,0x01,                      // HID Spec Release Number in BCD format (1.11)
    0x00,                           // Country Code (0x00 for Not supported)
    0x01,                           // Number of class descriptors, see usbcfg.h
    USB_HID_DESCRIPTOR_TYPES_REPORT,// Report descriptor type
    HID_RPT01_SIZE,0x00,            // Size of the report descriptor

    /* Endpoint Descriptor */

    0x07,
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    0x1 | USB_EP_DIRECTION_IN,      // EndpointAddress
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x08,0x00,                      // Size
    0x01,                           // Interval

    /* Endpoint Descriptor */

    0x07,
    USB_DESCRIPTOR_ENDPOINT,        // Endpoint Descriptor
    0x1 | USB_EP_DIRECTION_OUT,     // EndpointAddress
    USB_TRANSFER_TYPE_INTERRUPT,    // Attributes
    0x08,0x00,                      // Size
    0x01                            // Interval
};

/**************************************
 *  String descriptors.
 *************************************/

/* Language code string descriptor 0 */
const struct
{
    uint8_t bLength;    // Length of this descriptor.
    uint8_t bDscType;   // String Descritpor type
    uint16_t string[1]; // String
}
sd000 =
{
    sizeof(sd000),
    USB_DESCRIPTOR_STRING,
    {0x0409 }
};

/* Manufacturer string descriptor 1*/
const struct
{
    uint8_t bLength;    // Length of this descriptor.
    uint8_t bDscType;   // String Descritpor type
    uint16_t string[25];// String
}
sd001 =
{
    sizeof(sd001),
    USB_DESCRIPTOR_STRING,
    {'M','i','c','r','o','c','h','i','p',' ',
    'T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'}
};

/* Product string descriptor 2 */
const struct
{
    uint8_t bLength;    // Length of this descriptor.
    uint8_t bDscType;   // String Descritpor type
    uint16_t string[22];// String
}
sd002 =
{
    sizeof(sd002),
    USB_DESCRIPTOR_STRING,
    {'S','i','m','p','l','e',' ','H','I','D',' ',
    'D','e','v','i','c','e',' ','D','e','m','o' }
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
    fullSpeedConfigurationDescriptor1
};


/*******************************************
 * USB Device Layer Master Descriptor Table 
 *******************************************/
const USB_DEVICE_MASTER_DESCRIPTOR usbMasterDescriptor =
{
    &fullSpeedDeviceDescriptor,     /* Full speed descriptor */
    1,                              /* Total number of full speed configurations available */
    &fullSpeedConfigDescSet[0],     /* Pointer to array of full speed configurations descriptors*/

    NULL,                           /* High speed device desc is not supported*/
    0,                              /* Total number of high speed configurations available */
    NULL,                           /* Pointer to array of high speed configurations descriptors. Not supported*/

    3,                              /* Total number of string descriptors available */
    stringDescriptors,              /* Pointer to array of string descriptors */

    NULL,                           /* Pointer to full speed dev qualifier. Not supported */
    NULL,                           /* Pointer to high speed dev qualifier. Not supported */
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


/* Structure to hold the object handles for the modules in the system. */
SYSTEM_OBJECTS sysObj;

/*** System Device Control Initialization Data ***/

const SYS_DEVCON_INIT sysDevconInit =
{
    .moduleInit = {0},
};


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

    //Initialize the screen

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

