#include "mouse.h"

// *****************************************************************************
/* Function:
    void MOUSE_ReportCreate
    (
        MOUSE_COORDINATE x, 
        MOUSE_COORDINATE y,
        MOUSE_BUTTON_STATE * buttonArray,
        MOUSE_REPORT * mouseReport
    )

  Summary:
    This function creates a mouse report.

  Description:
    This function creates a mouse report. The application can use this function 
    to create the mouse report. The report can then be sent to the host by using
    the USB_DEVICE_HID_ReportSend() function.

  Precondition:
    None.

  Parameters:
    x - Mouse X Coordinate

    y - Mouse Y Coordinate

    buttonArray - Pointer to an array of button states. Size of the array is
    defined by USB_HID_MOUSE_BUTTON_NUBMERS.
    
    mouseReport - Output only variable that will contain the mouse report.

  Returns:
    None.

  Example:
    <code>
    // This example shows how to create and send the mouse report. Assume
    // that device is configured. Number of mouse buttons is 3. Assume that
    // this mouse is associated with HID Device Instance 0.

	MOUSE_REPORT mouseReport;
    MOUSE_BUTTON_STATE mouseButtons[USB_HID_MOUSE_BUTTON_NUMBERS];
    MOUSE_COORDINATE xCoordinate;
    MOUSE_COORDINATE yCoordinate;
    USB_DEVICE_HID_TRANSFER_HANDLE transferHandle;
    USB_DEVICE_HID_RESULT result;

    // Update the mouse state

    xCoordinate = someXCoordinate;
    yCoordinate = someYCoordinate;
    mouseButtons[0] = USB_HID_MOUSE_BUTTON_STATE_PRESSED;
    mouseButtons[1] = USB_HID_MOUSE_BUTTON_STATE_RELEASED;
    mouseButtons[2] = USB_HID_MOUSE_BUTTON_STATE_RELEASED;

    // Create the report.

    MOUSE_ReportCreate( xCoordinate, yCoordinate, 
        mouseButtons, &mouseReport);

    // Now send the report
  
    result = USB_DEVICE_HID_ReportSend( 0, &transferHandle,  
               (uint8_t *)mouseReport, sizeof(USB_HID_MOUSE_REPORT));

    if(USB_DEVICE_HID_RESULT_OK != result)
    {
        // Do error handling here.
    } 

    // The completion of the report send is indicated by the 
    // USB_DEVICE_HID_EVENT_REPORT_SEND_COMPLETE event.

    </code>

  Remarks:
    None.
*/

void MOUSE_ReportCreate
(
    MOUSE_COORDINATE x,
    MOUSE_COORDINATE y,
    MOUSE_BUTTON_STATE * buttonArray,
    MOUSE_REPORT * mouseReport
)
{
    int index;

    /* Initialize the mouse buttons byte */
    mouseReport->data[0] = 0;

    for (index = 0; index < MOUSE_BUTTON_NUMBERS; index ++)
    {
        /* Create the mouse button bit map */
        mouseReport->data[0] |= buttonArray[index];
    }

    /* Update the x and y co-ordinate */
	mouseReport->data[1] = x;
	mouseReport->data[2] = y;

	return;	
}


