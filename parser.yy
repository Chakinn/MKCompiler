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

%token END_FILE
%token NUM
%token PIDENTIFIER

%token DECLARE
%token P_BEGIN
%token P_END
%token ASSIGN

%token IF
%token THEN
%token ELSE
%token ENDIF

%token WHILE
%token DO
%token ENDWHILE
%token ENDDO

%token FOR
%token FROM
%token TO
%token DOWNTO
%token ENDFOR

%token READ
%token WRITE

%token PLUS
%token MINUS
%token TIMES
%token DIV
%token MOD

%token EQ
%token NEQ
%token LE
%token GE
%token LEQ
%token GEQ


%locations

%%

program
    : DECLARE declarations P_BEGIN commands P_END
    | P_BEGIN commands P_END
    ;

declarations
    : declarations ',' PIDENTIFIER
    | declarations ',' PIDENTIFIER'('NUM':'NUM')'
    | PIDENTIFIER
    | PIDENTIFIER'('NUM':'NUM')'
    ;

commands
    : commands command
    | command
    ;

command 
    : identifier ASSIGN expression';'
    | IF condition THEN commands ELSE commands ENDIF
    | IF condition THEN commands ENDIF
    | WHILE condition DO commands ENDWHILE
    | DO commands WHILE condition ENDDO
    | FOR PIDENTIFIER FROM value TO value DO commands ENDFOR
    | FOR PIDENTIFIER FROM value DOWNTO value DO commands ENDFOR
    | READ identifier';'
    | WRITE value';'
    ;

expression
    : value
    | value PLUS value
    | value MINUS value
    | value TIMES value
    | value DIV value
    | value MOD value
    ;

condition
    : value EQ value
    : value NEQ value
    : value LE value
    : value GE value
    : value LEQ value
    : value GEQ value
    ;

value
    : NUM
    | identifier
    ;

identifier
    : PIDENTIFIER
    | PIDENTIFIER'('PIDENTIFIER')'
    | PIDENTIFIER'('NUM')'
    ;
%%


void FB::Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
