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
    #include <typeinfo>
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
    : DECLARE declarations P_BEGIN commands P_END {handler.handleProgram();return 0;}
    | P_BEGIN commands P_END {handler.handleProgram();return 0;}
    ;

declarations
    : declarations ',' PIDENTIFIER {handler.handleVarDeclaration($3);}
    | declarations ',' PIDENTIFIER'('NUM':'NUM')' {handler.handleArrayDeclaration($3,$5,$7);}
    | PIDENTIFIER {handler.handleVarDeclaration($1);}
    | PIDENTIFIER'('NUM':'NUM')' {handler.handleArrayDeclaration($1,$3,$5);}
    ;

commands
    : commands command {handler.handleCommand($2);}
    | command {handler.handleFirstCommand($1);}
    ;

command 
    : identifier ASSIGN expression';' {$$ = handler.handleAssign($1,$3);}
    | IF condition THEN commands ELSE commands ENDIF {$$ = handler.handleIf($2,true);}
    | IF condition THEN commands ENDIF  {$$ = handler.handleIf($2,false);}
    | WHILE condition DO commands ENDWHILE  {$$ = handler.handleWhile($2,false);}
    | DO commands WHILE condition ENDDO {$$ = handler.handleWhile($4,true);}
    | FOR PIDENTIFIER FROM value TO value DO commands ENDFOR {$$ = handler.handleFor($2,$4,$6,false);}
    | FOR PIDENTIFIER FROM value DOWNTO value DO commands ENDFOR {$$ = handler.handleFor($2,$4,$6,true);}
    | READ identifier';' {$$ = handler.handleRead($2);}
    | WRITE value';' {$$ = handler.handleWrite($2);}
    ;

expression
    : value {$$ = handler.handleExpression($1,"","");}
    | value PLUS value {$$ = handler.handleExpression($1,"PLUS",$3);}
    | value MINUS value {$$ = handler.handleExpression($1,"MINUS",$3);}
    | value TIMES value {$$ = handler.handleExpression($1,"TIMES",$3);}
    | value DIV value {$$ = handler.handleExpression($1,"DIV",$3);}
    | value MOD value {$$ = handler.handleExpression($1,"MOD",$3);}
    ;

condition
    : value EQ value {$$ = handler.handleCondition($1, "EQ", $3);}
    | value NEQ value {$$ = handler.handleCondition($1, "NEQ", $3);}
    | value LE value {$$ = handler.handleCondition($1, "LE", $3);}
    | value GE value {$$ = handler.handleCondition($1, "GE", $3);}
    | value LEQ value {$$ = handler.handleCondition($1, "LEQ", $3);}
    | value GEQ value {$$ = handler.handleCondition($1, "GEQ", $3);}
    ;

value
    : NUM
    | identifier
    ;

identifier
    : PIDENTIFIER
    | PIDENTIFIER'('PIDENTIFIER')'  {$$ = $1+"("+$3+")";}
    | PIDENTIFIER'('NUM')' {$$ = $1+"("+$3+")";}
    ;
%%


void FB::Parser::error( const location_type &l, const std::string &err_message )
{
   std::cerr << "Error: " << err_message << " at " << l << "\n";
}
