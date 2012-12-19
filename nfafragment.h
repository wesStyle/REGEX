#ifndef H_FRAG
#define H_FRAG
#include "nfastate.h"

/*
 * �������� NFA, ��� * start - ��������� ��������� ���������, � *out - ������ ��������� ��������� ���������
 */

typedef struct Frag Frag;
typedef union Ptrlist Ptrlist;

struct Frag
{
	State *start;
	Ptrlist *out;
};

/* ������������� ��������� */
Frag frag(State* , Ptrlist*);

#endif