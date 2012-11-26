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


static void info_callback (const char* event, const char* node, const void* msg, size_t len)
{
	printf("Event %s, node %s, len %d\n", event, node, (int)len);
}

static void sub_callback(
		shuppan_handle_t* handle, const char* group, const char* peer, 
		const void* data, size_t len) 
{
	printf("SUBSCRIBED %s, from node %s : len %d\n", group, peer, (int)len);
}

int main (int argc, char *argv [])
{
	shuppan_handle_t** handles;

	//  Get number of interfaces to simulate, default 100
	int max_handles = 100;
	int max_loops = 100;
	if (argc > 1)
		max_handles = atoi (argv [1]);
	if (argc > 2)
		max_loops = atoi (argv [2]);

	handles = zmalloc(max_handles*sizeof(shuppan_handle_t*));

	while(max_loops > 0) {

		uint index = randof(max_handles);

		if(handles[index]) {

			shuppan_handle_t* handle = handles[index];
			uint r = randof(4);
			uint group_num = randof(10);
			char group[50], msg[50];
			sprintf(group, "GROUP%02d", (int)group_num);
			sprintf(msg, "Hello group #%02d !", (int)group_num);

			if(r == 0) {
				shuppan_publish(handle, group, (const void*)msg,strlen(msg));
			} else if(r == 1) {
				shuppan_join(handle, group, sub_callback);
			} else if(r == 2) {
				shuppan_leave(handle, group);
			} else {
				shuppan_destroy(handle);
				handles[index] = NULL;
			}

		} else {
			handles[index] = shuppan_init(info_callback);
		}

		zclock_sleep (500);
		max_loops--;
		printf("Remaining loops: %d\n", max_loops);
	}

	for(int i=0;i<max_handles; i++) {
		if(handles[i]) {
			shuppan_destroy(handles[i]);
		}
	}

	free(handles);

	return 0;
}