#include "nfastate.h"
#include "nfafragment.h"

union Ptrlist
{
	Ptrlist *next;
	State *s;
};

/* �������� ����� ���������� � ����� ������� */
Ptrlist* list1(State**);

void patch(Ptrlist*, State*);

Ptrlist* append(Ptrlist*, Ptrlist*);