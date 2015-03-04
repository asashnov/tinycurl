#include <iostream>
#include <iostream>
#include <string>
#include <exception>

extern "C"
{
#include <curl/curl.h>
#include <stdlib.h>
}

//Exception class for curl exception

class CurlException : std::exception
{
public:
   CurlException(std::string message):m_message(message) {  }
   CurlException(CURLcode error)
   {
      m_message = curl_easy_strerror(error);
   }
   const char* what() throw()
   {
      return m_message.c_str();
   }
   ~CurlException() throw() { }

private:
   std::string m_message;
};


//A tiny wrapper around Curl C Library

class CppCurl
{
public:
   CppCurl(std::string url) throw (CurlException)
   {
      m_handle = curl_easy_init();
      if ( m_handle == NULL )
         throw CurlException("Unable to initialize curl handler");
      if ( url.length() == 0 )
         throw CurlException("URL can't be of zero length");
      m_url = url;
   }

   std::string Fetch() throw (CurlException)
   {
      SetOptions();
      SendGetRequest();
      return m_data;
   }


   ~CppCurl() throw()
   {
      curl_easy_cleanup(m_handle);
   }

private:

   void SetOptions() throw (CurlException)
   {
      CURLcode res;

      //set the url
      res = curl_easy_setopt(m_handle, CURLOPT_URL, m_url.c_str());
      if ( res != CURLE_OK)
         throw CurlException(res);

      //progress bar is not require
      res = curl_easy_setopt(m_handle, CURLOPT_NOPROGRESS, 1L);
      if ( res != CURLE_OK )
         throw CurlException(res);

      //set the callback function
      res = curl_easy_setopt(m_handle, CURLOPT_WRITEFUNCTION,
                              CppCurl::WriteDataCallback);
      if ( res != CURLE_OK )
         throw CurlException(res);

      //set pointer in call back function
      res = curl_easy_setopt(m_handle, CURLOPT_WRITEDATA, this);
      if ( res != CURLE_OK )
         throw CurlException(res);

   }

   void SendGetRequest()
   {
      CURLcode res;
      res = curl_easy_perform(m_handle);
      if ( res != CURLE_OK )
         throw CurlException(res);
   }

   static size_t WriteDataCallback(void *ptr, size_t size,
                                    size_t nmemb, void* pInstance)
   {
      return (static_cast<CppCurl*>(pInstance))->write_data(ptr, size, nmemb);
   }

   size_t write_data(void* ptr, size_t size, size_t nmemb)
   {
      size_t numOfBytes = size * nmemb;

      char *iter = (char*)ptr;
      char *iterEnd = iter + numOfBytes;

      //while ( iter != iterEnd )
      //{
      //   cout<<*iter;
      //   iter ++;
      //}

      m_data += std::string(iter, iterEnd);

      return numOfBytes;
   }

   CURL *m_handle;
   std::string m_url;
   std::string m_data;

};
