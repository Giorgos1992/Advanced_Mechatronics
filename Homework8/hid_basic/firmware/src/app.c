#include "system_definitions.h"
#include "app.h"
#include "accel.h" //For accelerometer reading

 char Message[50];
 int i;
 short accels[3];

/* Recieve data buffer */
uint8_t receiveDataBuffer[64] APP_MAKE_BUFFER_DMA_READY;

/* Transmit data buffer */
uint8_t  transmitDataBuffer[64] APP_MAKE_BUFFER_DMA_READY;

APP_DATA appData;

USB_DEVICE_HID_EVENT_RESPONSE APP_USBDeviceHIDEventHandler
(
    USB_DEVICE_HID_INDEX iHID,
    USB_DEVICE_HID_EVENT event,
    void * eventData,
    uintptr_t userData
)
{
    USB_DEVICE_HID_EVENT_DATA_REPORT_SENT * reportSent;
    USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED * reportReceived;

    /* Check type of event */
    switch (event)
    {
        case USB_DEVICE_HID_EVENT_REPORT_SENT:

            /* The eventData parameter will be USB_DEVICE_HID_EVENT_REPORT_SENT
             * pointer type containing details about the report that was
             * sent. */
            reportSent = (USB_DEVICE_HID_EVENT_DATA_REPORT_SENT *) eventData;
            if(reportSent->handle == appData.txTransferHandle )
            {
                // Transfer progressed.
                appData.hidDataTransmitted = true;
            }

            break;

        case USB_DEVICE_HID_EVENT_REPORT_RECEIVED:

            /* The eventData parameter will be USB_DEVICE_HID_EVENT_REPORT_RECEIVED
             * pointer type containing details about the report that was
             * received. */

            reportReceived = (USB_DEVICE_HID_EVENT_DATA_REPORT_RECEIVED *) eventData;
            if(reportReceived->handle == appData.rxTransferHandle )
            {
                // Transfer progressed.
                appData.hidDataReceived = true;
            }

            break;

        case USB_DEVICE_HID_EVENT_SET_IDLE:

            /* For now we just accept this request as is. We acknowledge
             * this request using the USB_DEVICE_HID_ControlStatus()
             * function with a USB_DEVICE_CONTROL_STATUS_OK flag */

            USB_DEVICE_ControlStatus(appData.usbDevHandle, USB_DEVICE_CONTROL_STATUS_OK);

            /* Save Idle rate recieved from Host */
            appData.idleRate = ((USB_DEVICE_HID_EVENT_DATA_SET_IDLE*)eventData)->duration;
            break;

        case USB_DEVICE_HID_EVENT_GET_IDLE:

            /* Host is requesting for Idle rate. Now send the Idle rate */
            USB_DEVICE_ControlSend(appData.usbDevHandle, & (appData.idleRate),1);

            /* On successfully reciveing Idle rate, the Host would acknowledge back with a
               Zero Length packet. The HID function drvier returns an event
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT to the application upon
               receiving this Zero Length packet from Host.
               USB_DEVICE_HID_EVENT_CONTROL_TRANSFER_DATA_SENT event indicates this control transfer
               event is complete */

            break;
        default:
            // Nothing to do.
            break;
    }
    return USB_DEVICE_HID_EVENT_RESPONSE_NONE;
}

