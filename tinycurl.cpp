/*
 * This file is a part of Tiny Curl C++ wrapper.
 *
 * License: Unlicensed (Public Domain)
 * Homepage: https://github.com/asashnov/tinycurl
 */
 #include "tinycurl.h"

TinyCurl::TinyCurl(const std::string &url)
{
    m_handle = curl_easy_init();
    if ( m_handle == NULL )
        throw Exception("Unable to initialize curl handler");
    if ( url.length() == 0 )
        throw Exception("URL can't be of zero length");
    m_url = url;
}

TinyCurl::~TinyCurl()
{
    curl_easy_cleanup(m_handle);
}

std::string TinyCurl::fetch()
{
    setOptions();
    sendGetRequest();
    return m_data;
}

void TinyCurl::setOptions()
{
    CURLcode res;

    //set the url
    res = curl_easy_setopt(m_handle, CURLOPT_URL, m_url.c_str());
    if ( res != CURLE_OK)
        throw Exception(res);

    //progress bar is not require
    res = curl_easy_setopt(m_handle, CURLOPT_NOPROGRESS, 1L);
    if ( res != CURLE_OK )
        throw Exception(res);

    //set the callback function
    res = curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION, tinycurl_write_data_cb);
    if ( res != CURLE_OK )
        throw Exception(res);

    //set pointer in call back function
    res = curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, this);
    if ( res != CURLE_OK )
        throw Exception(res);
}

void TinyCurl::sendGetRequest()
{
    CURLcode res;
    res = curl_easy_perform(m_handle);
    if ( res != CURLE_OK )
        throw Exception(res);
}

size_t TinyCurl::append_data(void* ptr, size_t size, size_t nmemb)
{
    size_t bytes = size * nmemb;
    const char *s = static_cast<char*>(ptr);
    m_data += std::string(s, s + bytes);
    return bytes;
}

size_t tinycurl_write_data_cb(void *ptr, size_t size, size_t nmemb, void* pInstance)
{
    TinyCurl* obj = reinterpret_cast<TinyCurl*>(pInstance);
    return obj->append_data(ptr, size, nmemb);
}
