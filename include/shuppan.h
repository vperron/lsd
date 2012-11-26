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

#ifndef _SHUPPAN_H_INCLUDED_
#define _SHUPPAN_H_INCLUDED_

#include <zre/zre.h>

#define SHUPPAN_VERSION_MAJOR 0
#define SHUPPAN_VERSION_MINOR 0
#define SHUPPAN_VERSION_PATCH 1

#define SHUPPAN_MAKE_VERSION(major, minor, patch) \
    ((major) * 10000 + (minor) * 100 + (patch))
#define SHUPPAN_VERSION \
    SHUPPAN_MAKE_VERSION(SHUPPAN_VERSION_MAJOR, SHUPPAN_VERSION_MINOR, SHUPPAN_VERSION_PATCH)

#if ZRE_VERSION < 200
#   error "Shuppan needs ZRE/0.2.0 or later"
#endif



#ifdef __cplusplus
extern "C" {
#endif

typedef struct _shuppan_handle_t shuppan_handle_t;

typedef void (shuppan_info_callback_fn) 
	(const char* event, const char* peer, const void* arg0, size_t len);

typedef void (shuppan_subscribe_callback_fn) 
	(shuppan_handle_t* handle, const char* group, 
	 const char* peer, const void* data, size_t len);


// Init node, announce to network
shuppan_handle_t* shuppan_init(shuppan_info_callback_fn* fn);

// Destroy node
void shuppan_destroy(shuppan_handle_t* handle);

// Join group 
void shuppan_join(shuppan_handle_t* self, const char* group, shuppan_subscribe_callback_fn* callback);

// Leave group
void shuppan_leave(shuppan_handle_t* self, const char* group);

// Publish to listeners (members of a group)
void shuppan_publish(shuppan_handle_t* self, const char* group, const void* msg, size_t len);

#ifdef __cplusplus
}
#endif

#endif // _SHUPPAN_H_INCLUDED_