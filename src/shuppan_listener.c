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


static void info_callback (const char* event, const char* node, const void* msg, size_t len)
{
	printf("Event %s, node %s, len %d\n", event, node, (int)len);
}

static void sub_callback_0(
		shuppan_handle_t* handle, const char* group, const char* peer, 
		const void* data, size_t len) 
{
	printf("SUBSCRIBED CHANNEL 0 %s, from node %s : len %d\n", group, peer, (int)len);
}

static void sub_callback_1(
		shuppan_handle_t* handle, const char* group, const char* peer, 
		const void* data, size_t len) 
{
	printf("SUBSCRIBED CHANNEL 1 %s, from node %s : len %d\n", group, peer, (int)len);
}

int main (int argc, char *argv [])
{
	shuppan_handle_t* handle = shuppan_init(info_callback);

	shuppan_join(handle, GROUP_0, sub_callback_0);
	shuppan_join(handle, GROUP_1, sub_callback_1);

		zclock_sleep (10000);

	shuppan_leave(handle, GROUP_0);
	shuppan_leave(handle, GROUP_1);
	shuppan_destroy(handle);
	return 0;
}
