/**
 * =====================================================================================
 *
 *   @file lsd_tester.c
 *   @author Victor Perron (), victor@iso3103.net
 *   
 *        Version:  1.0
 *        Created:  11/24/2012 03:02:43 PM
 *        Company:  iso3103 Labs
 *
 *   @section DESCRIPTION
 *
 *       Lsd routines tester tool
 *       
 *   @section LICENSE
 *
 *       
 *
 * =====================================================================================
 */

#include "lsd.h"
#include "zre.h"

#define GROUP_0 "GROUP0"
#define GROUP_1 "GROUP1"

#ifdef __cplusplus
extern "C" {
#endif

static void info_callback (lsd_handle_t* handle,
			int event,
			const char *node,
			const char *group, 
			const uint8_t *msg,
			size_t len,
			void* reserved)
{
	printf("Event %d, node %s, len %d\n", event, node, (int)len);
}

int main (int argc, char *argv [])
{
	lsd_handle_t* handle = lsd_init(NULL, info_callback, NULL);

	lsd_join(handle, GROUP_0);
	lsd_join(handle, GROUP_1);

	zclock_sleep (100000);

	lsd_leave(handle, GROUP_0);
	lsd_leave(handle, GROUP_1);
	lsd_destroy(handle);
	return 0;
}

#ifdef __cplusplus
}
#endif
