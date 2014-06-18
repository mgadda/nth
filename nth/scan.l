%option noyywrap nodefault yylineno

%{
  #include "y.tab.h"
%}

%x STR
%x COMMENT

EXP ([Ee][-+]?[0-9]+)
DIGIT [0-9]

%%

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

  /* comparison */
"=="      { return CMP; }
"!="      { return CMP; }
"<"       { return CMP; }
">"       { return CMP; }
"<="      { return CMP; }
">="      { return CMP; }

  /* boolean */
"&&"      { return AND; }
"||"      { return OR; }
"!"       { return NOT; }

  /* keywords */
"if"      { return IF; }
"else"    { return ELSE; }
"def"     { return DEF; }
"val"     { return VAL; }
"class"   { return CLASS; }

  /* literals */
"true"          { return TRUE; }
"false"         { return FALSE; }

  /* integers and floats */
[-+]?{DIGIT}+"."{DIGIT}+{EXP}?  { yylval.f = strtod(yytext, NULL); return FLOAT; }
[-+]?{DIGIT}+{EXP}?             { yylval.f = strtod(yytext, NULL); return FLOAT; }

[-+]?{DIGIT}+                   { yylval.l = strtol(yytext, NULL, 10); return INT; }
"0x"[a-fA-F0-9]+                { yylval.l = strtol(yytext, NULL, 16); return INT; }
"0b"[01]+                       { yylval.l = strtol(yytext, NULL, 2); return INT; }
0[0-7]+                         { yylval.l = strtol(yytext, NULL, 8); return INT; }

  /* strings */
\"[^"]*\"   { /* is the character before close quote a \ ? */
              if (yytext[yyleng-2] == '\\') {
                yyless(yyleng-1); /* return last quote */
                yymore(); /* append next string */
              } else {
                yylval.str = strdup(yytext);
                return STRING;
              }
            }

[ \s\t\n]+

  /* Comments */

"//".*    

  /* Multiline Comments */
"/*"                  { BEGIN(COMMENT); }
<COMMENT>([^\*]|\n)+|.
<COMMENT>"*/"         { BEGIN(INITIAL); }
<COMMENT><<EOF>>      { yyerror("Line %d: Unterminated comment\n", yylineno); }

  /* Identifiers */
[a-zA-Z][a-zA-Z0-9]*  { yylval.str = strdup(yytext); return IDENT; }


.     { yyerror("Unknown character '%c'\n", *yytext); }

%%
