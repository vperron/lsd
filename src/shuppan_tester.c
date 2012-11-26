/**
 * =====================================================================================
 *
 *   @file shuppan_tester.c
 *   @author Victor Perron (), victor@iso3103.net
 *   
 *        Version:  1.0
 *        Created:  11/24/2012 03:02:43 PM
 *        Company:  iso3103 Labs
 *
 *   @section DESCRIPTION
 *
 *       Shuppan routines tester tool
 *       
 *   @section LICENSE
 *
 *       
 *
 * =====================================================================================
 */


#include "shuppan.h"

#define GROUP_0 "GROUP0"
#define GROUP_1 "GROUP1"

int main (int argc, char *argv [])
{

	const char* msg_fmt= "Hello group %s !";
	char msg_0[50], msg_1[50];

	shuppan_handle_t* handle = shuppan_init(NULL);

	zclock_sleep (500); 

	sprintf(msg_0,msg_fmt, GROUP_0);
	sprintf(msg_1,msg_fmt, GROUP_1);

	shuppan_publish(handle, GROUP_0, (const void*)msg_0,strlen(msg_0));
	shuppan_publish(handle, GROUP_1, (const void*)msg_1,strlen(msg_1));

	while (!zctx_interrupted) {
		zclock_sleep (500);
	}

	shuppan_destroy(handle);

	return 0;
}
