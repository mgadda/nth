%option noyywrap yylineno

%{
  #include <cstdlib>
  #include "parse.hh"
  #include "driver.h"
  
  #define YY_DECL int yylex(yy::parser::semantic_type *yylval, \
                            yy::parser::location_type *yylloc, \
                            nth::Driver &ctx)
  #define YY_USER_ACTION yylloc->columns (yyleng);

  typedef yy::parser::token token;

  extern void yyerror(const char *, ...);
%}

%x STR
%x COMMENT

EXP ([Ee][-+]?[0-9]+)
DIGIT [0-9]
LEADING_DIGIT [1-9]

%%

%{
  // start where previous token ended
  yylloc->step();
%}


  /* basic math */
"+" |
"-" |
"*" |
"/" |
"=" |
"^" |
"%" |

   /* bitwise */
"|" |
"&" |

 /* misc */
"." | 
"," |
"(" |
")" |
"[" |
"]" |
"{" |
"}" |
":"       { return yytext[0]; }

"<<"      { return token::LSHIFT; }
">>"      { return token::RSHIFT; }
"=>"      { return token::HASH_ROCKET; }
"..."     { return token::TRIPLE_DOT; }
".."      { return token::DOUBLE_DOT; }

  /* comparison */
"=="      { return token::CMP; }
"!="      { return token::CMP; }
"<"       { return token::CMP; }
">"       { return token::CMP; }
"<="      { return token::CMP; }
">="      { return token::CMP; }

  /* boolean */
"&&"      { return token::AND; }
"||"      { return token::OR; }
"!"       { return token::NOT; }

  /* keywords */
"if"      { return token::IF; }
"else"    { return token::ELSE; }
"def"     { return token::DEF; }
"val"     { return token::VAL; }
"class"   { return token::CLASS; }

  /* literals */
"true"    { return token::T_TRUE; }
"false"   { return token::T_FALSE; }

  /* integers and floats */
[-+]?{DIGIT}+"."{DIGIT}+{EXP}?  { yylval->build<float>() = strtod(yytext, NULL); return token::FLOAT; }
[-+]?[1-9]{DIGIT}*{EXP}         { yylval->build<float>() = strtod(yytext, NULL); return token::FLOAT; }

[-+]?(0|([1-9]{DIGIT}*))        { yylval->build<long>() = strtol(yytext, NULL, 10); return token::INT; }
"0x"[a-fA-F0-9]+                { yylval->build<long>() = strtol(yytext, NULL, 16); return token::INT; }
"0b"[01]+                       { yylval->build<long>() = strtol(yytext, NULL, 2); return token::INT; }
0[0-7]+                         { yylval->build<long>() = strtol(yytext, NULL, 8); return token::INT; }

  /* strings */
\"[^"]*\"   { /* is the character before close quote a \ ? */
              if (yytext[yyleng-2] == '\\') {
                yyless(yyleng-1); /* return token::last quote */
                yymore(); /* append next string */
              } else {
                yylval->build<std::string>() = std::string(yytext);
                return token::STRING;
              }
            }

  /* Whitespace */

[ \s\t]     { yylloc->step(); }
\n          { yylloc->lines(1); }

  /* Comments */

"//".*      { yylloc->step(); }

  /* Multiline Comments */
"/*"                  { BEGIN(COMMENT); }
<COMMENT>\n           { yylloc->lines(1); }
<COMMENT>[^\*\n]+|.
<COMMENT>"*/"         { BEGIN(INITIAL); }
<COMMENT><<EOF>>      { printf("Line %d: Unterminated comment\n", yylineno); }

  /* Identifiers */
[a-zA-Z][a-zA-Z0-9]*  { yylval->build<std::string>() = std::string(yytext); return token::IDENT; }


.     { printf("Unknown character '%c'\n", *yytext); }

%%
