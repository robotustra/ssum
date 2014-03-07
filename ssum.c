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
		if (t != NULL ) printf("%s\n", t);
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
					strcpy(rapp1, t);
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

int main ()
{
	FILE* fp;
	char il[MLEN];
	char ribbon[MRIBBON]; //the ribbon of calculations.
	int rp; // the ribbon phrase beggining
	int rn; // ribbon new expressions starts here.
	int re; // ribbon end.
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
		rsize = getRule(soffs, rules); // extracting the rule from
		crule = strtok( &rules[rsize], rdel );

		printf("crule: %s\n", crule);

		applyRule(il, crule);

		printf("%s\n", crule);

	}

	// now the main loop of pattern search and defs application



	pname = strtok (il," ");






	close(fp);
	return 0;
}