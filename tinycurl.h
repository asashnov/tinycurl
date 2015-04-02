/*
 * Tiny Curl C++ wrapper.
 *
 * This work is based on code by
 *   Asit Kumar Dhal <dhal.asitk@gmail.com> (https://github.com/asit-dhal)
 *   Loki Astari <Loki.Astari@gmail.com> (https://github.com/Loki-Astari)
 *  http://codereview.stackexchange.com/questions/14389/tiny-curl-c-wrapper
 * Thank you guys!
 *
 * License: Unlicensed (Public Domain)
 * Homepage: https://github.com/asashnov/tinycurl
 */
#include <string>
#include <map>
#include <stdexcept>
#include <cstdlib>

#include <curl/curl.h>

extern "C" {
  size_t tinycurl_write_data_cb(void *ptr, size_t size, size_t nmemb, void* pInstance);
  size_t tinycurl_read_data_cb (void *ptr, size_t size, size_t nmemb, void* pInstance);
}

//! A tiny wrapper around Curl C Library
class TinyCurl
{
public:
    //! Exception is used for curl errors
    class Exception : public std::runtime_error
    {
    public:
        Exception(std::string const& message): std::runtime_error(message) {}
        Exception(CURLcode error): std::runtime_error(curl_easy_strerror(error)) {}
    };

    TinyCurl(const std::string &url);
    ~TinyCurl();

    typedef std::map<std::string,std::string> headers_t;

    //! Execute GET request
    std::string fetch();

    std::string post(const std::string &data, const headers_t &hdrs);

    std::string put(const std::string &data, const headers_t &hdrs);

    //! true if HTTP error occurs (404 for example)
    bool isHTTPError() const;

    //! append received data into internal buffer by write_data_callback
    size_t append_data(void* ptr, size_t size, size_t nmemb);
    size_t upload_data(void* ptr, size_t size, size_t nmemb);

private:
    //! no compiler-generated copy constructor
    TinyCurl(const TinyCurl &);

    void setOptions();
    void setHTTPHeaders(const headers_t &hdrs) const;

    void curlPerform();

    CURL *m_handle;
    std::string m_url;
    std::string m_data;
    std::string m_upload_data;
};
