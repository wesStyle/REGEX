#include <stdlib.h>
#include "nfastate.h"

/*
 * ���������, �������������� ����� ��������� NFA
 * ���� c == ����������, �� ��� �������� ���������.
 * ���� c == Split, �� ����������� �������� � out � out1.
 * ���� c < 256, �������� �������� � �� ��������� c �� ������.
 */



State matchstate = { Match };	/* matching state */

/* ������������� ��������� */
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