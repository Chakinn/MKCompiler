#ifndef __SCANNER_HPP__
#define __SCANNER_HPP__ 1

#if ! defined(yyFlexLexerOnce)
#include <FlexLexer.h>
#endif

#include "parser.tab.hh"
#include "location.hh"

namespace FB{

class Scanner : public yyFlexLexer{
public:
   
   Scanner(std::istream *in) : yyFlexLexer(in)
   {
   };
   virtual ~Scanner() {
   };

   using FlexLexer::yylex;

   virtual int yylex( FB::Parser::semantic_type * const lval, 
              FB::Parser::location_type *location );
   // YY_DECL defined in lexer.l
   // Method body created by flex in lexer.yy.cc

   void skipComment() {
      char c;
      while(c = yyinput() != ']');
   }


private:
   /* yyval ptr */
   FB::Parser::semantic_type *yylval = nullptr;
};

} /* end namespace FB */

#endif /* END __SCANNER_HPP__ */
