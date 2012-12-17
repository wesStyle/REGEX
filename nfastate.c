#include <stdlib.h>
#include "nfastate.h"

/*
 * Represents an NFA state plus zero or one or two arrows exiting.
 * if c == Match, no arrows out; matching state.
 * If c == Split, unlabeled arrows to out and out1 (if != NULL).
 * If c < 256, labeled arrow with character c to out.
 */



State matchstate = { Match };	/* matching state */

/* Allocate and initialize State */
State* state(int c, State *out, State *out1)
{
	State *s;
	
	nstate++;
	s =(State*) malloc(sizeof *s);
	s->lastlist = 0;
	s->c = c;
	s->out = out;
	s->out1 = out1;
	return s;
}