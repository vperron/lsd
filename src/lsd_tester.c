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
	lsd_handle_t** handles;

	//  Get number of interfaces to simulate, default 100
	int max_handles = 100;
	int max_loops = 100;
	if (argc > 1)
		max_handles = atoi (argv [1]);
	if (argc > 2)
		max_loops = atoi (argv [2]);

	handles = (lsd_handle_t**) zmalloc(max_handles*sizeof(lsd_handle_t*));

	while(max_loops > 0) {

		uint index = randof(max_handles);

		if(handles[index]) {

			lsd_handle_t* handle = handles[index];
			uint r = randof(4);
			uint group_num = randof(10);
			char group[50], msg[50];
			sprintf(group, "GROUP%02d", (int)group_num);
			sprintf(msg, "Hello group #%02d !", (int)group_num);

			if(r == 0) {
				lsd_shout(handle, group, (const uint8_t*)msg, strlen(msg));
			} else if(r == 1) {
				lsd_join(handle, group);
			} else if(r == 2) {
				lsd_leave(handle, group);
			} else {
				lsd_destroy(handle);
				handles[index] = NULL;
			}

		} else {
			handles[index] = lsd_init(NULL, info_callback, NULL);
		}

		zclock_sleep (500);
		max_loops--;
		printf("Remaining loops: %d\n", max_loops);
	}

	for(int i=0;i<max_handles; i++) {
		if(handles[i]) {
			lsd_destroy(handles[i]);
		}
	}

	free(handles);

	return 0;
}

#ifdef __cplusplus
}
#endif
