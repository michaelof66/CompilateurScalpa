%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/fct_utilitaires.h"
#include "include/function.h"
extern quad globalcode[100];
extern int nextquad;
extern int ntp;

void yyerror(char*);
int yylex();
void lex_free();


%}

%union {
	char *strval;
	int intval;
	struct P_symb **psymb;
	struct ident_list* list;
	struct quadop* exprval;
	struct {
		struct lpos* false;
		struct lpos* true;
	} tf;
	struct lpos* lpos;
	int actualquad;
	struct typelist *typelist;
}

%token PROGRAM  VAR
%token <strval> ID STR
%token <intval> NUM UNIT BOOL INT

%token <intval> PLUS AFFECT TIMES MINUS DIVIDE POWER TRUE FALSE
%token <intval> INF INFEQ SUP SUPEQ DIFF EQ
%token <intval> AND OR XOR NOT

%token SBEGIN SEND WRITE READ SFUNCTION REF
%token IF THEN ELSE WHILE DO RETURN


%type <list> identlist
%type <strval> atomictype typename
%type <intval> opb oprel
%type <exprval> E
%type <tf> cond
%type <actualquad> M
%type <lpos> instr tag sequence
%type <typelist> par parlist Elist

%nonassoc LOWER_THAN_ELSE
%nonassoc ELSE

%left RETURN
%left INF INFEQ SUP SUPEQ DIFF EQ
%right AFFECT
%left PLUS MINUS OR XOR
%left TIMES DIVIDE AND
%right POWER
%left NEG NOT
%left ID


%start program
%%


/* Grammaire à complémenté au fur et à mesure de l'implémentation */
program: PROGRAM ID vardecllist fundecllist instr
        ;

vardecllist: varsdecl
            | varsdecl ';' vardecllist
			| //element vide
            ;
varsdecl: VAR identlist ':' typename {create_symblist("var",$2, $4);}
        ;

identlist: ID                 {$$ = create_identlist($1);}
         | identlist ',' ID   {$$ = add_to_identlist($1, $3);}
         ;

typename: atomictype   {$$ = $1;}

atomictype: UNIT  {$$ = "unit";}
          | BOOL  {$$ = "bool";}
          | INT   {$$ = "int";}
          ;

fundecllist : fundecl ';' fundecllist
			|
			;

fundecl : SFUNCTION ID '(' parlist ')' ':' atomictype
		{
			create_symblist("function",create_identlist($2), $7);
			add_typelist_to_symb($2, $4);
		}
		vardecllist instr
		;

parlist : par {$$ = $1;}
		| par ',' parlist { $$ = add_to_typelist($1, $3);}
		| { $$ = NULL;}
		;

par : ID ':' typename
	{
		create_symblist("param",create_identlist($1), $3);
		$$ = create_typelist($1, $3);
	}
	//| REF ID ':' typename //a faire plus tard
	;


