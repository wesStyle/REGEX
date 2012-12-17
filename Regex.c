#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nfabuilder.h"

int main()
{
	int argc = 4;
	char **argv;

	int i;
	char *post;
	State *start;

	char a[] = {"Tom Jones 4424 5/12/66 543354"};
	char b[] = {"Mary Adams 5346 11/4/63 28765"};
	char c[] = {"Sally Chang 1654 7/22/54 650000"};
	char d[] = {"Billy Black 1683 9/23/44 336500"};

	char ch[80];
	char *p[4];

	p[0] = a;
	p[1] = b;
	p[2] = c;
	p[3] = d;

	for (i = 0; i < 4; i++)
	{
		printf(p[i]);
		printf("\n");
	}

	printf("\n>");
	scanf("%s", ch);
	printf("\n\n");

	argv = p;
	
	post = re2post(ch);
	if(post == NULL){
		fprintf(stderr, "bad regexp %s\n", argv[1]);
		getchar();
		return 1;
	}

	start = post2nfa(post);
	if(start == NULL){
		fprintf(stderr, "error in post2nfa %s\n", post);
		getchar();
		return 1;
	}
	
	l1.s = (State**)malloc(nstate*sizeof l1.s[0]);
	l2.s = (State**)malloc(nstate*sizeof l2.s[0]);
	for(i = 0; i < argc; i++)
		if (match(start, argv[i]))
			printf("%s\n", argv[i]);
	getch();
	return 0;
}