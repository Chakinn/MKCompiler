#include <iostream>
#include <cstdlib>
#include <cstring>
#include <string>

#include "driver.hpp"
#include "writer.hpp"

int main( const int argc, const char **argv )
{
    std::string outputFilename = std::string(argv[2]);
    Writer::setOutput(outputFilename);
    FB::Driver driver;
    driver.parse(argv[1]);
    std::cout<<std::endl;
}