instr : ID AFFECT E //ID correspond a lvalue sans les listes
	  {
		  chk_symb_declared($1);
		  chk_symb_type($1,$3);
	 	  quad q = quad_make(Q_AFFECT, $3, NULL, quadop_name($1));
		  gencode(q);
		  $$ = crelist(nextquad);
	  }
	  | ID AFFECT cond
	  {
		  chk_symb_declared($1);
		  chk_symb_type($1,NULL);
		  quad q = quad_make(Q_AFFECT, reify($3.true, $3.false), NULL, quadop_name($1));
		  gencode(q);
		  $$ = crelist(nextquad);
	  }
	  | IF cond THEN M instr %prec LOWER_THAN_ELSE
	  {
		  $$ = NULL;
		  complete($2.true,$4);
		  $$ = concat($2.false,$5);
	  }
	  | IF cond THEN M instr ELSE tag M instr
	  {
		  complete($2.true, $4);
		  complete($2.false, $8);
		  $$ = concat($5, $7);
		  $$ = concat($$, crelist(nextquad));
		  quad q = quad_make(Q_GOTO,NULL,NULL,quadop_cst(-1));
		  gencode(q);
	  }
	  | WHILE M cond DO M instr
	  {
	  		complete($3.true, $5);
			complete($6, $2);
			quad q = quad_make(Q_GOTO,NULL,NULL,quadop_cst($2));
			gencode(q);
			$$ = $3.false;
    }
	  | RETURN E
	  {
		  quad q = quad_make(Q_RET,NULL,NULL,$2);
		  gencode(q);
		  $$ = crelist(nextquad);
	  }
	  | RETURN
	  {
		  quad q = quad_make(Q_RET,NULL,NULL,NULL);
		  gencode(q);
		  $$ = crelist(nextquad);
	  }
	  | SBEGIN sequence SEND {$$ = $2;}
	  | SBEGIN SEND  { }
	  | READ ID //lvalue a l'origine, a changer apres les tableaux
	  {
		  quad q = quad_make(Q_READ, NULL, NULL, quadop_name($2));
		  gencode(q);
		  $$ = crelist(nextquad);
	  }
	  | WRITE E
	  {
		  quad q = quad_make(Q_WRITE, NULL, NULL, $2);
		  gencode(q);
		  $$ = crelist(nextquad);

	  }
	  | ID '(' Elist ')'
	  {
		  chk_symb_declared($1);
		  chk_symb_fct($1);
		  typelist* l = get_typelist($1);
		  cmp_typelist($3, l);
		  int len = gencode_param($3);
		  quad q = quad_make(Q_CALL, quadop_cst(len), NULL, quadop_name($1));
		  gencode(q);
		  $$ = crelist(nextquad);
	  }
	  | ID '(' ')'
	  {
		  chk_symb_declared($1);
		  chk_symb_fct($1);
		  typelist* l = get_typelist($1);
		  cmp_typelist(NULL, l);
		  int len = gencode_param(NULL);
		  quad q = quad_make(Q_CALL, quadop_cst(len), NULL, quadop_name($1));
		  gencode(q);
		  $$ = crelist(nextquad);
	  }
	  ;

sequence : sequence semcol M instr semcol {complete($1, $3);$$ = $4;}
		 | instr semcol { $$ = $1;}
		 ;

semcol : ';' | ;

Elist : E
	  {
		  if ($1->type == QO_CST)
		  {
			  quadop *t = new_temp();
			  quad q = quad_make(Q_AFFECT, $1, NULL, t);
			  gencode(q);
			  create_symblist("var", create_identlist(t->u.name), "int");
			  char *s = get_symb_type_A(t->u.name);
			  $$ = create_typelist(t->u.name, s);
		  }
		  else
			  $$ = create_typelist($1->u.name, get_symb_type_A($1->u.name));
	  }
	  | E ',' Elist
	  {
		  if ($1->type == QO_CST)
		  {
			  quadop *t = new_temp();
			  quad q = quad_make(Q_AFFECT, $1, NULL, t);
			  gencode(q);
			  create_symblist("var", create_identlist(t->u.name), "int");
			  char *s = get_symb_type_A(t->u.name);
				  $$ = add_to_typelist(create_typelist(t->u.name, get_symb_type_A(t->u.name)), $3);
		  }
		  else
			  $$ = add_to_typelist(create_typelist($1->u.name, get_symb_type_A($1->u.name)), $3);

	  }
	  | cond ',' Elist {$$ = add_to_typelist(create_typelist((reify($1.true, $1.false))->u.name, "bool"), $3);}
	  | cond {$$ = create_typelist(reify($1.true, $1.false)->u.name, "bool");}
	  ;


