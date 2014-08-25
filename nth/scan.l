%option noyywrap yylineno debug

%{
  #include <cerrno>
  #include <cstdlib>
  #include <string>
  #include "parse.hh"
  #include "driver.h"

  static yy::location loc;

  int line_count(char *str, int n, char m) {
    int count=0;
    for (int i=0; i < n; i++) {
      if (str[i] == m) count++;
    }
    return count;
  }
%}

%x STR
%x COMMENT

EXP ([Ee][-+]?[0-9]+)
DIGIT [0-9]
LEADING_DIGIT [1-9]

%{
  // Runs for every match
  #define YY_USER_ACTION loc.columns(yyleng);
%}

%%

%{
  // Run each time yylex() is called
  loc.step();
%}


  /* basic math */
"+"       return yy::parser::make_PLUS(loc);
"-"       { return yy::parser::make_MINUS(loc); }
"*"       { return yy::parser::make_TIMES(loc); }
"/"       { return yy::parser::make_DIVIDE(loc); }
"="       { return yy::parser::make_ASSIGN(loc); }
"^"       { return yy::parser::make_POW(loc); }
"%"       { return yy::parser::make_MODULO(loc); }

   /* bitwise */
"|"       { return yy::parser::make_BIT_OR(loc); }
"&"       { return yy::parser::make_BIT_AND(loc); }
"~"       { return yy::parser::make_BIT_NOT(loc); }

 /* misc */
"."       { return yy::parser::make_PERIOD(loc);  }
","       { return yy::parser::make_COMMA(loc); }
"("       { return yy::parser::make_LPAREN(loc); }
")"       { return yy::parser::make_RPAREN(loc); }
"["       { return yy::parser::make_LBRACKET(loc); }
"]"       { return yy::parser::make_RBRACKET(loc); }
"{"       { return yy::parser::make_LCURLY(loc); }
"}"       { return yy::parser::make_RCURLY(loc); }
":"       { return yy::parser::make_COLON(loc); }
";"       { return yy::parser::make_SEMICOLON(loc); }

"<<"      { return yy::parser::make_LSHIFT(loc); }
">>"      { return yy::parser::make_RSHIFT(loc); }
"=>"      { return yy::parser::make_HASH_ROCKET(loc); }
"..."     { return yy::parser::make_TRIPLE_DOT(loc); }
".."      { return yy::parser::make_DOUBLE_DOT(loc); }

  /* comparison */
"=="      { return yy::parser::make_CMP(nth::Comparison::Type::Equality, loc); }
"!="      { return yy::parser::make_CMP(nth::Comparison::Type::Inequality, loc); }
"<"       { return yy::parser::make_CMP(nth::Comparison::Type::LessThan, loc); }
">"       { return yy::parser::make_CMP(nth::Comparison::Type::GreaterThan, loc); }
"<="      { return yy::parser::make_CMP(nth::Comparison::Type::LessThanOrEqualTo, loc); }
">="      { return yy::parser::make_CMP(nth::Comparison::Type::GreaterThanOrEqualTo, loc); }

  /* boolean */
"&&"      { return yy::parser::make_AND(loc); }
"||"      { return yy::parser::make_OR(loc); }
"!"       { return yy::parser::make_NOT(loc); }

  /* keywords */
"if"      { return yy::parser::make_IF(loc); }
"else"    { return yy::parser::make_ELSE(loc); }
"def"     { return yy::parser::make_DEF(loc); }
"val"     { return yy::parser::make_VAL(loc); }
"class"   { return yy::parser::make_CLASS(loc); }

  /* literals */
"true"    { return yy::parser::make_TRUE(loc); }
"false"   { return yy::parser::make_FALSE(loc); }

  /* integers and floats */
[-+]?{DIGIT}+"."{DIGIT}+{EXP}?  { return yy::parser::make_FLOAT(strtod(yytext, NULL), loc); }
[-+]?[1-9]{DIGIT}*{EXP}         { return yy::parser::make_FLOAT(strtod(yytext, NULL), loc); }

[-+]?(0|([1-9]{DIGIT}*))        { return yy::parser::make_INT(strtol(yytext, NULL, 10), loc); }
"0x"[a-fA-F0-9]+                { return yy::parser::make_INT(strtol(yytext, NULL, 16), loc); }
"0b"[01]+                       { return yy::parser::make_INT(strtol(yytext+2, NULL, 2), loc); }
0[0-7]+                         { return yy::parser::make_INT(strtol(yytext, NULL, 8), loc); }

  /* strings */
\"[^"]*\"   { /* is the character before close quote a \ ? */
              if (yytext[yyleng-2] == '\\') {
                yyless(yyleng-1); /* return yy::parser::make_last quote */
                yymore(); /* append next string */
              } else {
                return yy::parser::make_STRING(yytext, loc);
              }
            }

  /* Whitespace */

[ \s\t]+     { loc.step(); }
  /* newlines are significant! */
\n+          { loc.lines(yyleng); return yy::parser::make_NEWLINE(loc); }

  /* Comments */

^"//".*\n*      { loc.lines(line_count(yytext, yyleng, '\n')); loc.step(); }
"//".*\n*      {
                  loc.lines(line_count(yytext, yyleng, '\n')-1);
                  loc.step();
                  yyless(yyleng-1); // put back one newline
                }

  /* Multiline Comments */
"/*"                  { BEGIN(COMMENT); }
<COMMENT>\n           { loc.lines(1); }
<COMMENT>[^\*\n]+|.
<COMMENT>"*/"\n*         { BEGIN(INITIAL); loc.lines(line_count(yytext, yyleng, '\n')); loc.step(); }
<COMMENT><<EOF>>      { printf("Line %d: Unterminated comment\n", yylineno); }
  /* Identifiers */
  /* yylval->build<std::string>() = std::string(yytext); */
[a-zA-Z][a-zA-Z0-9_]*  { return yy::parser::make_IDENT(yytext, loc); }


.         { driver.error(loc, "invalid character"); }
<<EOF>>   { return yy::parser::make_END(loc); }
%%
