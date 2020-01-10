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

[ \t\n] //skip whitespaces and newlines

-?[0-9]+ {return token::NUM;}
[_a-z]+ {return token::PIDENTIFIER;}

DECLARE {return token::DECLARE;}
BEGIN {return token::P_BEGIN;}
END {return token::P_END;}
ASSIGN {return token::ASSIGN;}

IF {printf("lexif\n");return token::IF;}
THEN {return token::THEN;}
ELSE {return token::ELSE;}
ENDIF {return token::ENDIF;}

WHILE {return token::WHILE;}
DO {return token::DO;}
ENDWHILE {return token::ENDWHILE;}
ENDDO {return token::ENDDO;}

FOR {return token::FOR;}
FROM {return token::FROM;}
TO {return token::TO;}
DOWNTO {return token::DOWNTO;}
ENDFOR {return token::ENDFOR;}

READ {return token::READ;}
WRITE {return token::WRITE;}

PLUS {return token::PLUS;}
MINUS {return token::MINUS;}
TIMES {return token::TIMES;}
DIV {return token::DIV;}
MOD {return token::MOD;}

EQ {return token::EQ;}
NEQ {return token::NEQ;}
LE {return token::LE;}
GE {return token::GE;}
LEQ {return token::LEQ;}
GEQ {return token::GEQ;}

. {return yytext[0];}
<<EOF>> {return token::END_FILE;}

%%


