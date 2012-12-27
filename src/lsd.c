/**
 * =====================================================================================
 *
 *   @file lsd.c
 *   @author Victor Perron (), victor@iso3103.net
 *   
 *        Version:  1.0
 *        Created:  11/25/2012 05:54:44 PM
 *        Company:  iso3103 Labs
 *
 *   @section DESCRIPTION
 *
 *       Lsd main library implementation
 *       
 *   @section LICENSE
 *
 *       
 *
 * =====================================================================================
 */

#include "czmq.h"
#include "lsd.h"

#include "zre.h"

#ifdef DEBUG
#ifdef LIBLSD_HAVE_ANDROID
#include <android/log.h>
#define APPNAME "liblsd"
#define debugLog(fmt, ...) \
	do { \
		__android_log_print(ANDROID_LOG_DEBUG, APPNAME, \
				("%s [%s:%d] " fmt "\n", __func__,__FILE__,__LINE__, ##__VA_ARGS__); \
	} while(0)
#else
#define debugLog(fmt, ...) \
	do { \
		printf("%s [%s:%d] " fmt "\n", __func__,__FILE__,__LINE__, ##__VA_ARGS__); \
		fflush(stdout); \
	} while(0)
#endif
#else
#define debugLog(fmt, ...) 
#endif


#ifdef __cplusplus
extern "C" {
#endif

struct _lsd_handle_t {
	zctx_t* ctx;
	bool owns_ctx;
	zre_interface_t* interface;
	void* pipe;
	lsd_callback_fn* callback;
	void* class_ptr;
};


static void interface_task (void *args, zctx_t *ctx, void *pipe);


//  ---------------------------------------------------------------------
//  Init
lsd_handle_t* lsd_init(void* ctx, lsd_callback_fn* fn, void* reserved) 
{
	lsd_handle_t* self = (lsd_handle_t *) zmalloc (sizeof (lsd_handle_t));
	self->callback = fn;
	if(!ctx) { 
		self->ctx = zctx_new();
		zre_global_ctx = self->ctx;
		self->owns_ctx = true;
	} else { 
		self->ctx = ctx;
		zre_global_ctx = ctx;
		self->owns_ctx = false;
	}
	
	self->interface = zre_interface_new();
	self->pipe = zthread_fork (self->ctx, interface_task, self);
	self->class_ptr = reserved;
	return self;
}


//  ---------------------------------------------------------------------
//  Destroy node
void lsd_destroy(lsd_handle_t* self) 
{
	assert(self);

	zstr_send (self->pipe, "STOP");
	zre_interface_destroy (&(self->interface));
	zsocket_destroy (self->ctx, self->pipe);
	if(self->owns_ctx)
		zctx_destroy (&self->ctx);
	free(self);
}

//  ---------------------------------------------------------------------
//  Join group 
void lsd_join(lsd_handle_t* self, const char* group) 
{
	assert(self);
	assert(group);
  zre_interface_join(self->interface, group);
}

//  ---------------------------------------------------------------------
//  Leave group
void lsd_leave(lsd_handle_t* self, const char* group)
{
	assert(self);
	assert(group);
	zre_interface_leave(self->interface, group);
}

//  ---------------------------------------------------------------------
//  Shout to listeners (members of a group)
void lsd_shout(lsd_handle_t* self, const char* group, const uint8_t * msg, size_t len) 
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

//  ---------------------------------------------------------------------
//  Whisper to peer
void lsd_whisper(lsd_handle_t* self, const char* peer, const uint8_t * msg, size_t len) 
{
	assert(self);
	assert(peer);

	zmsg_t* outgoing = zmsg_new();
	zmsg_addstr (outgoing, peer);
	if(msg && len) {
		zmsg_addmem(outgoing, msg, len);
	}
	zre_interface_shout(self->interface, &outgoing);
	zmsg_destroy(&outgoing);
}


//  ---------------------------------------------------------------------
//  Publish file
void lsd_publish(lsd_handle_t* self, const char* filename) 
{
	char* short_name;
	char buffer[1024];
	char full_name[1024];
	char virtual_name[1024];

	assert(self);
	assert(filename);

	memset(buffer,0,sizeof(buffer));
	memset(virtual_name,0,sizeof(virtual_name));
	memset(full_name,0,sizeof(full_name));

	if(filename[0] != '/' && getcwd(buffer, sizeof(buffer)) != NULL) {
		snprintf(full_name, sizeof(full_name), "%s/%s", buffer, filename);
	} else {
		strncpy(full_name, filename, sizeof(full_name));
	}

	short_name = basename(full_name);
	snprintf(virtual_name, sizeof(virtual_name), "/%s", short_name);

	zre_interface_publish (self->interface, full_name, virtual_name);
}

//  ---------------------------------------------------------------------
//  Retract file
void lsd_retract(lsd_handle_t* self, const char* filename) 
{
	assert(self);
	assert(filename);

	zre_interface_retract (self->interface, (char*)filename);
}

static void interface_task (void *args, zctx_t *ctx, void *pipe )
{
	lsd_handle_t *self = (lsd_handle_t*) args;

	assert(self);

	char *peer = NULL;
	char *group = NULL;
	zframe_t* msg_frame = NULL;

	zmq_pollitem_t pollitems [] = {
		{ pipe,                             0, ZMQ_POLLIN, 0 },
		{ zre_interface_handle (self->interface), 0, ZMQ_POLLIN, 0 }
	};

	while (!zctx_interrupted) {
		if (zmq_poll (pollitems, 2, randof (1000) * ZMQ_POLL_MSEC) == -1) {
			debugLog ("I: Interrupted by user action.\n");
			break;              //  Interrupted
		}

		if (pollitems [0].revents & ZMQ_POLLIN) {
			debugLog ("I: Interrupted by parent.\n");
			break;              //  Any command fom parent means EXIT
		}

		//  Process an event from interface
		if (pollitems [1].revents & ZMQ_POLLIN) {
			zmsg_t *incoming = zre_interface_recv (self->interface);
			if (!incoming) {
				debugLog ("I: Interrupted before end of read.\n");
				break;              //  Interrupted
			}

			char *event = zmsg_popstr (incoming);
			debugLog("I EVENT == %s", event);
			if (streq (event, "ENTER")) {
				peer = zmsg_popstr (incoming);
				debugLog ("I: ENTER '%s'", peer);
				if(self->callback) {
					(*self->callback)(self,
							LSD_EVENT_ENTER,
							peer,
							NULL,
							NULL,
							0, 
							self->class_ptr);
				}
			} else if (streq (event, "EXIT")) {
				peer = zmsg_popstr (incoming);
				debugLog ("I: EXIT '%s'", peer);
				if(self->callback) {
					(*self->callback)(self,
							LSD_EVENT_EXIT,
							peer,
							NULL,
							NULL,
							0,
							self->class_ptr);
				}
			} else if (streq (event, "WHISPER")) {
				peer = zmsg_popstr (incoming);
				msg_frame = zmsg_pop (incoming);
				debugLog ("I: WHISPER '%s' msglen %d", peer, (int)zframe_size(msg_frame));
				if(self->callback) {
					(*self->callback)(self,
							LSD_EVENT_WHISPER,
							peer,
							NULL,
							(const uint8_t*)zframe_data(msg_frame),
							zframe_size(msg_frame),
							self->class_ptr);
				}
			} else 	if (streq (event, "SHOUT")) {
				peer = zmsg_popstr (incoming);
				group = zmsg_popstr (incoming);
				msg_frame = zmsg_pop (incoming); 
				debugLog ("I: SHOUT from '%s' group '%s' msglen %d", peer, group, (int)zframe_size(msg_frame));
				if(self->callback) {
					(*self->callback)(self,
							LSD_EVENT_SHOUT,
							peer,
							group,
							zframe_data(msg_frame),
							zframe_size(msg_frame),
							self->class_ptr);
				}
			} else if (streq (event, "DELIVER")) {
				char *filename = zmsg_popstr (incoming);
				char *fullname = zmsg_popstr (incoming);
				debugLog ("I: DELIVER file %s", fullname);
				if(self->callback) {
					(*self->callback)(self,
							LSD_EVENT_DELIVER,
							NULL,
							NULL,
							(const uint8_t*)fullname,
							strlen(fullname),
							self->class_ptr);
				}
				free (fullname);
				free (filename);
			}else if (streq (event, "JOIN")) {
				peer = zmsg_popstr (incoming);
				group = zmsg_popstr (incoming);
				debugLog ("I: JOIN '%s - %s'", peer, group);
				if(self->callback) {
					(*self->callback)(self,
							LSD_EVENT_JOIN,
							peer,
							group,
							NULL,
							0,
							self->class_ptr);
				}
			} else if (streq (event, "LEAVE")) {
				peer = zmsg_popstr (incoming);
				group = zmsg_popstr (incoming);
				debugLog ("I: LEAVE '%s - %s'", peer, group);
				if(self->callback) {
					(*self->callback)(self,
							LSD_EVENT_LEAVE,
							peer,
							group,
							NULL,
							0,
							self->class_ptr);
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

#ifdef __cplusplus
}
#endif