E : ID { chk_symb_declared($1); $$ = quadop_name($1);}
| NUM {	$$ = quadop_cst($1);}
| STR { $$ = quadop_str($1);}
| '(' E ')' { $$ = $2;}
| ID '(' Elist ')'
| ID '(' ')'
| E %prec ID opb E //%prec ID pour regler conflits avec opb
{
	  if ($1->type == QO_STR || $3->type == QO_STR)
	  {
		  yyerror("erreur de type");
		  return 1;
	  }
	  quadop* t = new_temp();
	  create_symblist("var", create_identlist(t->u.name), "int");
	  quad q = quad_make($2, $1, $3, t);
	  gencode(q);
	  $$ = t;
}
| MINUS E %prec NEG
{
	if ($2->type == QO_STR)
	{
		yyerror("erreur de type");
		return 1;
	}
	quadop* t = new_temp();
	create_symblist("var", create_identlist(t->u.name), "int");
	quad q = quad_make(Q_NEG, $2, NULL, t);
	gencode(q);
	$$ = t;
}
;

cond : cond OR M cond
	{
		$$.true = concat ($1.true, $4.true);
		complete($1.false, $3);
		$$.false = $4.false;
	}
	| cond AND M cond
	{
		$$.false = concat ($1.false, $4.false);
		complete($1.true, $3);
		$$.true = $4.true;
	}
	| NOT cond
	{
		$$.true = $2.false;
		$$.false = $2.true;
	}
	| '(' cond ')'
	{
		$$.true = $2.true;
		$$.false = $2.false;
	}
	| E oprel E
	{
		chk_symb_typeE($1, $3);
		$$.true = crelist(nextquad);
		quad q = quad_make($2,$1,$3,NULL);
		gencode (q); // if ($1 rel $3)     goto ?
		$$.false = crelist(nextquad);
		quad q2 = quad_make(Q_GOTO,NULL,NULL,quadop_cst(-1));
		gencode(q2);
	}
	| TRUE
	{
		$$.true = crelist(nextquad);
		quad q2 = quad_make(Q_GOTO,NULL,NULL,quadop_cst(-1));
		gencode(q2);
		$$.false = NULL;
	}
	| FALSE
	{
		$$.false = crelist(nextquad);
		quad q2 = quad_make(Q_GOTO,NULL,NULL,quadop_cst(-1));
		gencode(q2);
		$$.true = NULL;
	}
	;

opb : PLUS { $$ = Q_PLUS; }
	| MINUS { $$ = Q_MINUS; }
	| TIMES { $$ = Q_TIMES; }
	| DIVIDE { $$ = Q_DIVIDE; }
	| POWER { $$ = Q_POWER; }
	;

oprel :	INF { $$ = Q_INF; }
	  | INFEQ { $$ = Q_INFEQ; }
	  | SUP { $$ = Q_SUP; }
	  | SUPEQ { $$ = Q_SUPEQ; }
	  | EQ { $$ = Q_EQ; }
	  | DIFF { $$ = Q_DIFF; }

M : { $$ = nextquad;}
;

tag:
{
	  $$ = crelist(nextquad);
	  quad q = quad_make(Q_GOTO,NULL,NULL,quadop_cst(-1));
	  gencode(q);
}
;

%%
void yyerror (char *s) {
	fprintf(stderr, "[Yacc] error: %s\n", s);
}


int main() {
	init_symb_tab();
	printf("Enter your code:\n");

	yyparse();
	printf("-----------------\nSymbol table:\n-----------------\n");
	print_tab();
	printf("Quad list:\n");
	for (int i=0; i<nextquad; i++) {
		printf("%i ", i);
		affiche(globalcode[i]);
	}

	// Be clean.===> Ofc As always
	lex_free();
	return 0;
}

/*
*	Test fonctionnel : creation de variable:
*
*	Ce test contient tout type de symbole afin de recouvrir la totalité
*	des cas : symboles doublons d'indice de hachage mais symb différent,
*	test avec symbole doublon (et donc refus d'ajouter dans la table),
*	ajout symbole classique.
*
*	./ar < file_test/test_declaration_var
*/
