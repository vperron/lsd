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
#define DEFGROUP2 "GROUP2"

static void interface_task (void *args, zctx_t *ctx, void *pipe )
{
	zre_interface_t *interface = (zre_interface_t*) args;
	int64_t counter = 0;
	char *to_peer = NULL;        //  Either of these set,
	char *to_group = NULL;       //    and we set a message
	char *cookie = NULL;

	zmq_pollitem_t pollitems [] = {
		{ pipe,                             0, ZMQ_POLLIN, 0 },
		{ zre_interface_handle (interface), 0, ZMQ_POLLIN, 0 }
	};

	while (!zctx_interrupted) {
		if (zmq_poll (pollitems, 2, randof (1000) * ZMQ_POLL_MSEC) == -1) {
			zclock_log ("I: Interrupted by user action.");
			break;              //  Interrupted
		}

		if (pollitems [0].revents & ZMQ_POLLIN) {
			zclock_log ("I: Interrupted by parent.");
			break;              //  Any command fom parent means EXIT
		}

		//  Process an event from interface
		if (pollitems [1].revents & ZMQ_POLLIN) {
			zmsg_t *incoming = zre_interface_recv (interface);
			if (!incoming) {
				zclock_log ("I: Interrupted before end of read.");
				break;              //  Interrupted
			}

			char *event = zmsg_popstr (incoming);
			
			if (streq (event, "ENTER")) {
				//  Always say hello to new peer
				to_peer = zmsg_popstr (incoming);
				zclock_log ("I: ENTER '%s'", to_peer);
			} else if (streq (event, "EXIT")) {
				//  Always try talk to departed peer
				to_peer = zmsg_popstr (incoming);
				zclock_log ("I: EXIT '%s'", to_peer);
			} else if (streq (event, "WHISPER")) {
				to_peer = zmsg_popstr (incoming);
				zclock_log ("I: WHISPER '%s'", to_peer);
			} else 	if (streq (event, "SHOUT")) {
				to_peer = zmsg_popstr (incoming);
				to_group = zmsg_popstr (incoming);
				cookie = zmsg_popstr (incoming); 
				zclock_log ("I: SHOUT from '%s' group '%s' cookie '%s'", to_peer, to_group, cookie);
			} else if (streq (event, "JOIN")) {
				char *from_peer = zmsg_popstr (incoming);
				char *group = zmsg_popstr (incoming);
				zclock_log ("I: JOIN '%s - %s'", from_peer, group);
				free (from_peer);
				free (group);
			} else if (streq (event, "LEAVE")) {
				char *from_peer = zmsg_popstr (incoming);
				char *group = zmsg_popstr (incoming);
				zclock_log ("I: LEAVE '%s - %s'", from_peer, group);
				free (from_peer);
				free (group);
			}

			free (event);
			zmsg_destroy (&incoming);

			//  Send outgoing messages if needed
			if (to_peer) {
				zmsg_t *outgoing = zmsg_new ();
				zmsg_addstr (outgoing, to_peer);
				zmsg_addstr (outgoing, "%lu", counter++);
				free (to_peer);
				to_peer = NULL;
			}
			if (to_group) {
				zmsg_t *outgoing = zmsg_new ();
				zmsg_addstr (outgoing, to_group);
				zmsg_addstr (outgoing, "%lu", counter++);
				free (to_group);
				to_group = NULL;
			}
			if (cookie) {
				free (cookie);
				cookie = NULL;
			}
		}
	}
}

int main (int argc, char *argv [])
{
	zctx_t *ctx = zctx_new ();
	zctx_set_linger (ctx, 0);

	zre_interface_t* interface = zre_interface_new();

	void* pipe = zthread_fork (ctx, interface_task, interface);
	zclock_log ("I: Started interface %08x", pipe);

  zre_interface_join(interface, DEFGROUP);
  zre_interface_join(interface, DEFGROUP2);
	zclock_sleep (500);

	while (!zctx_interrupted) {
		zclock_sleep (500);
	}

	zstr_send (pipe, "STOP");

	zre_interface_destroy (&interface);
	zsocket_destroy (ctx, pipe);
	zclock_log ("I: Stopped interface %08x", pipe);
	pipe = NULL;

	zctx_destroy (&ctx);
	return 0;
}
