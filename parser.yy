%skeleton "lalr1.cc"
%require  "3.0"
%debug 
%defines 
%define api.namespace {FB}
%define parser_class_name {Parser}

%code requires{
   namespace FB {
      class Driver;
      class Scanner;
   }

// The following definitions is missing when %locations isn't used
# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

}

%parse-param { Scanner  &scanner  }
%parse-param { Driver  &driver  }

%code{
    #include <iostream>
    #include <cstdlib>
    #include <fstream>
   
    #include "driver.hpp"

    #undef yylex
    #define yylex scanner.yylex
}

%define api.value.type variant
%define parse.assert

%token               END_FILE    0     "end of file"
%token <std::string>             CHAR

%locations

%%

program
    : stuff END_FILE
    ;

stuff
    : stuff CHAR    {std::cout << $2;}
    | CHAR {std::cout << $1;}
    ;

%%


void FB::Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
