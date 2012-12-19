#include <stdio.h>
#include <string.h>
#include "nfabuilder.h"
#include "nfafragment.h"
#include "ptrlist.h"

/*
 * Перевод регулярного выражения в постфиксный вид, используя символ "'" как оператор конкатенации
 */
char* re2post(char *re)
{
	int nalt, natom;
	static char buf[8000];
	char *dst;

	struct {
		int nalt;
		int natom;
	} paren[100], *p;
	
	p = paren;
	dst = buf;
	nalt = 0;
	natom = 0;

	if (strlen(re) >= sizeof buf/2)
		return NULL;
	for (; *re; re++) {

		switch (*re) {
		case '(':
			if (natom > 1) {
				--natom;
				*dst++ = '`';
			}
			if (p >= paren+100)
				return NULL;
			p->nalt = nalt;
			p->natom = natom;
			p++;
			nalt = 0;
			natom = 0;
			break;
		case '|':
			if (natom == 0)
				return NULL;
			while (--natom > 0)
				*dst++ = '`';
			nalt++;
			break;
		case ')':
			if (p == paren)
				return NULL;
			if (natom == 0)
				return NULL;
			while (--natom > 0)
				*dst++ = '`';
			for (; nalt > 0; nalt--)
				*dst++ = '|';
			--p;
			nalt = p->nalt;
			natom = p->natom;
			natom++;
			break;
		case '*':
		case '+':
		case '?':
			if (natom == 0)
				return NULL;
			*dst++ = *re;
			break;
		default:
			if (natom > 1) {
				--natom;
				*dst++ = '`';
			}
			*dst++ = *re;
			natom++;
			break;
		}
	}
	if (p != paren)
		return NULL;
	while (--natom > 0)
		*dst++ = '`';
	for (; nalt > 0; nalt--)
		*dst++ = '|';
	*dst = 0;
	return buf;
}

/*
 * Построение Недетерминированного Конечного Автомата по ругулярному выражению в постфиксном виде
 * Функция возвращает начальное состояние автомата
 */
State* post2nfa(char *postfix)
{
	char *p;
	Frag stack[1000], *stackp, e1, e2, e;
	State *s;

	if (postfix == NULL)
		return NULL;

	#define push(s) *stackp++ = s
	#define pop() *--stackp

	stackp = stack;
	for (p=postfix; *p; p++) {
		switch(*p){
		default:
			s = state(*p, NULL, NULL);
			push(frag(s, list1(&s->out)));
			break;
		case '`':	/* Конкатенация */
			e2 = pop();
			e1 = pop();
			patch(e1.out, e2.start);
			push(frag(e1.start, e2.out));
			break;
		case '|':	/* ИЛИ */
			e2 = pop();
			e1 = pop();
			s = state(Split, e1.start, e2.start);
			push(frag(s, append(e1.out, e2.out)));
			break;
		case '?':	/* один или ноль */
			e = pop();
			s = state(Split, e.start, NULL);
			push(frag(s, append(e.out, list1(&s->out1))));
			break;
		case '*':	/* больше нуля */
			e = pop();
			s = state(Split, e.start, NULL);
			patch(e.out, s);
			push(frag(s, list1(&s->out1)));
			break;
		case '+':	/* один или более */
			e = pop();
			s = state(Split, e.start, NULL);
			patch(e.out, s);
			push(frag(e.start, list1(&s->out1)));
			break;
		case '^': /* Начало строки */
			s = state(StartAnchor, NULL, NULL);
			push(frag(s, list1(&s->out)));
			break;
		case '$': /* Конец строки */
			s = state(EndAnchor, NULL, NULL);
			push(frag(s, list1(&s->out)));
			break;
		case '.': /* любой символ */
			s = state(Any, NULL, NULL);
			push(frag(s, list1(&s->out)));
			break;
		}
	}

	e = pop();
	if (stackp != stack)
		return NULL;

	patch(e.out, &matchstate);
	return e.start;
#undef pop
#undef push
}

/* Создание начального листа состояний NFA */
List* startlist(State *start, List *l)
{
	l->n = 0;
	listid++;
	addstate(l, start);
	return l;
}

/* Проверка, соответствует ли текущий лист состояний сходству */
int ismatch(List *l)
{
	int i;

	for (i=0; i < l->n; i++)
		if (l->s[i] == &matchstate)
			return 1;
	return 0;
}

/* Добавление состояния s в лист l, следуя безусловным переходам */
void addstate(List *l, State *s)
{
	if (s == NULL || s->lastlist == listid)
		return;
	s->lastlist = listid;

	if (s->c == Split) {
		/* следование безусловным переходам */
		addstate(l, s->out);
		addstate(l, s->out1);
		return;
	}

	l->s[l->n++] = s;
}

/*
 * Шаг NFA по листу состояний и создания листа следующих состояний NFA
 */
void step(List *clist, int c, List *nlist, int len, int elem, int iFlag)
{
	int i;
	State *s;
	char tmp1, tmp2;
	int tmp = -1;
	listid++;
	nlist->n = 0;
	for (i=0; i < (clist->n); i++){
		s = clist->s[i];
		if (s->c == StartAnchor) {
			if (elem == 1) {
				addstate(nlist, s->out);
				s = nlist->s[nlist->n -1];
				nlist->n--;
				if (iFlag == 0) {
					if ((s->c == c) || (s->c == Any))
					{
						addstate(nlist, s->out);
					}
				}
				else if (iFlag == 1) {
					tmp1 = s->c;
					tmp2 = c;
					if ((s->c == Any) || (strnicmp(&tmp1, &tmp2, 1) == 0)) {
						addstate(nlist, s->out);
					}
				}
			}
		}
		else if (elem == len) {
			if (iFlag == 0) {
				if ((s->c == c) || (s->c == Any)) {
					addstate(nlist, s->out);
					s = nlist->s[i];
					nlist->n--;
					if (s->c == EndAnchor) {
					addstate(nlist, s->out);
					}
				}
			}
			else if (iFlag == 1) {
				tmp1 = s->c;
				tmp2 = c;
				if ((s->c == Any) || (strnicmp(&tmp1, &tmp2, 1) == 0)) {
					addstate(nlist, s->out);
					s = nlist->s[i];
					nlist->n--;
					if (s->c == EndAnchor) {
					addstate(nlist, s->out);
					}
				}
			}
		}
		else {
			if (iFlag == 0) {
				if ((s->c == c) || (s->c == Any))
				addstate(nlist, s->out);
			}
			else if (iFlag == 1) {
				tmp1 = s->c;
				tmp2 = c;
				if ((s->c == Any) || (strnicmp(&tmp1, &tmp2, 1) == 0)) {
					addstate(nlist, s->out);
				}
			}
		}
	}
}

/* Запуск NFA и проверка на совпадение строки */
int match(State *start, char *s, int iFlag)
{
	int i, c;
	List *clist, *nlist, *t;
	int len = strlen(s);
	int elem = 0;

	clist = startlist(start, &l1);
	nlist = &l2;

	for (; *s; s++) {
		
		elem++;
		c = *s & 0xFF;
		if (clist -> n == 0) {
			clist = startlist(start, &l1);
			nlist = &l2;
		}

		step(clist, c, nlist, len, elem, iFlag);
		t = clist; clist = nlist; nlist = t;	/* смена листов текущих и следующих состояний местами */

		if (ismatch(clist)) return 1;
	}
	return 0;
}