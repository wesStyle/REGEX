#include <stdlib.h>
#include "ptrlist.h"

Ptrlist* list1(State **outp)
{
	Ptrlist *l;
	
	l = (Ptrlist*)outp;
	l->next = NULL;
	return l;
}

/* ���������� ������ ������ ����� �� ������� ������� (���������� ����������)*/
void patch(Ptrlist *l, State *s)
{
	Ptrlist *next;
	
	for (; l; l = next) {
		next = l->next;
		l->s = s;
	}
}

/* ���������� ������ l1 � l2 */
Ptrlist* append(Ptrlist *l1, Ptrlist *l2)
{
	Ptrlist *oldl1;
	
	oldl1 = l1;
	while (l1->next)
		l1 = l1->next;
	l1->next = l2;
	return oldl1;
}