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
	this->handle = shuppan_init(this->cb_wrapper, this);
};

ShuppanApi::~ShuppanApi() { 
	shuppan_destroy(this->handle);
};

void ShuppanApi::join(std::string group) { 
	shuppan_join(this->handle, group.c_str());
};

void ShuppanApi::whisper(std::string peer, const uint8_t *arg0, size_t len) { 
	shuppan_shout(this->handle, peer.c_str(), arg0, len);
};

void ShuppanApi::shout(std::string group, const uint8_t *arg0, size_t len) { 
	shuppan_shout(this->handle, group.c_str(), arg0, len);
};

void ShuppanApi::publish(std::string filename) { 
	shuppan_publish(this->handle, filename.c_str());
};

void ShuppanApi::leave(std::string group) { 
	shuppan_leave(this->handle, group.c_str());
};

void ShuppanApi::cb_wrapper (	shuppan_handle_t* handle,	int code,
				const char *peer,	const char *group, const uint8_t* arg0, 
				size_t len,	void* class_ptr) {
	if(class_ptr) {
		ShuppanApi* shuppan = (ShuppanApi*) class_ptr;
		std::string _peer(peer == NULL ? "" : peer);
		std::string _group(group == NULL ? "" : group);
		shuppan->callback(code, _peer, _group, (const char*)arg0, len);
	}
};