void APP_USBDeviceEventHandler(USB_DEVICE_EVENT event, void * eventData, uintptr_t context)
{
    switch(event)
    {
        case USB_DEVICE_EVENT_RESET:
        case USB_DEVICE_EVENT_DECONFIGURED:

            /* Host has de configured the device or a bus reset has happened.
             * Device layer is going to de-initialize all function drivers.
             * Hence close handles to all function drivers (Only if they are
             * opened previously. */

            BSP_LEDOff  (APP_USB_LED_1);
            BSP_LEDOn  (APP_USB_LED_2);
//            BSP_LEDOff (APP_USB_LED_3);
            appData.deviceConfigured = false;
            appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            break;

        case USB_DEVICE_EVENT_CONFIGURED:
            /* Set the flag indicating device is configured. */
            appData.deviceConfigured = true;

            /* Save the other details for later use. */
            appData.configurationValue = ((USB_DEVICE_EVENT_DATA_CONFIGURED*)eventData)->configurationValue;

            /* Register application HID event handler */
            USB_DEVICE_HID_EventHandlerSet(USB_DEVICE_HID_INDEX_0, APP_USBDeviceHIDEventHandler, (uintptr_t)&appData);

            /* Update the LEDs */

            BSP_LEDOff (APP_USB_LED_1);
            BSP_LEDOff (APP_USB_LED_2);
//            BSP_LEDOn  (APP_USB_LED_3);



            break;

        case USB_DEVICE_EVENT_SUSPENDED:

            /* Switch on green and orange, switch off red */
            BSP_LEDOn (APP_USB_LED_1);
            BSP_LEDOff  (APP_USB_LED_2);
//            BSP_LEDOn  (APP_USB_LED_3);
            break;

        case USB_DEVICE_EVENT_POWER_DETECTED:

            /* VBUS was detected. We can attach the device */

            USB_DEVICE_Attach (appData.usbDevHandle);
            break;

        case USB_DEVICE_EVENT_POWER_REMOVED:

            /* VBUS is not available */
            USB_DEVICE_Detach(appData.usbDevHandle);
            break;

        /* These events are not used in this demo */
        case USB_DEVICE_EVENT_RESUMED:
        case USB_DEVICE_EVENT_ERROR:
        default:
            break;
    }
}

void APP_Initialize ( void )
{
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;

    appData.usbDevHandle = USB_DEVICE_HANDLE_INVALID;
    appData.deviceConfigured = false;
    appData.txTransferHandle = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID;
    appData.rxTransferHandle = USB_DEVICE_HID_TRANSFER_HANDLE_INVALID;
    appData.hidDataReceived = false;
    appData.hidDataTransmitted = true;
    appData.receiveDataBuffer = &receiveDataBuffer[0];
    appData.transmitDataBuffer = &transmitDataBuffer[0];
}

