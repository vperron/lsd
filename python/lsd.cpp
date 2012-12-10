/**
 * =====================================================================================
 *
 *   @file lsd_api.cpp
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

#include "lsd.h"
#include "lsd.hpp"

LsdApi::LsdApi() {
	this->handle = lsd_init(this->cb_wrapper, this);
};

LsdApi::~LsdApi() { 
	lsd_destroy(this->handle);
};

void LsdApi::join(std::string group) { 
	lsd_join(this->handle, group.c_str());
};

void LsdApi::whisper(std::string peer, const uint8_t *arg0, size_t len) { 
	lsd_shout(this->handle, peer.c_str(), arg0, len);
};

void LsdApi::shout(std::string group, const uint8_t *arg0, size_t len) { 
	lsd_shout(this->handle, group.c_str(), arg0, len);
};

void LsdApi::publish(std::string filename) { 
	lsd_publish(this->handle, filename.c_str());
};

void LsdApi::leave(std::string group) { 
	lsd_leave(this->handle, group.c_str());
};

void LsdApi::cb_wrapper (	lsd_handle_t* handle,	int code,
				const char *peer,	const char *group, const uint8_t* arg0, 
				size_t len,	void* class_ptr) {
	if(class_ptr) {
		LsdApi* lsd = (LsdApi*) class_ptr;
		std::string _peer(peer == NULL ? "" : peer);
		std::string _group(group == NULL ? "" : group);
		lsd->callback(code, _peer, _group, (const char*)arg0, len);
	}
};
