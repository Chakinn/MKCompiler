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
    #include <string>
   
    #include "driver.hpp"
    #include "../handling/handler.hpp"

    Handler handler;

    #undef yylex
    #define yylex scanner.yylex
}

%define api.value.type {std::string}
%define parse.assert

%token END_FILE
%token  NUM
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
    : DECLARE declarations P_BEGIN commands P_END {return 0;}
    | P_BEGIN commands P_END {return 0;}
    ;

declarations
    : declarations ',' PIDENTIFIER {handler.handleVarDeclaration($3);}
    | declarations ',' PIDENTIFIER'('NUM':'NUM')'
    | PIDENTIFIER {handler.handleVarDeclaration($1);}
    | PIDENTIFIER'('NUM':'NUM')'
    ;

commands
    : commands command
    | command
    ;

command 
    : identifier ASSIGN expression';'
    | IF condition THEN commands ELSE commands ENDIF {std::cout<<"ifelse\n";}
    | IF condition THEN commands ENDIF  {std::cout<<"if\n";}
    | WHILE condition DO commands ENDWHILE  {std::cout<<"whiledo\n";}
    | DO commands WHILE condition ENDDO {std::cout<<"dowhile\n";}
    | FOR PIDENTIFIER FROM value TO value DO commands ENDFOR {std::cout<<"forto\n";}
    | FOR PIDENTIFIER FROM value DOWNTO value DO commands ENDFOR {std::cout<<"fordownto\n";}
    | READ identifier';' {std::cout<<"read\n";}
    | WRITE value';' {std::cout<<"write\n";}
    ;

expression
    : value 
    | value PLUS value {std::cout<<"plus\n";}
    | value MINUS value
    | value TIMES value
    | value DIV value
    | value MOD value
    ;

condition
    : value EQ value {std::cout<<"eq\n";}
    | value NEQ value 
    | value LE value
    | value GE value
    | value LEQ value
    | value GEQ value
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
