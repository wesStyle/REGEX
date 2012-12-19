#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nfabuilder.h"

void fromArgs(int, char**);
char** fromArr();


int i = 0;
int iFlag = 1;
char *post;
int count;

char *str[4];
char a[] = {"Tom Jones 4424 5/12/66 543354"};
char b[] = {"Mary Tom Adams 5346 11/4/63 28765"};
char c[] = {"Sally Chang 1654 7/22/54 650000"};
char d[] = {"Billy Black 1683 9/23/44 336500"};

int main(int argCount, char** argStrings)
{
	char** stringsArr;
	State *start;

	if (argCount != 1) {
		fromArgs(argCount, argStrings);
		stringsArr == argStrings;
	}
	else stringsArr = fromArr();

	//printf("\niFlag = %i\npost = %s\ni = %i\ncount = %i\nstr[i] = %s", iFlag, post, i, count, argStrings[i]);
	
	if (post == NULL){
		printf("bad regexp %s\n");
		if (argCount == 1) getch();
		return 1;
	}
	
	start = post2nfa(post);
	if(start == NULL){
		fprintf(stderr, "error in post2nfa %s\n", post);
		if (argCount == 1) getch();
		return 1;
	}

	l1.s = (State**)malloc(nstate*sizeof l1.s[0]);
	l2.s = (State**)malloc(nstate*sizeof l2.s[0]);
	for(i ; i < count; i++)
		if (match(start, stringsArr[i], iFlag))
			printf("\n%s", stringsArr[i]);
	if (argCount == 1) getch();
	return 0;
}


void fromArgs(int argCount, char** argStrings) {
	if (argCount <= 2) {
		printf("arguments error"); 
		exit;
	}

	if (!strcmp(argStrings[1], "-i")) {
		iFlag = 1;
		post = re2post(argStrings[2]);
		i = 3;
	}

	else {
		post = re2post(argStrings[1]);
		i = 2;
	}
	count = argCount;
}

char** fromArr() {
	int i;

	char regex[80];

	str[0] = a;
	str[1] = b;
	str[2] = c;
	str[3] = d;

	for (i = 0; i < 4; i++)
	{
		printf(str[i]);
		printf("\n");
	}

	printf("\n> ");
	scanf("%s", regex);
	printf("\n\n");

	post = re2post(regex);
	count = 4;

	return str;
}