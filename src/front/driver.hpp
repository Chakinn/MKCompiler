#ifndef __DRIVER_HPP__
#define __DRIVER_HPP__ 1

#include <string>
#include <cstddef>
#include <istream>

#include "scanner.hpp"
#include "parser.tab.hh"

/**
 *  Flex + Bison namespace
*/
namespace FB{
/**
 * Class responsible for joining Parser and Scanner together.
*/
class Driver{
private:
   FB::Parser* parser  = nullptr;
   FB::Scanner* scanner = nullptr;

public:
   Driver() = default;
   virtual ~Driver();
   
   void parse( const char * const inputFileName );

};

} /* end namespace FB */
#endif /* END __DRIVER_HPP__ */
