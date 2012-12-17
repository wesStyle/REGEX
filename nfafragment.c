#include "nfafragment.h"
#include "nfastate.h"

Frag frag(State *start, Ptrlist *out)
{
	Frag n = { start, out };
	return n;
}