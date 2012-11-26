/**
 * =====================================================================================
 *
 *   @file shuppan.c
 *   @author Victor Perron (), victor@iso3103.net
 *   
 *        Version:  1.0
 *        Created:  11/25/2012 05:54:44 PM
 *        Company:  iso3103 Labs
 *
 *   @section DESCRIPTION
 *
 *       Shuppan main library implementation
 *       
 *   @section LICENSE
 *
 *       
 *
 * =====================================================================================
 */

#include "shuppan.h"


#ifdef DEBUG
#define debugLog(fmt, ...) \
	do { \
		printf("%s [%s:%d] " fmt "\n", __func__,__FILE__,__LINE__, ##__VA_ARGS__); \
		fflush(stdout); \
	} while(0)

#else
#define debugLog(fmt, ...) 
#endif


struct _shuppan_handle_t {
	zctx_t* ctx;
	zre_interface_t* interface;
	void* pipe;
	shuppan_callback_fn* callback;
};


static void interface_task (void *args, zctx_t *ctx, void *pipe);


//  ---------------------------------------------------------------------
//  Init
shuppan_handle_t* shuppan_init(shuppan_callback_fn* fn) 
{
	shuppan_handle_t* self = (shuppan_handle_t *) zmalloc (sizeof (shuppan_handle_t));
	self->ctx = zctx_new();
	self->callback = fn;
	self->interface = zre_interface_new();
	self->pipe = zthread_fork (self->ctx, interface_task, self);
	return self;
}


//  ---------------------------------------------------------------------
//  Destroy node
void shuppan_destroy(shuppan_handle_t* self) 
{
	zstr_send (self->pipe, "STOP");
	zre_interface_destroy (&(self->interface));
	zsocket_destroy (self->ctx, self->pipe);
	zctx_destroy (&self->ctx);
	free(self);
}

//  ---------------------------------------------------------------------
//  Join group 
//  TODO: Later on may be good to ghave one callback per joined group
void shuppan_join(shuppan_handle_t* self, const char* group) 
{
  zre_interface_join(self->interface, group);
}

//  ---------------------------------------------------------------------
//  Leave group
void shuppan_leave(shuppan_handle_t* self, const char* group)
{
	zre_interface_leave(self->interface, group);
}

//  ---------------------------------------------------------------------
//  Publish to listeners (members of a group)
void shuppan_publish(shuppan_handle_t* self, const char* group, const char* msg) 
{
	zmsg_t* outgoing = zmsg_new();
	zmsg_addstr (outgoing, group);
	zmsg_addstr (outgoing, msg);
	zre_interface_shout(self->interface, &outgoing);
	zmsg_destroy(&outgoing);
}


static void interface_task (void *args, zctx_t *ctx, void *pipe )
{
	shuppan_handle_t *self = (shuppan_handle_t*) args;

	int64_t counter = 0;
	char *to_peer = NULL;        //  Either of these set,
	char *to_group = NULL;       //    and we set a message
	char *cookie = NULL;

	zmq_pollitem_t pollitems [] = {
		{ pipe,                             0, ZMQ_POLLIN, 0 },
		{ zre_interface_handle (self->interface), 0, ZMQ_POLLIN, 0 }
	};

	while (!zctx_interrupted) {
		if (zmq_poll (pollitems, 2, randof (1000) * ZMQ_POLL_MSEC) == -1) {
			printf ("I: Interrupted by user action.");
			break;              //  Interrupted
		}

		if (pollitems [0].revents & ZMQ_POLLIN) {
			printf ("I: Interrupted by parent.");
			break;              //  Any command fom parent means EXIT
		}

		//  Process an event from interface
		if (pollitems [1].revents & ZMQ_POLLIN) {
			zmsg_t *incoming = zre_interface_recv (self->interface);
			if (!incoming) {
				printf ("I: Interrupted before end of read.");
				break;              //  Interrupted
			}

			char *event = zmsg_popstr (incoming);
			if (streq (event, "ENTER")) {
				//  Always say hello to new peer
				to_peer = zmsg_popstr (incoming);
				debugLog ("I: ENTER '%s'", to_peer);
			} else if (streq (event, "EXIT")) {
				//  Always try talk to departed peer
				to_peer = zmsg_popstr (incoming);
				debugLog ("I: EXIT '%s'", to_peer);
			} else if (streq (event, "WHISPER")) {
				to_peer = zmsg_popstr (incoming);
				debugLog ("I: WHISPER '%s'", to_peer);
			} else 	if (streq (event, "SHOUT")) {
				to_peer = zmsg_popstr (incoming);
				to_group = zmsg_popstr (incoming);
				cookie = zmsg_popstr (incoming); 
				self->callback(to_group, to_peer, cookie);
				debugLog ("I: SHOUT from '%s' group '%s' cookie '%s'", to_peer, to_group, cookie);
			} else if (streq (event, "JOIN")) {
				char *from_peer = zmsg_popstr (incoming);
				char *group = zmsg_popstr (incoming);
				debugLog ("I: JOIN '%s - %s'", from_peer, group);
				free (from_peer);
				free (group);
			} else if (streq (event, "LEAVE")) {
				char *from_peer = zmsg_popstr (incoming);
				char *group = zmsg_popstr (incoming);
				debugLog ("I: LEAVE '%s - %s'", from_peer, group);
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
