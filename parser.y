%{
#include "node.h"
NBlock *programBlock; /* the top level root node of our final AST */

extern unsigned long long linecount;
extern int yylex();
void yyerror(const char *s) { printf("error at line: %llu %s\n", linecount, s); }
%}

/* Represents the many different ways we can access our data */
%union {
  Node *node;
  NBlock *block;
  NExpression *expr;
  NStatement *stmt;
  NIdentifier *ident;
  NVariableDeclaration *var_decl;
  NClass* nclass;
  std::vector<NVariableDeclaration*> *varvec;
  std::vector<NExpression*> *exprvec;
  std::vector<NIdentifier*>* identvec;
  std::string *string;
  int token;
}

/* Define our terminal symbols (tokens). This should
 match our tokens.l lex file. We also define the node type
 they represent.
*/
%token <string> TIDENTIFIER TINTEGER TDOUBLE TSLITERAL
%token <token> TBREAKKW TCONSTKW TCLSKW TCONTKW TDOKW TDBLKW TELSEKW
%token <token> TENDKW TEXITKW TFORKW TFCTNKW TIFKW TLONGKW TUNLKW
%token <token> TWHLKW TASKW TPROTOKW TNEWKW
%token <token> TQUOTE TSQUOTE TCOLON
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT
%token <token> TPLUS TMINUS TMUL TDIV


/* Define the type of node our nonterminal symbols represent.
 The types refer to the %union declaration above. Ex: when
 we call an ident (defined by union type ident) we are really
 calling an (NIdentifier*). It makes the compiler happy.
*/
%type <ident> ident
%type <expr> numeric expr string_literal
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <identvec> identifier_list
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl class_decl if_stmt new_stmnt
%type <token> comparison

/* Operator precedence for mathematical operators */
%left TPLUS TMINUS
%left TMUL TDIV

%start program

%%

program
: stmts { programBlock = $1; }
;

stmts
: stmt {
	$$ = new NBlock();
	$$->statements.push_back($<stmt>1);
}
| stmts stmt { $1->statements.push_back($<stmt>2); }
;

stmt : new_stmnt | if_stmt | class_decl | var_decl | func_decl | expr { $$ = new NExpressionStatement($1); }
   ;

block : stmts TENDKW { $$ = $1; }
    | TLBRACE stmts TRBRACE { $$ = $2; }
    | TLBRACE TRBRACE { $$ = new NBlock(); }
    ;

var_decl
: ident ident {
	$$ = new NVariableDeclaration(*$1, *$2);
}
| ident ident TEQUAL expr {
	// printf("Found variable def: %s: %s\n", $1->name.c_str(),$2->name.c_str());
	$$ = new NVariableDeclaration(*$1, *$2, $4);
}
;

if_stmt
: TIFKW TLPAREN expr TRPAREN stmts TENDKW {
	// somethin
	$$ = new IfNode($3, $5->statements, nullptr);  //expr, stmts, els);
	delete $5;
}
| TIFKW TLPAREN expr TRPAREN stmts TELSEKW stmts TENDKW {
	$$ = new IfNode($3, $5->statements, new ElseNode(
		$7->statements));
	delete $5;
};

identifier_list
: /*blank*/  {
	$$ = new IdentifierList();
}
| ident {
	$$ = new IdentifierList();
	$$->push_back($1);
}
| identifier_list TCOMMA ident {
	$1->push_back($3);
};

class_decl
: TCLSKW ident stmts TENDKW {
	$$ = new NClass($2->name,$3->statements);
	delete $2; delete $3;
}
| TCLSKW ident TCOLON identifier_list stmts TENDKW {
	$$ = new NClass($2->name, $5->statements, *$4);
	delete $2; delete $4; delete $5;
};

func_decl
/************************
*  Function prototype
***********************/
: TPROTOKW ident {
	VariableList vl;
	NIdentifier type("void");
	$$ = new NFunctionPrototype(type, *$2, vl);
}
| TPROTOKW ident TLPAREN func_decl_args TRPAREN TASKW ident {
	$$=new NFunctionPrototype(*$7,*$2,*$4);
	delete $4;
}
/**************************
* Function definition: function name ... end
*************************/
| TFCTNKW ident block {
	//printf("Found function def: %s\n", $2->name.c_str());
	VariableList vl;
	//NIdentifier type("void");
	$$ = new NFunctionDeclaration("void", *$2, vl, *$3);
}
/********************************
* Function def: function name(...) ... end
********************************/
| TFCTNKW ident TLPAREN func_decl_args TRPAREN block {
	// NIdentifier type("void");
	$$ = new NFunctionDeclaration("void", *$2, *$4, *$6);
}
| TFCTNKW ident TASKW ident block {
	VariableList vl;
	$$ = new NFunctionDeclaration(*$4, *$2, vl, *$5);
}
| TFCTNKW ident TLPAREN func_decl_args TRPAREN TASKW ident block {
	$$ = new NFunctionDeclaration(*$7, *$2, *$4, *$8);
	delete $4;
};

func_decl_args : /*blank*/  { $$ = new VariableList(); }
        | var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
        | func_decl_args TCOMMA var_decl { $1->push_back($<var_decl>3); }
        ;

ident : TIDENTIFIER { $$ = new NIdentifier(*$1); delete $1; }
    ;

string_literal : TSLITERAL { $$ = new NString(*$1); delete $1; } ;

numeric : TINTEGER { $$ = new NInteger(atol($1->c_str())); delete $1; }
      | TDOUBLE { $$ = new NDouble(atof($1->c_str())); delete $1; }
      ;

expr : ident TEQUAL expr { $$ = new NAssignment(*$<ident>1, *$3); }
   | ident TLPAREN call_args TRPAREN { $$ = new NMethodCall(*$1, *$3); delete $3; }
   | ident { $<ident>$ = $1; }
   | numeric
   | string_literal
   | expr comparison expr { $$ = new NBinaryOperator(*$1, $2, *$3); }
   | TLPAREN expr TRPAREN { $$ = $2; }
   ;

call_args : /*blank*/  { $$ = new ExpressionList(); }
        | expr { $$ = new ExpressionList(); $$->push_back($1); }
        | call_args TCOMMA expr  { $1->push_back($3); }
        ;

comparison : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE
         | TPLUS | TMINUS | TMUL | TDIV
         ;

new_stmnt : TNEWKW ident {
				// add a call to malloc
				// bitcast to the type of the ident
				$$ = new NewNode($2->name);
				delete $2;
			}

%%
