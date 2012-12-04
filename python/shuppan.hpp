/**
 * =====================================================================================
 *
 *   @file shuppan_api.hpp
 *   @author Victor Perron (), victor@iso3103.net
 *   
 *        Version:  1.0
 *        Created:  11/28/2012 09:29:58 PM
 *        Company:  iso3103 Labs
 *
 *   @section DESCRIPTION
 *
 *       API for SWiG
 *       
 *   @section LICENSE
 *
 *       
 *
 * =====================================================================================
 */

#include <string>

typedef struct _shuppan_handle_t shuppan_handle_t;

class ShuppanApi {

  public:
    virtual void info_callback(std::string event, std::string peer, 
        const char* data, size_t len) const {} ;
    virtual void subscribe_callback(std::string group, std::string peer, 
				const char* data, size_t len) const {} ;

		ShuppanApi();
		virtual ~ShuppanApi();

    virtual void join(std::string group);
    virtual void publish(std::string group, const char* data, size_t len);
    virtual void leave(std::string group);

  private:
    shuppan_handle_t* handle;

    static void info_wrapper(shuppan_handle_t* handle,
				const char * event, const char * peer, 
        const char* arg0, size_t len,void* class_ptr);
    static void subscribe_wrapper(shuppan_handle_t* handle, 
				const char * group, const char * peer, 
				const char* data, size_t len,void* class_ptr);

};