void APP_Tasks (void )
{

    /* Check if device is configured.  See if it is configured with correct
     * configuration value  */

    switch(appData.state)
    {
        case APP_STATE_INIT:

            /* Open the device layer */
            appData.usbDevHandle = USB_DEVICE_Open( USB_DEVICE_INDEX_0, DRV_IO_INTENT_READWRITE );

            if(appData.usbDevHandle != USB_DEVICE_HANDLE_INVALID)
            {
                /* Register a callback with device layer to get event notification (for end point 0) */
                USB_DEVICE_EventHandlerSet(appData.usbDevHandle, APP_USBDeviceEventHandler, 0);

                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else
            {
                /* The Device Layer is not ready to be opened. We should try
                 * again later. */
            }

            break;

        case APP_STATE_WAIT_FOR_CONFIGURATION:

            if(appData.deviceConfigured == true)
            {
                /* Device is ready to run the main task */
                appData.hidDataReceived = false;
                appData.hidDataTransmitted = true;
                appData.state = APP_STATE_MAIN_TASK;

                /* Place a new read request. */
                USB_DEVICE_HID_ReportReceive (USB_DEVICE_HID_INDEX_0,
                        &appData.rxTransferHandle, appData.receiveDataBuffer, 64);
            }
            break;

        case APP_STATE_MAIN_TASK:

            if(!appData.deviceConfigured)
            {
                /* Device is not configured */
                appData.state = APP_STATE_WAIT_FOR_CONFIGURATION;
            }
            else if( appData.hidDataReceived )
            {
                /* Look at the data the host sent, to see what
                 * kind of application specific command it sent. */

                switch(appData.receiveDataBuffer[0])
                {
                    case 0x80:

                        /* Toggle on board LED1 to LED2. */
                        BSP_LEDToggle( APP_USB_LED_1 );
                        BSP_LEDToggle( APP_USB_LED_2 );

                        /*The following code block is added by the user (Giorgos).
                         * It prints
                         a message input by the user at a row specified by the user in the first 1-2 spaces of the message:
                         e.g. "12Hello world" will print "Hello world" on the 12th row  */

                        // <editor-fold defaultstate="collapsed" desc="Prints message on user-specified row">
//                        writemessage("Giorgos",28,32);
                        /*  If the row # > 9, two integers are sent in the array.
                            In the future, I can safe test to make sure rows fall within the 64 range   */

                        int row = appData.receiveDataBuffer[1] - '0';
                        if (appData.receiveDataBuffer[2]>= '0' && appData.receiveDataBuffer[2] <= '9') //check if the next element is a number
                            {row = 10*row + (appData.receiveDataBuffer[2] - '0');
                            strcpy(Message, &appData.receiveDataBuffer[3]); //If the 2nd cell is a row number, copy the array from 3rd cell
                            }
                        else
                        {
                         strcpy(Message, &appData.receiveDataBuffer[2]); //If the 2nd cell is not a row number, copy the array from 2nd cell
                        }
                         //sprintf(Message, "%s",appData.receiveDataBuffer);
                        writemessage(Message,row,1);
                        display_draw();

                        /*Clear the message after 3 seconds - prepare the screen for next message*/
                        _CP0_SET_COUNT(0);
                        while (_CP0_GET_COUNT()<20000000)
                        {;} //Wait 1 second
                         display_clear();
                        // </editor-fold>

                        appData.hidDataReceived = false;

                        /* Place a new read request. */
                        USB_DEVICE_HID_ReportReceive (USB_DEVICE_HID_INDEX_0,
                                &appData.rxTransferHandle, appData.receiveDataBuffer, 64 );

                        break;

                    case 0x81:

                        if(appData.hidDataTransmitted)
                        {
                            /* Echo back to the host PC the command we are fulfilling in
                             * the first byte.  In this case, the Get Push-button State
                             * command. */

                            appData.transmitDataBuffer[0] = 0x81; //This is 129 in decimal, that is what buffer prints in the first cell

//                            if( BSP_SwitchStateGet(APP_USB_SWITCH_1) == BSP_SWITCH_STATE_PRESSED )
//                            {
//                                appData.transmitDataBuffer[1] = 0x00;
//                            }
//                            else
//                            {
//                                appData.transmitDataBuffer[1] = 0x01;
//                              }

                                  //User's added code *********************************************************
                                  acc_read_register(OUT_X_L_A, (unsigned char *) appData.transmitDataBuffer+3, 6);
//                               i=0;
//                                //The z-data should be saved on the 3rd-4th (1st iteration), 5th-6th (2nd iter) and so on cell
//                                acc_read_register(OUT_X_L_A, (unsigned char *) ((appData.transmitDataBuffer+3)+2*i), 6);
//                                //On 1st iter, x data are on 3rd-4th cell, y on 5th-6h, z on 7th-8th
//                                //Thus, copy/overwrite the z data over the x data
//                                appData.transmitDataBuffer[(3+2*i)]=appData.transmitDataBuffer[(3+2*i)+4];
//                                appData.transmitDataBuffer[(3+2*i)+1] = appData.transmitDataBuffer[(3+2*i)+5];
//                                //Erase y-data
//                                appData.transmitDataBuffer[(3+2*i)+2] = 0;
//                                appData.transmitDataBuffer[(3+2*i)+3] = 0;
                            
                            appData.hidDataTransmitted = false;
                            
                            /* Prepare the USB module to send the data packet to the host */
                            USB_DEVICE_HID_ReportSend (USB_DEVICE_HID_INDEX_0,
                                    &appData.txTransferHandle, appData.transmitDataBuffer, 64 );
                         

                            appData.hidDataReceived = false;

                            /* Place a new read request. */
                            USB_DEVICE_HID_ReportReceive (USB_DEVICE_HID_INDEX_0,
                                    &appData.rxTransferHandle, appData.receiveDataBuffer, 64 );
                        }
                        break;

                    default:

                        appData.hidDataReceived = false;

                        /* Place a new read request. */
                        USB_DEVICE_HID_ReportReceive (USB_DEVICE_HID_INDEX_0,
                                &appData.rxTransferHandle, appData.receiveDataBuffer, 64 );
                        break;
                }
            }
        case APP_STATE_ERROR:
            break;
        default:
            break;
    }
}