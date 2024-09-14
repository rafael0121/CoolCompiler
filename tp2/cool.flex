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
int cursor = 0;

extern int curr_lineno;
extern int verbose_flag;

extern YYSTYPE cool_yylval;

/*
 *  Add Your own definitions here
 */

%}
%option noyywrap

%x com_block
%x string_parse
%x eat_string

DIGIT      [0-9]
NUM        [+-]?{DIGIT}+
OBJ_ID     [a-z][0-9a-zA-Z_]*
TYPE_ID    ([A-Z][0-9a-zA-Z_]*)|self
CLOSE_COM   \*)

/* Keywords are case insensitive, except true and false, which must start with lowercase */
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

/* Special characters and exceptions that may and cannot appear on a string */
STRING_EXCPT    [EOF\0\b\t\n\f]

WHITE_SPACES    [\f\r\t\v ]+
NEWLINE         \n

OPERATORS       [+"/"\-"*"=<.~,;:()@{}]

%%

<INITIAL>{
    "(*"    BEGIN(com_block);
    "\""    BEGIN(string_parse);
    "*)" {
        cool_yylval.error_msg = "Unmatched *)";
        return (ERROR);
    }
}

<com_block>{
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

<eat_string>{
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

    . {
    }
}

<string_parse>{
    "\""    {
        if (cursor < MAX_STR_CONST)
            string_buf[cursor] = '\0';
        cool_yylval.symbol = stringtable.add_string(string_buf);
        cursor = 0;
        BEGIN(INITIAL);
        return (STR_CONST);
    }

    {STRING_EXCPT} {
        BEGIN(INITIAL);
        cursor = 0;
        cool_yylval.error_msg = "Unterminated string constant";
        return (ERROR);
    }

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

{OPERATORS} {
    cool_yylval.symbol = stringtable.add_string(yytext);
    return (*yytext);
}

{NEWLINE} { curr_lineno++; }

{WHITE_SPACES} { }

<*>. {
    cool_yylval.error_msg = yytext;
    return (ERROR);
}

%%
