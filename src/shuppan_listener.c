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

#define DEFGROUP "MYGROUP"


static void callback (char* group, char* node, char* msg)
{
	printf("Received shout: %s - %s - %s\n", group, node, msg);
}

int main (int argc, char *argv [])
{
	shuppan_handle_t* handle = shuppan_init(callback);
	shuppan_join(handle, DEFGROUP);

	while (!zctx_interrupted) {
		zclock_sleep (500);
	}

	shuppan_leave(handle, DEFGROUP);
	shuppan_destroy(handle);
	return 0;
}
