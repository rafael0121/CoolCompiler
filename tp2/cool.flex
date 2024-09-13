/*
 *  The scanner definition for COOL.
 */

/*
 *  Stuff enclosed in %{ %} in the first section is copied verbatim to the
 *  output, so headers and global definitions are placed here to be visible
 * to the code in the file.  Don't remove anything that was here initially
 */
%{
#include <cool-parse.h>
#include <stringtab.h>
#include <utilities.h>

/* The compiler assumes these identifiers. */
#define yylval cool_yylval
#define yylex  cool_yylex

/* Max size of string constants */
#define MAX_STR_CONST 1025
#define YY_NO_UNPUT   /* keep g++ happy */

extern FILE *fin; /* we read from this file */

/* define YY_INPUT so we read from the FILE fin:
 * This change makes it possible to use this scanner in
 * the Cool compiler.
 */
#undef YY_INPUT
#define YY_INPUT(buf,result,max_size) \
	if ( (result = fread( (char*)buf, sizeof(char), max_size, fin)) < 0) \
		YY_FATAL_ERROR( "read() in flex scanner failed");

char string_buf[MAX_STR_CONST]; /* to assemble string constants */
char *string_buf_ptr;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */

%}
%option noyywrap

%s com_block

DIGIT      [0-9]
NUM        [+-]?{DIGIT}+
OBJ_ID     [a-z][0-9a-zA-Z_]*
TYPE_ID    ([A-Z][0-9a-zA-Z_]*)|self

/* Keywords are case insensitive, except true and false, which must start with lowercase */
NEW_KW      [?i:new]
CLASS_KW    [?i:class]
INHERITS_KW [?i:inherits]

LET_KW      [?i:let]
IN_KW       [?i:in]

IF_KW       (?i:if)
THEN_KW     [?i:then]
ELSE_KW     [?i:else]
FI_KW       [?i:FI]

WHILE_KW    [?i:while]
LOOP_KW     [?i:loop]
POOL_KW     [?i:pool]

CASE_KW     [?i:case]
OF_KW       [?i:of]
ESAC_KW     [?i:esac]

NOT_KW      [?i:not]
TRUE_KW     t(?i:rue)
FALSE_KW    f(?i:alse)
ISVOID_KW   [?i:isvoid]

/* Special characters and exceptions that may and cannot appear on a string */
STRING_SPEC     [\b\t\n\f]
STRING_EXCPT    [EOF\0]

WHITE_SPACES    [\f\r\t\v ]+
NEWLINE         \n

%%

<INITIAL>{
"(*"    BEGIN(com_block);
}

<com_block>{
"*)"    BEGIN(INITIAL);
[^*\n]+
"*"
\n      curr_lineno++;
}

{IF_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (IF);
}

{TRUE_KW} {
    printf("token: %s ", yytext);
    stringtable.add_string(yytext);
    cool_yylval.boolean = true;
    return (BOOL_CONST);
} 

{FALSE_KW} {
    printf("token: %s ", yytext);
    stringtable.add_string(yytext);
    cool_yylval.boolean = false;
    return (BOOL_CONST);
}

{TYPE_ID} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (TYPEID);
}

{OBJ_ID} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (OBJECTID);
}

{NUM} {
    cool_yylval.symbol = inttable.add_string(yytext);
    return (INT_CONST);
}

{NEWLINE} { curr_lineno++; }

{WHITE_SPACES} { }

. {
    printf("Erro na linha: %i token: %s \n", curr_lineno, yytext);
    return (ERROR);
}

%%
