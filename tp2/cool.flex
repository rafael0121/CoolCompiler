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

DIGIT    [0-9]+
ID       [a-z][0-9a-zA-Z_]*
TYPE_ID  [A-Z][0-9a-zA-Z]*

COMMENT_LINE  --.*
COMMENT_BLOCK \(\*(.|\n)*\*\)
IF       if

TYPES   (Int|String|Object|Class|SELF_TYPE|self)

\\ These are case insensitive, except true and false, which must start with lowercase, need to think how that works
KEYOWRDS    (new|class|inherits|let|in|not|if|else|then|fi|while|loop|pool|case|of|esac|true|false|isvoid)

WHITE_SPACES    (\n|\f|\r|\t|\v| )+

%%

{COMMENT_LINE} {
    cool_yylval.symbol = inttable.add_string(yytext);
    return (LE);
}

{COMMENT_BLOCK} {
    cool_yylval.symbol = inttable.add_string(yytext);
    return (LE);
}

{IF} {
    cool_yylval.symbol = inttable.add_string(yytext);
    return (IF);
}

{DIGIT} {
    cool_yylval.symbol = inttable.add_string(yytext);
    return (INT_CONST);
}

{} {
}

%%

int main(int argc, char* argv){
    \\ Must test how the lexer iterates through the input file
    cool_yylex* lexer = new cool_yylex;
    while(lexer->yylex() != 0)
        ;
    return 0;
}