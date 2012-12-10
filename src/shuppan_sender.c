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
#include "zre/zre.h"

#define GROUP_0 "GROUP0"
#define GROUP_1 "GROUP1"

#define MSG_0 "Hello Group0 !"
#define MSG_1 "Hello Group1 !"

#ifdef __cplusplus
extern "C" {
#endif

static void info_callback (shuppan_handle_t* handle,
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
	shuppan_handle_t* handle = shuppan_init(info_callback, NULL);

	shuppan_join(handle, GROUP_0);
	shuppan_join(handle, GROUP_1);

	zclock_sleep (500);

	shuppan_shout(handle, GROUP_0, (const uint8_t*)MSG_0,strlen(MSG_0));
	zclock_sleep (500);
	shuppan_shout(handle, GROUP_1, (const uint8_t*)MSG_1,strlen(MSG_1));
	zclock_sleep (500);
	shuppan_publish(handle, "core");

	zclock_sleep (100000);

	shuppan_leave(handle, GROUP_0);
	shuppan_leave(handle, GROUP_1);
	shuppan_destroy(handle);
	return 0;
}

#ifdef __cplusplus
}
#endif
