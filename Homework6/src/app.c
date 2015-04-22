#include "app.h"

APP_DATA appData;
unsigned int i = 0;

void APP_Initialize ( void )
{
    /* Put the application into its initial state */
    appData.state = APP_STATE_COUNT;
}
void APP_Tasks ( void )
{
   /* Take appropriate action, depending on the current state. */
    switch (appData.state)
    {

        /* Application is stuck in a counting loop. */
        case APP_STATE_COUNT:
        {
            /* Keep incrementing the count if it's less than the blink delay */
            if (i < APP_LED_BLINK_DELAY)
            {
                /* Increment count. */
                i++;
            }
            else
            {
                /* If count is reached, switch states */
                appData.state = APP_STATE_BLINK_LED;
            }
            break;
        }

        /* Toggle the LED and switch back to the delay loop. */
        case APP_STATE_BLINK_LED:
        {
            /* Toggle LED */
//            BSP_LEDToggle(BSP_LED_1);
            BSP_LEDToggle(7);
            /* Put the application back to the count state */
            appData.state = APP_STATE_COUNT;
            /* Restart count. */
            i = 0;
            break;
        }
        /* Should not come here during normal operation */
        default:
        {
            PLIB_ASSERT(false , "unknown application state");
            break;
        }
    }
}

