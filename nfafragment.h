#ifndef H_FRAG
#define H_FRAG
#include "nfastate.h"

/*
 * Фрагмент NFA, где * start - начальное состояние фрагмента, а *out - список следующих возможных состояний
 */

typedef struct Frag Frag;
typedef union Ptrlist Ptrlist;

struct Frag
{
	State *start;
	Ptrlist *out;
};

/* Инициализация структуры */
Frag frag(State* , Ptrlist*);

#endif