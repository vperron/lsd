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
	shuppan_info_callback_fn* info_callback;
	zhash_t* callbacks;
};

typedef struct _fn_ptr_wrapper_t {
	shuppan_subscribe_callback_fn* fn;
} fn_ptr_wrapper_t;
	

static void interface_task (void *args, zctx_t *ctx, void *pipe);


//  ---------------------------------------------------------------------
//  Init
shuppan_handle_t* shuppan_init(shuppan_info_callback_fn* fn) 
{
	shuppan_handle_t* self = (shuppan_handle_t *) zmalloc (sizeof (shuppan_handle_t));
	self->info_callback = fn;
	self->ctx = zctx_new();
	self->callbacks = zhash_new();
	self->interface = zre_interface_new();
	self->pipe = zthread_fork (self->ctx, interface_task, self);
	return self;
}


//  ---------------------------------------------------------------------
//  Destroy node
void shuppan_destroy(shuppan_handle_t* self) 
{
	assert(self);

	zstr_send (self->pipe, "STOP");
	zre_interface_destroy (&(self->interface));
	zsocket_destroy (self->ctx, self->pipe);
	zhash_destroy(&self->callbacks);
	zctx_destroy (&self->ctx);
	free(self);
}

//  ---------------------------------------------------------------------
//  Join group 
void shuppan_join(shuppan_handle_t* self, const char* group, shuppan_subscribe_callback_fn* fn) 
{
	fn_ptr_wrapper_t* wrapper;

	assert(self);
	assert(group);

	if(fn) {
		wrapper = zmalloc(sizeof(fn_ptr_wrapper_t));
		wrapper->fn = fn;
		zhash_insert(self->callbacks, group, wrapper);
		zhash_freefn(self->callbacks, group, free);
	}

  zre_interface_join(self->interface, group);
}

//  ---------------------------------------------------------------------
//  Leave group
void shuppan_leave(shuppan_handle_t* self, const char* group)
{
	assert(self);
	assert(group);
	zhash_delete(self->callbacks, group);
	zre_interface_leave(self->interface, group);
}

//  ---------------------------------------------------------------------
//  Publish to listeners (members of a group)
void shuppan_publish(shuppan_handle_t* self, const char* group, const void* msg, size_t len) 
{
	assert(self);
	assert(group);

	zmsg_t* outgoing = zmsg_new();
	zmsg_addstr (outgoing, group);
	if(msg && len) {
		zmsg_addmem(outgoing, msg, len);
	}
	zre_interface_shout(self->interface, &outgoing);
	zmsg_destroy(&outgoing);
}


static void interface_task (void *args, zctx_t *ctx, void *pipe )
{
	shuppan_handle_t *self = (shuppan_handle_t*) args;

	assert(self);

	char *peer = NULL;
	char *group = NULL;
	zframe_t* msg_frame = NULL;
	fn_ptr_wrapper_t* fn_wrapper = NULL;

	zmq_pollitem_t pollitems [] = {
		{ pipe,                             0, ZMQ_POLLIN, 0 },
		{ zre_interface_handle (self->interface), 0, ZMQ_POLLIN, 0 }
	};

	while (!zctx_interrupted) {
		if (zmq_poll (pollitems, 2, randof (1000) * ZMQ_POLL_MSEC) == -1) {
			printf ("I: Interrupted by user action.\n");
			break;              //  Interrupted
		}

		if (pollitems [0].revents & ZMQ_POLLIN) {
			printf ("I: Interrupted by parent.\n");
			break;              //  Any command fom parent means EXIT
		}

		//  Process an event from interface
		if (pollitems [1].revents & ZMQ_POLLIN) {
			zmsg_t *incoming = zre_interface_recv (self->interface);
			if (!incoming) {
				printf ("I: Interrupted before end of read.\n");
				break;              //  Interrupted
			}

			char *event = zmsg_popstr (incoming);
			if (streq (event, "ENTER")) {
				peer = zmsg_popstr (incoming);
				debugLog ("I: ENTER '%s'", peer);
				if(self->info_callback) {
					(*self->info_callback)(event,peer,NULL,0);
				}
			} else if (streq (event, "EXIT")) {
				peer = zmsg_popstr (incoming);
				debugLog ("I: EXIT '%s'", peer);
				if(self->info_callback) {
					(*self->info_callback)(event,peer,NULL,0);
				}
			} else if (streq (event, "WHISPER")) {
				peer = zmsg_popstr (incoming);
				msg_frame = zmsg_pop (incoming);
				debugLog ("I: WHISPER '%s' msglen %d", peer, (int)zframe_size(msg_frame));
				if(self->info_callback) {
					(*self->info_callback)(event,peer,zframe_data(msg_frame),zframe_size(msg_frame));
				}
			} else 	if (streq (event, "SHOUT")) {
				peer = zmsg_popstr (incoming);
				group = zmsg_popstr (incoming);
				msg_frame = zmsg_pop (incoming); 
				debugLog ("I: SHOUT from '%s' group '%s' msglen %d", peer, group, (int)zframe_size(msg_frame));
				if(self->info_callback) {
					(*self->info_callback)(event,peer,group,strlen(group));
				}
				fn_wrapper =  (fn_ptr_wrapper_t* ) zhash_lookup(self->callbacks, group);
				if(fn_wrapper) {
					shuppan_subscribe_callback_fn* fn = fn_wrapper->fn;
					if(fn) {
						(*fn)(self,group,peer,zframe_data(msg_frame), zframe_size(msg_frame));
					}
				}
			} else if (streq (event, "JOIN")) {
				peer = zmsg_popstr (incoming);
				group = zmsg_popstr (incoming);
				debugLog ("I: JOIN '%s - %s'", peer, group);
				if(self->info_callback) {
					(*self->info_callback)(event,peer,group,strlen(group));
				}
			} else if (streq (event, "LEAVE")) {
				peer = zmsg_popstr (incoming);
				group = zmsg_popstr (incoming);
				debugLog ("I: LEAVE '%s - %s'", peer, group);
				if(self->info_callback) {
					(*self->info_callback)(event,peer,group,strlen(group));
				}
			}

			if(peer) {
				free(peer);
				peer = NULL;
			}
			if(group) {
				free(group);
				group = NULL;
			}
			if(msg_frame) {
				zframe_destroy(&msg_frame);
				msg_frame = NULL;
			}
			
			free (event);
			zmsg_destroy (&incoming);
		}
	}
}
