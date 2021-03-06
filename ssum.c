#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MLEN 	80
#define MRIBBON 1000
#define MRULES	2000
#define MRS 	100
#define MPNAME 	10

char rdel[] = ":";
char edel[] = ";";


int normalize(char * il)
{
	return 0;
}

// function returns the offset of rule where the expression match, or -1 if there is no match.
int checkExpr(char * il, char * rules)
{
	char *string3 = strstr(rules, il);
	int addr;

	//printf("%s\n", rules);
	//printf("%s\n", il);

	if (string3 != NULL)
	{
		addr = string3 - rules;
		return addr;
	}
	return -1;
}

int findDef(char * pname, char * rules)
{
	char pn[MLEN];
	int lpn = strlen(pname);
	memset(pn, 0 , MLEN);
	strcpy (pn, pname);
	pn[lpn] = ' ';
	pn[lpn+1] = '=';
	pn[lpn+2] = '=';

	return checkExpr(pn, rules);
}

int getRule(int soffs, char * rules)
{
	// looking for the beggining of rule
	int i, b;

	for(i = soffs; i>=0; i--)
	{
		if (rules[i] == rdel[0]) 
		{
			b=i+1;
			break;
		}
		else b = i;
	}
	// now b is the index of the first byte of rule
	return b;
}

// just replace one part to another.
int applyRule(char * il, char * rule)
{
	int rl = strlen(rule);
	int ril = strlen(il);
	int rb = (strstr (rule, il) - rule);
	char rapp[MRS]; // tempopary array to check the rule.
	char rapp1[MRS];
	char *t;
	int i, j;

	if ( (rule[rl-1] == '=') && (rule[rl-2] == '=') )
	{
		// now do sybstitution to whole expression in the rule
		for (i=0; i < rb; i++ )
		{
			rapp[i] = rule[i];
		}

		rapp[i++] = 'X';  // the signature of variable.


		for ( j = rb + ril; j < rl ; j++ )
		{
			rapp [i + j - rb - ril] = rule [j];
		}

		rapp [i + j - rb - ril] = 0;

		printf("substituted: %s\n", rapp);

		// check if we have expression of type "X Y =="
		t = strtok(rapp, " ");
		if (t != NULL ) 
		{
			printf("%s\n", t);
			if ( (t[0]!= 'X') && (t[0] != '='))		
			{
				strcpy(rapp1, t); // copy Y expression
			}
		}
			
		for (i=0; i<2; i++) {
			t = strtok(NULL, " ");
			
			if (t == NULL)
			{	
				printf("It's not 'X Y ==' type of expression, skipping rule.\n" );
				return -1;
			}
			else
			{
				printf("%s\n", t);

				if ( (t[0]!= 'X') && (t[0] != '='))		
				{
					strcpy(rapp1, t); // copy Y expression
				}
			}
		}
		t = strtok(NULL, " ");
		if (t != NULL)
		{
			printf("It's not 'X Y ==' type of expression, skipping rule.\n" );
			return -1;
		}

		// now replacing the expression.
		strcpy(rule, rapp1);
		
		
	}
	return 0;

}

int applyName(char * il, char * crule, char * pname)
{
	// substitute name with the rule.
	int rl = strlen(crule);
	int ril = strlen(il);
	int rb = (strstr(crule, pname) - crule);
	int rn = (strstr (il, pname) - il);
	char rapp[MRS]; // tempopary array to check the rule.
	char *t;
	int i, j;
	j=0;
	for (i=0; i<rn; i++)
	{
		rapp[j++] = il[i];
	}
	rapp[j] = 0;
	printf("%s\n", rapp);
	for (i=0; i<rb; i++)
	{
		rapp[j++] = crule[i];
	}
	rapp[j] = 0;
	printf("%s\n", rapp);

	printf("pname length:%d\n", strlen(pname) );
	printf("il: %s\n", il);
	
	for(i = rn + strlen(pname)+1; i<ril; i++)
	{
		rapp[j++] = il[i];
	}
	rapp[j] = 0;

	printf("%s\n", rapp);
	memset(crule, 0, rl);
	strcpy (crule, rapp);
	return 0;

}

