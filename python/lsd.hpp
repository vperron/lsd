/**
 * =====================================================================================
 *
 *   @file lsd_api.hpp
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

typedef struct _lsd_handle_t lsd_handle_t;

class LsdApi {

  public:
    virtual void callback(
				int code,
				std::string peer,
				std::string group, 
        const char* arg0, 
				size_t len) const {} ;

		LsdApi();
		virtual ~LsdApi();

    virtual void join(std::string group);
    virtual void whisper(std::string group, const uint8_t* arg0, size_t len);
    virtual void shout(std::string peer, const uint8_t* arg0, size_t len);
    virtual void publish(std::string filename);
    virtual void leave(std::string group);

  private:
    lsd_handle_t* handle;

    static void cb_wrapper(
				lsd_handle_t* handle,
				int code,
				const char *peer,
				const char *group, 
        const uint8_t* arg0, 
				size_t len,
				void* class_ptr);

};
