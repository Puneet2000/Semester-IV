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

int nests = 0;

%}

/*
 * Define names for regular expressions here.
 */

DARROW          =>
TYPEID      [A-Z][_a-zA-Z0-9]*
OBJECTID    [a-z][_a-zA-Z0-9]*
ASSIGN      <-
LE          <=
INT       [0-9]+
WHITESPACES  [ \t\f\r\v]+
VALID_CHARS     "+"|"-"|"*"|"/"|"@"|"<"|"="|"("|")"|"{"|"}"|";"|":"|"."|","|"~"

%x nest_comments string_const end

%%

 /*
  *  Nested comments
  * <nest_comments> is a state variable that marks the beginining of nested comments. 
  * <nest_comments><<EOF>> throws an error if EOF is encountered when all comments are still not closed.
  */

"*)"                    { cool_yylval.error_msg = "Unmatched *)";
                          return (ERROR);}

"(*"                    { nests++; 
                          BEGIN(nest_comments);}

<nest_comments>"(*"     { nests++;}

<nest_comments>"*)"    {nests--;
                          if(nests==0)
                            BEGIN(INITIAL);
                          else if(nests < 0){
                            cool_yylval.error_msg = "Unmatched *)";
                            nests = 0;
                            BEGIN(INITIAL);
                            return (ERROR);
                          }} 


<nest_comments>\n          { curr_lineno++;}
<nest_comments>WHITESPACES  ;
<nest_comments>.            ;

<nest_comments><<EOF>>    {BEGIN(INITIAL);
                              if(nests >0){
                                cool_yylval.error_msg = "EOF in comment.";
                                nests=0;
                                return ERROR;
                              }} 


 /*
  *  The multiple-character operators.
  */
{DARROW}    { return (DARROW); }
{LE}        { return (LE); }
{ASSIGN}    { return (ASSIGN); }

 /*
  * Keywords are case-insensitive except for the values true and false,
  * which must begin with a lower-case letter.
  * ?i: regex is used to match case insensitive keywords except true and false where first character should be lower case.
  * INT_CONST are added to inttable and TYPEID and OBJECTID to idtable.
  * "--"(.)* is used to math single line comments and ignore them.
  * \n when encountered increases the current line number.
  */

(?i:class)       {return (CLASS);}
(?i:else)        {return (ELSE);}
(?i:if)          {return (IF);}
(?i:fi)          {return (FI);}
(?i:in)          {return (IN);}
(?i:inherits)    {return (INHERITS);}
(?i:let)         {return (LET);}
(?i:loop)        {return (LOOP);}
(?i:pool)        {return (POOL);}
(?i:then)        {return (THEN);}
(?i:while)       {return (WHILE);}
(?i:case)        {return (CASE);}
(?i:esac)        {return (ESAC);}
(?i:of)          {return (OF);}
(?i:not)         {return (NOT);}
(?i:new)         {return (NEW);}
(?i:isvoid)      {return (ISVOID);}

t(?i:rue)        {cool_yylval.boolean = true;
                  return(BOOL_CONST); }
f(?i:alse)       {cool_yylval.boolean = false;
                    return(BOOL_CONST); }

{WHITESPACES}   ;
{INT}         {cool_yylval.symbol = inttable.add_string(yytext);
                return (INT_CONST);}
{TYPEID}      {cool_yylval.symbol = idtable.add_string(yytext);
                return (TYPEID);}
{OBJECTID}    {cool_yylval.symbol = idtable.add_string(yytext);
                return (OBJECTID);}
{VALID_CHARS} {return int(yytext[0]);}
"--"(.)*      ;
\n             curr_lineno++;


 /*
  *  String constants (C syntax)
  *  Escape sequence \c is accepted for all characters c. Except for 
  *  \n \t \b \f, the result is c.
  * <string_const> is a state variable that marks the beginining of string constants.
  * <string_const>"\\x" where x is in [f,n,t,b] are used to store escape sequences.
  * Whenever a ERROR is return while lexing string literals, all the enteries of string_buf are initialised to '\0' character
  * to avoid any garbage data using ```memset``` operation.
  * <string_const>"\\".  matches all the \c type characters, it also converts \0 to 0.
  * A final wildcard('.') is used to match all unwanted characters and throw an error.
  */


