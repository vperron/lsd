/**
 * =====================================================================================
 *
 *   @file shuppan.h
 *   @author Victor Perron (), victor@iso3103.net
 *   
 *        Version:  1.0
 *        Created:  11/24/2012 03:05:33 PM
 *        Company:  iso3103 Labs
 *
 *   @section DESCRIPTION
 *
 *       
 *       
 *   @section LICENSE
 *
 *       
 *
 * =====================================================================================
 */

#include <sys/types.h>
#include <stdint.h>

#ifndef _SHUPPAN_H_INCLUDED_
#define _SHUPPAN_H_INCLUDED_

#define SHUPPAN_VERSION_MAJOR 0
#define SHUPPAN_VERSION_MINOR 0
#define SHUPPAN_VERSION_PATCH 1

#define SHUPPAN_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define SHUPPAN_VERSION \
    SHUPPAN_MAKE_VERSION(SHUPPAN_VERSION_MAJOR, SHUPPAN_VERSION_MINOR, SHUPPAN_VERSION_PATCH)


#ifdef __cplusplus
extern "C" {
#endif


#define SHUPPAN_EVENT_ENTER   0
#define SHUPPAN_EVENT_EXIT    1
#define SHUPPAN_EVENT_WHISPER 2
#define SHUPPAN_EVENT_SHOUT   3
#define SHUPPAN_EVENT_DELIVER 4
#define SHUPPAN_EVENT_JOIN    5
#define SHUPPAN_EVENT_LEAVE   6

typedef struct _shuppan_handle_t shuppan_handle_t;

typedef void (shuppan_callback_fn) 
	(shuppan_handle_t* handle,                // Handle to the shuppan object, useful to speak from callback
	 int event_code,                          // Event code as defined above 
	 const char *peer,                        // Peer that originated the event, if applicable
	 const char *group,                       // Group towards the event was directed, if applicable
	 const uint8_t *arg0,                       // Variable-lemgth argument, if applicable
	 size_t len,                              // Its length
	 void* reserved);                         // Reserved for internal use, do not use


// Init node, announce to network
shuppan_handle_t* shuppan_init(shuppan_callback_fn* fn, void* reserved);

// Destroy node
void shuppan_destroy(shuppan_handle_t* handle);

// Join group 
void shuppan_join(shuppan_handle_t* self, const char* group);

// Leave group
void shuppan_leave(shuppan_handle_t* self, const char* group);

// Whisper to node 
void shuppan_whisper(shuppan_handle_t* self, const char* peer, const uint8_t* msg, size_t len);

// Shout to listeners (members of a group)
void shuppan_shout(shuppan_handle_t* self, const char* group, const uint8_t* msg, size_t len);

// Publish a file to everyone
void shuppan_publish(shuppan_handle_t* self, const char* filename);

#ifdef __cplusplus
}
#endif

#endif // _SHUPPAN_H_INCLUDED_
