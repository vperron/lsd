/**
 * =====================================================================================
 *
 *   @file lsd.h
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

#ifndef _LSD_H_INCLUDED_
#define _LSD_H_INCLUDED_

#define LSD_VERSION_MAJOR 0
#define LSD_VERSION_MINOR 0
#define LSD_VERSION_PATCH 1

#define LSD_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define LSD_VERSION \
    LSD_MAKE_VERSION(LSD_VERSION_MAJOR, LSD_VERSION_MINOR, LSD_VERSION_PATCH)


#ifdef __cplusplus
extern "C" {
#endif


#define LSD_EVENT_ENTER   0
#define LSD_EVENT_EXIT    1
#define LSD_EVENT_WHISPER 2
#define LSD_EVENT_SHOUT   3
#define LSD_EVENT_DELIVER 4
#define LSD_EVENT_JOIN    5
#define LSD_EVENT_LEAVE   6

typedef struct _lsd_handle_t lsd_handle_t;

typedef void (lsd_callback_fn) 
	(lsd_handle_t* handle,                // Handle to the lsd object, useful to speak from callback
	 int event_code,                          // Event code as defined above 
	 const char *peer,                        // Peer that originated the event, if applicable
	 const char *group,                       // Group towards the event was directed, if applicable
	 const uint8_t *arg0,                       // Variable-lemgth argument, if applicable
	 size_t len,                              // Its length
	 void* reserved);                         // Reserved for internal use, do not use


// Init node, announce to network
lsd_handle_t* lsd_init(lsd_callback_fn* fn, void* reserved);

// Destroy node
void lsd_destroy(lsd_handle_t* handle);

// Join group 
void lsd_join(lsd_handle_t* self, const char* group);

// Leave group
void lsd_leave(lsd_handle_t* self, const char* group);

// Whisper to node 
void lsd_whisper(lsd_handle_t* self, const char* peer, const uint8_t* msg, size_t len);

// Shout to listeners (members of a group)
void lsd_shout(lsd_handle_t* self, const char* group, const uint8_t* msg, size_t len);

// Publish a file to everyone
void lsd_publish(lsd_handle_t* self, const char* filename);

#ifdef __cplusplus
}
#endif

#endif // _LSD_H_INCLUDED_
