/*******************************************************
 Windows HID simplification

 Alan Ott
 Signal 11 Software

 8/22/2009

 Copyright 2009
 
 This contents of this file may be used by anyone
 for any reason without any conditions and may be
 used as a starting point for your own applications
 which use HIDAPI.
********************************************************/

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
	unsigned char buf[27];
	wchar_t wstr[MAX_STR];
	hid_device *handle;
	int i;
	
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
	buf[1] = 0x80;
	
	//Write hello to buf
	printf("Enter your message of max 25 characters\n");
	char c;
	i=2;
	while ((buf[i++]=getchar())!= '\n' && i < 27)
	;
	while (i<28){
	buf[i-1]=' ';
	i++;
	}
	printf("%s", buf);
	
	res = hid_write(handle, buf, 65);

	// Request state (cmd 0x81). The first byte is the report number (0x0).
	buf[0] = 0x0;
	buf[1] = 0x81;
	res = hid_write(handle, buf, 65);

	//clean buf
	for (i=0;i<27;i++)
	buf[i]=0;
	
	
	// Read requested state
	res = hid_read(handle, buf, 65);

	//Convert char (1 byte) to shorts (2 bytes) --not using buf because it is a char
	short x,y,z;
	x = (buf[4] << 8) | (buf[3] & 0xff);
	y = (buf[6] << 8) | (buf[5] & 0xff);
	z = (buf[8] << 8) | (buf [7] & 0xff);
	
	//Print the accelerations as SHORTS 
	printf("x accel is: %d\ny accel is: %d\nz accel is: %d\n", x,y,z);


	 ofp = fopen("accels.txt", "w");
	 for (i=0; i<10; i++)
	  fprintf(ofp,"%d\t%d\t%d\n",x,y,z);
  
	/*  for (i=0; i<9; i++) 
		{
		 fprintf(ofp,"your data\r\n",x[i],y[i],z[i]);
		} */
	fclose(ofp); 
	 
	 
	 
	// Finalize the hidapi library
	res = hid_exit(); 

	return 0;
}
