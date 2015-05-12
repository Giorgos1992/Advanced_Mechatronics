#ifdef WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "hidapi.h"

#define MAX_STR 255

int main(int argc, char* argv[])
{
	int res;
	unsigned char buf[65];
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;
	int iter;
	double Z[1000];
	short z[1000];
	
	
	for (i=0;i<65;i++)
		buf[i]=0; //clean buf array
	
	FILE *ofp;
	
 	// Initialize the hidapi library
	res = hid_init();
	
	// Open the device using the VID, PID,
	// and optionally the Serial number.
	handle = hid_open(0x4d8, 0x3f, NULL);

	// Read the Manufacturer String
	res = hid_get_manufacturer_string(handle, wstr, MAX_STR);
	wprintf(L"Manufacturer String: %s\n", wstr); 
	// Read the Product String
	res = hid_get_product_string(handle, wstr, MAX_STR);
	wprintf(L"Product String: %s\n", wstr);
	// Read the Serial Number String
	res = hid_get_serial_number_string(handle, wstr, MAX_STR);
	wprintf(L"Serial Number String: (%d) %s\n", wstr[0], wstr);
	// Read Indexed String 1
	res = hid_get_indexed_string(handle, 1, wstr, MAX_STR);
	wprintf(L"Indexed String 1: %s\n", wstr); 

	// Toggle LED (cmd 0x80). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = 0x01; //case 0x01 for the PIC to read the data - input by the user
	
	//User inputs message to be printed to the screen
	wprintf(L"Enter your message of max 25 characters\n");
	char c;
	i=2;
	while ((buf[i++]=getchar())!= '\n' && i < 27)
	;
	while (i<28){
	buf[i-1]=' ';
	i++;
	}
	res = hid_write(handle, buf, 65); 
	
	
	printf("Data collection started\n");
	for (iter = 0; iter < 50; iter++) //Each iteration saves data at 50kHz. 500 iterations take about 500*0.02 = 10 seconds
	{
		// Request state (cmd 0x81). The first byte is the report number (0x0).
		buf[0] = 0x0;
		buf[1] = 0x02; //case 0x02 in the PIC code (MPLAB IDE) -- the PIC should reply with (accelerometer) data
		res = hid_write(handle, buf, 65);
		res = hid_read(handle, buf, 65);
		while (buf[0]!=1) //waits until data are sent
		{;
		}
		//Convert char (1 byte) to shorts (2 bytes) --not using buf because it is a char
		z[iter] = (buf[2] << 8) | (buf[1] & 0xff);
		//Print the accelerations as FLOATS 
		/* printf("z accel is: %.2f\n", ((float)z[iter])/16000); */
		Z[iter] = (float)z[iter]/16000; 
	}
	printf("Data collection finished\n");
	
	ofp = fopen("accels.txt", "w");
	 for (i=0; i<50; i++)
	  fprintf(ofp,"%.3f\n",Z[i]);
  	fclose(ofp); 
	
	
	/*
	//Convert char (1 byte) to shorts (2 bytes) --not using buf because it is a char
	short x,y,z;
	x = (buf[4] << 8) | (buf[3] & 0xff);
	y = (buf[6] << 8) | (buf[5] & 0xff);
	z = (buf[8] << 8) | (buf [7] & 0xff);
		//Print the accelerations as FLOATS 
	printf("x accel is: %.2f\ny accel is: %.2f\nz accel is: %.2f\n", ((float)x)/16000,((float)y)/16000,((float)z)/16000);
	double xx,yy,zz;
	xx = (float)x/16000;
	yy = (float)y/16000;
	zz = (float)z/16000;
	ofp = fopen("accels.txt", "w");
	 for (i=0; i<10; i++)
	  fprintf(ofp,"%.3f\t%.3f\t%.3f\n",xx,yy,zz);
  	fclose(ofp); 
     
	 */
	// Finalize the hidapi library
	res = hid_exit(); 

	return 0;
}
