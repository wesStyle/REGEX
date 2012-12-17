#ifndef H_STATE
#define H_STATE

typedef struct State State;

struct State
{
	int c;
	State *out;
	State *out1;
	int lastlist;
};

enum
{
	Match = 256,
	Split = 257,
	Any = 258,
	StartAnchor = 259,
	EndAnchor = 260
};

int nstate;

State* state(int c, State *out, State *out1);

State matchstate;
#endif