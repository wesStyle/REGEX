#include "nfastate.h"

char* re2post(char*);
State* post2nfa(char*);

typedef struct List List;
struct List
{
	State **s;
	int n;
};

List l1, l2;
static int listid;

void addstate(List*, State*);
void step(List*, int, List*);

List* startlist(State*, List*);

int ismatch(List *);

int match(State*, char*, int);