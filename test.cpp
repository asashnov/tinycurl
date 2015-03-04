#include "tinycurl.h"

int main()
{
   try
   {
      CppCurl ob("http://kodeyard.blogspot.in/");
      std::cout<<ob.Fetch();
      std::cout<<std::endl;
   }
   catch ( CurlException e)
   {
      std::cerr<<"Exception thrown..."<<std::endl;
      std::cerr<<e.what()<<std::endl;
   }

   return 0;

}
