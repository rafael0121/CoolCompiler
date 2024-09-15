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
unsigned short int cursor = 0;     /* keeps track of the position inside the string buffer and its size */

%}

%option noyywrap

/* 
 * Definitions for certain conditions: 
 * com_block: parse commentary blocks
 * com_inline: parse inline commentary
 * string_parse: parses strings, counting its lenght
 * eat_string: parses remainder of string if an error occurs
 */
%x com_block
%x com_inline
%x string_parse
%x eat_string

/* 
 *  Definitions for the identifiers: 
 * The number can be signed and is composed of one or more digits
 * The object must allways start with a lowercase letter and can contain an underscore
 * The type must allways start with a uppercase letter and can contain an underscore, the keyword self is treated specialy as a type
 */
DIGIT      [0-9]
NUM        [+-]?{DIGIT}+
OBJ_ID     [a-z][0-9a-zA-Z_]*
TYPE_ID    ([A-Z][0-9a-zA-Z_]*)|self

/* Definitions for the keywords, they are case insensitive, except for true and false, which must start with lowercase */
NEW_KW      (?i:new)
CLASS_KW    (?i:class)
INHERITS_KW (?i:inherits)

LET_KW      (?i:let)
IN_KW       (?i:in)

IF_KW       (?i:if)
THEN_KW     (?i:then)
ELSE_KW     (?i:else)
FI_KW       (?i:FI)

WHILE_KW    (?i:while)
LOOP_KW     (?i:loop)
POOL_KW     (?i:pool)

CASE_KW     (?i:case)
OF_KW       (?i:of)
ESAC_KW     (?i:esac)

NOT_KW      (?i:not)
TRUE_KW     t(?i:rue)
FALSE_KW    f(?i:alse)
ISVOID_KW   (?i:isvoid)

/* Definitions for the assingment and darrow operator*/
DARROW      =>
ASSIGN      <-

/* Special characters and exceptions that cannot appear on a string */
STRING_EXCPT    [\0\b\t\n\f]

WHITE_SPACES    [\f\r\t\v ]+
NEWLINE         \n

/* Definition of operators and special symbols */
OPERATORS       [+"/"\-"*"=<.~,;:()@{}]

%%

<INITIAL>{
    /* Initial condition used to parse comments and strings, also check for an unmatched comment */
    "(*"    BEGIN(com_block);
    "--"    BEGIN(com_inline);
    "\""    BEGIN(string_parse);
    "*)" {
        cool_yylval.error_msg = "Unmatched *)";
        return (ERROR);
    }
}

<com_block>{
    /* Finds end of comment, ignoring everything inside it, but checking for an EOF */
    "*)"    BEGIN(INITIAL);
    [^*\n]+
    "*"
    \n      curr_lineno++;
    <<EOF>> {
        BEGIN(INITIAL);
        cool_yylval.error_msg = "EOF in comment";
        return (ERROR);
    }
}

<com_inline>{
    /* Finds end of inline comment, can be EOF, \n or --, ignores the rest*/
    "--"    BEGIN(INITIAL);
    \n      {
        curr_lineno++;
        BEGIN(INITIAL);
    }
    <<EOF>> {
        BEGIN(INITIAL);
    }
    .   {}
}

<eat_string>{
    /* If the string is too long we fall in this condition, the remainder of string is ignored, we can also return an extra error if we find EOF or ESCAPED char */
    "\""    {
        cursor = 0;
        BEGIN(INITIAL);
    }

    {STRING_EXCPT} {
        cursor = 0;
        BEGIN(INITIAL);
        cool_yylval.error_msg = "Unterminated string constant";
        return (ERROR);
    }

    <<EOF>> {
        BEGIN(INITIAL);
        cursor = 0;
        cool_yylval.error_msg = "Unterminated string constant";
        return (ERROR);
    }

    . {
    }
}

<string_parse>{
    /* Finds end of string, adding \0 as a final char and restarting the cursor */
    "\""    {
        if (cursor < MAX_STR_CONST)
            string_buf[cursor] = '\0';
        cool_yylval.symbol = stringtable.add_string(string_buf);
        cursor = 0;
        BEGIN(INITIAL);
        return (STR_CONST);
    }

    /* Catches error Unterminated string for EOF and ESCAPED chars */
    {STRING_EXCPT} {
        BEGIN(INITIAL);
        cursor = 0;
        cool_yylval.error_msg = "Unterminated string constant";
        return (ERROR);
    }
    
    <<EOF>> {
        BEGIN(INITIAL);
        cursor = 0;
        cool_yylval.error_msg = "Unterminated string constant";
        return (ERROR);
    }

    /* Concatenates two charactes into their ESCAPED notation */
    \\n {
        string_buf[cursor] = '\n';
        cursor++;
    }

    \\t {
        string_buf[cursor] = '\t';
        cursor++;
    }

    \\0 {
        string_buf[cursor] = '0';
        cursor++;
    }

    \\b {
        string_buf[cursor] = '\b';
        cursor++;
    }

    \\f {
        string_buf[cursor] = '\f';
        cursor++;
    }

    /* Adds a escaped duoble quoutes to the buffer*/
    "\\\"" {
        string_buf[cursor] = '\"';
        cursor++;
    }

    /* Adds a escaped bar to the buffer.*/
    "\\\\" {
        string_buf[cursor] = '\\';
        cursor++;
    }

    /* Adds normal characters to the buffer, while keeping track of its size */
    . {
        if (cursor >= MAX_STR_CONST) {
            cool_yylval.error_msg = "String Constant too long";
            BEGIN(eat_string);
            return (ERROR);
        }
        string_buf[cursor] = *yytext;
        cursor++;
    }
}

{TRUE_KW} {
    stringtable.add_string(yytext);
    cool_yylval.boolean = true;
    return (BOOL_CONST);
} 

{FALSE_KW} {
    stringtable.add_string(yytext);
    cool_yylval.boolean = false;
    return (BOOL_CONST);
}

{NEW_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (NEW);
}

{IF_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (IF);
}

{CLASS_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (CLASS);
}

{INHERITS_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (INHERITS);
}

{LET_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (LET);
}

{IN_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (IN);
}

{THEN_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (THEN);
}

{ELSE_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (ELSE);
}

{FI_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (FI);
}

{WHILE_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (WHILE);
}

{LOOP_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (LOOP);
}

{POOL_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (POOL);
}

{OF_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (OF);
}

{ESAC_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (ESAC);
}

{NOT_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (NOT);
}

{ISVOID_KW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (ISVOID);
}

{TYPE_ID} {
    cool_yylval.symbol = idtable.add_string(yytext);
    return (TYPEID);
}

{OBJ_ID} {
    cool_yylval.symbol = idtable.add_string(yytext);
    return (OBJECTID);
}

{NUM} {
    cool_yylval.symbol = inttable.add_string(yytext);
    return (INT_CONST);
}

{DARROW} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (DARROW);
}

{ASSIGN} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (ASSIGN);
}

{OPERATORS} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (*yytext);
}

{NEWLINE} { curr_lineno++; }

{WHITE_SPACES} { }

<*>. {
    /* Every other character that is not in the languages alphabet results in an error */
    cool_yylval.error_msg = yytext;
    return (ERROR);
}

%%
