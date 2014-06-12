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
[-+]?{DIGIT}+"."{DIGIT}+{EXP}?  { return NUMBER; }
[-+]?{DIGIT}+{EXP}?             { return NUMBER; }
[-+]?{DIGIT}+"."{DIGIT}+        { return FLOAT; }
[-+]?{DIGIT}+                   { return INT; }
"0x"[a-fA-F0-9]+                { return HEX_INT; }
"0b"[01]+                       { return BIN_INT; }
0[0-7]+                         { return OCT_INT; }

  /* strings */
\"[^"]*\"   { /* is the character before close quote a \ ? */
              if (yytext[yyleng-2] == '\\') {
                yyless(yyleng-1); /* return last quote */
                yymore(); /* append next string */
              } else {
                /* process string */
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
[a-zA-Z][a-zA-Z0-9]*  { return IDENT; }


.     { yyerror("Unknown character '%c'\n", *yytext); }

%%
