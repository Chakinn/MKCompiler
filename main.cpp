#include <iostream>
#include <cstdlib>
#include <cstring>

#include "driver.hpp"

int main( const int argc, const char **argv )
{
    FB::Driver driver;
    driver.parse(argv[1]);
    std::cout<<std::endl;
}
