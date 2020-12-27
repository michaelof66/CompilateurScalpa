#include "../include/token_tab.h"

P_symb** symb_tab;

/******************** begining of the functions ********************/

void init_symb_tab()
{
    symb_tab = malloc(SIZE_HASH_TABLE * sizeof(P_symb *));
}

ident_list *create_symblist(char *var, ident_list *list, char *typename)
{
    ident_list *list_parcour = list;
    while (list_parcour != NULL)
    {
        create_symb(var, typename, list_parcour->name);
        list_parcour = list_parcour->next;
    }
    return list;
}

int hachage(char *chaine)
{
    int i = 0, nombreHache = 0;
    for (i = 0; chaine[i] != '\0'; i++)
    {
        nombreHache += chaine[i];
    }
    nombreHache %= SIZE_HASH_TABLE;
    return nombreHache;
}

void create_symb(char *var, char *typename, char *id)
{
    P_symb *symb = malloc(sizeof(P_symb));
    symb->next_doublon = NULL;
    symb->name = malloc(SYMBNAME_SIZE);
    CHK_MALLOC(symb->name);
    sprintf(symb->name, "%s", id);

    if (strcmp(var, "var") == 0)
        symb->type_I = VARIABLE;

    if (strcmp(typename, "int") == 0)
        symb->type_A = T_INT;
    else if (strcmp(typename, "bool") == 0)
        symb->type_A = T_BOOL;
    else if (strcmp(typename, "unit") == 0)
        symb->type_A = T_UNIT;

    symb->idx = hachage(id);
    symb->addr = 0;  // on verra plus tard
    symb->scope = 0; //idem
    if (!add_symb(symb))
    {
        printf("we free the symb \"%s\" created because already exist in the symbole table\n", id);
        free(symb);
    }

    return;
}

int add_symb(P_symb *symb)
{
    int already_in = search_symb(symb);

    if (already_in == -1)
    {
        if (symb_tab[symb->idx] != NULL)
        {
            P_symb *symb_parcour = symb_tab[symb->idx];
            while (symb_parcour->next_doublon != NULL)
            {
                symb_parcour = symb_parcour->next_doublon;
            }
            symb_parcour->next_doublon = symb;
            printf("hash table case wasn't empty, symb %s added\n", symb->name);
        }
        else
        { // hash table case empty, just copy the symb
            symb_tab[symb->idx] = symb;
            printf("hash table case was empty, symb %s added\n", symb->name);
        }
        return 1;
    }
    return 0; //symb already exist, wasnt added
}

int search_symb(P_symb *symb)
{

    P_symb *symb_loop = symb_tab[symb->idx];

    if (symb_loop != NULL)
    {
        while (symb_loop != NULL)
        {
            if (same_symb(symb, symb_loop))
            {
                printf("symb present\n");
                return 1;
            }
            symb_loop = symb_loop->next_doublon;
        }
    }
    return -1;
}

int search_symb_char(char *id)
{
	int pos = hachage(id);

	if (symb_tab[pos] == NULL)
		return -1;

	P_symb *symb_loop = symb_tab[pos];

	while (symb_loop != NULL)
	{
		if (!strcmp(symb_loop->name, id))
			return 1;
		symb_loop = symb_loop->next_doublon;
	}
	return -1;
}

void chk_symb_declared(char *id)
{
	if (search_symb_char(id) == -1) //if symb not declared
	{
		printf("error : %s not declared\n", id);
		exit(1);
	}
	return ;
}

int same_symb(P_symb *symb1, P_symb *symb2)
{
    if (!strcmp(symb1->name, symb2->name))
    {
        if (symb1->type_I == symb2->type_I && symb1->type_A == symb2->type_A)
            return 1;
    }
    return 0;
}

void chk_symb_type(char *id, quadop* op1)
{
	int pos = hachage(id);
	P_symb *symb_loop = symb_tab[pos];

	while (strcmp(symb_loop->name, id))
		symb_loop = symb_loop->next_doublon;

	if (op1 == NULL && symb_loop->type_I == VARIABLE
		&& symb_loop->type_A == T_BOOL) // if op1 is a condition and id a boolean
		return;
	else if (op1 == NULL )
	{
		printf("erreur typage de %s\n", id);
		exit(1);

	}

	if (symb_loop->type_I == VARIABLE && symb_loop->type_A == T_INT
		&& op1->type == QO_CST) // if op1 and id are int
		return;
	if (op1->type == QO_NAME) // if op1 is an identifier
	{
		int pos2 = hachage(op1->u.name);
		P_symb *symb_loop2 = symb_tab[pos2];

		while (strcmp(symb_loop2->name, op1->u.name))
			symb_loop2 = symb_loop2->next_doublon;

		if (symb_loop->type_A == symb_loop2->type_A
			&& symb_loop->type_I == symb_loop2->type_I)
			return;
	}
	//otherwise
	printf("erreur typage de %s\n", id);
	exit(1);
}


void chk_symb_typeE(quadop* op1, quadop* op2)
{
	if (op1->type == QO_STR || op2->type == QO_STR)
	{
		printf("erreur typage comparaison\n");
		exit(1);
	}
	else if (op1->type == QO_CST && op2->type == QO_NAME)
		chk_symb_type(op2->u.name, op1);
	else if (op1->type == QO_NAME && op2->type == QO_CST)
		chk_symb_type(op1->u.name, op2);

	return;
}


ident_list *create_identlist(char *ident)
{
    ident_list *my_list = malloc(sizeof(ident_list));
    my_list->name = malloc(SYMBNAME_SIZE);
    sprintf(my_list->name, "%s", ident);
    my_list->next = NULL;
    return my_list;
}

ident_list *add_to_identlist(ident_list *old_list, char *ident)
{
    ident_list *new_ident = malloc(sizeof(ident_list));
    new_ident->name = malloc(SYMBNAME_SIZE);
    sprintf(new_ident->name, "%s", ident);
    new_ident->next = NULL;
    ident_list *loop_ident = old_list;
    while (loop_ident->next != NULL)
        loop_ident = loop_ident->next;
    loop_ident->next = new_ident;
    return old_list;
}

void print_symb(P_symb *symb)
{
    if (strlen(symb->name) >= 8)
        printf("|\t\t%i\t\t|\t\t%s\t|", symb->idx, symb->name);
    else
        printf("|\t\t%i\t\t|\t\t%s\t\t|", symb->idx, symb->name);
	if (symb->type_I == VARIABLE)
		switch (symb->type_A) {
			case T_INT:
				printf("int|\n");
				break;
			case T_BOOL:
				printf("bool|\n");
				break;
			case T_UNIT:
				printf("unit|\n");
				break;
		}
	else
		printf("\n" );
}

void print_tab()
{
    printf("|-------------------------------|");
    printf("-------------------------------|\n");
    printf("|\t\tindex\t\t|\t\tident\t\t|\n");
    printf("|-------------------------------|");
    printf("-------------------------------|\n");
    for (int i = 0; i < SIZE_HASH_TABLE; i++)
    {
        if (symb_tab[i] != NULL)
        {
            print_symb(symb_tab[i]);
            if (symb_tab[i]->next_doublon != NULL)
            {
                P_symb *symb_parcour = symb_tab[i]->next_doublon;
                while (symb_parcour != NULL)
                {
                    print_symb(symb_parcour);
                    symb_parcour = symb_parcour->next_doublon;
                }
            }
        }
    }
    printf("|-------------------------------|");
    printf("-------------------------------|\n");
}
