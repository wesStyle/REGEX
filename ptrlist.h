#include "nfastate.h"
#include "nfafragment.h"

/*
 * Since the out pointers in the list are always 
 * uninitialized, we use the pointers themselves
 * as storage for the Ptrlists.
 */
union Ptrlist
{
	Ptrlist *next;
	State *s;
};

/* Create singleton list containing just outp. */
Ptrlist* list1(State**);

void patch(Ptrlist*, State*);

/* Join the two lists l1 and l2, returning the combination. */
Ptrlist* append(Ptrlist*, Ptrlist*);