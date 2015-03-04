/* Example of usage "Tiny Curl C++ wrapper":
 *  https://github.com/asashnov/tinycurl
 */
#include <iostream>

#include "tinycurl.h"

int main(int argc, char **argv)
{
   try
   {
      TinyCurl ob("http://kodeyard.blogspot.in/");
      std::cout << ob.fetch();
      std::cout << std::endl;
   }
   catch (const TinyCurl::Exception &e)
   {
      std::cerr << "Exception thrown: " << e.what() << std::endl;
   }

   return 0;
}
