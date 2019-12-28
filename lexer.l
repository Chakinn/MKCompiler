%{
#include "scanner.hpp"
#undef  YY_DECL
#define YY_DECL int FB::Scanner::yylex( FB::Parser::semantic_type* const lval, FB::Parser::location_type* loc )

/* typedef to make the returns for the tokens shorter */
using token = FB::Parser::token;

/* define yyterminate as this instead of NULL */
#define yyterminate() return (token::END_FILE)


%}

%option debug
%option nodefault
%option yyclass="FB::Scanner"
%option noyywrap
%option c++

%%
%{          
    yylval = lval;
%}

\n

. {yylval->build<std::string>(yytext);return token::CHAR;}

%%