int addToRibbon(char * ribbon, char * str)
{
	int l = strlen(ribbon);
	strcpy (ribbon + l, str);
	l += strlen(str);
	ribbon[l++] = edel[0];
	ribbon[l++] = '\n';

	return 0;
}


int main ()
{
	FILE* fp;
	char il[MLEN];
	char ilc[MLEN];
	char ribbon[MRIBBON]; //the ribbon of calculations.
	int rp = 0; // the ribbon phrase beggining
	int rn = 0; // ribbon new expressions starts here.
	int re = 0; // ribbon end.
	char rules[MRULES]; 	// list of rules applicable to the case.
	char * crule;
	int rsize;	// rule size;
	int c;
	char * pname;
	int soffs; 	// input offset


	printf("Ssum\n");

	fp = fopen ("defs", "r");
	if (fp == NULL)
	{
		perror("Can't open file with definitions.");
		return -1;
	}
	c = 0;
	do 
	{
		rules[c++] = fgetc(fp);
		if (rules[c-1] == '\n' ) rules[c-1] = rdel[0];
		if (rules[c-1] == '\r' ) rules[c-1] = rdel[0];
	}
	while ( (c < MRULES) && (rules[c-1] != EOF) );

	rules[c-1] = 0; // clear EOL symbol

	printf("%d\n", strlen(rules));
	printf("%s\n", rules );	

	memset((void*)il, 0, MLEN);
	//read input line.
	c = 0;
	do 
	{
		il[c++] = getchar();
	}
	while ( (c < MLEN) && (il[c-1] != '\n') );

	il[c-1] = 0;	
	//clear trailing spaces
	c--;
	while (il [c-1] == ' ' && c>0)	il[--c] = 0;

	if (strlen (il) == 0) 
	{
		printf("Empty input, skipping.\n");
		return 0;
	}
	printf("%d\n", strlen(il));
	printf("%s\n", il );

	
	// normalize string and look for pattern in the definitions.

	normalize(il);

	// check if we have definition of the equation.

	soffs = checkExpr(il, rules);
	printf("Offset = %d\n", soffs);

	if ( soffs >=0 )
	{
		rsize = getRule(soffs, rules); // extracting the rule from the list of rules
		crule = strtok( &rules[rsize], rdel );	//extract full rule string

		printf("crule: %s\n", crule);

		if (applyRule(il, crule) >=0 ) // apply rule type of 'X Y ==' or 'Y X =='.
		{
			printf("Answer: %s\n", crule);
			return 0;
		}

	}else
	{
		printf("Does not match any rule. Continue...\n");
	}

	// now the main loop of pattern search and defs application
	memset(ribbon, 0, MRIBBON);

	strcpy (ribbon, il);
	re += strlen(il);
	ribbon[re++] = edel[0];
	ribbon[re++] = '\n';


	printf("%s\n", ribbon);

	strcpy(ilc, il); // copy input line.

	// getting names from rules list.
	pname = strtok (il," ");
	printf("Name: %s\n", pname);

	soffs = findDef(pname, rules); // looking for definition of name in the rules list.
	printf("Offset = %d\n", soffs);

	if ( soffs >=0 )
	{
		rsize = getRule(soffs, rules); // extracting the rule from the list of rules
		crule = strtok( &rules[rsize], rdel );	//extract full rule string

		printf("Definition: %s\n", crule);

		// apply name
		printf("il before: %s\n", ilc);
		if ( applyName(ilc, crule, pname) >=0 )
		{
			printf("Converted string: %s\n", crule);

			//add to ribbon
			addToRibbon(ribbon, crule);
		}

	}else
	{
		printf("Definition of name is not found. Please define the name and retry.\n");
		return 0;
	}

	printf("Ribbon: %s\n", ribbon);

	// find all names in the definition.
	// find all operations in rules.




	close(fp);
	return 0;
}