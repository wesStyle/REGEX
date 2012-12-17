#ifndef H_FRAG
#define H_FRAG
#include "nfastate.h"

/*
 * A partially built NFA without the matching state filled in.
 * Frag.start points at the start state.
 * Frag.out is a list of places that need to be set to the
 * next state for this fragment.
 */

typedef struct Frag Frag;
typedef union Ptrlist Ptrlist;

struct Frag
{
	State *start;
	Ptrlist *out;
};

/* Initialize Frag struct. */
Frag frag(State* , Ptrlist*);


#endif