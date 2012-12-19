#include <stdlib.h>
#include "nfastate.h"

/*
 * —труктура, представл€юща€ собой состо€ние NFA
 * ≈сли c == совпадение, то это конечное состо€ние.
 * ≈сли c == Split, то безусловные переходы в out и out1.
 * ≈сли c < 256, условные переходы с со значением c на выходе.
 */



State matchstate = { Match };	/* matching state */

/* »нициализаци€ состо€ни€ */
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