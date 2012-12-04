/**
 * =====================================================================================
 *
 *   @file shuppan_api.cpp
 *   @author Victor Perron (), victor@iso3103.net
 *   
 *        Version:  1.0
 *        Created:  11/28/2012 09:17:32 PM
 *        Company:  iso3103 Labs
 *
 *   @section DESCRIPTION
 *
 *       SWiG interface API implementation
 *       
 *   @section LICENSE
 *
 *       
 *
 * =====================================================================================
 */

#include "shuppan.h"
#include "shuppan.hpp"

ShuppanApi::ShuppanApi() {
	this->handle = shuppan_init(this->info_wrapper, this);
};

ShuppanApi::~ShuppanApi() { 
	shuppan_destroy(this->handle);
};

void ShuppanApi::join(std::string group) { 
	shuppan_join(this->handle, group.c_str(), subscribe_wrapper);
};

void ShuppanApi::publish(std::string group, const char * msg, size_t len) { 
	shuppan_publish(this->handle, group.c_str(), msg, len);
};

void ShuppanApi::leave(std::string group) { 
	shuppan_leave(this->handle, group.c_str());
};

void ShuppanApi::info_wrapper(shuppan_handle_t* handle, const char * event, 
		const char * peer, const char * arg0, size_t len, void* class_ptr) {
	if(class_ptr) {
		ShuppanApi* shuppan = (ShuppanApi*) class_ptr;
		std::string s_event(event);
		std::string s_peer(peer);
		shuppan->info_callback(s_event,s_peer,arg0,len);
	}
};

void ShuppanApi::subscribe_wrapper(shuppan_handle_t* handle, const char * group, 
		const char * peer, const char * data, size_t len, void* class_ptr) {
	if(class_ptr) {
		ShuppanApi* shuppan = (ShuppanApi*) class_ptr;
		std::string s_group(group);
		std::string s_peer(peer);
		shuppan->subscribe_callback(s_group,s_peer,data,len);
	}
};
