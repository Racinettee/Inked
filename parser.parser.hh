/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_HOME_STEVE_DOCUMENTS_INKED_PARSER_PARSER_HH_INCLUDED
# define YY_YY_HOME_STEVE_DOCUMENTS_INKED_PARSER_PARSER_HH_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TIDENTIFIER = 258,
    TINTEGER = 259,
    TDOUBLE = 260,
    TSLITERAL = 261,
    TBREAKKW = 262,
    TCONSTKW = 263,
    TCLSKW = 264,
    TCONTKW = 265,
    TDOKW = 266,
    TDBLKW = 267,
    TELSEKW = 268,
    TENDKW = 269,
    TEXITKW = 270,
    TFORKW = 271,
    TFCTNKW = 272,
    TIFKW = 273,
    TLONGKW = 274,
    TUNLKW = 275,
    TWHLKW = 276,
    TASKW = 277,
    TPROTOKW = 278,
    TQUOTE = 279,
    TSQUOTE = 280,
    TCEQ = 281,
    TCNE = 282,
    TCLT = 283,
    TCLE = 284,
    TCGT = 285,
    TCGE = 286,
    TEQUAL = 287,
    TLPAREN = 288,
    TRPAREN = 289,
    TLBRACE = 290,
    TRBRACE = 291,
    TCOMMA = 292,
    TDOT = 293,
    TPLUS = 294,
    TMINUS = 295,
    TMUL = 296,
    TDIV = 297
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 11 "/home/steve/Documents/Inked/parser.y" /* yacc.c:1909  */

  Node *node;
  NBlock *block;
  NExpression *expr;
  NStatement *stmt;
  NIdentifier *ident;
  NVariableDeclaration *var_decl;
  NClass* nclass;
  std::vector<NVariableDeclaration*> *varvec;
  std::vector<NExpression*> *exprvec;
  std::string *string;
  int token;

#line 111 "/home/steve/Documents/Inked/parser.parser.hh" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_HOME_STEVE_DOCUMENTS_INKED_PARSER_PARSER_HH_INCLUDED  */
