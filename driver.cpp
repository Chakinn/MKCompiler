#include <cctype>
#include <fstream>
#include <cassert>

#include "driver.hpp"

FB::Driver::~Driver()
{
   delete(scanner);
   scanner = nullptr;
   delete(parser);
   parser = nullptr;
}

void FB::Driver::parse( const char * const inputFileName )
{
   std::ifstream inputFileStream( inputFileName );
   if( ! inputFileStream.good() )
   {
       exit( EXIT_FAILURE );
   }

   delete(scanner);
   scanner = new FB::Scanner(&inputFileStream);
   delete(parser);
   parser = new FB::Parser(*scanner,*this);

   if(parser->parse() != 0) {
       std::cerr << "Parse failed!!\n";
   }
   return;
}