"\""      {
            BEGIN(string_const);
            memset(string_buf, '\0', MAX_STR_CONST);
            string_buf_ptr = string_buf;
          }
<string_const>"\""  {
                      *string_buf_ptr = '\0';
                      cool_yylval.symbol = stringtable.add_string(string_buf);
                      BEGIN(INITIAL);
                      return STR_CONST;
                    }

<string_const><<EOF>>   {
                          cool_yylval.error_msg = "EOF in string constant";
                          BEGIN(INITIAL);
                          return ERROR;
                        }

<string_const>\0       {
                          memset(string_buf, '\0', MAX_STR_CONST);
                          cool_yylval.error_msg = "String contains null character";
                          BEGIN(end);
                          return (ERROR);
                        }
                        
<string_const>\n        {
                          memset(string_buf, '\0', MAX_STR_CONST);
                          BEGIN(INITIAL);
                          cool_yylval.error_msg = "Unterminated string constant";
                          curr_lineno++;
                          return (ERROR);
                        }
<string_const>"\\n"         {
                              *string_buf_ptr++ = '\n';
                              if(string_buf_ptr - string_buf > MAX_STR_CONST-1){
                                  cool_yylval.error_msg = "String constant too long";
                                  memset(string_buf, '\0', MAX_STR_CONST);
                                  BEGIN(end);
                                  return ERROR;
                              }
                            }

<string_const>"\\t"         {
                              *string_buf_ptr++ = '\t';
                              if(string_buf_ptr - string_buf > MAX_STR_CONST-1){
                                  cool_yylval.error_msg = "String constant too long";
                                  memset(string_buf, '\0', MAX_STR_CONST);
                                  BEGIN(end);
                                  return ERROR;
                              }
                            }

<string_const>"\\b"         {
                              *string_buf_ptr++ = '\b';
                              if(string_buf_ptr - string_buf > MAX_STR_CONST-1){
                                  cool_yylval.error_msg = "String constant too long";
                                  memset(string_buf, '\0', MAX_STR_CONST);
                                  BEGIN(end);
                                  return ERROR;
                              }
                            }

<string_const>"\\f"          {
                              *string_buf_ptr++ = '\f';
                              if(string_buf_ptr - string_buf > MAX_STR_CONST-1){
                                  cool_yylval.error_msg = "String constant too long";
                                  memset(string_buf, '\0', MAX_STR_CONST);
                                  BEGIN(end);
                                  return ERROR;
                              }
                            }


<string_const>"\\".    {  
                              *string_buf_ptr++ = yytext[1];
                              if(string_buf_ptr - string_buf > MAX_STR_CONST-1){
                                  cool_yylval.error_msg = "String constant too long";
                                  memset(string_buf, '\0', MAX_STR_CONST);
                                  BEGIN(end);
                                  return ERROR;
                              }
                            }

<string_const>.             {
                              *string_buf_ptr++ = *yytext;
                              if(string_buf_ptr - string_buf > MAX_STR_CONST-1){
                                  cool_yylval.error_msg = "String constant too long";
                                  memset(string_buf, '\0', MAX_STR_CONST);
                                  BEGIN(end);
                                  return ERROR;
                              }
                            }

<end>\n           { curr_lineno++; BEGIN(INITIAL);}
<end>"\""            {BEGIN(INITIAL);}
<end>[^\n|"] 

\n    curr_lineno++;
WHITESPACES       ;

.   {
      cool_yylval.error_msg = yytext;
      return ERROR;
}

%%
